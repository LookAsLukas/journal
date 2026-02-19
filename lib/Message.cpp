#include "Message.h"

std::ostream& operator<<(std::ostream& os, const MessageLevel& level) {
    switch (level) {
        case INFO:
            os << "INFO";
            break;
        case WARNING:
            os << "WARNING";
            break;
        case ERROR:
            os << "ERROR";
            break;
    }
    return os;
}
