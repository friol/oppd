//
// this is the last one
// oppd's car race
// friol 2k22
//

#include "include/oppd.h"

#define fbDimX 80
#define fbDimY 30

class oppdcar
{
private:
	int px, py;
	int velocity;

public:

	oppdcar(int x, int y)
	{
		this->px = x;
		this->py = y;
		this->velocity = 0;
	}

	void draw(wchar_t fb[][fbDimX])
	{

	}
};

void clearFb(wchar_t fb[][fbDimX])
{
	for (int r = 0;r < fbDimY;r++)
	{
		for (int c = 0;c < fbDimX;c++)
		{
			fb[r][c] = L' ';
		}
	}
}

void drawFinishLine(wchar_t fb[][fbDimX])
{
	const int finishLineX = 70;

	for (int r = 0;r < fbDimY;r++)
	{
		if (r % 2)
		{
			fb[r][finishLineX] = L'▒';
			fb[r][finishLineX+1] = L' ';
		}
		else
		{
			fb[r][finishLineX] = L' ';
			fb[r][finishLineX+1] = L'▒';
		}
	}
}

void day30bitblit(int dimx, int dimy, int posx, int posy, wchar_t fb[][fbDimX])
{
	for (int r = 0;r < dimy;r++)
	{
		consoleGotoxy(posx, posy + r);
		wchar_t line[fbDimX+1];
		line[fbDimX] = 0;
		for (int c = 0;c < dimx;c++)
		{
			line[c] = fb[r][c];
		}
		std::wcout << line;
	}
}

//
//
//

void day30()
{
	setUnicodeConsole();
	consoleHideCursor();
	clearScreen();

	wchar_t framebuffer[fbDimY][fbDimX];

	bool goout = false;
	while (!goout)
	{
		clearFb(framebuffer);
		drawFinishLine(framebuffer);
		day30bitblit(fbDimX, fbDimY, 0, 0, framebuffer);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27) goout = true;
		}
	}

	consoleGotoxy(0, fbDimY + 1);
}
