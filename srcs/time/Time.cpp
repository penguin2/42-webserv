#include "Time.hpp"

Time::Time() : time_sec_(0), time_nsec_(0) {}

Time::Time(const std::time_t& time_sec, const long& time_nsec)
    : time_sec_(time_sec), time_nsec_(time_nsec) {}

Time::Time(const Time& other)
    : time_sec_(other.time_sec_), time_nsec_(other.time_nsec_) {}

Time& Time::operator=(const Time& other) {
  if (this != &other) {
    time_sec_ = other.time_sec_;
    time_nsec_ = other.time_nsec_;
  }
  return *this;
}

Time::~Time() {}

std::time_t Time::getTimeSec() const { return time_sec_; }

long Time::getTimeNsec() const { return time_nsec_; }

Time Time::operator+(const Time& other) const {
  if (time_nsec_ + other.time_nsec_ >= Time::kOneSecInNano)
    return Time(time_sec_ + other.time_sec_ + 1,
                time_nsec_ + other.time_nsec_ - Time::kOneSecInNano);

  return Time(time_sec_ + other.time_sec_, time_nsec_ + other.time_nsec_);
}

bool Time::operator>(const Time& other) const {
  if (time_sec_ == other.time_sec_) return time_nsec_ > other.time_nsec_;
  return time_sec_ > other.time_sec_;
}

bool Time::operator<(const Time& other) const {
  if (time_sec_ == other.time_sec_) return time_nsec_ < other.time_nsec_;
  return time_sec_ < other.time_sec_;
}

bool Time::operator>=(const Time& other) const {
  return *this > other || *this == other;
}

bool Time::operator<=(const Time& other) const {
  return *this < other || *this == other;
}

bool Time::operator==(const Time& other) const {
  return (time_sec_ == other.time_sec_) && (time_nsec_ == other.time_nsec_);
}

bool Time::operator!=(const Time& other) const { return !(*this == other); }

Time Time::getCurrentClockTime() {
  struct timespec tmp;
  const int status = clock_gettime(CLOCK_MONOTONIC, &tmp);
  if (status < 0) return Time();
  return Time(tmp.tv_sec, tmp.tv_nsec);
}

std::ostream& operator<<(std::ostream& os, const Time& time) {
  return os << time.getTimeSec() << "." << time.getTimeNsec();
}
