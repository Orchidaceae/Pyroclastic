#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include "debug_vulkan.h"

#include <iostream>
#include <vector>
#include <assert.h>

const bool validation_enabled = true;

void VK_CHECK(VkResult result) { assert(result == VK_SUCCESS); }

int main(int argc, char const *argv[]) {
  if (!glfwInit()) { std::cerr << "GLFW Init failed." << std::endl; }

  uint32_t count;
  const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&count);

  std::vector<const char*> extensions = { *glfw_extensions };
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  for (int i = 0; i < count; i++) {
    extensions.push_back(glfw_extensions[i]);
  }
  for (int i = 0; i < extensions.size(); i++) {
    std::cout << extensions[i] << std::endl;
  }

  VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
  app_info.apiVersion = VK_API_VERSION_1_0;
  app_info.pApplicationName = "Pyroclastic";
  app_info.pEngineName = "Pyroclastic Engine";

  std::vector<const char*> validation_layers = { "VK_LAYER_LUNARG_standard_validation" };

  VkInstanceCreateInfo ici = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
  ici.enabledExtensionCount = extensions.size();
  ici.ppEnabledExtensionNames = extensions.data();
  ici.enabledLayerCount = validation_layers.size();
  ici.ppEnabledLayerNames = validation_layers.data();
  ici.pApplicationInfo = &app_info;

  /// Vulkan instance
  VkInstance instance;
  VK_CHECK(vkCreateInstance(&ici, nullptr, &instance));
  
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Pyroclastic Engine", nullptr, nullptr);
  VkSurfaceKHR surface = {};
  VK_CHECK(glfwCreateWindowSurface(instance, window, nullptr, &surface));

  /// Physical device
  uint32_t num_devices = 0;
  VK_CHECK(vkEnumeratePhysicalDevices(instance, &num_devices, nullptr));

  std::vector<VkPhysicalDevice> devices(num_devices);
  VK_CHECK(vkEnumeratePhysicalDevices(instance, &num_devices, devices.data()));

  VkPhysicalDevice physical_device = devices.front();

  VkPhysicalDeviceProperties device_properties;
  vkGetPhysicalDeviceProperties(physical_device, &device_properties);
  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
    device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
    std::cout << "Found a GPU" << std::endl;
  }

  /// Queue 
  uint32_t num_queue_family = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &num_queue_family, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(num_queue_family);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &num_queue_family, queue_families.data());

  int queue_index = -1;
  for (int i = 0; i < num_queue_family; i++) {
    const auto& queue_family = queue_families[i];
    if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      queue_index = i;
      std::cout << "Found a queue with graphics capabilities" << std::endl;
    }
  }

  /// Logical device setup
  VkDeviceQueueCreateInfo dqci = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
  dqci.queueFamilyIndex = queue_index;
  dqci.queueCount = 1;
  const float queue_prio = 1.0f;
  dqci.pQueuePriorities = &queue_prio;

  VkPhysicalDeviceFeatures device_features = {};

  std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  
  VkDeviceCreateInfo dci = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
  dci.pQueueCreateInfos = &dqci;
  dci.queueCreateInfoCount = 1;
  dci.pEnabledFeatures = &device_features;
  dci.enabledExtensionCount = device_extensions.size();
  dci.ppEnabledExtensionNames = device_extensions.data();

  if (validation_enabled) {
    dci.enabledLayerCount = validation_layers.size();
    dci.ppEnabledLayerNames = validation_layers.data();
  } else {
    dci.enabledLayerCount = 0;
  }

  VkDevice device;
  VK_CHECK(vkCreateDevice(physical_device, &dci, nullptr, &device));

  VkQueue graphics_queue;
  vkGetDeviceQueue(device, queue_index, 0, &graphics_queue);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  return 0;
}
