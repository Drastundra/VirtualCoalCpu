#pragma once

#include "BaseType.h"

namespace Coal
{
    class CPU;

    class OperandAccessor
    {
    public:
        OperandAccessor(const Token& token);

        unsigned char evaluate(const CPU& cpu);
        void affect(CPU& cpu, unsigned char newValue);

    private:
        bool m_isRegister;
        unsigned char m_valueOrRegisterIndex;
    };
}
