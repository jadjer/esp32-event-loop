//
// Created by jadjer on 03.02.25.
//

#include <utility>

#include "event/EventLoop.hpp"

namespace event {

EventLoop::EventLoop() {
  esp_event_loop_args_t loopArgs = {
      .queue_size = 100,
      .task_name = "event_loop_task",
      .task_priority = 1,
      .task_stack_size = 4096,
      .task_core_id = tskNO_AFFINITY};
  esp_event_loop_create(&loopArgs, &m_eventLoop);
}

EventLoop::~EventLoop() {
  esp_event_loop_delete(m_eventLoop);
}

void EventLoop::subscribe(EventType eventType, EventHandler handler, EventGroup eventGroup) {
  m_handlers[eventType] = std::move(handler);
  esp_event_handler_register_with(m_eventLoop, eventGroup, eventType, eventHandler, this);
}

void EventLoop::post(EventType eventType, std::any const &data, EventGroup eventGroup) {
  auto const eventData = data.has_value() ? &data : nullptr;
  std::size_t eventDataSize = data.has_value() ? sizeof(data) : 0;

  esp_event_post_to(m_eventLoop, eventGroup, eventType, eventData, eventDataSize, portMAX_DELAY);
}

void EventLoop::eventHandler(void *handlerArgs, esp_event_base_t eventBase, EventType eventType, void *eventData) {
  auto self = static_cast<EventLoop *>(handlerArgs);

  if (self->m_handlers.find(eventType) == self->m_handlers.end()) {
    return;
  }

  auto const data = static_cast<std::any const *>(eventData);

  self->m_handlers[eventType](data ? *data : std::any());
}

}// namespace event
