//
// Created by jadjer on 03.02.25.
//

#pragma once

#include <any>
#include <cstdint>
#include <esp_event.h>
#include <functional>

namespace event {

using EventType = std::int32_t;
using EventGroup = esp_event_base_t;
using EventHandler = std::function<void(std::any const &)>;

auto const EVENT_GROUP_DEFAULT = "EVENT_BASE";

class EventLoop {
public:
  EventLoop();
  ~EventLoop();

public:
  void subscribe(EventType eventType, EventHandler handler, EventGroup eventGroup = EVENT_GROUP_DEFAULT);

public:
  void post(EventType eventType, std::any const &data = nullptr, EventGroup eventGroup = EVENT_GROUP_DEFAULT);

private:
  static void eventHandler(void *handlerArgs, esp_event_base_t eventBase, EventType eventType, void *eventData);

private:
  esp_event_loop_handle_t m_eventLoop = nullptr;

private:
  std::unordered_map<EventType, EventHandler> m_handlers;
};

}// namespace event
