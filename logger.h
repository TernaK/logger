#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <thread>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>

class Logger {
public:
  struct Header {
    std::unordered_map<int,std::string> index_keys;
    std::string title;

    Header() = default;
    Header(std::string title, std::vector<std::string>&& header);
  };

  struct Row {
    std::unordered_map<std::string,std::string> key_values;
    std::string title;

    Row() = default;
    Row(std::string title, std::unordered_map<std::string,std::string>&& key_values);
  };

  Logger();
  ~Logger();
  void push_row(Logger::Row& row);
  void add_header(Logger::Header& header);
  void write();
  std::string to_string(Row& row);
  std::string to_string(Header& row);

private:
  std::unordered_map<std::string,Logger::Header> headers;
  std::queue<Logger::Row> rows;
  std::ofstream file;
};

struct Loggable {
  Logger::Header header;

  Loggable() = default;
  virtual void log() { };
};
