#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <thread>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>
#include <chrono>

namespace logger_namespace {
  class Logger {
    static constexpr bool DEFAULT_FILE_MODE = false;
    static constexpr bool DEFAULT_ASYNC_MODE = false;
    static constexpr bool DEFAULT_PERIOD_MS = 100;

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
    void push_row(Logger::Row&& row);
    void log(Logger::Row&& row);
    void add_header(Logger::Header& header);
    std::string to_string(Row& row);
    std::string to_string(Header& row);

    static Logger* logger();
    ~Logger();

  private:
    Logger(std::string log_name = "log.csv",
           bool file_mode = DEFAULT_FILE_MODE,
           bool async = DEFAULT_ASYNC_MODE,
           int period_ms = DEFAULT_PERIOD_MS);
    void write(std::queue<Logger::Row>& rows);
    static std::unique_ptr<Logger> instance;
    std::unordered_map<std::string,Logger::Header> headers;
    std::queue<Logger::Row> rows;
    std::ofstream file;
    bool file_mode;
    std::thread write_thread;
    std::mutex write_mutex;
    std::atomic_bool run_flag {false};
  };

  class Loggable {
    Logger::Header header;

  public:
    Loggable() = default;
    Loggable(Logger::Header&& header);
    void log(Logger::Row&& row, bool immediately = false);
  };
}
