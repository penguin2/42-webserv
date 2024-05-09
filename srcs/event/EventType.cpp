#include "EventType.hpp"

bool EventType::isRead(int event_type) {
  return (event_type & EventType::READ) != 0;
}

bool EventType::isWrite(int event_type) {
  return (event_type & EventType::WRITE) != 0;
}
