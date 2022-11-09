//
// special bonus
// oppd
// friol 2k...argh, too tired to write the year
//

#include "include/oppd.h"

int playRPS(char* usersMove)
{
	const char* pcsmove[3] = { "rock","paper","scissors" };

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

void day09readScores(int& cpuWins, int& humanWins, int& ties)
{
	std::ifstream scoreFile;
	scoreFile.open("day09score.txt", std::ios::in);

	if (scoreFile.is_open())
	{
		std::string scpuwins;
		std::string shumanwins;
		std::string sties;
		std::getline(scoreFile, scpuwins);
		cpuWins = stoi(scpuwins);
		std::getline(scoreFile, shumanwins);
		humanWins = stoi(shumanwins);
		std::getline(scoreFile, sties);
		ties = stoi(sties);
	}
}

void day09writeNewScores(int cpuWins, int humanWins, int ties)
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

void day09bis(int argc, char** argv)
{
	if (argc != 2)
	{
		day09usage(argv);
		return;
	}

	if ((strcmp(argv[1], "rock")) && (strcmp(argv[1], "paper")) && (strcmp(argv[1], "scissors")))
	{
		day09usage(argv);
		return;
	}

	int cpuwins = 0;
	int humanwins = 0;
	int ties = 0;

	day09readScores(cpuwins, humanwins, ties);

	int result = playRPS(argv[1]);

	if (result == 0) cpuwins++;
	else if (result == 1) ties++;
	else if (result == 2) humanwins++;

	day09writeNewScores(cpuwins, humanwins, ties);

	std::cout << "Computer wins: " << cpuwins << " Humans wins: " << humanwins << " Ties: " << ties << std::endl;
}
