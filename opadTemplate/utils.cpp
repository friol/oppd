//
// some utils
//

#include "include/oppd.h"

void consoleGotoxy(int x, int y)
{
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
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

