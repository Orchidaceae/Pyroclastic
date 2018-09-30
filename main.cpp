#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <iostream>

int main(int argc, char const *argv[]) {
  if (!glfwInit()) {
    std::cerr << "GLFW Init failed." << std::endl;
  }

  uint32_t count;
  const char** extensions = glfwGetRequiredInstanceExtensions(&count);
  std::cout << *extensions << std::endl;

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Pyroclastic Engine", nullptr, nullptr);

  VkApplicationInfo app_info = {};
  app_info.apiVersion = 1;
  app_info.pApplicationName = "Pyroclastic";
  app_info.pEngineName = "Pyroclastic Engine";

  VkInstanceCreateInfo ici = {};
  ici.enabledExtensionCount = count;
  ici.ppEnabledExtensionNames = extensions;
  ici.pApplicationInfo = &app_info;

  VkInstance instance;
  VkResult result = vkCreateInstance(&ici, nullptr, &instance);
  if (result == VK_SUCCESS) {
    std::cout << "Vulkan initialized successfully" << std::endl;
  }

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  return 0;
}
