
//
// friol's easyest game(s) ever
// oppd 2k22
//

#include "include/oppd.h"
/*
int playRPS(char* usersMove)
{
	const char* pcsmove[3] = {"rock","paper","scissors"};

	srand((unsigned int)time(NULL));
	int computersMove = rand() % 3;
	int humansMove = -1;

	if (!strcmp(usersMove, "rock")) humansMove = 0;
	if (!strcmp(usersMove, "paper")) humansMove = 1;
	if (!strcmp(usersMove, "scissors")) humansMove = 2;

	int winMatrix[3][3] = {
		{1,2,0},
		{0,1,2},
		{2,0,1}
	};

	std::cout << "Computer played " << pcsmove[computersMove] << "." << std::endl;

	if (winMatrix[computersMove][humansMove] == 0) 
	{
		std::cout << "Computer wins." << std::endl; 
		return 0;
	}
	
	if (winMatrix[computersMove][humansMove] == 1)
	{
		std::cout << "It's a tie." << std::endl;
		return 1;
	}

	if (winMatrix[computersMove][humansMove] == 2)
	{
		std::cout << "Humans win." << std::endl;
		return 2;
	}

	return -1;
}

void day09readScores(int& cpuWins,int& humanWins, int& ties)
{
	std::ifstream scoreFile;
	scoreFile.open("day09score.txt", std::ios::in);

	if (scoreFile.is_open())
	{
		std::string scpuwins;
		std::string shumanwins;
		std::string sties;
		std::getline(scoreFile, scpuwins);
		cpuWins=stoi(scpuwins);
		std::getline(scoreFile, shumanwins);
		humanWins = stoi(shumanwins);
		std::getline(scoreFile, sties);
		ties = stoi(sties);
	}
}

void day09writeNewScores(int cpuWins,int humanWins,int ties)
{
	std::ofstream scoreFile;
	scoreFile.open("day09score.txt", std::ios::out);
	if (scoreFile.is_open())
	{
		std::string newcpuwins = std::to_string(cpuWins);
		scoreFile << newcpuwins << std::endl;
		std::string newhumanwins = std::to_string(humanWins);
		scoreFile << newhumanwins << std::endl;
		std::string newties = std::to_string(ties);
		scoreFile << newties << std::endl;
	}
}

void day09usage(char** argv)
{
	std::cout << "Usage:" << std::endl << argv[0] << " [rock|paper|scissors]" << std::endl;
}

void day09(int argc, char** argv)
{
	if (argc != 2)
	{
		day09usage(argv);
		return;
	}

	if ((strcmp(argv[1], "rock")) && (strcmp(argv[1], "paper")) && (strcmp(argv[1], "scissors")) )
	{
		day09usage(argv);
		return;
	}

	int cpuwins = 0;
	int humanwins = 0;
	int ties = 0;

	day09readScores(cpuwins, humanwins, ties);

	int result=playRPS(argv[1]);

	if (result == 0) cpuwins++;
	else if (result == 1) ties++;
	else if (result == 2) humanwins++;

	day09writeNewScores(cpuwins, humanwins, ties);

	std::cout << "Computer wins: " << cpuwins << " Humans wins: " << humanwins << " Ties: " << ties << std::endl;
}
*/

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
			//std::cout << scurline << std::endl;
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
				else
				{
					std::cout << "Error: unknonw char " << c << " in level file." << std::endl;
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
		std::cout << "Cannot open level file." << std::endl;
		return false;
	}

	return true;
}

void renderSokoScenario(std::vector<std::vector<int>>& scenario,std::vector<std::vector<int>>& listOfBarrels,std::vector<std::vector<int>>& listOfEndpoints,int pposx,int pposy)
{
	for (int r = 0;r < scenario.size();r++)
	{
		for (int c = 0;c < scenario[r].size();c++)
		{
			consoleGotoxy(c, r);
			if (scenario[r][c] == 1) std::cout << "#";
			else std::cout << " ";
		}
	}

	for (std::vector<int>& e : listOfEndpoints)
	{
		consoleGotoxy(e[0], e[1]);
		std::cout << ".";
	}

	for (std::vector<int>& b : listOfBarrels)
	{
		consoleGotoxy(b[0], b[1]);
		std::cout << "$";
	}

	consoleGotoxy(pposx, pposy);
	std::cout << "@";
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

void day09(int argc, char** argv)
{
	// read soko level
	std::vector<std::vector<int>> scenario;
	std::vector<std::vector<int>> listOfBarrels;
	std::vector<std::vector<int>> listOfEndpoints;
	int playerposx, playerposy;

	if (readSokoLevel("./day09.level1.txt", scenario, listOfBarrels, listOfEndpoints, playerposx, playerposy) != true)
	{
		std::cout << "Error reading level file." << std::endl;
		return;
	}

	// init

	consoleHideCursor();

	// gameloop

	int numberOfMoves = 0;
	bool goOut = false;
	bool win = false;
	while ((!goOut) && (!win))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27) goOut = true;
			if (ch == 72) // up
			{
				performMove(0, playerposx, playerposy, listOfBarrels, scenario);
			}
			else if (ch == 80) // down
			{
				performMove(2, playerposx, playerposy, listOfBarrels, scenario);
			}
			else if (ch == 75) // left
			{
				performMove(3, playerposx, playerposy, listOfBarrels, scenario);
			}
			else if (ch == 77) // right
			{
				performMove(1, playerposx, playerposy, listOfBarrels, scenario);
			}
		}

		renderSokoScenario(scenario, listOfBarrels, listOfEndpoints, playerposx, playerposy);

		if (sokobanIsWinPosition(listOfBarrels, listOfEndpoints))
		{
			goOut = true;
			win = true;
		}
	}

	consoleGotoxy(0, 20);

	if (win) std::cout << "You WON!" << std::endl;
}
