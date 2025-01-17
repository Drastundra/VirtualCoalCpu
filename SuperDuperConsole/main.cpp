#include "ConsoleFramebuffer.h"
#include "CoalComputer.h"

int main(int argc, char** argv)
{
    ConsoleFramebuffer cfb;
    
    Coal::CPU cpu;

    while (true)
    {
        Coal::showCPU(cfb, cpu);
        cfb.show();
    }
}
