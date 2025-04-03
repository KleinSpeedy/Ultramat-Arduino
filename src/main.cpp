#include "CommandHandler.h"
#include "util/ErrorHandling.h"
#include "SerialCommunication.h"

int main(void)
{
    using namespace Ultramat;

    SerialComms comms;
    if(!comms.start())
    {
        Errors::handleNotRecoverable(Errors::Type::StartComms);
    }

    CommandHandler cmdHandler{comms};
    while(true)
    {
        comms.process();
        cmdHandler.process();
    }
}
