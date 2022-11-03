//
// day3 - typing game
// friol, oppd 2022
//

#include "include/oppd.h"

// from https://stackoverflow.com/questions/479080/trim-is-not-part-of-the-standard-c-c-library
const std::string whiteSpaces(" \f\n\r\t\v");

void trimRight(std::string& str,
	const std::string& trimChars = whiteSpaces)
{
	std::string::size_type pos = str.find_last_not_of(trimChars);
	str.erase(pos + 1);
}


void trimLeft(std::string& str,
	const std::string& trimChars = whiteSpaces)
{
	std::string::size_type pos = str.find_first_not_of(trimChars);
	str.erase(0, pos);
}


void trim(std::string& str, const std::string& trimChars = whiteSpaces)
{
	trimRight(str, trimChars);
	trimLeft(str, trimChars);
}

int readBestScore()
{
	std::ifstream scoreFile;
	scoreFile.open("day03score.txt", std::ios::in );

	if (scoreFile.is_open())
	{
		std::string curscore;
		std::getline(scoreFile, curscore);
		return stoi(curscore);
	}

	return 0;
}

void writeNewScore(int newscore)
{
	std::ofstream scoreFile;
	scoreFile.open("day03score.txt", std::ios::out);
	if (scoreFile.is_open())
	{
		std::string snewscore= std::to_string(newscore);
		scoreFile << snewscore;
	}
}

void day03()
{
	// get random quote from https://zenquotes.io/api/random
	httplib::Client cli("https://zenquotes.io");
	auto res = cli.Get("/api/random");

	if (res->status != 200)
	{
		std::cout << "Unable to reach zenquotes.io" << std::endl;
		return;
	}

	int bestScore = readBestScore();
	std::cout << "Current highscore:" << bestScore << std::endl;

	// 3,2,1 countdown phase
	int countdown = 0;
	bool countdownEnds = false;
	while (!countdownEnds)
	{
		if (countdown == 0) std::cout << "3...";
		if (countdown == 50) std::cout << "2...";
		if (countdown == 100) std::cout << "1...";
		if (countdown == 150) 
		{
			std::cout << std::endl;  
			countdownEnds = true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		countdown++;
	}

	// game loop

	json quote = json::parse(res->body);

	std::string sentence = quote[0]["q"];
	std::transform(sentence.begin(), sentence.end(), sentence.begin(),[](unsigned char c) { return std::tolower(c); });
	trim(sentence);
	std::cout << sentence << std::endl;

	int cursorYpos = GetConsoleCursorPosition().Y;
	consoleGotoxy(0,cursorYpos-1);

	int hseconds = 0;
	int stringPos = 0;
	int numErrors = 0;
	int numWords = 0;
	bool win = false;
	bool goOut = false;
	while (!goOut)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		hseconds += 1;

		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27) goOut = true;
			else if (((ch >= 'a') && (ch <= 'z')) || (ch == ' ') || (ch == ':') || (ch == ';') || (ch==',') || (ch == '-') || (ch=='.') || (ch=='%') || (ch == '\'') || ((ch >= '0') && (ch <= '9')))
			{
				if (((char)ch) == (char)sentence[stringPos])
				{
					consolePrintReversedChar((char)ch,0);
					stringPos++;

					if ((ch == ' ') || (ch == ',') || (ch == '.'))
					{
						numWords += 1;
					}
				}
				else
				{
					consolePrintReversedChar((char)sentence[stringPos], 1);
					int cposx = GetConsoleCursorPosition().X;
					int cposy = GetConsoleCursorPosition().Y;
					consoleGotoxy(cposx - 1, cposy);
					numErrors += 1;
				}
			}
		}

		if (stringPos == sentence.size())
		{
			goOut = true;
			win = true;
		}
	}

	resetConsole();
	float secsElapsed = (((float)hseconds * 10.0f) / 1000.0f);
	if (win == false)
	{
		std::cout << std::endl << "You pressed ESC." << std::endl;
	}
	else
	{
		std::cout << std::endl << "Sentence completed!" << std::endl;
	}
	
	// words/secondsElapsed=wpm/60
	float cps = ((float)stringPos) / secsElapsed;
	float wpm = (float)((60.0 * numWords) / (float)secsElapsed);
	std::cout << "Seconds elapsed:" << secsElapsed << std::endl;
	std::cout << "Chars per second:" << cps << std::endl;
	std::cout << "Words per minute:" << wpm << std::endl;
	std::cout << "Errors:" << numErrors << std::endl;

	if (win == true)
	{
		int finalScore = (int)((cps * 100) - (numErrors * 5) + (stringPos * 10));
		std::cout << "Final score:" << finalScore << std::endl;
		if (finalScore > bestScore)
		{
			writeNewScore(finalScore);
			std::cout << "You have a new high score!" << std::endl;
		}
	}
}
