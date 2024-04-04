#ifndef WEBSERV_TIME_H
#define WEBSERV_TIME_H

#include <ctime>
#include <ostream>

class Time {
 public:
  Time();
  Time(const std::time_t& time_sec, const long& time_nsec);
  Time(const Time& other);
  Time& operator=(const Time& other);
  ~Time();

  std::time_t getTimeSec() const;
  long getTimeNsec() const;

  Time operator+(const Time& other) const;

  bool operator>(const Time& other) const;
  bool operator<(const Time& other) const;
  bool operator>=(const Time& other) const;
  bool operator<=(const Time& other) const;
  bool operator==(const Time& other) const;
  bool operator!=(const Time& other) const;

  static Time getCurrentClockTime();

 private:
  std::time_t time_sec_;
  long time_nsec_;

  static const long kOneSecInNano = 1000000000;
};

std::ostream& operator<<(std::ostream& os, const Time& time);

#endif
