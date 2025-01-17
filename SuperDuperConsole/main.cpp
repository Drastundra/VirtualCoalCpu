#include "ConsoleFramebuffer.h"
#include "CoalComputer.h"

#include <iostream>

int main(int argc, char** argv)
{
    ConsoleFramebuffer cfb;
    
    Coal::CPU cpu;

    Coal::InstructionList instructionList = {
        "MOV 42 r7"
      , "MOV 12 r0"
    };

    Coal::showCPU(cfb, cpu);

    while (!instructionList.empty())
    {
        Coal::Instruction instruction = instructionList.front();
        instructionList.erase(instructionList.begin());
        
        try
        {
            cpu.process(instruction);
        }
        catch (std::runtime_error& e)
        {
            std::cerr << e.what();
            return 0;
        }

        Coal::showCPU(cfb, cpu);
        cfb.show();
        ::getchar();
    }
}
