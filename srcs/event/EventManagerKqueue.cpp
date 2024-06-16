#if defined(__MACH__)
#include "EventManagerKqueue.hpp"

#include <sys/event.h>
#include <unistd.h>

#include "Logger.hpp"
#include "utils/SysUtils.hpp"

const struct timespec EventManagerKqueue::kWaitTimeout = {
    EventManagerKqueue::kWaitTimeoutSec,
    EventManagerKqueue::kWaitTimeoutNanoSec};

struct kevent
    EventManagerKqueue::ready_list_[EventManagerKqueue::kEvlistMaxSize];

EventManagerKqueue::EventManagerKqueue(
    const std::map<int, ASocket*>& listen_sockets) {
  kq_fd_ = kqueue();
  if (kq_fd_ < 0 || sys_utils::addCloseOnExecFlag(kq_fd_) < 0) {
    LOG(ERROR, "kqueue(create): ", std::strerror(errno));
    std::exit(EXIT_FAILURE);
  }
  for (std::map<int, ASocket*>::const_iterator it = listen_sockets.begin();
       it != listen_sockets.end(); ++it) {
    int fd = it->first;
    ASocket* socket = it->second;
    if (insert(fd, socket, EventType::READ) < 0) {
      LOG(ERROR, "EventManagerKqueue(constructor): ",
          "failed on initializing sockets");
      std::exit(EXIT_FAILURE);
    }
  }
}

EventManagerKqueue::~EventManagerKqueue() { close(kq_fd_); }

int EventManagerKqueue::wait(std::vector<ASocket*>& event_sockets,
                             std::vector<ASocket*>& closing_sockets) {
  const int ready_list_size = kevent(
      kq_fd_, NULL, 0, EventManagerKqueue::ready_list_,
      EventManagerKqueue::kEvlistMaxSize, &EventManagerKqueue::kWaitTimeout);
  if (ready_list_size < 0) {
    LOG(ERROR, "kevent(wait): ", std::strerror(errno));
    return -1;
  }
  if (ready_list_size == 0) return 0;

  for (int i = 0; i < ready_list_size; ++i) {
    ASocket* current_socket = reinterpret_cast<ASocket*>(ready_list_[i].udata);

    if (ready_list_[i].flags & EV_ERROR)
      closing_sockets.push_back(current_socket);
    else {
      current_socket->setEventType(
          EventManagerKqueue::makeEventType(ready_list_[i].filter));
      event_sockets.push_back(current_socket);
    }
  }
  return ready_list_size;
}

int EventManagerKqueue::insert(int fd, ASocket* socket, int event_type) {
  struct kevent ev;
  EV_SET(&ev, fd, EventManagerKqueue::makeKqueueFilter(event_type), EV_ADD, 0,
         0, reinterpret_cast<void*>(socket));
  if (kevent(kq_fd_, &ev, 1, NULL, 0, NULL) < 0) {
    LOG(WARN, "kevent(insert): ", std::strerror(errno));
    return -1;
  }
  return 0;
}

int EventManagerKqueue::modify(int fd, ASocket* socket, int new_event_type) {
  const int other_event_type =
      EventType::isRead(new_event_type) ? EventType::WRITE : EventType::READ;
  if (erase(fd, socket, other_event_type) < 0 ||
      insert(fd, socket, new_event_type) < 0) {
    LOG(WARN, "kevent(modify): ", std::strerror(errno));
    return -1;
  }
  return 0;
}

int EventManagerKqueue::erase(int fd, ASocket* socket, int event_type) {
  struct kevent ev;
  EV_SET(&ev, fd, EventManagerKqueue::makeKqueueFilter(event_type), EV_DELETE,
         0, 0, reinterpret_cast<void*>(socket));
  if (kevent(kq_fd_, &ev, 1, NULL, 0, NULL) < 0) {
    return -1;
  }
  return 0;
}

int EventManagerKqueue::makeKqueueFilter(int event_type) {
  if (EventType::isRead(event_type)) return EVFILT_READ;
  if (EventType::isWrite(event_type)) return EVFILT_WRITE;
  return 0;
}

int EventManagerKqueue::makeEventType(int kqueue_filter) {
  if (kqueue_filter == EVFILT_READ) return EventType::READ;
  if (kqueue_filter == EVFILT_WRITE) return EventType::WRITE;
  return 0;
}

#endif
