# logger

c++11 ready-to-use csv format `Logger` class. Simply change the namespace as desired and add to a project.
See [this](log.csv) sample output.

## features
- log to stdout or a file
- adjustable buffer size with asynchronous writing when full (file mode)
- inheritable `Loggable` protocol for specifying a custom csv header for a derived class (see [this](main.cpp) example program)
