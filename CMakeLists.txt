cmake_minimum_required(VERSION 3.17)

set(SOURCES
        src/EventLoop.cpp
)

set(COMPONENTS
        esp_event
)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})
