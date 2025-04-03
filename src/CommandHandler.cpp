#include "CommandHandler.h"

namespace Ultramat {

void CommandHandler::process()
{
    // are we currently executing a command?
    if(commandDone_)
    {
        const bool msgAvailable = comms_.getNextInMessage(currentCmdMsg_);
        if(!msgAvailable)
            return;
    }

    commandDone_ = executeHandler();
}

bool CommandHandler::executeHandler() const
{
    return true;
}

} // namespace Ultramat
