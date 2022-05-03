#include <iostream>
#include "logger.hpp"
#include <fstream>
#include <chrono>

void test(){
  DEBUG << "This is a debug level output\n";
  MESSAGE << "This is a message level output\n";
  IMPORTANT << "This is an important level output\n";
  WARNING << "This is a warning level output\n";
  ERROR << "This is an error level output\n";
  CRITICAL << "This is a critical level output\n";
}

void test2(){
  std::cout << "This is a debug level output\n";
  std::cout << "This is a message level output\n";
  std::cout << "This is an important level output\n";
  std::cout << "This is a warning level output\n";
  std::cout << "This is an error level output\n";
  std::cout << "This is a critical level output\n";
}



class MyCustomLoggerSink : public LogSink{
public:
  explicit MyCustomLoggerSink(const std::string& path):LogSink(),file(path){};
  std::ostream& printer() override{
    return file;
  }

private:
  std::ofstream file;
};

int main()
{

  LogManager::debugInfo = false;
  LogManager::get()->logSinks.push_back(new ConsoleOut);

  for(auto* item : LogManager::get()->logSinks){
    item->setOutputLevel(LogLevel::Debug);
  }

  auto start1 = std::chrono::steady_clock::now();

  for(int i = 0; i < 1000; ++i){
    test2();
  }
  auto end1 = std::chrono::steady_clock::now();

  auto start2 = std::chrono::steady_clock::now();

  for(int i = 0; i < 1000; ++i){
    test();
  }

  auto end2 = std::chrono::steady_clock::now();

  std::cout << "Ouput time with cout.\n";
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << "ms\n";

  std::cout << "Output time with logging.\n";
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << "ms\n";


  std::cout << "The test times vary considerably from run to run. Do not treat the results of one run as gospel.\n";

//
//  LogManager::debugInfo = false;
//  LogManager::get()->logSinks.push_back(new ConsoleOut);
//  std::cout << "1:\n";
//  test();
//
//  for(auto* item : LogManager::get()->logSinks){
//    item->setOutputLevel(LogLevel::Important);
//  }
//
//  std::cout << "2:\n";
//  test();
//
//  LogManager::get()->logSinks.emplace_back(new MyCustomLoggerSink("test.txt"));
//
//  for(auto* item : LogManager::get()->logSinks){
//    item->setOutputLevel(LogLevel::Debug);
//  }
//
//  std::cout << "3:\n";
//  test();

  return 0;
}
