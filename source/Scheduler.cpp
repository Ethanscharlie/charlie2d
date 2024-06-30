#include "Scheduler.h"

void Scheduler::update(float deltaTime) {
  std::vector<std::string> toRemove;
  for (auto &[name, schedule] : schedules) {
    if (schedule.timerStartTime + schedule.millisecondWaitTime <
        SDL_GetTicks()) {
      schedule.function();
      schedule.timerStartTime = SDL_GetTicks();

      schedule.timesCounter--;
      if (schedule.timesCounter <= 0) {
        if (schedule.removeTimerOnFinish) {
          schedule.onFinish();
          toRemove.push_back(name);
        }
      }
    }
  }

  for (std::string name : toRemove) {
    removeSchedule(name);
  }
}

/**
 * \brief Adds a schedule to the map
 * \param name The name to go in the map
 * \param millisecondWaitTime How long before the function runs and this
 * repeats \param function Code to run \param removeTimerOnFinish Disables
 * looping, only goes once (Good for cooldown wait timers)
 */
void Scheduler::addSchedule(const std::string &name, Uint32 millisecondWaitTime,
                            std::function<void()> function,
                            bool removeTimerOnFinish, int removeAfterTimes) {
  Schedule schedule = {function, millisecondWaitTime, removeTimerOnFinish,
                       removeAfterTimes};
  schedules[name] = schedule;
}

void Scheduler::removeSchedule(const std::string &name) {
  schedules.erase(name);
}
