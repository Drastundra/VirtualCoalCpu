#include "ConsoleFramebuffer.h"
#include "CoalComputer.h"

int main(int argc, char** argv)
{
    ConsoleFramebuffer cfb;
    
    Coal::CPU cpu;

    Coal::InstructionList instructionList = {
        "MOV 42 r7"
      , "MOV 12 r0"
    };

    Coal::showCPU(cfb, cpu);

    while (true)
    {
        Coal::Instruction instruction = instructionList.front();
        instructionList.erase(instructionList.begin());
        
        cpu.process(instruction);

        Coal::showCPU(cfb, cpu);
        cfb.show();
        ::getchar();
    }
}
