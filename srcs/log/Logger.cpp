#include "Logger.hpp"

#include <ctime>
#include <fstream>

Logger& Logger::getInstance() {
  static Logger instance;
  return instance;
}

void Logger::init() { addOutStream(std::cerr); }

void Logger::addOutStream(std::ostream& out_stream) {
  out_streams_.push_back(&out_stream);
}

void Logger::setDebugMode(bool debug_mode) { debug_mode_ = debug_mode; }

void Logger::setLimitLevel(Level limit_level) { limit_level_ = limit_level; }

Logger::Logger() : limit_level_(kDefaultLimitLevel_), debug_mode_(false) {}

Logger::~Logger() {}

bool Logger::checkLevel(Logger::Level level) const {
  if (debug_mode_) return level == Logger::DEBUG;
  return level <= Logger::limit_level_;
}

std::string Logger::levelToString(Logger::Level level) const {
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

std::string Logger::getTimeStamp() const {
  const std::time_t now = std::time(NULL);
  char buf[128];
  std::strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", std::localtime(&now));
  return std::string(buf);
}

std::string Logger::getLogPrefix(Level level) const {
  return getTimeStamp() + " [" + levelToString(level) + "] ";
}
