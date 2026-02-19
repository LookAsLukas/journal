#pragma once

#include "Message.h"
#include <fstream>

class Journal {
public:
    Journal(std::string& filename, MessageLevel defaultMessageLevel);
    ~Journal();

    void setDefaultMessageLevel(MessageLevel level);
    void writeMessage(Message& message);

private:
    MessageLevel _defaultMessageLevel;
    std::ofstream journalFile;
};
