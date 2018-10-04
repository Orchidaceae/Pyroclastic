#pragma once

#include <vulkan/vulkan.h>

#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
      void* userData) {
  std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}