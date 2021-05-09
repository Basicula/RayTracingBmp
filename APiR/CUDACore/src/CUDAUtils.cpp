#include <CUDACore/CUDAUtils.h>

#include <cuda_runtime.h>
#include <iostream>

void check_cuda(
  cudaError_t result,
  char const* const func,
  const char* const file,
  int const line) {
  if (result) {
    std::cerr << "CUDA error : " << static_cast<unsigned int>( result ) << " at "
      << file << ":" << line << " '" << func << std::endl;
    std::cerr << cudaGetErrorString(result) << std::endl;
    }
  }

void GPUInfo()
  {
  const int kb = 1024;
  const int mb = kb * kb;
  std::cout << "NBody.GPU" << std::endl << "=========" << std::endl << std::endl;

  std::cout << "CUDA version:   v" << CUDART_VERSION << std::endl;
  //std::cout << "Thrust version: v" << THRUST_MAJOR_VERSION << "." << THRUST_MINOR_VERSION << std::endl << std::endl;

  int devCount;
  cudaGetDeviceCount(&devCount);
  std::cout << "CUDA Devices: " << std::endl << std::endl;

  for (int i = 0; i < devCount; ++i)
    {
    cudaDeviceProp props;
    cudaGetDeviceProperties(&props, i);
    std::cout << i << ": " << props.name << ": " << props.major << "." << props.minor << std::endl;
    std::cout << "  Global memory:   " << props.totalGlobalMem / mb << "mb" << std::endl;
    std::cout << "  Shared memory:   " << props.sharedMemPerBlock / kb << "kb" << std::endl;
    std::cout << "  Constant memory: " << props.totalConstMem / kb << "kb" << std::endl;
    std::cout << "  Block registers: " << props.regsPerBlock << std::endl << std::endl;

    std::cout << "  Warp size:         " << props.warpSize << std::endl;
    std::cout << "  Threads per block: " << props.maxThreadsPerBlock << std::endl;
    std::cout << "  Max block dimensions: [ " << props.maxThreadsDim[0] << ", " << props.maxThreadsDim[1] << ", " << props.maxThreadsDim[2] << " ]" << std::endl;
    std::cout << "  Max grid dimensions:  [ " << props.maxGridSize[0] << ", " << props.maxGridSize[1] << ", " << props.maxGridSize[2] << " ]" << std::endl;
    std::cout << std::endl;
    }
  }