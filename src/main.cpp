#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include <volk.h>
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace {

uint32_t findGraphicsQueueFamily(vk::PhysicalDevice physicalDevice) {
  const auto queueFamilies = physicalDevice.getQueueFamilyProperties();
  for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
    if ((queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlags{}) {
      return i;
    }
  }
  throw std::runtime_error("No graphics queue family found.");
}

}  // namespace

int main() {
  try {
    // Initialize volk first so global Vulkan symbols become available.
    if (volkInitialize() != VK_SUCCESS) {
      throw std::runtime_error("volkInitialize failed.");
    }

    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

    vk::ApplicationInfo appInfo{
      "vulkan_sample",
      VK_MAKE_VERSION(0, 1, 0),
      "no_engine",
      VK_MAKE_VERSION(0, 1, 0),
      VK_API_VERSION_1_3,
    };

    vk::InstanceCreateInfo instanceInfo{};
    instanceInfo.setPApplicationInfo(&appInfo);

    vk::Instance instance = vk::createInstance(instanceInfo);

    // Load instance-level functions for both volk and Vulkan-Hpp dispatcher.
    volkLoadInstance(static_cast<VkInstance>(instance));
    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

    const auto physicalDevices = instance.enumeratePhysicalDevices();
    if (physicalDevices.empty()) {
      throw std::runtime_error("No Vulkan physical device available.");
    }
    vk::PhysicalDevice physicalDevice = physicalDevices.front();

    const uint32_t graphicsQueueFamily = findGraphicsQueueFamily(physicalDevice);
    const float queuePriority = 1.0f;

    vk::DeviceQueueCreateInfo queueInfo{};
    queueInfo.setQueueFamilyIndex(graphicsQueueFamily);
    queueInfo.setQueueCount(1);
    queueInfo.setPQueuePriorities(&queuePriority);

    vk::DeviceCreateInfo deviceInfo{};
    deviceInfo.setQueueCreateInfoCount(1);
    deviceInfo.setPQueueCreateInfos(&queueInfo);

    vk::Device device = physicalDevice.createDevice(deviceInfo);

    // Load device-level functions after device creation.
    volkLoadDevice(static_cast<VkDevice>(device));
    VULKAN_HPP_DEFAULT_DISPATCHER.init(device);

    VmaVulkanFunctions vmaFunctions{};
    vmaFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vmaFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorInfo{};
    allocatorInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_3;
    allocatorInfo.physicalDevice = static_cast<VkPhysicalDevice>(physicalDevice);
    allocatorInfo.device = static_cast<VkDevice>(device);
    allocatorInfo.instance = static_cast<VkInstance>(instance);
    allocatorInfo.pVulkanFunctions = &vmaFunctions;

    VmaAllocator allocator = VK_NULL_HANDLE;
    if (vmaCreateAllocator(&allocatorInfo, &allocator) != VK_SUCCESS) {
      throw std::runtime_error("vmaCreateAllocator failed.");
    }

    // Create a tiny buffer to validate VMA path end-to-end.
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = 1024;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocationInfo{};
    allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;

    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    if (vmaCreateBuffer(allocator, &bufferInfo, &allocationInfo, &buffer, &allocation, nullptr) != VK_SUCCESS) {
      throw std::runtime_error("vmaCreateBuffer failed.");
    }

    vmaDestroyBuffer(allocator, buffer, allocation);
    vmaDestroyAllocator(allocator);

    device.waitIdle();
    device.destroy();
    instance.destroy();

    std::cout << "Sample finished successfully." << std::endl;
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }
}
