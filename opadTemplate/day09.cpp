
//
// friol's easyest game ever
// oppd 2k22
//

#include "include/oppd.h"

// http://www.sokobano.de/wiki/index.php?title=Level_format
bool readSokoLevel(std::string levelFile, std::vector<std::vector<int>>& scenario, std::vector<std::vector<int>>& listOfBarrels, std::vector<std::vector<int>>& listOfEndpoints, int& pposx, int& pposy)
{
	std::ifstream lvlFile;
	lvlFile.open(levelFile, std::ios::in);

	if (lvlFile.is_open())
	{
		int row = 0;
		int col = 0;
		std::string scurline;
		while (std::getline(lvlFile, scurline))
		{
			std::vector<int> curScenarioLine;

			col = 0;
			for (char& c : scurline)
			{
				if (c == '#')
				{
					curScenarioLine.push_back(1); // wall
				}
				else if (c == ' ')
				{
					curScenarioLine.push_back(0); // space
				}
				else if (c == '$') // barrel
				{
					std::vector<int> b = { col,row };
					listOfBarrels.push_back(b);
					curScenarioLine.push_back(0); // space
				}
				else if (c == '.') // endpoints
				{
					std::vector<int> e = { col,row };
					listOfEndpoints.push_back(e);
					curScenarioLine.push_back(0); // space
				}
				else if (c == '@') // player
				{
					pposx = col;
					pposy = row;
					curScenarioLine.push_back(0); // space
				}
				else if (c == '+') // player on endpoint
				{
					pposx = col;
					pposy = row;
					std::vector<int> e = { col,row };
					listOfEndpoints.push_back(e);
					curScenarioLine.push_back(0); // space
				}
				else if (c == '*') // box on endpoint
				{
					std::vector<int> b = { col,row };
					listOfBarrels.push_back(b);
					std::vector<int> e = { col,row };
					listOfEndpoints.push_back(e);
					curScenarioLine.push_back(0); // space
				}
				else
				{
					std::wcout << "Error: unknonw char " << c << " in level file." << std::endl;
					return false;
				}

				col++;
			}

			scenario.push_back(curScenarioLine);
			row++;
		}
	}
	else
	{
		std::wcout << "Cannot open level file." << std::endl;
		return false;
	}

	return true;
}

void renderSokoScenario(std::vector<std::vector<int>>& scenario,std::vector<std::vector<int>>& listOfBarrels,std::vector<std::vector<int>>& listOfEndpoints,int pposx,int pposy,wchar_t framebuffer[][80])
{
	for (int r = 0;r < scenario.size();r++)
	{
		for (int c = 0;c < scenario[r].size();c++)
		{
			//consoleGotoxy(c*2, r*2);
			if (scenario[r][c] == 1)
			{
				framebuffer[r * 2][c * 2] = L'░';
				framebuffer[r * 2][c * 2+1] = L'░';
				framebuffer[r * 2+1][c * 2] = L'░';
				framebuffer[r * 2+1][c * 2+1] = L'░';
			}
			else
			{
				framebuffer[r * 2][c * 2] = L' ';
				framebuffer[r * 2][c * 2 + 1] = L' ';
				framebuffer[r * 2 + 1][c * 2] = L' ';
				framebuffer[r * 2 + 1][c * 2 + 1] = L' ';
			}
		}
	}

	for (std::vector<int>& e : listOfEndpoints)
	{
		framebuffer[e[1] * 2][e[0] * 2] = L'┌';
		framebuffer[e[1] * 2][e[0] * 2+1] = L'┐';
		framebuffer[e[1] * 2+1][e[0] * 2] = L'└';
		framebuffer[e[1] * 2+1][e[0] * 2+1] = L'┘';
	}

	for (std::vector<int>& b : listOfBarrels)
	{
		framebuffer[b[1] * 2][b[0] * 2] = L'▓';
		framebuffer[b[1] * 2][b[0] * 2 + 1] = L'▓';
		framebuffer[b[1] * 2 + 1][b[0] * 2] = L'▓';
		framebuffer[b[1] * 2 + 1][b[0] * 2 + 1] = L'▓';
	}
	
	framebuffer[pposy * 2][pposx * 2] = L'\\';
	framebuffer[pposy * 2][pposx * 2+1] = L'/';
	framebuffer[pposy * 2+1][pposx * 2] = L'/';
	framebuffer[pposy * 2+1][pposx * 2+1] = L'\\';
}

bool theresAbarrel(int x, int y, std::vector<std::vector<int>>& listOfBarrels)
{
	for (std::vector<int>& b : listOfBarrels)
	{
		if ((x == b[0]) && (y == b[1]))
		{
			return true;
		}
	}

	return false;
}

