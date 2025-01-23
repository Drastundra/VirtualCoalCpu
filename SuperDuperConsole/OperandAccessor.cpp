#include "OperandAccessor.h"

#include "CoalComputer.h"

namespace Coal
{
    OperandAccessor::OperandAccessor(const Token& token)
    {
        try
        {
            if (token[0] == 'r')
            {
                std::string newStr = std::string(&token[1], token.size() - 1);
                m_valueOrRegisterIndex = std::stoi(newStr);
                m_isRegister = true;
            }
            else
            {
                m_valueOrRegisterIndex = std::stoi(token);
                m_isRegister = false;
            }
        }
        catch (std::invalid_argument&)
        {
            throw InvalidOperandException();
        }
    }

    unsigned char OperandAccessor::evaluate(const CPU& cpu)
    {
        if (!m_isRegister)
            return m_valueOrRegisterIndex;

        return cpu.getRegister(m_valueOrRegisterIndex).getValue();
    }

    void OperandAccessor::affect(CPU& cpu, unsigned char newValue)
    {
        if (!m_isRegister)
            throw InvalidOperandException();

        cpu.getRegister(m_valueOrRegisterIndex).setValue(newValue);
    }
}
