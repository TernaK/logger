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

//Logger::Row
//--------------------------------------------------
std::unique_ptr<Logger> Logger::instance = nullptr;
Logger::Logger(std::string log_name, bool file_mode, bool async, int period_ms)
: file_mode(file_mode) {
  if(file_mode) {
    file.open(log_name);
  }
  if(async) {
    run_flag = true;
    write_thread = std::thread([this, period_ms](){
      while(run_flag){
        std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));
        write(this->rows);
      }
    });
  }
}

Logger::~Logger() {
  if(file_mode) {
    if(file.is_open())
      file.close();
  }

  run_flag = false;
  if(write_thread.joinable())
    write_thread.join();
}

Logger* Logger::logger() {
  if(!instance)
    instance = std::unique_ptr<Logger>(new Logger());
  return instance.get();
}

void Logger::push_row(Logger::Row&& row) {
  std::lock_guard<std::mutex> lg(write_mutex);
  rows.push(std::move(row));
}

void Logger::log(Logger::Row&& row) {
  std::lock_guard<std::mutex> lg(write_mutex);
  rows.push(std::move(row));
  write(rows);
}

void Logger::add_header(Logger::Header& header) {
  std::lock_guard<std::mutex> lg(write_mutex);
  if(headers.count(header.title) == 0) {
    headers.insert({header.title, header});
    (file_mode==true ? file : std::cout) << to_string(header) << std::endl;
  }
}

void Logger::write(std::queue<Logger::Row>& to_copy) {
  auto rows_copy = std::move(to_copy);
  rows = { };
  while(!rows_copy.empty()) {
    (file_mode==true ? file : std::cout) << to_string(rows_copy.front()) << std::endl;
    rows_copy.pop();
  }
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
  if(immediately)
    Logger::logger()->log(std::move(row));
  else
    Logger::logger()->push_row(std::move(row));
}
