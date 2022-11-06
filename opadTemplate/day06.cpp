//
// oppd, day06
// "gioco del quindici"
// friol 2o22
//

#include "include/oppd.h"

class board15
{
private:

	int board[4][4];
	int emptyPosx;
	int emptyPosy;

public:

	board15()
	{
		int num = 1;
		for (int r = 0;r < 4;r++)
		{
			for (int c = 0;c < 4;c++)
			{
				board[r][c] = num;
				num++;
			}
		}

		emptyPosx = 3;
		emptyPosy = 3;
	}

	bool isInInitialState()
	{
		int num = 1;
		for (int r = 0;r < 4;r++)
		{
			for (int c = 0;c < 4;c++)
			{
				if (board[r][c] != num) return false;
				num++;
			}
		}

		return true;
	}

	void drawCard(int px, int py, wchar_t fb[][28])
	{
		wchar_t numbers[10] = { L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9' };

		int fbpx = px * 7;
		int fbpy = py * 4;

		fb[fbpy][fbpx] = L'╔';
		fb[fbpy][fbpx+1] = L'═';
		fb[fbpy][fbpx+2] = L'═';
		fb[fbpy][fbpx+3] = L'═';
		fb[fbpy][fbpx+4] = L'═';
		fb[fbpy][fbpx+5] = L'╗';
		fb[fbpy][fbpx+6] = L' ';

		fb[fbpy+1][fbpx] = L'║';
		fb[fbpy+1][fbpx+1] = L' ';
		if (this->board[py][px] < 10) fb[fbpy + 1][fbpx + 2] = L'0';
		else fb[fbpy + 1][fbpx + 2] = L'1';
		fb[fbpy + 1][fbpx + 3] = numbers[this->board[py][px] % 10];
		fb[fbpy + 1][fbpx + 4] = L' ';
		fb[fbpy + 1][fbpx + 5] = L'║';
		fb[fbpy + 1][fbpx + 6] = L'\\';

		fb[fbpy+2][fbpx] = L'╚';
		fb[fbpy + 2][fbpx + 1] = L'═';
		fb[fbpy + 2][fbpx + 2] = L'═';
		fb[fbpy + 2][fbpx + 3] = L'═';
		fb[fbpy + 2][fbpx + 4] = L'═';
		fb[fbpy + 2][fbpx + 5] = L'╝';
		fb[fbpy + 2][fbpx + 6] = L'|';

		fb[fbpy + 3][fbpx] = L' ';
		fb[fbpy + 3][fbpx + 1] = L'\\';
		fb[fbpy + 3][fbpx + 2] = L'_';
		fb[fbpy + 3][fbpx + 3] = L'_';
		fb[fbpy + 3][fbpx + 4] = L'_';
		fb[fbpy + 3][fbpx + 5] = L'_';
		fb[fbpy + 3][fbpx + 6] = L'\\';
	}

	void drawEmptyCard(int px, int py, wchar_t fb[][28])
	{
		int fbpx = px * 7;
		int fbpy = py * 4;

		for (int r = 0;r < 4;r++)
		{
			for (int c = 0;c < 7;c++)
			{
				fb[fbpy+r][fbpx+c] = L' ';
			}
		}
	}

	void shuffle()
	{
		srand((unsigned int)time(NULL));

		const int repetitions = 128;
		for (int m = 0;m < repetitions;m++)
		{
			int dir = rand() % 4;
			this->move(dir);
		}
	}

	void move(int dir)
	{
		// 0 up, 1 right, 2 down, 3 left
		if ((dir==0)&&(emptyPosy==3)) return;
		if ((dir==1)&&(emptyPosx==0)) return;
		if ((dir==2)&&(emptyPosy==0)) return;
		if ((dir==3)&&(emptyPosx==3)) return;

		if (dir == 0)
		{
			this->board[emptyPosy][emptyPosx] = this->board[emptyPosy + 1][emptyPosx];
			this->board[emptyPosy + 1][emptyPosx] = 16;
			emptyPosy += 1;
		}
		else if (dir == 1)
		{
			this->board[emptyPosy][emptyPosx] = this->board[emptyPosy][emptyPosx-1];
			this->board[emptyPosy][emptyPosx-1] = 16;
			emptyPosx -= 1;
		}
		else if (dir == 2)
		{
			this->board[emptyPosy][emptyPosx] = this->board[emptyPosy - 1][emptyPosx];
			this->board[emptyPosy - 1][emptyPosx] = 16;
			emptyPosy -= 1;
		}
		else if (dir == 3)
		{
			this->board[emptyPosy][emptyPosx] = this->board[emptyPosy][emptyPosx + 1];
			this->board[emptyPosy][emptyPosx + 1] = 16;
			emptyPosx += 1;
		}
	}

	void draw(wchar_t fb[][28])
	{
		for (int r = 0;r < 4;r++)
		{
			for (int c = 0;c < 4;c++)
			{
				if (this->board[r][c] != 16) this->drawCard(c, r, fb);
				else this->drawEmptyCard(c, r, fb);
			}
		}
	}
};

void bitblit(int dimx, int dimy, int posx, int posy, wchar_t fb[][28])
{
	for (int r = 0;r < dimy;r++)
	{
		consoleGotoxy(posx, posy + r);
		wchar_t line[29];
		line[28] = 0;
		for (int c = 0;c < dimx;c++)
		{
			line[c] = fb[r][c];
		}
		std::wcout << line;
	}
}

// ╔════╗
// ║ 01 ║
// ╚════╝
// \_____\|

void day06()
{
	const int fbdimx = 4 * 7;
	const int fbdimy = 4 * 4;

	setUnicodeConsole();
	clearScreen();
	consoleHideCursor();

	wchar_t framebuffer[fbdimy][fbdimx];
	for (int r = 0;r < fbdimy;r++)
	{
		for (int c = 0;c < fbdimx;c++)
		{
			framebuffer[r][c] = L' ';
		}
	}

	board15 board;
	board.shuffle();

	int moves = 0;
	bool goout = false;
	bool win = false;
	while (!goout)
	{
		board.draw(framebuffer);
		bitblit(fbdimx, fbdimy, 0, 1, framebuffer);
		consoleGotoxy(0, 0);
		std::wcout << "Moves: " << moves;

		if (board.isInInitialState())
		{
			win = true;
			goout = true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27) goout = true;
			else if (ch == 72) // up
			{
				board.move(0);
				moves++;
			}
			else if (ch == 80) // down
			{
				board.move(2);
				moves++;
			}
			else if (ch == 75) // left
			{
				board.move(3);
				moves++;
			}
			else if (ch == 77) // right
			{
				board.move(1);
				moves++;
			}
		}
	}

	consoleGotoxy(0, fbdimy + 1);
	if (win) std::wcout << "You won!" << std::endl;
	else std::wcout << "You pressed ESC." << std::endl;
	std::wcout << L"Total moves: " << moves << std::endl;
}
