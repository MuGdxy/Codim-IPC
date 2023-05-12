import sys

sys.path.insert(0, "../../Python")
import math
import Drivers
from JGSL import *

class BraidingParams:
    def __init__(self):
        self.restLen: float = 0.05
        self.radius: float = 0.04
        self.totalSegCountPerFiber: int = 10
        self.yarnPath: str = "./yarns/simple/yarn_"
        self.range: range = range(1)
        self.initSegCount = 4
        self.moveUpSpeed = 0
        self.moveDownSpeed = 0.1
        self.maxDragLen:float = 0.1
        self.bobbinMovementScale = 1.0

def setVecComp(vec, i, value: float):
    Storage.SetComponent(vec, i, value)

def distance(x1:Vector3d,x2:Vector3d):
    return math.sqrt((x1[0]-x2[0])**2+(x1[1]-x2[1])**2+(x1[2]-x2[2])**2)


def BoundingBox(posList: list[Vector3d]):
    min = Vector3d(1e10, 1e10, 1e10)
    max = Vector3d(-1e10, -1e10, -1e10)
    for pos in posList:
        for i in range(3):
            if Storage.GetComponent(pos, i) < Storage.GetComponent(min, i):
                Storage.SetComponent(min, i, Storage.GetComponent(pos, i))
            if Storage.GetComponent(pos, i) > Storage.GetComponent(max, i):
                Storage.SetComponent(max, i, Storage.GetComponent(pos, i))
    return min - Vector3d(1e-6, 1e-6, 1e-6), max + Vector3d(1e-6, 1e-6, 1e-6)


class BobbinController:
    class Bobbin:
        def __init__(self):
            self.current = 0
            # (x, t, z)
            self.position = []
            self.lastT = 0.0
            self.deltaT = 0.0

        def __RoundIndex(self, i):
            return i % len(self.position)

        def Get(self, dt):
            aim = dt + self.lastT
            dIndex = int(math.floor(aim / self.deltaT))
            self.current = self.__RoundIndex(self.current + dIndex)
            next = self.__RoundIndex(self.current + 1)
            nextpos = self.position[next]
            setVecComp(nextpos, 1, 0.0)
            curpos = self.position[self.current]
            setVecComp(curpos, 1, 0.0)
            print("in", self.current, ",", next)
            self.lastT = aim - dIndex * self.deltaT
            temp = float(self.lastT / self.deltaT)
            v = nextpos - curpos
            dx = Vector3d(v[0] * temp, v[1] * temp, v[2] * temp)
            pos = curpos + dx
            return pos, dx

    def __GetVector(self, line: str):
        v = line.split(",")
        s = self.parms.bobbinMovementScale
        return Vector3d(s*float(v[0]), s*float(v[2]), s*float(v[1]))

    def __CreateBobbin(self, file):
        b = BobbinController.Bobbin()
        while True:
            line = file.readline()
            if line == "":
                break
            b.position.append(self.__GetVector(line))
        b.deltaT = abs(b.position[1][1] - b.position[0][1])
        self.bobbins.append(b)

    def __init__(self, filename, range: range, parms: BraidingParams):
        self.bobbins = []
        self.t = 0.0
        self.timeScale = 1.0
        self.parms: BraidingParams = parms
        for i in range:
            fi = filename + str(i) + ".txt"
            f = open(fi, "r")
            self.__CreateBobbin(f)
            f.close()

    def GetBobbinPositionVelocity(self, index: int, dt: float):
        b: BobbinController.Bobbin = self.bobbins[index]
        pos, dx = b.Get(dt * self.timeScale)
        return pos, dx / dt

