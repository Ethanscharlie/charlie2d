#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>

using EventListener = std::function<void()>;

class Event {
public:
  static void fireEvent(std::string eventName);
  static void addEventListener(std::string eventName, EventListener function);

private:
  static std::map<std::string, std::vector<EventListener>> eventListeners;
};
