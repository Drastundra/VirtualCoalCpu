#pragma once

#include <vector>
#include <string>
#include <stdexcept>

class ConsoleFramebuffer;

namespace Coal
{
    class Register
    {
        friend class CPU;
        friend class Debuger;
        friend class OperandAccessor;

    public:
        const unsigned char& getValue() const { return m_value; }

    protected:
        void setValue(unsigned char newValue) { m_value = newValue; }

    private:
        unsigned char m_value = 0;
    };

    using Instruction = std::string;
    using InstructionList = std::vector<Instruction>;

#define DECLARE_NEW_EXCEPTION_CLASS(classname) \
class classname : public std::runtime_error\
{\
public:\
    classname();\
};

#define IMPLEMENT_NEW_EXCEPTION_CLASS(classname, message) \
classname::classname()\
    : std::runtime_error(message)\
{\
}

    DECLARE_NEW_EXCEPTION_CLASS(UnknownInstructionException);
    DECLARE_NEW_EXCEPTION_CLASS(InvalidNumberOfOperandInstruction);
    DECLARE_NEW_EXCEPTION_CLASS(InvalidOperandException);

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