class Fiber:
    BoundMin = Vector3d(-1e4, -1e4, -1e4)
    BoundMax = Vector3d(1e4, 1e4, 1e4)
    def __init__(self, sim: Drivers.FEMDiscreteShellBase, braidingController):
        self.nodeOffset: int = -1
        self.sim: Drivers.FEMDiscreteShellBase = sim
        self.controller: BobbinController = None
        self.nodeAtBobbin: int = -1
        self.fiberID = -1
        self.fiberIndex = -1
        self.parms: BraidingParams = None
        self.nodeCount: int = -1
        self.boundaryCount:int = 0
        self.braidingController = braidingController

    def createFiber(
        self,
        fiberID: int,
        fiberIndex: int,
        parms: BraidingParams,
        controller: BobbinController,
    ):
        height = parms.totalSegCountPerFiber * parms.restLen
        self.nodeAtBobbin = parms.initSegCount
        self.fiberID = fiberID
        self.fiberIndex = fiberIndex
        self.controller = controller
        self.parms = parms

        self.nodeCount = parms.totalSegCountPerFiber + 1
        self.nodeOffset = fiberIndex * self.nodeCount
        

        pos, v = controller.GetBobbinPositionVelocity(fiberIndex, 0.0)
        
        self.upperX = Vector3d(pos[0], pos[1], pos[2])
        
        self.sim.make_and_add_rod_3D(
            height,
            parms.totalSegCountPerFiber,
            Vector3d(pos[0], - height / 2 + parms.initSegCount * parms.restLen, pos[2]),  # move up and set position xz
            Vector3d(0, 0, 0),  # ignore
            Vector3d(0, 0, 1),  # rotate around z-axis
            -90,  # degree
            Vector3d(1, 1, 1),  # scale
        )

    def getNodeAtBobbin(self):
        return self.nodeOffset + self.nodeAtBobbin

    def move(self, dt: float, upperXy: float, lowerXy: float):
        self.boundaryCount = 0
        # move down + rotate
        pos, v = self.controller.GetBobbinPositionVelocity(self.fiberIndex, dt)
        lowerX = Storage.Get(self.sim.X, self.getNodeAtBobbin())
        Vxz = (pos - lowerX)/dt
        moveLowerVy = (lowerXy - lowerX[1])/dt
        self.setBoundaryVelocity(self.getNodeAtBobbin(), self.nodeOffset + self.nodeCount, Vector3d(Vxz[0], moveLowerVy, Vxz[2]))

        # move up
        upperX = Storage.Get(self.sim.X, self.nodeOffset)
        moveUpperVy = (upperXy - upperX[1])/dt
        self.setBoundaryVelocity(self.nodeOffset, self.nodeOffset+1, Vector3d(0, moveUpperVy, 0))

        XAtBobbin = Storage.Get(self.sim.X, self.getNodeAtBobbin())
        XAtBottom = Storage.Get(self.sim.X, self.nodeOffset + self.nodeCount - 1)
        return upperX, XAtBobbin, XAtBottom

    def releaseNode(self):
        if self.nodeAtBobbin < self.nodeCount - 1: # has node to release
            last = self.nodeOffset + self.nodeAtBobbin - 1
            current = self.nodeOffset + self.nodeAtBobbin
            Xlast = Storage.Get(self.sim.X, last)
            Xcurrent = Storage.Get(self.sim.X, current)
            dis = distance(Xlast, Xcurrent)
            if dis >= self.parms.maxDragLen:
                print("release node", self.nodeAtBobbin)
                self.nodeAtBobbin += 1
    
    def getUpperX(self):
        return Storage.Get(self.sim.X, self.nodeOffset)
    
    def setBoundaryVelocity(self, i:int, v:Vector3d):
        self.setBoundaryVelocity(i, i+1, v)
    
    def setBoundaryVelocity(self, iBegin:int, iEnd:int , v:Vector3d):
        self.braidingController.incBoundaryCount()
        self.sim.set_DBC_with_range(
            Fiber.BoundMin, Fiber.BoundMax, v,
            Vector3d(0, 0, 0),
            Vector3d(0, 1, 0),
            0,
            Vector4i(iBegin, 0, iEnd, 0))


class BraidingController:
    def __init__(self, sim: Drivers.FEMDiscreteShellBase, parms: BraidingParams):
        self.sim: Drivers.FEMDiscreteShellBase = sim
        self.parms: BraidingParams = parms
        self.bobbinController: BobbinController = BobbinController(
            parms.yarnPath, parms.range, parms
        )
        self.fibers: list[Fiber] = []
        self.upperXy:float = parms.initSegCount * parms.restLen
        self.lowerXy:float = 0
        self.boundaryCount:int = 0
        
        
        idx = 0
        for ID in range(parms.range.stop):
            f = Fiber(sim, self)
            f.createFiber(ID, idx, parms, self.bobbinController)
            self.fibers.append(f)
            idx += 1
        print("fiber count:", len(self.fibers))
        
    def incBoundaryCount(self):
        self.boundaryCount += 1

    def braid(self, dt: float):
        if self.boundaryCount > 0:
            self.sim.pop_DBC(self.boundaryCount)
        
        self.boundaryCount = 0
        
        self.upperXy += self.parms.moveUpSpeed * dt
        self.lowerXy -= self.parms.moveDownSpeed * dt
        
        for f in self.fibers:
            f.releaseNode()
        
        for f in self.fibers:
            f.move(dt, self.upperXy, self.lowerXy)
    
    def export(self, frame):
        folder = self.sim.output_folder
        content = ""
        for f in self.fibers:
            content += f"# {f.fiberIndex} {f.nodeAtBobbin} {f.nodeCount}\n"
            for i in range(f.nodeCount):
                x = Storage.Get(self.sim.X, f.nodeOffset + i)
                content += f"{x[0]} {x[1]} {x[2]}\n"
        
        with open(folder + f"/fibers{frame}.txt", "w") as f:
            f.write(content)




if __name__ == "__main__":
    b = BobbinController("./yarns/simple/yarn_", range(1))
    b.timeScale = 1
    pos, v = b.GetBobbinPositionVelocity(0, 0.0)
    print(pos[0], pos[1], pos[2])
