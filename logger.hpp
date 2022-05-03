//
// Created by Gabriel Motta on 5/2/22.
//

#ifndef LOGGING_LOGGER_HPP
#define LOGGING_LOGGER_HPP

#include <iostream>
#include <list>

//#ifdef DEBUG
//#define LOG if(LogManager::debugInfo) {std::cout<<"["<<__FILE__<<", ln. "<<__LINE__<<" : "<<__func__<<"] ";} std::cout

#define DEBUG       LogManager::get()->logLevel = LogLevel::Debug; \
                    *LogManager::get()<<"["<<__FILE__<<", ln. "<<__LINE__<<" : "<<__func__<<"] "; \
                    *LogManager::get()

#define MESSAGE     LogManager::get()->logLevel = LogLevel::Message; \
                    if(LogManager::debugInfo){                 \
                      *LogManager::get()<<"["<<__FILE__<<", ln. "<<__LINE__<<" : "<<__func__<<"] "; \
                    }                                          \
                    *LogManager::get()

#define IMPORTANT   LogManager::get()->logLevel = LogLevel::Important; \
                    *LogManager::get() << "!!! ";                    \
                    if(LogManager::debugInfo){                      \
                      *LogManager::get()<<"["<<__FILE__<<", ln. "<<__LINE__<<" : "<<__func__<<"] "; \
                    }                                               \
                    *LogManager::get()

#define WARNING     LogManager::get()->logLevel = LogLevel::Warning; \
                    *LogManager::get() << "[WARNING] ";         \
                    if(LogManager::debugInfo){                 \
                      *LogManager::get()<<"["<<__FILE__<<", ln. "<<__LINE__<<" : "<<__func__<<"] "; \
                    }                                          \
                    *LogManager::get()

#define ERROR       LogManager::get()->logLevel = LogLevel::Error; \
                    *LogManager::get() << "[ERROR] ";            \
                    if(LogManager::debugInfo){                 \
                      *LogManager::get()<<"["<<__FILE__<<", ln. "<<__LINE__<<" : "<<__func__<<"] "; \
                    }                                          \
                    *LogManager::get()

#define CRITICAL    LogManager::get()->logLevel = LogLevel::Critical; \
                    *LogManager::get() << "[CRITICAL] ";         \
                    if(LogManager::debugInfo){                 \
                      *LogManager::get()<<"["<<__FILE__<<", ln. "<<__LINE__<<" : "<<__func__<<"] "; \
                    }                                          \
                    *LogManager::get()

//#else
//#define LOG std::cout<<
//#endif

enum class LogLevel{
  Debug = 0,
  Message = 1,
  Important = 2,
  Warning = 3,
  Error = 4,
  Critical = 5
};

class LogSink{
public:
  LogSink():outputLevel(LogLevel::Message){}

  virtual std::ostream& printer() = 0;

  LogLevel getOutputLevel(){return outputLevel;};
  void setOutputLevel(LogLevel newLevel){outputLevel = newLevel;};

protected:
  LogLevel outputLevel;
};

class ConsoleOut : public LogSink
{
public:
  ConsoleOut() = default;
  std::ostream& printer() override {
    return std::cout;
  }
};

class LogManager{

public:
  LogManager(LogManager&) = delete;
  LogManager(LogManager&&) = delete;

  template<typename T>
  friend LogManager& operator<<(LogManager& logger, const T& val);
  static bool debugInfo;

  static LogManager* get();
  LogLevel logLevel;
  std::list<LogSink*> logSinks;
private:
  LogManager() = default;
  static LogManager* logManSingleton;
};

LogManager* LogManager::logManSingleton = nullptr;
LogManager* LogManager::get(){if(!logManSingleton){logManSingleton = new LogManager();} return logManSingleton;}


bool LogManager::debugInfo = true;

template<typename T>
LogManager& operator<<(LogManager& logger, const T& val) {

  for(auto* sink : logger.logSinks){
    if(sink->getOutputLevel() <= logger.logLevel){
      sink->printer() << val << std::flush;
    }
  }
  return logger;
}

#endif //LOGGING_LOGGER_HPP
