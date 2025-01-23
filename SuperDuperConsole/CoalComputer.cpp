#include "CoalComputer.h"

#include "ConsoleFramebuffer.h"
#include "Exception.h"

#include <sstream>

namespace
{
    using Token = Coal::Token;
    using TokenList = Coal::TokenList;
    TokenList splitStringBySpace(const std::string& str)
    {
        std::string workingStr = str;
        TokenList tokenList;

        while (true)
        {
            auto index = workingStr.find_first_of(' ');
            if (index == std::string::npos)
                break;

            std::string tmp(&workingStr[0], index);
            tokenList.push_back(tmp);
            workingStr = std::string(&workingStr[index + 1], workingStr.size() - (index + 1));
        }
        tokenList.push_back(workingStr);

        return tokenList;
    }

    TokenList removeEmptyTokens(const TokenList& tokens)
    {
        TokenList result;
        for (size_t i = 0; i < tokens.size(); ++i)
        {
            if (tokens[i] != "")
                result.push_back(tokens[i]);
        }
        return result;
    }

    TokenList splitIntoTokens(const std::string& str)
    {
        return removeEmptyTokens(splitStringBySpace(str));
    }
}

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
            ss << static_cast<unsigned int>(cpu.getRegister(row + 8).getValue());
            cfb.printString(row + 1, 6, ss.str(), Green, White);
        }
    }

    const Register& CPU::getRegister(int idx)const { return m_registers[idx]; }
    Register& CPU::getRegister(int idx) { return m_registers[idx]; }



    void CPU::process(const InstructionLine& instruction)
    {
        std::vector<std::string> tokenList = splitIntoTokens(instruction);
        const std::string& instructionType = tokenList[0];

        if (instructionType == "MOV")
        {
            Mov mov(tokenList);
            mov.apply(*this);   
        }
        else if (instructionType == "ADD")
        {
            Add add(tokenList);
            add.apply(*this);
        }
        else if (instructionType == "SUB")
        {
            if (tokenList.size() != 4)
                throw InvalidNumberOfOperandInstruction();

            OperandAccessor src1(tokenList[1]);
            OperandAccessor src2(tokenList[2]);
            OperandAccessor dest(tokenList[3]);
            dest.affect(*this, src1.evaluate(*this) - src2.evaluate(*this));
        }
        else if (instructionType == "MUL")
        {
            if (tokenList.size() != 4)
                throw InvalidNumberOfOperandInstruction();

            OperandAccessor src1(tokenList[1]);
            OperandAccessor src2(tokenList[2]);
            OperandAccessor dest(tokenList[3]);
            dest.affect(*this, src1.evaluate(*this) * src2.evaluate(*this));
        }
        else if (instructionType == "DIV")
        {
            if (tokenList.size() != 4)
                throw InvalidNumberOfOperandInstruction();

            OperandAccessor src1(tokenList[1]);
            OperandAccessor src2(tokenList[2]);
            OperandAccessor dest(tokenList[3]);
            if (src2.evaluate(*this) == 0)
                throw DivisionByZeroException();

            dest.affect(*this, src1.evaluate(*this) / src2.evaluate(*this));
        }
        else
        {
            throw UnknownInstructionException();
        }
        m_registers[15].setValue(m_registers[15].getValue() + 1);
    }
}
