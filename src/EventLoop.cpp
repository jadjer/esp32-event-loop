//
// Created by jadjer on 03.02.25.
//

#include "event/EventLoop.hpp"

#include <memory>

auto const EVENT_BASE = "EVENT_BASE";

namespace event {

EventLoop::EventLoop() {
  esp_event_loop_args_t loopArgs = {
      .queue_size = 100,
      .task_name = "event_loop_task",
      .task_priority = 1,
      .task_stack_size = 4096,
      .task_core_id = tskNO_AFFINITY,
  };
  esp_event_loop_create(&loopArgs, &m_eventLoop);
}

EventLoop::~EventLoop() {
  esp_event_loop_delete(m_eventLoop);
}

void EventLoop::subscribe(EventType const eventType, EventHandler const &handler) {
  m_handlers[eventType].push_back(handler);

  esp_event_handler_register_with(m_eventLoop, EVENT_BASE, eventType, eventHandler, this);
}

void EventLoop::post(EventType const eventType) {
  post(eventType, nullptr, 0);
}

void EventLoop::post(EventType const eventType, void const *const data, std::size_t const dataSize) {
  esp_event_post_to(m_eventLoop, EVENT_BASE, eventType, data, dataSize, portMAX_DELAY);
}

void EventLoop::post(EventType const eventType, std::any const &value) {
  auto const data = std::make_shared<std::any>(value);
  auto const dataSize = sizeof(data);

  post(eventType, data.get(), dataSize);
}

void EventLoop::eventHandler(void *const handlerArgs, esp_event_base_t const eventBase, EventType const eventType, void *const eventData) {
  auto const self = reinterpret_cast<EventLoop *>(handlerArgs);

  if (auto const it = self->m_handlers.find(eventType); it == self->m_handlers.end()) {
    return;
  }

  auto const data = reinterpret_cast<std::shared_ptr<std::any> *>(eventData);

  for (auto const &handler : self->m_handlers[eventType]) {
    handler(**data);
  }
}

}// namespace event
