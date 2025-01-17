#pragma once

#include <string>

enum Color
{
    Red
  , Green
  , Blue
  , White
  , Black
};

class ConsoleFramebufferPrivateImpl;

class ConsoleFramebuffer
{
public:
    ConsoleFramebuffer();
    ~ConsoleFramebuffer();
    void setCharacter(int row, int col, char car, Color foreground = White, Color background = Black);
    void show();
    void printString(int row, int col, const std::string& str, Color foreground = White, Color background = Black);

private:
    ConsoleFramebufferPrivateImpl* m_pimpl;
};