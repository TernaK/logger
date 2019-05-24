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
  /// @class Logger
  /// @brief Log data to a file.
  class Logger {
  public:
    struct Header {
      std::string title;
      std::unordered_map<int,std::string> index_keys;

      Header() = default;
      Header(std::string title, std::vector<std::string>&& header);
    };

    struct Row {
      std::string title;
      std::unordered_map<std::string,std::string> key_values;

      Row() = default;
      Row(std::string title, std::unordered_map<std::string,std::string>&& key_values);
    };

    /// @brief return singleton
    static Logger* logger();

    /// @brief log to specified file & buffer logged rows
    /// @param write out buffer if size > max_buffer_length
    void set_file_output(std::string file_name);

    /// @brief log a row
    void log(Logger::Row&& row);

    /// @brief log a header
    void add_header(Logger::Header& header);

    std::string to_string(Row& row);

    std::string to_string(Header& row);

    ~Logger();

  private:
    Logger() = default;

    void write_row(Logger::Row&& row);

    static std::unique_ptr<Logger> singleton; ///< singleton
    bool file_mode = false; ///< if true write to file else write to stdout
    std::unordered_map<std::string,Logger::Header> headers; ///unique headers
    std::ofstream file;     ///< for writing to file
    std::mutex write_mutex; ///< thread safety
  };

  class Loggable {
    Logger::Header header;

  public:
    Loggable() = default;

    Loggable(Logger::Header&& header);

    void log(Logger::Row&& row);
  };
}
