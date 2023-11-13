#include "util/Device.h"
#include "Communicator.h"
#include "ErrorHandling.h"

int main()
{
    // initialize MCU
    Device::initMcu();

    Communicator comm;

    // initialize communication
    if(!comm.start())
    {
        Errors::handleNotRecoverable(Errors::Type::Communication);
    }

    // begin
    while(true)
    {
        // handle queue

        // handle command

        // handle command success?
    }
}
