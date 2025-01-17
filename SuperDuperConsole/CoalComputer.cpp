#include "CoalComputer.h"

#include "ConsoleFramebuffer.h"

#include <sstream>

namespace Coal
{
    void showCPU(ConsoleFramebuffer& cfb, const CPU& cpu)
    {
        for (int row = 0; row < 10; ++row)
            for (int col = 0; col < 10; ++col)
                cfb.setCharacter(row, col, ' ', Green, Green);

        for (int row = 0; row < 8; ++row)
        {
            std::stringstream ss;

            ss << static_cast<unsigned int>(cpu.getRegister(row).getValue());

            cfb.printString(row + 1, 1, ss.str(), Green, White);
        }

        for (int row = 0; row < 8; ++row)
        {
            std::stringstream ss;

            ss << static_cast<unsigned int>(cpu.getRegister(row).getValue());

            cfb.printString(row + 1, 6, ss.str(), Green, White);
        }
    }

    const Register& CPU::getRegister(int idx)const { return m_registers[idx]; }
}
