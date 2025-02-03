//
// Created by jadjer on 03.02.25.
//

#pragma once

#include <any>
#include <cstdint>
#include <esp_event.h>
#include <functional>
#include <unordered_map>
#include <vector>

/**
 * @namespace event
 */
namespace event {

using EventType = std::int32_t;
using EventHandler = std::function<void(std::any const &)>;
using EventHandlers = std::vector<EventHandler>;

/**
 * @class EventLoop
 */
class EventLoop {
public:
  EventLoop();
  ~EventLoop();

public:
  /**
   * Subscribe on event
   * @param eventType Event type
   * @param handler Event handler
   */
  void subscribe(EventType eventType, EventHandler const &handler);

public:
  /**
   * Send event (without any data)
   * @param eventType Event type
   */
  void post(EventType eventType);
  /**
   * Send event (with raw data)
   * @param eventType Event type
   * @param data Pointer on data
   * @param dataSize Data size
   */
  void post(EventType eventType, void const *data, std::size_t dataSize);

  /**
   * Send event (with any data)
   * @param eventType Event type
   * @param value Data value
   */
  void post(EventType eventType, std::any const &value);

private:
  static void eventHandler(void *handlerArgs, esp_event_base_t eventBase, EventType eventType, void *eventData);

private:
  esp_event_loop_handle_t m_eventLoop = nullptr;

private:
  std::unordered_map<EventType, EventHandlers> m_handlers;
};

}// namespace event
