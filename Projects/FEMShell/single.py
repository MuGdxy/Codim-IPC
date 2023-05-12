import sys
sys.path.insert(0, "../../Python")
import math
import Drivers
from JGSL import *
import BraidingController

if __name__ == "__main__":
    sim = Drivers.FEMDiscreteShellBase("double", 3)
    sim.mu = 0.1
    sim.epsv2 = 1e-8

    sim.make_and_add_rod_3D(
        2,
        2,
        Vector3d(0, 0, 0),
        Vector3d(0, 0, 0),
        Vector3d(0, 0, 1),
        -90,
        Vector3d(1, 1, 1),
    )

    # sim.make_and_add_rod_3D(
    #     0.15,
    #     1,
    #     Vector3d(0.1, 0, 0),
    #     Vector3d(0, 0, 0),
    #     Vector3d(0, 1, 0),
    #     0,
    #     Vector3d(1, 1, 1),
    # )

    sim.set_DBC(Vector3d(-10, 0.5, -10), #min
                Vector3d(10, 10, 10), #max
                Vector3d(0, 10, 0), #v
                Vector3d(0, 0, 0), #rotation center
                Vector3d(0, 1, 0), #rotation axis
                0) #angVelDeg
    
    sim.set_DBC(Vector3d(-10, -0.5, -10), #min
                Vector3d(10, 0, 10), #max
                Vector3d(0, 0, 0), #v
                Vector3d(0, 0, 0), #rotation center
                Vector3d(0, 1, 0), #rotation axis
                0) #angVelDeg

    # sim.set_DBC(Vector3d(-0.1, -0.1, -0.1), Vector3d(1.1, 1.0 / (int(size) - 2), 1.1),
    #     Vector3d(0, 0.001, 0), Vector3d(0, 0, 0), Vector3d(0, 1, 0), 270)

    sim.dt = 0.01
    sim.frame_dt = 0.01
    sim.frame_num = 100
    sim.withCollision = True

    sim.MDBC_tmax = 1e10
    sim.DBCPopBackTStart = 1e10
    sim.DBCPopBackTEnd = 1e10
    sim.DBCPopBackStep = 1
    sim.DBCPopBackAmt = 1

    sim.initialize(
        sim.cloth_density[0], sim.cloth_Ebase[0], 0.4, sim.cloth_thickness[0], 0
    )
    sim.initialize_rod(1300, 4e8, 0.01, 8e-5)
    sim.initialize_OIPC(1e-4, 8e-5)

    v = Storage.Get(sim.X, 0)
    h = v[1]
    print("X.size=",Storage.Size(sim.X))
    #     sim.run()

    sim.write(0)
    for f in range(sim.frame_num):
        sim.current_frame = f + 1
        # v = Storage.Get(sim.X0, 0)
        # Storage.Set(sim.X0, 0, Vector3d(v[0], v[1] + sim.dt * 0.1, v[2]))
        # v = Storage.Get(sim.X, 0)
        # Storage.Set(sim.X, 0, Vector3d(v[0], v[1] + sim.dt * 0.1, v[2]))
        # v = Storage.Get(sim.X_stage, 0)
        # Storage.Set(sim.X_stage, 0, Vector3d(v[0], v[1] + sim.dt * 0.1, v[2]))
        
        # v = Storage.Get(sim.X0, 2)
        # Storage.Set(sim.X0, 2, Vector3d(v[0], v[1] - sim.dt * 0.1, v[2]))
        
        sim.advance_one_frame(f + 1)
        sim.write(f + 1)
        if Get_Parameter("Terminate", False):
            break