void moveBarrel(std::vector<std::vector<int>>& listOfBarrels, int bx, int by, int direction)
{
	int increments[4][2] = {
		{0,-1},
		{1,0},
		{0,1},
		{-1,0}
	};

	for (std::vector<int>& b : listOfBarrels)
	{
		if ((b[0] == bx) && (b[1] == by))
		{
			b[0] += increments[direction][0];
			b[1] += increments[direction][1];
		}
	}
}

void performMove(int direction,int& pposx, int& pposy, std::vector<std::vector<int>>& listOfBarrels, std::vector<std::vector<int>>& scenario)
{
	int increments[4][2] = {
		{0,-1},
		{1,0},
		{0,1},
		{-1,0}
	};

	if (scenario[pposy +increments[direction][1]][pposx + increments[direction][0]] == 1)
	{
		// can't move, there's a wall
	}
	else if (theresAbarrel(pposx + increments[direction][0], pposy + increments[direction][1], listOfBarrels))
	{
		if ((scenario[pposy + increments[direction][1]*2][pposx + increments[direction][0] * 2] == 1) || (theresAbarrel(pposx + increments[direction][0] * 2, pposy + increments[direction][1] * 2, listOfBarrels)))
		{
			// can't move
		}
		else
		{
			// move and move barrel
			moveBarrel(listOfBarrels, pposx + increments[direction][0], pposy + increments[direction][1], direction);
			pposx += increments[direction][0];
			pposy += increments[direction][1];
		}
	}
	else
	{
		pposx += increments[direction][0];
		pposy += increments[direction][1];
	}

}

bool sokobanIsWinPosition(std::vector<std::vector<int>>& listOfBarrels, std::vector<std::vector<int>>& listOfEndpoints)
{
	for (std::vector<int>& b : listOfBarrels)
	{
		bool found = false;
		for (std::vector<int>& e : listOfEndpoints)
		{
			if ((b[0] == e[0]) && (b[1] == e[1]))
			{
				found = true;
			}
		}

		if (!found) return false;
	}

	return true;
}

void day09bitblit(int dimx, int dimy, int posx, int posy, wchar_t fb[][80])
{
	for (int r = 0;r < dimy;r++)
	{
		consoleGotoxy(posx, posy + r);
		wchar_t line[81];
		line[80] = 0;
		for (int c = 0;c < dimx;c++)
		{
			line[c] = fb[r][c];
		}
		std::wcout << line;
	}
}

void day09sokoUsage(char** argv)
{
	std::cout << "Usage:" << std::endl << argv[0] << " [levelFilename]" << std::endl;
}

void day09(int argc, char** argv)
{
	// read soko level
	std::string sokoLevelName = "./day09.level1.txt";
	std::vector<std::vector<int>> scenario;
	std::vector<std::vector<int>> listOfBarrels;
	std::vector<std::vector<int>> listOfEndpoints;
	int playerposx, playerposy;

	if (argc == 2)
	{
		sokoLevelName = argv[1];
	}
	else if (argc > 2)
	{
		day09sokoUsage(argv);
	}

	if (readSokoLevel(sokoLevelName, scenario, listOfBarrels, listOfEndpoints, playerposx, playerposy) != true)
	{
		std::wcout << "Error reading level file." << std::endl;
		return;
	}

	// init

	setUnicodeConsole();
	consoleHideCursor();
	clearScreen();

	// gameloop

	const int fbwidth = 80;
	const int fbheight = 20;
	wchar_t framebuffer[fbheight][fbwidth];
	for (int r = 0;r < fbheight;r++)
	{
		for (int c = 0;c < fbwidth;c++)
		{
			framebuffer[r][c] = L' ';
		}
	}

	int numberOfMoves = 0;
	bool goOut = false;
	bool win = false;
	while ((!goOut) && (!win))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		int keyCode[4] = { 72,77,80,75 };

		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27) goOut = true;
			for (int dir = 0;dir < 4;dir++)
			{
				if (ch == keyCode[dir])
				{
					numberOfMoves++;
					performMove(dir, playerposx, playerposy, listOfBarrels, scenario);
				}

			}
		}

		renderSokoScenario(scenario, listOfBarrels, listOfEndpoints, playerposx, playerposy,framebuffer);
		consoleGotoxy(0, 0);
		std::wcout << "Moves: " << numberOfMoves;
		day09bitblit(fbwidth, fbheight, 0, 1, framebuffer);

		if (sokobanIsWinPosition(listOfBarrels, listOfEndpoints))
		{
			goOut = true;
			win = true;
		}
	}

	consoleGotoxy(0, 20);

	if (win) std::wcout << "You WON!" << std::endl;
	std::wcout << "Number of moves: " << numberOfMoves << std::endl;
}
