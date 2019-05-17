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
    static constexpr int DEFAULT_MAX_BUFFER = 100;

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

    /// @brief return singleton
    static Logger* logger();

    /// @brief add a row the logger and optionally write immediately
    void log(Logger::Row&& row, bool immediately = false);

    /// @brief add a header to the log
    void add_header(Logger::Header& header);

    std::string to_string(Row& row);

    std::string to_string(Header& row);

    ~Logger();

  private:
    Logger(std::string log_name = "log.csv",
           bool file_mode = DEFAULT_FILE_MODE,
           int max_buffer = DEFAULT_MAX_BUFFER);

    /// @brief write out each row in the buffer then empty
    void write_rows_async();

    void write_rows(std::queue<Logger::Row>&& rows);

    void write_row(Logger::Row&& row);

    static std::unique_ptr<Logger> instance;  ///< singleton
    std::unordered_map<std::string,Logger::Header> headers; ///unique headers
    std::queue<Logger::Row> rows; ///< row buffer
    std::ofstream file; ///< for writing to file
    bool file_mode;     ///< if true write to file else write to stdout
    int max_buffer;     ///< max length of the row buffer
    std::mutex write_mutex;   ///< thread safety
    std::thread write_thread; ///< used to asynchronously clear the row buffer
  };

  class Loggable {
    Logger::Header header;

  public:
    Loggable() = default;

    Loggable(Logger::Header&& header);

    void log(Logger::Row&& row, bool immediately = false);
  };
}
