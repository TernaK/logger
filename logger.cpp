#include "logger.h"
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
Logger::Logger() {
  // file.open("planck_vio_log.csv");
}

Logger::~Logger() {
  // if(file.is_open())
  //   file.close();
}

void Logger::push_row(Logger::Row& row) {
  rows.push(std::move(row));
}

void Logger::add_header(Logger::Header& header) {
  if(headers.count(header.title) == 0) {
    headers.insert({header.title, header});
    std::cout << to_string(header) << std::endl;
  }
}

void Logger::write() {
  while(!rows.empty()) {
    std::cout << to_string(rows.front()) << std::endl;
    rows.pop();
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
