#include "logger.h"
using namespace logger_namespace;

struct LoggableTestClass1 : public Loggable {
  LoggableTestClass1() : Loggable({"POSITION", {"x", "y", "good"}}) { }

  void run() {
    log({"POSITION", {{"x",std::to_string(5)}, {"y",std::to_string(5)}, {"good","yes"}}});
  }
};

struct LoggableTestClass2 : public Loggable {
  LoggableTestClass2() : Loggable({"ATTITUDE", {"pitch", "roll", "yaw"}}) { }

  void run() {
    log({"ATTITUDE",
      {{"yaw",std::to_string(-1)}, {"pitch",std::to_string(1)}, {"roll",std::to_string(-2)}}
    });
  }
};

int main(int argc, char* args[]) {

  std::string log_file_name = "log.csv";
  Logger::logger()->set_file_output(log_file_name);

  LoggableTestClass1 tester1;
  LoggableTestClass2 tester2;
  tester1.run();
  tester2.run();
  tester1.run();
  tester1.run();
  tester2.run();
  tester1.run();

}
