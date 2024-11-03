#include "Event.hpp"
#include "Math.hpp"

std::map<std::string, std::map<EventIdentifer, EventListener>>
    Event::eventListeners;

void Event::fireEvent(std::string eventName) {
  for (auto &[id, eventListener] : eventListeners[eventName]) {
    eventListener();
  }
}

EventIdentifer Event::addEventListener(std::string eventName,
                                       EventListener function) {
  EventIdentifer id = randFloat(0, 1000000);
  eventListeners[eventName][id] = function;
  return id;
}

void Event::removeEventListener(EventIdentifer id) {
  for (auto &[eventName, eventListener] : eventListeners) {
    for (auto &[eventId, eventListener] : eventListener) {
      if (eventId == id) {
        eventListeners[eventName].erase(eventId);
        return;
      }
    }
  }

  throw std::runtime_error("Event Listener identifer could not be found");
}
