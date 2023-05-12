import sys
sys.path.insert(0, "../../Python")
import math
import Drivers
from JGSL import *
from BraidingController import BraidingController, BraidingParams

if __name__ == "__main__":
    SetMaxIter(10)
    sim = Drivers.FEMDiscreteShellBase("double", 3)
    sim.mu = 0.1
    sim.epsv2 = 1e-8
    sim.gravity = Vector3d(0, 0, 0)
    sim.dt = 0.01
    sim.frame_dt = 0.01
    sim.frame_num = 1000
    sim.withCollision = True

    sim.MDBC_tmax = 1e10
    sim.DBCPopBackTStart = -1
    sim.DBCPopBackTEnd = 1e10
    sim.DBCPopBackStep = 0
    sim.DBCPopBackAmt = 1
    # sim.PNTol = 1e-2
    # sim.dHat2 = 1e-3

    parms = BraidingParams()
    parms.yarnPath = "./yarns/y120/yarn_"
    parms.range = range(120)
    parms.moveUpSpeed = 0
    # 
    parms.moveDownSpeed = 0.4
    parms.totalSegCountPerFiber = 150
    
    parms.restLen = 0.012
    parms.maxDragLen = 0.014
    
    #
    parms.initSegCount = 15
    parms.bobbinMovementScale = 0.12
    
    braider = BraidingController(sim, parms)
    braider.bobbinController.timeScale = 1.0

    sim.initialize(
        sim.cloth_density[0], sim.cloth_Ebase[0], 0.4, sim.cloth_thickness[0], 0
    )
    sim.initialize_rod(800, 3e5, 0.1, 0.01)
    sim.initialize_OIPC(0.01,0.005)

    sim.write(0)
    braider.export(0)
    for f in range(sim.frame_num):
        sim.current_frame = f + 1
        braider.braid(sim.dt) # push boundary conditions
        sim.advance_one_frame(f + 1) # solve
        # sim.write(f + 1)
        braider.export(f + 1)
        if Get_Parameter("Terminate", False):
            break