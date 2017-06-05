"""
Computes, plots, and saves the 2D vorticity field from a cuIBM simulation at
saved time-steps.
"""

from snake.cuibm.simulation import CuIBMSimulation


simulation = CuIBMSimulation()
simulation.read_grid()

for time_step in simulation.get_time_steps():
  simulation.read_fields('vorticity', time_step)
  simulation.plot_contour('vorticity',
                          field_range=(-5.0, 5.0, 40),
                          filled_contour=False,
                          view=[-1.0, -3.0, 15.0, 3.0],
                          colorbar=False,
                          cmap=None,
                          colors='k',
                          style='seaborn-dark',
                          width=8.0)
