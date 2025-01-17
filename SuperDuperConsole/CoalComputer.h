#pragma once

#include <vector>

class ConsoleFramebuffer;

namespace Coal
{
    class Register
    {
    public:
        const unsigned char& getValue() const { return m_value; }

    private:
        unsigned char m_value = 0;
    };

    class Instruction;
    using InstructionList = std::vector<Instruction>;

    class CPU
    {
    public:
        Register& getRegister(int idx);
        const Register& getRegister(int idx) const;
        void process(const Instruction&);

    private:
        Register m_registers[16];
    };
    
    void showCPU(ConsoleFramebuffer& cfb, const CPU& cpu);

    class Computer
    {
    public:
        CPU& getCPU();
        const CPU& getCPU() const;
    
    private:
        CPU m_cpu;
        InstructionList m_instructions;
    };
}
