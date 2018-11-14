import multiprocessing as mp
import numpy as np
import time
from body import Body
import rk_ext
import matplotlib.pyplot as plt

class System(object):
    """System encapsulates all data and methods relating to an
    n-body system.
    """
    def __init__(self, body_list):
        # Instantiation only
        self.iterator = 0
        self.multi = False
        self.printer = False
        self.consts = {'G':6.67408e-11}
        self.body_list = body_list
        self.num_bodies = len(body_list)
        self.masses = [body.mass for body in self.body_list]
        self.mg_list = [self.consts['G']*mass for mass in self.masses]

        self.x_positions = self.set_positions('x')
        self.y_positions = self.set_positions('y')
        self.z_positions = self.set_positions('z')
        self.x_velocities = self.set_velocities('x')
        self.y_velocities = self.set_velocities('y')
        self.z_velocities = self.set_velocities('z')
        self.radii = self.set_radii()

        self.acceleration_coeffs = self.set_acceleration_coeffs()

        self.data = {'vx':[[v] for v in self.x_velocities],#[None] for i in range(self.num_bodies)],
                'vy':[[v] for v in self.y_velocities],#[None] for i in range(self.num_bodies)],
                'vz':[[v] for v in self.z_velocities],#[None] for i in range(self.num_bodies)],
                'x': [[p] for p in self.x_positions],#[None] for i in range(self.num_bodies)],
                'y': [[p] for p in self.y_positions],#[None] for i in range(self.num_bodies)],
                'z': [[p] for p in self.z_positions],#[None] for i in range(self.num_bodies)],
                'ac': [[ac] for ac in self.acceleration_coeffs],#[None] for i in range(self.num_bodies)],
                }

    def setup_integrators(self, tableau, t_init, t_fin, steps):
        self.vx_integrator = rk_ext.RKIntegrator(self.compute_acceleration, tableau, steps, t_init, t_fin, False)
        self.vy_integrator = rk_ext.RKIntegrator(self.compute_acceleration, tableau, steps, t_init, t_fin, False)
        self.vz_integrator = rk_ext.RKIntegrator(self.compute_acceleration, tableau, steps, t_init, t_fin, False)
        self.x_integrator  = rk_ext.RKIntegrator(self.get_x_velocity, tableau, steps, t_init, t_fin, False)
        self.y_integrator  = rk_ext.RKIntegrator(self.get_y_velocity, tableau, steps, t_init, t_fin, False)
        self.z_integrator  = rk_ext.RKIntegrator(self.get_z_velocity, tableau, steps, t_init, t_fin, False)


    def run(self):
        while (not self.z_integrator.isFinished()):
            if self.multi:
                vx = mp.Process(target = self.update_velocities, args=(self.vx_integrator.step(self.x_velocities), 'x'))
                vy = mp.Process(target = self.update_velocities, args=(self.vy_integrator.step(self.y_velocities), 'y'))
                vz = mp.Process(target = self.update_velocities, args=(self.vz_integrator.step(self.z_velocities), 'z'))
                x = mp.Process(target = self.update_positions, args=(self.x_integrator.step(self.x_positions), 'x'))
                y = mp.Process(target = self.update_positions, args=(self.y_integrator.step(self.y_positions), 'y'))
                z = mp.Process(target = self.update_positions, args=(self.z_integrator.step(self.z_positions), 'z'))

                vx.start()
                vy.start()
                vz.start()
                vx.join()
                vy.join()
                vz.join()

                x .start()
                y .start()
                z .start()
                x .join()
                y .join()
                z .join()

            else:
                #self.update_velocities(self.vx_integrator.step(self.x_velocities), 'x')
                #self.update_velocities(self.vy_integrator.step(self.y_velocities), 'y')
                #self.update_velocities(self.vz_integrator.step(self.z_velocities), 'z')
                #self.update_positions(self.x_integrator.step(self.x_positions), 'x')
                #self.update_positions(self.y_integrator.step(self.y_positions), 'y')
                #self.update_positions(self.z_integrator.step(self.z_positions), 'z')

                self.x_velocities = self.vx_integrator.step(self.x_velocities)
                self.y_velocities = self.vy_integrator.step(self.y_velocities)
                self.z_velocities = self.vz_integrator.step(self.z_velocities)
                self.x_positions = self.x_integrator.step(self.x_positions)
                self.y_positions = self.y_integrator.step(self.y_positions)
                self.z_positions = self.z_integrator.step(self.z_positions)

            #print(self.vx_integrator.step(self.x_velocities))
            #print(self.vy_integrator.step(self.y_velocities))
            #print(self.vz_integrator.step(self.z_velocities))
            #print(self.x_integrator.step(self.x_positions))
            #print(self.y_integrator.step(self.y_positions))
            #print(self.z_integrator.step(self.z_positions))

            self.acceleration_coeffs = self.set_acceleration_coeffs()
            self.update_data()

            #for i in range(self.num_bodies):
            #    self.data['vx'][i].append(self.x_velocities[i])
            #    self.data['vy'][i].append(self.y_velocities[i])
            #    self.data['vz'][i].append(self.z_velocities[i])
            #    self.data['x'][i].append(self.x_positions[i])
            #    self.data['y'][i].append(self.y_positions[i])
            #    self.data['z'][i].append(self.z_positions[i])
            #    self.data['ac'][i].append(self.acceleration_coeffs[i])


    def update_data(self):
        for i in range(self.num_bodies):
            self.data['vx'][i].append(self.x_velocities[i])
            self.data['vy'][i].append(self.y_velocities[i])
            self.data['vz'][i].append(self.z_velocities[i])
            self.data['x'][i].append(self.x_positions[i])
            self.data['y'][i].append(self.y_positions[i])
            self.data['z'][i].append(self.z_positions[i])
            self.data['ac'][i].append(self.acceleration_coeffs[i])


    def set_acceleration_coeffs(self):
        """Holy list comprehension batman!
        """
        #acceleration_coeffs = [sum([(Gm/np.power(r, 3) if r != 0 else 0)
        #                       for Gm, r in zip(self.mg_list, radii)])
        #                       for radii in self.radii]

        self.radii = self.set_radii();
        acceleration_coeffs = [self.consts['G']*sum([(m/np.power(r, 3) if r != 0 else 0)
            for m, r in zip(self.masses, radii)])
            for radii in self.radii]

        if self.printer:
            if self.iterator <= 1:
                print([[self.consts['G']*m/np.power(r, 3) if r != 0 else 0
                        for m, r in zip(self.masses, radii)]
                        for radii in self.radii]
                        )
                print("Acceleration Coefficients")
                print("----------------------------------")
                for i in acceleration_coeffs:
                    print(i)
                self.iterator += 1

        return acceleration_coeffs


    def set_positions(self, coordinate):
        """Sets up the appropriate list for x, y, or z coordinates
        for the system.
        """
        coord_dict = {'x':0, 'y':1, 'z':2}
        positions = [body.position[coord_dict[coordinate]] for body in self.body_list]

        return positions


    def set_velocities(self, coordinate):
        """Sets up the appropriate list for x, y, or z velocities
        for the system.
        """
        coord_dict = {'x':0, 'y':1, 'z':2}
        velocities = [body.velocity[coord_dict[coordinate]] for body in self.body_list]

        return velocities


    def set_radii(self):
        """Computes the radius separating each body from every
        other body.
        """
        radii = [[None for i in range(self.num_bodies)] for i in range(self.num_bodies)]

        for i in range(self.num_bodies):
            for j in range(self.num_bodies):
                radii[i][j] = np.sqrt(
                        np.power(self.x_positions[j] - self.x_positions[i], 2)
                        + np.power(self.y_positions[j] - self.y_positions[i], 2)
                        + np.power(self.z_positions[j] - self.z_positions[i], 2))

                #if self.printer:
                #    if self.iterator <= 5:
                #        print("Radii")

                #self.iterator += 1

        return radii


    def update_positions(self, new_positions_list, coord):
        if coord == 'x':
            self.x_positions = new_positions_list
        elif coord == 'y':
            self.y_positions = new_positions_list
        elif coord == 'z':
            self.y_positions = new_positions_list

    def update_velocities(self, new_velocities_list, coord):
        if coord == 'x':
            self.x_velocities = new_velocities_list
        elif coord == 'y':
            self.y_velocities = new_velocities_list
        elif coord == 'z':
            self.y_velocities = new_velocities_list


    def update_radii(self):
        self.radii = self.set_radii();


    def update_acceleration_coeffs(self):
        self.acceleration_coeffs = self.set_acceleration_coeffs()


    def get_x_velocity(self, x, i):
        return self.x_velocities[i]


    def get_y_velocity(self, y, i):
        return self.y_velocities[i]


    def get_z_velocity(self, z, i):
        return self.z_velocities[i]


    def compute_acceleration(self, x, i):
        return -1*self.acceleration_coeffs[i]*x


if __name__ == '__main__':
    sun = Body(mass=1.988e30,
               position=[0, 0, 0],
               velocity=[0, 0, 0],
               name='sun',
              )

    earth = Body(mass=5.972e24,
               position=[149597870e3, 0, 0],
               velocity=[0, 29.8e3, 0],
               name='earth',
              )
    bt = [[   0.,   0.,   0.,   0.,   0.],
          [ 1/2., 1/2.,   0.,   0.,   0.],
          [ 1/2.,   0., 1/2.,   0.,   0.],
          [   1.,   0.,   0.,   1.,   0.],
          [   0., 1/6., 1/3., 1/3., 1/6.]]

    bodies = [sun, earth]
    the_system = System(bodies)
    t_init = 0.
    t_fin = 365*24*3600
    #steps = 1000
    steps = (365*24*3600)//1000
    t_interval = np.linspace(t_init, t_fin, steps)

    the_system.setup_integrators(bt, t_init, t_fin, steps)
    start = time.process_time()
    the_system.run()
    end = time.process_time()
    elapsed = end - start
    print(elapsed)
    print(the_system.x_positions)
    print(the_system.data['x'][1])
    #plt.plot(the_system.data['x'][1], the_system.data['y'][1])
