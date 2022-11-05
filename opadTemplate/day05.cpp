//
// oppd 2o22
// day05 - reptilian game
//

#include "include/oppd.h"

class snake
{
private:

	std::vector<std::vector<int>> body;
	unsigned int direction; // 0 up, 1 right, 2 down, 3 left
	int growPhase;

	int increment[4][2] = {
		{0,-1},{1,0},{0,1},{-1,0}
	};

public:

	snake(int boardDimX,int boardDimY)
	{
		const int snakeLength = 5;
		this->direction = 1;
		this->growPhase = 0;
		int iniPosX = boardDimX / 2;
		int iniPosY = boardDimY / 2;

		for (int p = 0;p < snakeLength;p++)
		{
			std::vector<int> pos = { iniPosX,iniPosY };
			this->body.push_back(pos);
			iniPosX -= this->increment[this->direction][0];
			iniPosY -= this->increment[this->direction][1];
		}
	}

	void startGrowthProcess()
	{
		this->growPhase = 3;
	}

	void render(int posx,int posy, wchar_t fb[][80])
	{
		for (int el = 0;el < this->body.size();el++)
		{
			fb[posy + this->body[el][1]][posx+this->body[el][0]]=L'▒';
		}
	}

	int snizzle(int board[][80],int&hpx,int&hpy)
	{
		std::vector<int> newEl = { this->body[0][0],this->body[0][1] };
		newEl[0] += this->increment[this->direction][0];
		newEl[1] += this->increment[this->direction][1];
		this->body.insert(this->body.begin(), newEl);
		
		if (this->growPhase == 0)
		{
			this->body.pop_back();
		}
		else
		{
			this->growPhase--;
		}

		hpx = newEl[0];
		hpy = newEl[1];

		int val = board[newEl[1]][newEl[0]];

		// 0 all ok, 1 hit a fruit, 2 hit a wall
		
		// check if we hit ourselves
		for (int p = 1;p < this->body.size();p++)
		{
			if ((newEl[0] == this->body[p][0]) && (newEl[1] == this->body[p][1])) return 2;
		}

		if (val == 0) return 0;
		if (val == 1) return 2;
		if (val == 2) return 1;

		return 0;
	}

	void newdir(int dir)
	{
		// avoid opposite directions
		if ((this->direction == 0) && (dir == 2)) return;
		if ((this->direction == 2) && (dir == 0)) return;
		if ((this->direction == 1) && (dir == 3)) return;
		if ((this->direction == 3) && (dir == 1)) return;

		this->direction = dir;
	}

};

//

void render(int board[][80], int dimx, int dimy,wchar_t fb[][80])
{
	for (int r = 0;r < dimy;r++)
	{
		for (int c = 0;c < dimx;c++)
		{
			if (board[r][c] == 1)
			{
				fb[r][c]= L'█';
			}
			else if (board[r][c]==0)
			{
				fb[r][c] = L' ';
			}
			else if (board[r][c] == 2)
			{
				fb[r][c] = L'F';
			}
		}
	}

}

void bitblit(int dimx, int dimy, int posx, int posy, wchar_t fb[][80])
{
	for (int r = 0;r < dimy;r++)
	{
		consoleGotoxy(posx, posy+r);
		wchar_t line[81];
		line[80] = 0;
		for (int c = 0;c < dimx;c++)
		{
			line[c]=fb[r][c];
		}
		std::wcout << line ;
	}
}

void newRandomFruit(int board[][80], int dimx, int dimy)
{
	srand((unsigned int)time(NULL));

	int randx = rand() % (dimx - 2);
	int randy = rand() % (dimy - 2);

	board[randy + 1][randx + 1] = 2;
}

void printStatusBar(int s)
{
	consoleGotoxy(0, 0);
	std::wcout << L"╔══════════════════════════════════════════════════════════════════════════════╗" << std::endl;
	std::wcout << L"║ OPPD Snake                                                         Score: " << std::setw(3) << s << L"║" << std::endl;
	std::wcout << L"╚══════════════════════════════════════════════════════════════════════════════╝" << std::endl;
}

int day05readBestScore()
{
	std::ifstream scoreFile;
	scoreFile.open("day05score.txt", std::ios::in);

	if (scoreFile.is_open())
	{
		std::string curscore;
		std::getline(scoreFile, curscore);
		return stoi(curscore);
	}

	return 0;
}

void day05writeNewScore(int newscore)
{
	std::ofstream scoreFile;
	scoreFile.open("day05score.txt", std::ios::out);
	if (scoreFile.is_open())
	{
		std::string snewscore = std::to_string(newscore);
		scoreFile << snewscore;
	}
}

void day05()
{
	int curScore = 0;
	const int boardDimX = 80;
	const int boardDimY = 20;

	int board[boardDimY][boardDimX];

	for (int r = 0;r < boardDimY;r++)
	{
		for (int c = 0;c < boardDimX;c++)
		{
			if ((r == 0) || (c == 0) || (r == boardDimY - 1) || (c == boardDimX - 1)) board[r][c] = 1; // wall
			else board[r][c] = 0; // empty place
		}
	}

	snake theSnake(boardDimX, boardDimY);

	setUnicodeConsole();
	clearScreen();
	consoleHideCursor();

	wchar_t framebuffer[boardDimY][boardDimX];
	for (int r = 0;r < boardDimY;r++)
	{
		for (int c = 0;c < boardDimX;c++)
		{
			framebuffer[r][c] = L' ';
		}
	}

	// gameloop

	int maxScore = day05readBestScore();

	bool goout = false;
	newRandomFruit(board, boardDimX, boardDimY);
	while (!goout)
	{
		printStatusBar(curScore);
		render(board, boardDimX, boardDimY,framebuffer);
		theSnake.render(0, 0,framebuffer);
		bitblit(boardDimX, boardDimY, 0, 3, framebuffer);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27) goout = true;
			else if (ch == 72) // up
			{
				theSnake.newdir(0);
			}
			else if (ch == 80) // down
			{
				theSnake.newdir(2);
			}
			else if (ch == 75) // left
			{
				theSnake.newdir(3);
			}
			else if (ch == 77) // right
			{
				theSnake.newdir(1);
			}
		}

		int hpx, hpy;
		int res=theSnake.snizzle(board,hpx,hpy);
		if (res == 2) goout = true; // hit a wall
		if (res == 1) // eaten a fruit
		{
			board[hpy][hpx] = 0;
			curScore += 10;
			newRandomFruit(board, boardDimX, boardDimY);
			theSnake.startGrowthProcess();
		}
	}

	consoleGotoxy(0, boardDimY + 3);
	std::wcout << "Game over!" << std::endl;

	if (curScore > maxScore)
	{
		std::wcout << "You've got a new high score!" << std::endl;
		day05writeNewScore(curScore);
	}
}
