//
// wordle clone
// oppd 2o22
// friol
//

#include "include/oppd.h"

class evaluation
{
private:

public:

	char character;
	int score; // 0=not in word, 1=in word but in the wrong place, 2=in word at the right place, 3=unknown (in status list)
};

void evaluateAnswer(std::string answer, std::string rightWord, std::vector<evaluation>& newTentative)
{
	int pos = 0;
	for (char& c : answer)
	{
		std::size_t found = rightWord.find(c);

		if (found != std::string::npos)
		{
			if (rightWord.at(pos) == c)
			{
				evaluation e;
				e.character = c;
				e.score = 2;
				newTentative.push_back(e);
			}
			else
			{
				// count number of chars c in rightWord and current tentative
				// if curr tentative is less, it's yellow; otherwise, it's grey

				std::string::difference_type nOfCInRightWord = std::count(rightWord.begin(), rightWord.end(), c);
				int nOfCInTentative = 0;
				for (evaluation& eva : newTentative)
				{
					if (eva.character == c) nOfCInTentative++;
				}

				if (nOfCInTentative < nOfCInRightWord)
				{
					evaluation e;
					e.character = c;
					e.score = 1;
					newTentative.push_back(e);
				}
				else
				{
					evaluation e;
					e.character = c;
					e.score = 0;
					newTentative.push_back(e);
				}
			}
		}
		else
		{
			evaluation e;
			e.character = c;
			e.score = 0;
			newTentative.push_back(e);
		}

		pos++;
	}

	// now we need a purge step
	for (evaluation& ev : newTentative)
	{
		char c = ev.character;

		std::string::difference_type nOfCInRightWord = std::count(rightWord.begin(), rightWord.end(), c);
		int nOfCInTentative = 0;
		for (evaluation& eva : newTentative)
		{
			if (eva.character == c) nOfCInTentative++;
		}

		if (nOfCInTentative > nOfCInRightWord)
		{
			// if there's a green c in tentative,
			// degrade yellow cs to gray

			bool greenC = false;
			for (evaluation& evc : newTentative)
			{
				if ((evc.character == c) && (evc.score == 2)) greenC = true;
			}

			if ((greenC)&&(ev.score == 1)) ev.score = 0;
		}
	}
}

void printStatus(std::vector<std::vector<evaluation>>& listOfTentatives)
{
	for (std::vector<evaluation>& tent : listOfTentatives)
	{
		int pos = 0;
		for (evaluation& ev : tent)
		{
			if (ev.score == 0)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 7);
				std::cout << ev.character;
			}
			else if (ev.score == 1)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 224);
				std::cout << ev.character;
			}
			else if (ev.score == 2)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
				std::cout << ev.character;
			}

			pos++;
		}

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 7);

		std::cout << std::endl;
	}

	std::cout << std::endl;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);
}

void initStatusList(std::vector<evaluation>& sl)
{
	std::string letters = "abcdefghijklmnopqrstuvwxyz";

	for (char& c : letters)
	{
		evaluation eva;
		eva.character = c;
		eva.score = 3;
		sl.push_back(eva);
	}
}

void updateStatusList(std::vector<evaluation>& sl, std::vector<evaluation>& newTentative)
{
	for (evaluation& ev : newTentative)
	{
		for (evaluation& evsl : sl)
		{
			if (evsl.character == ev.character)
			{
				if (ev.score == 0)
				{
					evsl.score = 0;
				}
				else if (ev.score == 1)
				{
					if ((evsl.score < 2) || (evsl.score == 3))
					{
						evsl.score = ev.score;
					}
				}
				else if (ev.score == 2)
				{
					evsl.score = 2;
				}
			}
		}
	}
}

void printStatusList(std::vector<evaluation>& sl)
{
	for (evaluation& ev : sl)
	{
		if (ev.score == 1)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 224);
		}
		else if (ev.score == 2)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
		}
		else if (ev.score == 0)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
		}
		else
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 7);
		}

		std::cout << ev.character;
	}
	std::cout << std::endl;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);
}

void day13()
{
	std::vector<std::string> listOfWordleWords;
	std::vector<std::string> listOfWinningWords;

	std::ifstream lvlFile;
	std::ifstream wwFile;
	lvlFile.open("wordlewords.txt", std::ios::in);
	if (lvlFile.is_open())
	{
		std::string scurline;
		while (std::getline(lvlFile, scurline))
		{
			listOfWordleWords.push_back(scurline);
		}
	}
	else
	{
		std::cout << "Error opening words file." << std::endl;
		return;
	}

	wwFile.open("wordle-nyt-answers-alphabetical.txt", std::ios::in);
	if (wwFile.is_open())
	{
		std::string scurline;
		while (std::getline(wwFile, scurline))
		{
			listOfWinningWords.push_back(scurline);
		}
	}
	else
	{
		std::cout << "Error opening winning words file." << std::endl;
		return;
	}

	srand((unsigned int)time(NULL));

	int winningwordNum = rand() % (int)listOfWinningWords.size();

	//std::cout << listOfWinningWords[winningwordNum] << std::endl;

	//

	std::vector<std::vector<evaluation>> listOfTentatives;
	
	std::vector<evaluation> statusList;
	initStatusList(statusList);

	int numTentativesRemaining = 6;
	bool goout = false;
	bool win = false;

	while ((!goout) && (numTentativesRemaining > 0))
	{
		std::string answer;
		std::cout << ">";
		std::cin >> answer;

		std::transform(answer.begin(), answer.end(), answer.begin(),[](unsigned char c) { return std::tolower(c); });

		if ((answer == "quit")||(answer=="exit"))
		{
			goout = true;
			win = false;
		}
		else if (answer == "status")
		{
			printStatusList(statusList);
		}
		else if (answer.size() != 5)
		{
			std::cout << "The word must be 5 characters long" << std::endl;
		}
		else if (std::find(listOfWordleWords.begin(), listOfWordleWords.end(), answer) == listOfWordleWords.end())
		{
			std::cout << "Word [" << answer << "] is not in the wordlist." << std::endl;
		}
		else if (answer == listOfWinningWords[winningwordNum])
		{
			std::cout << "You WON with " << 6-numTentativesRemaining+1 << " tentatives!" << std::endl;
			goout = true;
			win = true;
		}
		else
		{
			std::vector<evaluation> newTentative;
			evaluateAnswer(answer, listOfWinningWords[winningwordNum],newTentative);
			listOfTentatives.push_back(newTentative);
			printStatus(listOfTentatives);
			updateStatusList(statusList, newTentative);
			numTentativesRemaining--;
		}
	}

	if (!win)
	{
		std::cout << "Word was " << listOfWinningWords[winningwordNum] << std::endl;
	}
}
