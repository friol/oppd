//
// stop counter
// friol's oppd 2022
//

#include "include/oppd.h"

void day16readBestScores(int& iscoreEasy, int& iscoreNormal, int& iscoreHard)
{
	std::ifstream scoreFile;
	scoreFile.open("day16scores.txt", std::ios::in);

	if (scoreFile.is_open())
	{
		std::string curscoreEasy;
		std::getline(scoreFile, curscoreEasy);
		iscoreEasy = stoi(curscoreEasy);

		std::string curscoreNormal;
		std::getline(scoreFile, curscoreNormal);
		iscoreNormal = stoi(curscoreNormal);

		std::string curscoreHard;
		std::getline(scoreFile, curscoreHard);
		iscoreHard = stoi(curscoreHard);
	}
}

void day16usage(char** argv)
{
	std::cout << "Usage:" << std::endl << argv[0] << " [easy|normal|hard]" << std::endl;
}

void day16(int argc, char** argv)
{
	std::string difficulty = "normal";

	if (argc != 1)
	{
		if (argc != 2)
		{
			day16usage(argv);
			return;
		}
		else if ((strcmp(argv[1], "easy")) && (strcmp(argv[1], "normal")) && (strcmp(argv[1], "hard")))
		{
			day16usage(argv);
			return;
		}

		difficulty = argv[1];
	}

	std::map<std::string, int> targetMap;
	targetMap["easy"] = 100;
	targetMap["normal"] = 200;
	targetMap["hard"] = 500;

	//

	int scoreEasy = 999;
	int scoreNormal = 999;
	int scoreHard = 999;
	int curscore=0;
	day16readBestScores(scoreEasy, scoreNormal, scoreHard);
	if (difficulty == "easy") curscore = scoreEasy;
	else if (difficulty == "normal") curscore = scoreNormal;
	else if (difficulty == "hard") curscore = scoreHard;

	//

	consoleHideCursor();
	clearScreen();
	consoleGotoxy(0, 0);

	std::cout << "Press a key to counter stop at " << targetMap[difficulty] << std::endl;

	int cntr = 0;
	while (!_kbhit())
	{
		consoleGotoxy(0, 1);
		std::cout << cntr;

		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		cntr++;
	}

	int newscore = (int)fabs(cntr - targetMap[difficulty]);
	if (newscore < curscore)
	{
		std::cout << std::endl << "You've got a new highscore!" << std::endl;
	}

	consoleGotoxy(0, 2);
}
