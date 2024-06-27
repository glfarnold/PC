#include <mpi.h>
#include "heat.h"
#include "constants.h"
#include <iostream>

// Include header files if necessary

void start_halo_exchange(Field *temperature, ParallelData *parallel) {
    
    // This function should initiate the halo exchange to communicate boundary data between neighboring processes.

    // Width for accessing and navigating through the temperature field
    int width = temperature->ny + 2;

    // (up <-> down)
    // Communication 1: Send data to the upper neighbor and receive from the lower neighbor
    // This exchanges the ghost cells in the top row of the local temperature field
    MPI_Isend(&temperature->data[width], 1, parallel->rowtype, parallel->nup, ROW_TAG_UP,
              parallel->comm,  &parallel->requests[0]);

    MPI_Irecv(&temperature->data[width*(temperature->nx+1)], 1, parallel->rowtype, parallel->ndown, ROW_TAG_UP, 
              parallel->comm, &parallel->requests[1]);
    
    // (down <-> up)
    // Communication 2: Send data to the lower neighbor and receive from the upper neighbor
    // This exchanges the ghost cells in the bottom row of the local temperature field
    MPI_Isend(&temperature->data[width*(temperature->nx)], 1, parallel->rowtype, parallel->ndown, ROW_TAG_DOWN, 
              parallel->comm, &parallel->requests[2]);

    MPI_Irecv(&temperature->data[0], 1, parallel->rowtype, parallel->nup, ROW_TAG_DOWN, 
              parallel->comm, &parallel->requests[3]);

    // (left <-> right)
    // Communication 3: Send data to the left neighbor and receive from the right neighbor
    // This exchanges the ghost cells in the leftmost column of the local temperature field
    MPI_Isend(&temperature->data[1], 1, parallel->columntype, parallel->nleft, COLUMN_TAG_LEFT,
              parallel->comm, &parallel->requests[4]);

    MPI_Irecv(&temperature->data[width-1], 1, parallel->columntype, parallel->nright, COLUMN_TAG_LEFT, 
              parallel->comm, &parallel->requests[5]);

    
    // (right <-> left)
    // Communication 4: Send data to the right neighbor and receive from the left neighbor
    // This exchanges the ghost cells in the rightmost column of the local temperature field
    MPI_Isend(&temperature->data[width-2], 1, parallel->columntype, parallel->nright, COLUMN_TAG_RIGHT,
              parallel->comm,  &parallel->requests[6]);
    
    MPI_Irecv(&temperature->data[0], 1, parallel->columntype, parallel->nleft, COLUMN_TAG_RIGHT, 
            parallel->comm, &parallel->requests[7]);
}

void complete_halo_exchange(ParallelData *parallel) {
    // Wait for the completion of non-blocking communication requests related to halo exchange
    MPI_Waitall(8, parallel->requests, MPI_STATUSES_IGNORE);
}

void update_interior_temperature(Field *curr, Field *prev, double a, double dt) {

    // This function should update the interior temperature field based on the five-point stencil.

    // Indices for center, up, down, left, right
    // These indices are used for accessing neighboring grid points during the update.
    int i, j;
    int ic, iu, id, il, ir;
    double dx2, dy2;

    dx2 = prev->dx * prev->dx;
    dy2 = prev->dy * prev->dy;


    // Width of the grid (number of columns)
    // The width is used to calculate the indices and navigate through the temperature field.
    int width = curr->ny + 2;

    // Parameters for the update
    // a: Thermal diffusivity
    // dt: Time step size

    // Determine the temperature field at the next time step
    // As fixed boundary conditions are applied, the outermost grid points are not updated.

    // Loop over the interior grid points for the update
    for (i = 2; i < prev->nx; i++) {
        for (j = 2; j < prev->ny; j++) {
            // update indices using idx
            ic = idx(i, j, width);
            id = idx(i + 1, j, width);    
            iu = idx(i - 1, j, width);
            ir = idx(i, j + 1, width); 
            il = idx(i, j - 1, width);

            // Update the temperature using the five-point stencil
            curr->data[ic] = prev->data[ic] + a*dt/dy2*(prev->data[id] -2 * prev->data[ic] + prev->data[iu])
                                            + a*dt/dx2*(prev->data[ir] -2 * prev->data[ic] + prev->data[il]);
        }
        
    }  
}

void update_boundary_temperature(Field *curr, Field *prev, double a, double dt) {
  int i, j;
  int ic, iu, id, il, ir; // Indices for center, up, down, left, right
  int width;
  width = curr->ny + 2;
  double dx2, dy2;

  // Determine the temperature field at the next time step. As fixed boundary conditions are applied, the outermost grid points are not updated.
  dx2 = prev->dx * prev->dx; 
  dy2 = prev->dy * prev->dy;

  // Update the left and right borders 
  i = 1;
  for (j = 1; j < curr->ny + 1; j++) {
      ic = idx(i, j, width);
      id = idx(i + 1, j, width);    
      iu = idx(i - 1, j, width);
      ir = idx(i, j + 1, width); 
      il = idx(i, j - 1, width);

      // Apply the five-point stencil to update the temperature at the left and right borders.
      curr->data[ic] = prev->data[ic] + a*dt/dy2*(prev->data[id] -2 * prev->data[ic] + prev->data[iu])
                                      + a*dt/dx2*(prev->data[ir] -2 * prev->data[ic] + prev->data[il]);
  }

  i = curr->nx;
  for (j = 1; j < curr->ny + 1; j++) {
      // Update indicies using idx
      ic = idx(i, j, width);
      id = idx(i + 1, j, width);    
      iu = idx(i - 1, j, width);
      ir = idx(i, j + 1, width); 
      il = idx(i, j - 1, width);

      // Apply the five-point stencil to update the temperature at the left and right borders.
      curr->data[ic] = prev->data[ic] + a*dt/dy2*(prev->data[id] -2 * prev->data[ic] + prev->data[iu])
                                      + a*dt/dx2*(prev->data[ir] -2 * prev->data[ic] + prev->data[il]);
  }

  // Update the upper and lower borders
  j = 1;
  for (i = 1; i < curr->nx + 1; i++) {
      // Update indicies using idx
      ic = idx(i, j, width);
      id = idx(i + 1, j, width);    
      iu = idx(i - 1, j, width);
      ir = idx(i, j + 1, width); 
      il = idx(i, j - 1, width);

      // Apply the five-point stencil to update the temperature at the upper and lower borders.
      curr->data[ic] = prev->data[ic] + a*dt/dy2*(prev->data[id] -2 * prev->data[ic] + prev->data[iu])
                                      + a*dt/dx2*(prev->data[ir] -2 * prev->data[ic] + prev->data[il]);
  }

  // Update the lower and upper borders
  j = curr->ny;
  for (i = 1; i < curr->nx + 1; i++) {
      // Update indicies using idx
      ic = idx(i, j, width);
      id = idx(i + 1, j, width);    
      iu = idx(i - 1, j, width);
      ir = idx(i, j + 1, width); 
      il = idx(i, j - 1, width);
      
      // Apply the five-point stencil to update the temperature at the upper and lower borders.
      curr->data[ic] = prev->data[ic] + a*dt/dy2*(prev->data[id] -2 * prev->data[ic] + prev->data[iu])
                                      + a*dt/dx2*(prev->data[ir] -2 * prev->data[ic] + prev->data[il]);
  }
}