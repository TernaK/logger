#include "logger.h"
using namespace logger_namespace;

//Logger::Row
//--------------------------------------------------
Logger::Row::Row(std::string title, std::unordered_map<std::string,std::string>&& key_values)
: title(title), key_values(std::move(key_values)) {

}

//Logger::Header
//--------------------------------------------------
Logger::Header::Header(std::string title, std::vector<std::string>&& header)
: title(title) {
  for(int i = 0; i < header.size(); i++)
    index_keys.insert({i,header.at(i)});
}

//Logger
//--------------------------------------------------
std::unique_ptr<Logger> Logger::instance = nullptr;

Logger::~Logger() {
  write_rows_async();

  if(write_thread.joinable())
    write_thread.join();

  if(file_mode) {
    if(file.is_open())
      file.close();
  }
}

Logger* Logger::logger() {
  if(!instance)
    instance = std::unique_ptr<Logger>(new Logger());
  return instance.get();
}

void Logger::set_file_output(std::string file_name, int max_buffer_length) {
  std::lock_guard<std::mutex> lg(write_mutex);
  file_mode = true;
  file = std::ofstream(file_name);
  max_buffer = max_buffer_length;
}

void Logger::log(Logger::Row&& row, bool immediately) {
  std::lock_guard<std::mutex> lg(write_mutex);
  if(immediately) {
    write_row(std::move(row));
  } else {
    rows_buffer.push(std::move(row));
    if(rows_buffer.size() > max_buffer) {
      write_rows_async();
    }
  }
}

void Logger::add_header(Logger::Header& header) {
  std::lock_guard<std::mutex> lg(write_mutex);
  if(headers.count(header.title) == 0) {
    headers.insert({header.title, header});
    (file_mode==true ? file : std::cout) << to_string(header) << std::endl;
  }
}

void Logger::write_row(Logger::Row&& row) {
  (file_mode==true ? file : std::cout) << to_string(row) << std::endl;
}

void Logger::write_rows(std::queue<Logger::Row>&& to_write) {
  while(!to_write.empty()) {
    write_row(std::move(to_write.front()));
    to_write.pop();
  }
}

void Logger::write_rows_async() {
  auto rows_copy = std::move(rows_buffer);
  rows_buffer = { };
  if(write_thread.joinable())
    write_thread.join();
  write_thread = std::thread(&Logger::write_rows, this, rows_copy);
}

std::string Logger::to_string(Row& row) {
  auto& header = headers.at(row.title);
  std::stringstream ss;
  ss << row.title;
  for(int i = 0; i < header.index_keys.size(); i++) {
    ss << "," << row.key_values[header.index_keys.at(i)];
  }
  return ss.str();
}

std::string Logger::to_string(Header& header) {
  std::stringstream ss;
  ss << header.title;
  for(int i = 0; i < header.index_keys.size(); i++) {
    ss << "," << header.index_keys.at(i);
  }
  return ss.str();
}

//Logger::Loggable
//--------------------------------------------------
Loggable::Loggable(Logger::Header&& header)
: header(header) {
  Logger::logger()->add_header(header);
}

void Loggable::log(Logger::Row&& row, bool immediately) {
  Logger::logger()->log(std::move(row), immediately);
}
