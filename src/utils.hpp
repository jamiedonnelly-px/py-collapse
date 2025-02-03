#ifndef UTILS_H
#define UTILS_H

#include<omp.h>
#include<utility>
#include<vector>

#include "types.hpp"

double vertDistance(const RowVectorD& r1, const RowVectorD& r2){
    return (r1 - r2).transpose().dot((r1-r2));
};

// Using #pragma omp for negates these blocks
// std::vector<size_t> partitionSizes(int num_threads, size_t max){
//     int partition_size = (max / num_threads);
//     int remainder = max % num_threads;
//     std::vector<size_t> sizes(num_threads);
//     for (auto _tid=0; _tid<num_threads; _tid++){
//         sizes[_tid] = partition_size + (_tid < remainder ? 1 : 0);
//     }
//     return sizes;
// };

// std::pair<size_t, size_t> partitionIndices(int num_threads, int tid, size_t max){
//     std::pair<size_t, size_t> result;
//     // make static to ensure other threads dont access 
//     // un-initialised array and cause segfault.
//     static std::vector<size_t> sizes; 
//     #pragma omp single
//     {
//         sizes = partitionSizes(num_threads, max);
//     }
//     // all threads wait here; implicit barrier.
//     size_t start = 0;
//     for (int i=0; i<tid; i++){
//         start += sizes[i];
//     }
//     size_t end = start + sizes[tid];
//     return {start, end};
// };

#endif