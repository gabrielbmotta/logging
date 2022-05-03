#include <iostream>
#include "logger.hpp"
#include <fstream>
#include <chrono>
#include <tuple>
#include <vector>

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

std::pair<int,int> speedTest(){
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

  return {std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count(),
          std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count()};
};


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

  int numTrials = 200;

  std::vector<std::pair<int,int>> results;
  results.reserve(numTrials);

  for(int i = 0; i < numTrials; ++i){
    results.emplace_back(speedTest());
  }

  int avgCout = 0, avgLog = 0;

  for(auto& result : results){
    avgCout += result.first;
    avgLog += result.second;
  }

  avgCout /= numTrials;
  avgLog /= numTrials;


  std::cout << "Avg cout time: " << avgCout << "\n";
  std::cout << "Avg log time: " << avgLog << "\n";

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
