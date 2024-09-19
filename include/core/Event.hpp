#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>

using EventListener = std::function<void()>;
using EventIdentifer = int;

class Event {
public:
  static void fireEvent(std::string eventName);
  static EventIdentifer addEventListener(std::string eventName, EventListener function);
  static void removeEventListener(EventIdentifer id);

private:
  static std::map<std::string, std::map<EventIdentifer, EventListener>> eventListeners;
};
