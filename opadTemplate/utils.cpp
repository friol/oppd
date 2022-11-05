//
// some utils
//

#include "include/oppd.h"

void consoleGotoxy(int x, int y)
{
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void consoleGetDimensions(int& dimx, int& dimy)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    dimx = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    dimy = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void clearScreen()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int dimx = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int dimy = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    consoleGotoxy(0, 0);

    for (int y = 0;y < dimy;y++)
    {
        for (int x = 0;x < dimx;x++)
        {
            std::wcout << " ";
        }
        std::wcout << std::endl;
    }

    consoleGotoxy(0, 0);
}

void setUnicodeConsole()
{
    // seems it's the only way in windows to print unicode chars in the console
    int err = _setmode(_fileno(stdout), _O_U16TEXT);
}

void consolePrintReversedChar(char ch,int col)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (col==0) SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
    if (col==1) SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
    std::cout << ch;
}

void resetConsole()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
}

COORD GetConsoleCursorPosition()
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        // The function failed. Call GetLastError() for details.
        COORD invalid = { 0, 0 };
        return invalid;
    }
}

void consoleHideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
