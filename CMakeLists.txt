cmake_minimum_required(VERSION 3.24)
project(vulkan_sample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

find_package(Vulkan REQUIRED)

FetchContent_Declare(
  volk
  GIT_REPOSITORY https://github.com/zeux/volk.git
  GIT_TAG vulkan-sdk-1.3.283.0
)

FetchContent_Declare(
  VulkanMemoryAllocator
  GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
  GIT_TAG v3.0.1
)

FetchContent_MakeAvailable(volk VulkanMemoryAllocator)

add_executable(vulkan_sample
  src/main.cpp
  src/vma_impl.cpp
)

target_compile_definitions(vulkan_sample PRIVATE
  VK_NO_PROTOTYPES
  VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
)

target_include_directories(vulkan_sample PRIVATE
  ${vulkanmemoryallocator_SOURCE_DIR}/include
)

if(TARGET volk::volk)
  target_link_libraries(vulkan_sample PRIVATE volk::volk)
else()
  target_link_libraries(vulkan_sample PRIVATE volk)
endif()

target_link_libraries(vulkan_sample PRIVATE Vulkan::Vulkan)
