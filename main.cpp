#include <iostream>
#include "logger.hpp"

void test(){
  DEBUG << "This is a debug level output\n";
  MESSAGE << "This is a message level output\n";
  IMPORTANT <<  "This is an important level output\n";
  WARNING << "This is a warning level output\n";
  ERROR << "This is an error level output\n";
  CRITICAL << "This is a critical level output\n";
}

int main()
{
  LogManager::debugInfo = false;
  LogManager::get()->logSinks.push_back(new ConsoleOut);
  std::cout << "1:\n";
  test();

  for(auto* item : LogManager::get()->logSinks){
    item->setOutputLevel(LogLevel::Important);
  }

  std::cout << "2:\n";
  test();


  for(auto* item : LogManager::get()->logSinks){
    item->setOutputLevel(LogLevel::Debug);
  }

  std::cout << "3:\n";
  test();
  return 0;
}
