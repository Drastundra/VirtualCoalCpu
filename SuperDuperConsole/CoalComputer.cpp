#include "CoalComputer.h"

#include "ConsoleFramebuffer.h"

#include <sstream>

namespace
{
    using Token = std::string;
    using TokenList = std::vector<Token>;

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

    IMPLEMENT_NEW_EXCEPTION_CLASS(UnknownInstructionException, "Trying to process an unknown instruction.")
    IMPLEMENT_NEW_EXCEPTION_CLASS(InvalidNumberOfOperandInstruction, "Invalid number of operand.")
    IMPLEMENT_NEW_EXCEPTION_CLASS(InvalidOperandException, "Operand is invalid.")

    void CPU::process(const Instruction& instruction)
    {
        std::vector<std::string> tokenList = splitIntoTokens(instruction);
        const std::string& instructionType = tokenList[0];

        if (instructionType == "MOV")
        {
            if (tokenList.size() != 3)
                throw InvalidNumberOfOperandInstruction();

            const std::string& op1 = tokenList[1];
            const std::string& op2 = tokenList[2];

            try
            {
                unsigned char val = 0;
                if (op1[0] == 'r')
                {
                    std::string newStr = std::string(&op1[1], op1.size() - 1);
                    unsigned char regIdx = std::stoi(newStr);
                    val = m_registers[regIdx].getValue();
                }
                else
                {
                    val = std::stoi(op1);
                }

                if (op2[0] != 'r')
                    throw InvalidOperandException();

                std::string newStr = std::string(&op2[1], op2.size() - 1);
                unsigned char regIdx = std::stoi(newStr);
                m_registers[regIdx].setValue(val);
            }
            catch (std::invalid_argument&)
            {
                throw InvalidOperandException();
            }
        }
        else
        {
            throw UnknownInstructionException();
        }
        m_registers[15].setValue(m_registers[15].getValue() + 1);
    }
}
