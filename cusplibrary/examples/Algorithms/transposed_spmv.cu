/*
 *  Copyright 2014-2015 The University of Queensland
 *  http://www.uq.edu.au
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <cusp/multiply.h>
#include <cusp/cds_matrix.h>
#include <cusp/print.h>

typedef cusp::device_memory MemorySpace;
typedef double ValueType;

int main(void)
{
    // initialize matrix
    // args are: num_rows, num_entries, num_diagonals, blocksize
    cusp::cds_matrix<int, ValueType, MemorySpace> A(8,28,3,2);
    A.diagonal_offsets[0] = -3;
    A.diagonal_offsets[1] = 0;
    A.diagonal_offsets[2] = 2;

    A.values(6,0) = 11;
    A.values(7,0) = 17;
    A.values(6,1) = 23;
    A.values(7,1) = 27;

    A.values(0,2) =  1;
    A.values(1,2) =  3;
    A.values(0,3) =  5;
    A.values(1,3) =  7;
    A.values(2,2) =  8;
    A.values(3,2) = 29;
    A.values(2,3) = 31;
    A.values(3,3) = 37;
    A.values(4,2) = 41;
    A.values(5,2) = -1;
    A.values(4,3) = -3;
    A.values(5,3) = -5;
    A.values(6,2) = -7;
    A.values(7,2) =-11;
    A.values(6,3) =-13;
    A.values(7,3) =-17;

    A.values(0,4) = 21;
    A.values(1,4) = 12;
    A.values(0,5) = 14;
    A.values(1,5) = -3;
    A.values(2,4) = -7;
    A.values(3,4) = 22;
    A.values(2,5) =-31;
    A.values(3,5) =  5;

    // initialize input vector
    cusp::array1d<ValueType, MemorySpace> x(8);
    x[0] = 1;
    x[1] = 2;
    x[2] = 3;
    x[3] = 5;
    x[4] =-1;
    x[5] =-3;
    x[6] =-7;
    x[7] =-5;

    // allocate output vector
    cusp::array1d<ValueType, MemorySpace> y(8);

    // compute y = A * x
    cusp::multiply(A, x, y);

    // print y
    cusp::print(y);

    cusp::transposed_multiply(A, y, x);

    // print x
    cusp::print(x);

    return 0;
}

