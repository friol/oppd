
//
// this is the last one
// oppd's car race
// friol 2k22
//

#include "include/oppd.h"

#define fbDimX 80
#define fbDimY 30
#define winningLine 70

class oppdcar
{
private:
	int px, py;
	int velocity;
	bool inPanne; // italian for "broken down" :)
	int panneCounter;

public:

	oppdcar(int x, int y)
	{
		this->px = x;
		this->py = y;
		this->velocity = 0;
		this->inPanne = false;
	}

	void draw(wchar_t fb[][fbDimX],int carNumber)
	{
		wchar_t carArr[3][10] = {
			{L"  o   o  "},
			{L"||=[*]==]"},
			{L"  o   o  "}
		};

		wchar_t numArr[4] = { L'1',L'2',L'3',L'4' };

		for (int r = 0;r < 3;r++)
		{
			for (int c = 0;c < 10;c++)
			{
				if (carArr[r][c] == L'*')
				{
					fb[this->py + r][this->px + c] = numArr[carNumber];
				}
				else
				{
					fb[this->py + r][this->px + c] = carArr[r][c];
				}
			}
		}
	}

	bool isWinner(int finishLineX)
	{
		if (this->px >= (finishLineX - 9))
		{
			return true;
		}

		return false;
	}

	void update()
	{
		if (this->inPanne)
		{
			this->panneCounter--;
			if (this->panneCounter == 0)
			{
				this->inPanne = false;
			}
		}
		else
		{
			int rndnum = rand() % 10;
			// x% probability of engine failure
			if (rndnum >= 7)
			{
				// car breaks down
				int numStopTurns = 1+(rand() % 4);
				this->inPanne = true;
				this->panneCounter = numStopTurns;
			}
			else
			{
				// car goes on
				this->px++;
			}
		}
	}
};

void clearFb(wchar_t fb[][fbDimX])
{
	for (int r = 0;r < fbDimY;r++)
	{
		for (int c = 0;c < fbDimX;c++)
		{
			fb[r][c] = L' ';
		}
	}
}

void drawFinishLine(wchar_t fb[][fbDimX])
{
	const int finishLineX = winningLine;

	for (int r = 0;r < fbDimY;r++)
	{
		if (r % 2)
		{
			fb[r][finishLineX] = L'▒';
			fb[r][finishLineX+1] = L' ';
		}
		else
		{
			fb[r][finishLineX] = L' ';
			fb[r][finishLineX+1] = L'▒';
		}
	}
}

void day30bitblit(int dimx, int dimy, int posx, int posy, wchar_t fb[][fbDimX])
{
	for (int r = 0;r < dimy;r++)
	{
		consoleGotoxy(posx, posy + r);
		//wchar_t line[fbDimX+1];
		//line[fbDimX] = 0;
		for (int c = 0;c < dimx;c++)
		{
			std::wcout << fb[r][c];
		}
	}
}

//
//
//

void day30()
{
	setUnicodeConsole();
	consoleHideCursor();
	clearScreen();

	wchar_t framebuffer[fbDimY][fbDimX];

	std::vector<oppdcar> listOfCars;
	oppdcar c1(5, 5);
	oppdcar c2(5, 10);
	oppdcar c3(5, 15);
	oppdcar c4(5, 20);
	listOfCars.push_back(c1);
	listOfCars.push_back(c2);
	listOfCars.push_back(c3);
	listOfCars.push_back(c4);

	// phase 1: ask for predicted winning car

	clearFb(framebuffer);
	drawFinishLine(framebuffer);
	for (int c = 0;c < listOfCars.size();c++)
	{
		listOfCars[c].draw(framebuffer,c);
	}
	day30bitblit(fbDimX, fbDimY, 0, 0, framebuffer);

	int iPredictedCar = -1;
	bool gaveThatDamnedAnswer = false;
	while (!gaveThatDamnedAnswer)
	{
		std::string answer;
		consoleGotoxy(0, fbDimY + 1);
		std::wcout << "Which car will win? Enter a number from 1 to 4" << std::endl << ">                                      ";
		consoleGotoxy(1, fbDimY + 2);
		std::cin >> answer;

		if ((answer != "1") && (answer != "2") && (answer != "3") && (answer != "4"))
		{
		}
		else
		{
			iPredictedCar = stoi(answer);
			gaveThatDamnedAnswer = true;
		}
	}

	// phase 2: race!

	consoleGotoxy(0, fbDimY + 1);
	std::wcout << "                                                     " << std::endl;
	consoleGotoxy(0, fbDimY + 2);
	std::wcout << "                                                     " << std::endl;

	srand((unsigned int)time(NULL));

	int winnerCar = -1;
	bool goout = false;
	while (!goout)
	{
		clearFb(framebuffer);
		drawFinishLine(framebuffer);
		for (int c = 0;c < listOfCars.size();c++)
		{
			listOfCars[c].draw(framebuffer,c);
		}
		day30bitblit(fbDimX, fbDimY, 0, 0, framebuffer);

		// did someone win?
		for (int c = 0;c < listOfCars.size();c++)
		{
			if (listOfCars[c].isWinner(winningLine+1))
			{
				winnerCar = c+1;
				goout = true;
				break;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27) goout = true;
		}

		for (int c = 0;c < listOfCars.size();c++)
		{
			listOfCars[c].update();
		}
	}

	consoleGotoxy(0, fbDimY + 1);
	if (winnerCar != -1)
	{
		std::wcout << "Car number " << winnerCar << " wins!" << std::endl;
		if (iPredictedCar == winnerCar)
		{
			std::wcout << "You WIN!" << std::endl;
		}
		else
		{
			std::wcout << "But you lose." << std::endl;
		}
	}
}
