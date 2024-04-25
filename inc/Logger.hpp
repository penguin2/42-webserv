#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>

#define LOG(LEVEL, CATEGORY, VAR) \
  Logger::getInstance().log(Logger::LEVEL, CATEGORY, VAR)

#define LOG_VAR(LEVEL, VAR) \
  Logger::getInstance().log(Logger::LEVEL, #VAR ": ", VAR)

class Logger {
 public:
  typedef enum eLevel { ERROR = 0, WARN = 10, INFO = 20, DEBUG = 30 } Level;

  static Logger& getInstance();

  template <typename X>
  void log(Level level, const X& x);

  template <typename X, typename Y>
  void log(Level level, const X& x, const Y& y);

  void setDebugMode(bool debug_mode);

 private:
  static const Level kDefaultLimitLevel_ = INFO;

  std::ostream& out_stream_;
  Level limit_level_;
  bool debug_mode_;

  Logger(std::ostream& out_stream);
  ~Logger();

  bool checkLevel(Level level);

  std::string levelToString(Level level);
  std::string getTimeStamp();

  Logger();
  Logger(const Logger&);
  Logger& operator=(const Logger&);
};

template <typename X>
void Logger::log(Level level, const X& x) {
  if (checkLevel(level) == false) return;
  out_stream_ << Logger::getTimeStamp() << " [" << Logger::levelToString(level)
              << "] " << x << std::endl;
}

template <typename X, typename Y>
void Logger::log(Level level, const X& x, const Y& y) {
  if (checkLevel(level) == false) return;
  out_stream_ << Logger::getTimeStamp() << " [" << Logger::levelToString(level)
              << "] " << x << y << std::endl;
}

#endif
