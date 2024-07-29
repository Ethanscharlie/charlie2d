#include "Event.hpp"

std::map<std::string, std::vector<EventListener>> Event::eventListeners;

void Event::fireEvent(std::string eventName) {
  for (EventListener eventListener : eventListeners[eventName]) {
    eventListener();
  }
}

void Event::addEventListener(std::string eventName, EventListener function) {
  eventListeners[eventName].push_back(function);
}
