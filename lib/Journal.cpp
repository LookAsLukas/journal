#include "Journal.h"
#include "Message.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <stdexcept>

Journal::Journal(std::string& filename, MessageLevel defaultMessageLevel) {
    journalFile.open(filename);
    if (!journalFile.is_open()) {
        throw std::runtime_error("libjournal: Could not open file " + filename);
    }

    _defaultMessageLevel = defaultMessageLevel;
}

Journal::~Journal() {
    journalFile.close();
}

void Journal::setDefaultMessageLevel(MessageLevel level) {
    _defaultMessageLevel = level;
}

void Journal::writeMessage(Message& message) {
    if (_defaultMessageLevel > message.level) {
        return;
    }

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm timeStruct = *std::localtime(&time);
    journalFile << "[" << std::put_time(&timeStruct, "%H:%M:%S") << "] "
        << "(" << message.level << ") "
        << message.content << std::endl;
}

