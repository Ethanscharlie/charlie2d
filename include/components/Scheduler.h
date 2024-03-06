#pragma once

#include "Component.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include <cstdio>
#include <functional>
#include <map>
#include <string>
#include <vector>

/**
 * \brief Based struct for schedules used in the Scheduler
 */
struct Schedule {
  Schedule() = default;
  Schedule(std::function<void()> _function, Uint32 _millisecondWaitTime,
           bool _removeTimerOnFinish, int _removeAfterTimes)
      : function(_function), millisecondWaitTime(_millisecondWaitTime),
        timerStartTime(SDL_GetTicks()),
        removeTimerOnFinish(_removeTimerOnFinish),
        timesCounter(_removeAfterTimes) {}
  std::function<void()> function = []() {};
  Uint32 millisecondWaitTime = 0;
  Uint32 timerStartTime = 0;
  bool removeTimerOnFinish = false;
  int timesCounter = 0;
  std::function<void()> onFinish = []() {};
};

/**
 * \brief Made for background timers, for example a bullet that fires every 2
 * seconds, as well as cooldown timers with using removeTimerOnFinish
 */
class Scheduler : public Component {
public:
  void start() override {}

  void update(float deltaTime) override {
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
  void addSchedule(
      const std::string &name, Uint32 millisecondWaitTime,
      std::function<void()> function, std::function<void()> onFinish = []() {},
      bool removeTimerOnFinish = false, int removeAfterTimes = 0) {
    Schedule schedule = {function, millisecondWaitTime, removeTimerOnFinish,
                         removeAfterTimes};
    schedule.onFinish = onFinish;
    schedules[name] = schedule;
  }


  void removeSchedule(const std::string &name) { schedules.erase(name); }

  std::map<std::string, Schedule> schedules;
};
