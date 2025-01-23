#pragma once

#include "Exception.h"
#include "OperandAccessor.h"

#include <vector>
#include <string>

namespace Coal
{
    using Token = std::string;
    using TokenList = std::vector<Token>;

    class CPU;

    class IInstruction
    {
    public:
        virtual ~IInstruction()
        {

        }

        IInstruction(const TokenList& tokenList, int nbOperand)
        {
            if (tokenList.size() != nbOperand)
                throw InvalidNumberOfOperandInstruction();
        }

        virtual void apply(CPU& cpu) = 0;
    };

    class Mov : public IInstruction
    {
    public:
        Mov(const TokenList& tokenList) : IInstruction(tokenList, 3), src(tokenList[1]), dest(tokenList[2])
        {}

        void apply(CPU& cpu) override
        {
            dest.affect(cpu, src.evaluate(cpu));
        }

        OperandAccessor src;
        OperandAccessor dest;
    };

    class Add : public IInstruction
    {
    public:
        Add(const TokenList& tokenList) : IInstruction(tokenList, 4), src1(tokenList[1]), src2(tokenList[2]), dest(tokenList[3])
        {
        }

        void apply(CPU& cpu) override
        {
            dest.affect(cpu, src1.evaluate(cpu) + src2.evaluate(cpu));
        }

        OperandAccessor src1;
        OperandAccessor src2;
        OperandAccessor dest;
    };
}
