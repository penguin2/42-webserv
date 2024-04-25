#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <sstream>
#include <vector>

#define LOG(LEVEL, CATEGORY, VAR) \
  Logger::getInstance().log(Logger::LEVEL, CATEGORY, VAR)

#define LOG_VAR(LEVEL, VAR) \
  Logger::getInstance().log(Logger::LEVEL, #VAR ": ", VAR)

#define LOG_DEBUG(VAR) LOG_VAR(Logger::DEBUG, VAR)

class Logger {
 public:
  typedef enum eLevel { ERROR = 0, WARN = 10, INFO = 20, DEBUG = 30 } Level;

  static Logger& getInstance();

  template <typename X>
  void log(Level level, const X& x);
  template <typename X, typename Y>
  void log(Level level, const X& x, const Y& y);

  void init();
  void addOutStream(std::ostream& out_stream);

  void setDebugMode(bool debug_mode);
  void setLimitLevel(Level limit_level);

 private:
  static const Level kDefaultLimitLevel_ = INFO;

  std::vector<std::ostream*> out_streams_;
  Level limit_level_;
  bool debug_mode_;

  Logger();
  ~Logger();

  bool checkLevel(Level level) const;

  std::string levelToString(Level level) const;
  std::string getTimeStamp() const;
  std::string getLogPrefix(Level level) const;

  template <typename X>
  std::string writeLogMessage(Level level, const X& x);
  template <typename X, typename Y>
  std::string writeLogMessage(Level level, const X& x, const Y& y);

  Logger(const Logger&);
  Logger& operator=(const Logger&);
};

template <typename X>
std::string Logger::writeLogMessage(Level level, const X& x) {
  std::stringstream ss;
  ss << getLogPrefix(level) << x << '\n';
  return ss.str();
}

template <typename X, typename Y>
std::string Logger::writeLogMessage(Level level, const X& x, const Y& y) {
  std::stringstream ss;
  ss << getLogPrefix(level) << x << y << '\n';
  return ss.str();
}

template <typename X>
void Logger::log(Level level, const X& x) {
  if (checkLevel(level) == false) return;
  const std::string& log_msg = Logger::writeLogMessage(level, x);
  for (std::vector<std::ostream*>::iterator it = out_streams_.begin();
       it != out_streams_.end(); ++it)
    **it << log_msg << std::flush;
}

template <typename X, typename Y>
void Logger::log(Level level, const X& x, const Y& y) {
  if (checkLevel(level) == false) return;
  const std::string& log_msg = Logger::writeLogMessage(level, x, y);
  for (std::vector<std::ostream*>::iterator it = out_streams_.begin();
       it != out_streams_.end(); ++it)
    **it << log_msg << std::flush;
}

#endif
