#pragma once

#include <ostream>

enum MessageLevel {
    INFO,
    WARNING,
    ERROR
};

typedef struct {
    std::string content;
    MessageLevel level;
} Message;

std::ostream& operator<<(std::ostream& os, const MessageLevel& level);

