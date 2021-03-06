/**
 * \file generateA.inl
 * \brief Generates matrix A, which is the left-hand side matrix in the step 
 *        where the intermediate velocity flux is solved. It consists of the 
 *        time-stepping term and the implicit diffusion terms.
 */


#include <solvers/kernels/generateA.h>


/**
 * \brief Assembles the matrix from implicit terms of momentum equation.
 * 
 * Generates the matrix A which solves the implicit velocity flux.
 * Calculated from the matrices M (which contains the term from the time derivative)
 * and the matrix L, which is the implicit diffusion matrix.
 *
 * \param alpha implicit coefficient of the diffusive scheme
 */
template <>
void NavierStokesSolver<device_memory>::generateA(real alpha)
{
	int nx = domInfo->nx,
	    ny = domInfo->ny;

	int ASize = 5*( (nx-1)*ny + nx*(ny-1) ) - 4*(nx+ny) + 4,
	    numUV = (nx-1)*ny + nx*(ny-1);

	A.resize(numUV, numUV, ASize);

	int *LRows = thrust::raw_pointer_cast(&(L.row_indices[0])),
	    *LCols = thrust::raw_pointer_cast(&(L.column_indices[0])),
	    *ARows = thrust::raw_pointer_cast(&(A.row_indices[0])),
	    *ACols = thrust::raw_pointer_cast(&(A.column_indices[0]));
	     

	real *MVals = thrust::raw_pointer_cast(&(M.values[0])),
	     *LVals = thrust::raw_pointer_cast(&(L.values[0])),
	     *AVals = thrust::raw_pointer_cast(&(A.values[0]));

	const int blockSize = 256,
	          gridSize  = 1000;
	dim3 dimGrid(gridSize, 1);
	dim3 dimBlock(blockSize, 1);
	
	kernels::generateA <<<dimGrid, dimBlock>>> (ARows, ACols, AVals, MVals, LRows, LCols, LVals, ASize, alpha);
} // generateA
