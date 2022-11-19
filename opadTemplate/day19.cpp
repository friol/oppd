
//
// oppd - slots!
// friol 2k22
//

#include "include/oppd.h"

class slotmachine
{
private:

#define numGears 3
#define numOfIconsPerGear 16
	std::vector<wchar_t> gears[numGears];

public:

	slotmachine()
	{
		wchar_t matrix[numOfIconsPerGear] = { L'*',L'$',L'@',L'▒',L'?',L'▓',L'%',L'¾',L'█',L'ß',L'¼',L'½',L'¥',L'#',L'£',L'§' };

		// init gears
		for (int g = 0;g < numGears;g++)
		{
			for (int el = 0;el < numOfIconsPerGear;el++)
			{
				gears[g].push_back(matrix[el]);
			}
		}

	}

	void printGears(int x,int y,bool isLastSpin)
	{
		consoleGotoxy(1 + x, y + 1);

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		std::wcout << L"╔═════╗";
		SetConsoleTextAttribute(hConsole, 7);

		int yg = (numOfIconsPerGear / 2) - 1;
		for (int r = 0;r < 3;r++)
		{
			if ((r == 0)||(r==2))
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole,8);
			}

			consoleGotoxy(2+x, r+2+y);
			for (int g = 0;g < numGears;g++)
			{
				std::wcout << gears[g][yg] << L' ';
			}

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 7);

			if ((r==1)&&(!isLastSpin)) std::wcout << L'|';
			else if ((r==1)&&(isLastSpin)) std::wcout << L'O';
			if ((r == 2) && (!isLastSpin))
			{
				std::wcout << L'|';
			}
			else std::wcout << L" ";

			yg++;
		}

		SetConsoleTextAttribute(hConsole, 12);
		consoleGotoxy(1 + x, y + 5);
		std::wcout << L"╚═════╝";
		
		if (!isLastSpin)
		{
			SetConsoleTextAttribute(hConsole, 14);
			std::wcout << L"O";
		}
		else std::wcout << L" ";

		consoleGotoxy(1 + x, y + 6);
		SetConsoleTextAttribute(hConsole, 12);
		std::wcout << L"╔═════╗";
		SetConsoleTextAttribute(hConsole, 7);
	}

	int spinSlot(int bet, int budget,std::string& statusLine)
	{
		statusLine = "";

		if (bet > budget)
		{
			statusLine = "You don't have that amount of dollars.";
			return budget;
		}

		if ((bet != 1) && (bet != 5) && (bet != 10))
		{
			statusLine = "Only 1$, 5$ and 10$ bets are allowed.";
			return budget;
		}

		int newbudget = budget;
		const int minNumSpins = 50;
		const int adder = rand() % 50;
		int numSpins = minNumSpins + adder;

		int totalWon = 0;

		int s = 0;
		while (s < numSpins)
		{
			for (int g = 0;g < numGears;g++)
			{
				int rotAmount = rand() % 3;
				std::rotate(gears[g].begin(), gears[g].begin() +rotAmount+ 1, gears[g].end());
			}

			bool lastSpin = false;
			if (s == (numSpins - 1)) lastSpin = true;
			printGears(0, 0,lastSpin);

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			s++;
		}

		// check result of spin
		// 1 $ sign gives you back 1$
		// 2 equal signs give you back 2x the bet
		// 3 equal signs give you back 10x the bet

		bool awin = false;
		for (int g = 0;g < numGears;g++)
		{
			if (gears[g][(numOfIconsPerGear / 2)] == L'$')
			{
				statusLine += "You got a $.";
				totalWon += 1;
				newbudget++;
			}
		}

		if (
			(gears[0][(numOfIconsPerGear / 2)] == gears[1][(numOfIconsPerGear / 2)]) &&
			(gears[1][(numOfIconsPerGear / 2)] == gears[2][(numOfIconsPerGear / 2)])
			)
		{
			awin = true;
			newbudget += bet * 10;
			totalWon += bet * 10;
			statusLine += "Three in a row!";
		}
		else
		{
			if (
				(gears[0][(numOfIconsPerGear / 2)] == gears[1][(numOfIconsPerGear / 2)]) ||
				(gears[1][(numOfIconsPerGear / 2)] == gears[2][(numOfIconsPerGear / 2)]) ||
				(gears[0][(numOfIconsPerGear / 2)] == gears[2][(numOfIconsPerGear / 2)])
				)
			{
				awin = true;
				newbudget += bet * 2;
				totalWon += bet * 2;
				statusLine += "Two in a row!";
			}
		}
		
		if (!awin)
		{
			newbudget -= bet;
			statusLine += "You loose "+std::to_string(bet)+"$.";
		}
		else
		{
			statusLine += " You won " + std::to_string(totalWon) + "$!";
		}

		return newbudget;
	}
};

void day19()
{
	srand((unsigned int)time(NULL));

	// 100$ starting budget
	int yourBudget = 100;

	slotmachine theSlot;

	setUnicodeConsole();
	clearScreen();

	theSlot.printGears(0, 0,true);

	// gameloop

	bool exitGame = false;
	while (!exitGame)
	{
		consoleGotoxy(0, 0);
		std::wcout << "                            ";
		consoleGotoxy(0, 0);
		std::wcout << "Your budget: " << yourBudget << "$" << std::endl;

		consoleGotoxy(0, 10);

		std::string answer;
		std::wcout << "Enter your bet amount in dollars (1,5,10) or type 'exit' to quit" << std::endl;
		std::wcout << ">                                          ";
		consoleGotoxy(1, 11);

		std::cin >> answer;

		if ((answer == "exit") || (answer == "quit")||(answer=="q"))
		{
			exitGame = true;
		}
		else
		{
			int yourBet = 0;
			
			if ((answer != "1") && (answer != "5") && (answer != "10"))
			{
				yourBet = 0;
			}
			else yourBet=stoi(answer);

			// do a spin and update yourbudget
			consoleHideCursor();

			std::string statusLine;
			int newBudget = theSlot.spinSlot(yourBet, yourBudget,statusLine);

			consoleGotoxy(0, 8);
			std::wcout << "                                                                         ";
			consoleGotoxy(0, 8);
			std::wstring widestr = std::wstring(statusLine.begin(), statusLine.end());
			std::wcout << widestr;

			consoleShowCursor();

			yourBudget = newBudget;
		}
	}
}
