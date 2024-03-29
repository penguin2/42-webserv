#include "Logger.hpp"

#include <ctime>

Logger& Logger::getInstance() {
  static Logger instance(std::cerr);
  return instance;
}

void Logger::setDebugMode(bool debug_mode) { debug_mode_ = debug_mode; }

Logger::Logger(std::ostream& out_stream)
    : out_stream_(out_stream), limit_level_(Logger::INFO), debug_mode_(false) {}

Logger::~Logger() {}

bool Logger::checkLevel(Logger::Level level) {
  if (debug_mode_) return level == Logger::DEBUG;
  return level <= Logger::limit_level_;
}

std::string Logger::levelToString(Logger::Level level) {
  switch (level) {
    case Logger::ERROR:
      return "ERROR";
    case Logger::WARN:
      return "WARN";
    case Logger::INFO:
      return "INFO";
    case Logger::DEBUG:
      return "DEBUG";
  }
  return "UNSPECIFIED";
}

std::string Logger::getTimeStamp() {
  const std::time_t now = std::time(NULL);
  char buf[128];
  std::strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", std::localtime(&now));
  return std::string(buf);
}
