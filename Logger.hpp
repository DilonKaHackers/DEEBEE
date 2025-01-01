#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>
#include <mutex>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

class Logger {
private:
    fs::path logFilePath;
    ofstream logFileStream;
    mutex logMutex;

    string getCurrentTime() const {
        auto currentTime = chrono::system_clock::now();
        time_t time = chrono::system_clock::to_time_t(currentTime);
        tm localTime;

        if (localtime_s(&localTime, &time) != 0) {
            cout<<"Failed to get local time"<<endl;
        }

        char buffer[20]; 
        if (strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime)) {
            return string(buffer);
        }
        else cout<<"Failed to format time"<<endl;
        
    }

public:

    Logger(const string& fileName = "logs.txt") {
        logFilePath = fs::current_path() / fileName;

        logFileStream.open(logFilePath, ios::app);
        if (!logFileStream.is_open()) {
            throw runtime_error("Failed to open log file: " + logFilePath.string());
        }
    }
 
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    ~Logger() {
        if (logFileStream.is_open()) {
            logFileStream.close();
        }
    }

    void log(const string& message) {
        lock_guard<mutex> lock(logMutex);
        logFileStream <<"DB LOG -> AT : " << "[" << getCurrentTime() << "], " << "OPERATION : " << message   << endl;
    }
};

#endif // LOGGER_HPP
