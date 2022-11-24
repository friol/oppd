//
// oppd day 24
// the unknown day (pssst: it's lights out!)
// friol 2k22
//

#include "include/oppd.h"

class lightsOutBoard
{
private:

	int board[5][5];
	int markerPosx, markerPosy;

public:

	lightsOutBoard()
	{
		int initpattern[5][5] = {
			{1,0,1,0,1},
			{1,0,0,0,1},
			{0,0,1,0,0},
			{0,1,1,1,1},
			{1,0,1,0,1}
		};

		for (int r = 0;r < 5;r++)
		{
			for (int c = 0;c < 5;c++)
			{
				board[r][c] = initpattern[r][c];
			}
		}

		markerPosx = 0;
		markerPosy = 0;
	}

	void toggleCurrentPosition()
	{
		int crux[5][2] = {
			{0,-1},{0,0},{-1,0},{1,0},{0,1}
		};

		for (int pos = 0;pos < 5;pos++)
		{
			int xpos = this->markerPosx + crux[pos][0];
			int ypos = this->markerPosy + crux[pos][1];
			if ((xpos >= 0) && (xpos < 5) && (ypos >= 0) && (ypos < 5))
			{
				this->board[ypos][xpos] = !this->board[ypos][xpos];
			}
		}

	}

	bool isWinPosition()
	{
		for (int r = 0;r < 5;r++)
		{
			for (int c = 0;c < 5;c++)
			{
				if (this->board[r][c] != 0) return false;
			}
		}

		return true;
	}

	void moveMarker(int dir,int x,int y)
	{
		// hide marker
		consoleGotoxy(x + (this->markerPosx * 2), y + (this->markerPosy * 2));
		std::wcout << L"   ";
		consoleGotoxy(x + (this->markerPosx * 2), y + (this->markerPosy * 2) + 2);
		std::wcout << L"   ";

		if (dir == 0)
		{
			if (markerPosy > 0) markerPosy--;
		}
		else if (dir == 1)
		{
			if (markerPosx < 4) markerPosx++;
		}
		else if (dir == 2)
		{
			if (markerPosy < 4) markerPosy++;
		}
		else if (dir == 3)
		{
			if (markerPosx > 0) markerPosx--;
		}
	}

	void draw(int x,int y)
	{
		int px = x + 1;
		int py = y + 1;

		for (int r = 0;r < 5;r++)
		{
			for (int c = 0;c < 5;c++)
			{
				consoleGotoxy(px, py);
				if (board[r][c] == 1)
				{
					std::wcout << L"█";
				}
				else
				{
					std::wcout << L".";
				}

				px += 2;
			}
			px = x + 1;
			py += 2;
		}

		// draw marker
		consoleGotoxy(x+(this->markerPosx*2),y+(this->markerPosy*2));
		std::wcout << L"╔═╗";
		consoleGotoxy(x + (this->markerPosx * 2), y + (this->markerPosy * 2)+2);
		std::wcout << L"╚═╝";
	}

};

//
//
//

void day24()
{
	consoleGotoxy(0, 0);
	setUnicodeConsole();
	consoleHideCursor();
	clearScreen();

	lightsOutBoard theBoard;

	bool win = false;
	bool goout = false;
	int moves = 0;

	while (!goout)
	{
		theBoard.draw(0, 0);

		if (theBoard.isWinPosition())
		{
			goout = true;
			win = true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (_kbhit()&&(!goout))
		{
			char ch = _getch();
			if (ch == 27)
			{
				goout = true;
			}
			if (ch == 72) // up
			{
				theBoard.moveMarker(0,0,0);
				moves++;
			}
			else if (ch == 80) // down
			{
				theBoard.moveMarker(2, 0, 0);
				moves++;
			}
			else if (ch == 75) // left
			{
				theBoard.moveMarker(3, 0, 0);
				moves++;
			}
			else if (ch == 77) // right
			{
				theBoard.moveMarker(1, 0, 0);
				moves++;
			}
			else if (ch == 32) // space
			{
				// lights out! (or in...)
				theBoard.toggleCurrentPosition();
				moves++;
			}
		}
	}

	consoleGotoxy(0, 16);

	if (win)
	{
		std::wcout << "You WON! (in just " << moves << " moves)" << std::endl;
	}

	consoleShowCursor();
}
