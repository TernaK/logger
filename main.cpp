#include "logger.h"
using namespace logger_namespace;

struct LoggableTestClass1 : public Loggable {
  LoggableTestClass1() : Loggable({"POS", {"x", "y", "good"}}) { }

  void run() {
    log({"POS", {{"x",std::to_string(5)}, {"y",std::to_string(5)}, {"good","yes"}}});
  }
};

struct LoggableTestClass2 : public Loggable {
  LoggableTestClass2() : Loggable({"ALT", {"p", "r", "y"}}) { }

  void run() {
    log({"ALT", {{"y",std::to_string(-1)}, {"p",std::to_string(1)}, {"r",std::to_string(-2)}}});
  }
};

int main(int argc, char* args[]) {
  LoggableTestClass1 tester1;
  LoggableTestClass2 tester2;
  tester1.run();
  tester2.run();
  tester1.run();
  tester2.run();
}
