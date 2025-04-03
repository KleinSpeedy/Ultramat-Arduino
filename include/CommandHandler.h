#pragma once

#include "MessageBuffer.h"
#include "SerialCommunication.h"

namespace Ultramat {

class CommandHandler
{
public:

    CommandHandler(SerialComms &comms)
        : comms_{comms}
    {
    }

    void process();

private:

    SerialComms &comms_;

    bool commandDone_ = false;
    MessageBuffer currentCmdMsg_;

    bool executeHandler() const;
};

} // namespace Ultramat
