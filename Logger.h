#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "User.h"

#define _Log(x) Logger::getInstance().log((x));

using namespace std;

class Logger {
public:
    static Logger& getInstance();

public:
    void printLogs();
    void log(std::string lg);
    std::vector<std::string>& getLogs ();

private:
    Logger();
    std::vector<std::string> logs;
};

