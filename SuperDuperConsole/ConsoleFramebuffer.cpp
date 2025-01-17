#include "ConsoleFramebuffer.h"

#include <Windows.h>
#include <vector>

class ConsoleFramebufferPrivateImpl
{
public:
    ConsoleFramebufferPrivateImpl();
    void setCharacter(int row, int col, char car, Color foreground, Color background);
    void printString(int row, int col, const std::string& str, Color foreground, Color background);
    void show();

private:
    void resize(int numRows, int numCols);

private:
    HANDLE m_handle;
    std::vector<CHAR_INFO> m_buffer;
    int m_numRows;
    int m_numCols;
};



ConsoleFramebufferPrivateImpl::ConsoleFramebufferPrivateImpl() : m_handle(::GetStdHandle(STD_OUTPUT_HANDLE))
{
    CONSOLE_SCREEN_BUFFER_INFO sbi;
    BOOL result = GetConsoleScreenBufferInfo(m_handle, &sbi);
    resize(sbi.dwSize.Y, sbi.dwSize.X);
}

void ConsoleFramebufferPrivateImpl::resize(int numRows, int numCols)
{
    m_numRows = numRows;
    m_numCols = numCols;

    m_buffer.resize(m_numRows * m_numCols);
    ::ZeroMemory(&m_buffer[0], m_numRows * m_numCols * sizeof(CHAR_INFO));
}

WORD convertForegroundColorToWindows(Color foreground)
{
    switch (foreground)
    {
    case Red:
        return FOREGROUND_RED;

    case Green:
        return FOREGROUND_GREEN;

    case Blue:
        return FOREGROUND_BLUE;

    case White:
        return FOREGROUND_RED + FOREGROUND_GREEN + FOREGROUND_BLUE;

    case Black:
        return 0;

    default:
        throw;
    }
}

WORD convertBackgroundColorToWindows(Color background)
{
    return convertForegroundColorToWindows(background) * 16;
}

WORD convertColorToWindows(Color foreground, Color background)
{
    return convertForegroundColorToWindows(foreground) + convertBackgroundColorToWindows(background);
}

void ConsoleFramebufferPrivateImpl::setCharacter(int row, int col, char car, Color foreground, Color background)
{
    CHAR_INFO& cell = m_buffer[row * m_numCols + col];
    cell.Char.AsciiChar = car;
    cell.Attributes = convertColorToWindows(foreground, background);
}

void ConsoleFramebufferPrivateImpl::show()
{
    COORD dwBufferSize;
    dwBufferSize.X = m_numCols;
    dwBufferSize.Y = m_numRows;
    COORD dwBufferCoord;
    dwBufferCoord.X = 0;
    dwBufferCoord.Y = 0;

    SMALL_RECT writeRegion;
    writeRegion.Top = 0;
    writeRegion.Left = 0;
    writeRegion.Right = m_numCols - 1;
    writeRegion.Bottom = m_numRows - 1;

    BOOL result = WriteConsoleOutput(
        m_handle,
        &m_buffer[0],
        dwBufferSize,
        dwBufferCoord,
        &writeRegion
    );
}

int myMin(int a, int b)
{
    return (a > b) ? b : a;
}

void ConsoleFramebufferPrivateImpl::printString(int row, int col, const std::string& str, Color foreground, Color background)
{
    int numCarToWrite = myMin(m_numRows - row, static_cast<int>(str.size()));
    for (int idx = 0; idx < numCarToWrite; ++idx)
    {
        setCharacter(row, col + idx, str[idx], foreground, background);
    }
}



ConsoleFramebuffer::ConsoleFramebuffer() : m_pimpl(new ConsoleFramebufferPrivateImpl)
{}

ConsoleFramebuffer::~ConsoleFramebuffer()
{
    delete m_pimpl;
    m_pimpl = nullptr;
}

void ConsoleFramebuffer::setCharacter(int row, int col, char car, Color foreground, Color background)
{
    m_pimpl->setCharacter(row, col, car, foreground, background);
}

void ConsoleFramebuffer::show()
{
    m_pimpl->show();
}

void ConsoleFramebuffer::printString(int row, int col, const std::string& str, Color foreground, Color background)
{
    m_pimpl->printString(row, col, str, foreground, background);
}
