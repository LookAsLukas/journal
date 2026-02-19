#include "Message.h"
#include "Journal.h"
#include <condition_variable>
#include <ios>
#include <mutex>
#include <queue>
#include <iostream>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>

std::queue<Message> _messageQueue; 
std::mutex _mutex;
std::condition_variable _conditionVariable;
bool _end;

void writer_thread(Journal *journal);
void send_message(MessageLevel defaultMessageLevel);
MessageLevel request_lvl();

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Usage: journal_writer [FILENAME] [DEFAULT MESSAGE LEVEL]" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string level_str = argv[2];
    MessageLevel defaultMessageLevel;
    if (level_str == "INFO") {
        defaultMessageLevel = INFO;
    } else if (level_str == "WARNING") {
        defaultMessageLevel = WARNING;
    } else if (level_str == "ERROR") {
        defaultMessageLevel = ERROR;
    } else {
        std::cout << "Invalid default message level" << std::endl;
        return 1;
    }

    Journal journal(filename, defaultMessageLevel);
    std::thread t(writer_thread, &journal);
    _end = false;

    std::cout << "Initializing journal " << filename
        << " with default message level " << defaultMessageLevel << std::endl;
    std::cout << "Commands:" << std::endl
        << "  msg - write a message" << std::endl
        << "  lvl - change default message level" << std::endl
        << "  exit - leave the program" << std::endl;

    std::string line;
    std::cout << "Enter a command: ";
    while (std::getline(std::cin, line)) {
        if (line == "msg") {
            send_message(defaultMessageLevel);
        } else if (line == "lvl") {
            defaultMessageLevel = request_lvl();
            journal.setDefaultMessageLevel(defaultMessageLevel);
        } else if (line == "exit") {
            break;
        } else {
            std::cout << "Unknown command: " << line << std::endl;
        }

        std::cout << "Commands:" << std::endl
            << "  msg - write a message" << std::endl
            << "  lvl - change default message level" << std::endl
            << "  exit - leave the program" << std::endl;
        std::cout << "Enter a command: ";
    }

    _end = true;
    _conditionVariable.notify_one();
    if (t.joinable()) {
        t.join();
    }

    return 0;
}

void writer_thread(Journal *journal) {
    while (true) {
        std::unique_lock<std::mutex> lock(_mutex);
        _conditionVariable.wait(lock, []{ return !_messageQueue.empty() || _end; });

        if (_end && _messageQueue.empty()) {
            break;
        }

        if (!_messageQueue.empty()) {
            Message message = _messageQueue.front();
            _messageQueue.pop();
            lock.unlock();

            journal->writeMessage(message);
        }
    }
}

void send_message(MessageLevel defaultMessageLevel) {
    std::cout << "First enter the message level, then the message" << std::endl;
    std::cout << "Leave level field empty for default" << std::endl;

    std::string line;
    std::cout << "Level (INFO, WARNING, ERROR): ";
    while (std::getline(std::cin, line)) {
        MessageLevel level = defaultMessageLevel;
        if (line == "INFO") {
            level = INFO;
        } else if (line == "WARNING") {
            level = WARNING;
        } else if (line == "ERROR") {
            level = ERROR;
        } else if (line != "") {
            std::cout << "Please, choose a valid level" << std::endl;
            std::cout << "Level (INFO, WARNING, ERROR): ";
            continue;
        }

        std::cout << "Message: ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        {
            std::lock_guard<std::mutex> lock(_mutex);
            _messageQueue.push((Message){line, level});
        }
        _conditionVariable.notify_one();

        break;
    }
}

MessageLevel request_lvl() {
    std::cout << "Enter new default message level" << std::endl;

    MessageLevel level;
    std::string line;
    std::cout << "Level (INFO, WARNING, ERROR): ";
    while (std::getline(std::cin, line)) {
        if (line == "INFO") {
            level = INFO;
        } else if (line == "WARNING") {
            level = WARNING;
        } else if (line == "ERROR") {
            level = ERROR;
        } else {
            std::cout << "Please, choose a valid level" << std::endl;
            std::cout << "Level (INFO, WARNING, ERROR): ";
            continue;
        }
        break;
    }

    std::cout << "Default message level is now " << level << std::endl;
    return level;
}

