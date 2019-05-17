#include "logger.h"

int main(int argc, char* args[]) {
  Logger logger;
  Logger::Header header("POS", {"x", "y", "z"});
  Logger::Row row("POS", {{"x",std::to_string(0)}, {"y",std::to_string(1)}, {"z",std::to_string(2)}});
  logger.add_header(header);
  logger.push_row(row);
  logger.write();
}
