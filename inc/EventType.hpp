#ifndef WEBSERV_EVENT_TYPE_H
#define WEBSERV_EVENT_TYPE_H

class EventType {
 public:
  // bits for event type
  static const int READ = 1;
  static const int WRITE = 2;

  static bool isRead(int event_type);
  static bool isWrite(int event_type);

 private:
  EventType();
  ~EventType();
  EventType(const EventType& other);
  EventType& operator=(const EventType& other);
};

#endif
