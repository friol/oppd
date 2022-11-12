//
// stochastic ball flipper
// friol 2k22
// oppd
//

#include "include/oppd.h"

class stochasticBall
{
private:
	int posx;
	int posy;
	bool landed;
public:

	int id;

	stochasticBall(int px, int py, int theid)
	{
		this->posx = px;
		this->posy = py;
		this->landed = false;
		this->id = theid;
	}

	int getx()
	{
		return this->posx;
	}

	int gety()
	{
		return this->posy;
	}

	void update(std::vector<std::vector<int>>& nails,int maxy,std::vector<stochasticBall>& balls)
	{
		if (!this->landed)
		{
			this->posy += 1;

			for (int n = 0;n < nails.size();n++)
			{
				if ((nails[n][0] == this->posx) && (nails[n][1] == this->posy))
				{
					int r = rand() % 100;
					if (r > 50)
					{
						this->posx += 1;
					}
					else
					{
						this->posx -= 1;
					}
				}
			}

			if (this->posy >= (maxy-2))
			{
				this->landed = true;
			}

			for (int b = 0;b < balls.size();b++)
			{
				if ((this->posx == balls[b].getx()) && (this->posy == balls[b].gety()))
				{
					if (balls[b].id != this->id)
					{
						this->posy -= 1;
						this->landed = true;
					}
				}
			}
		}
	}

	void draw(wchar_t fb[][40])
	{
		fb[this->posy][this->posx]=L'O';
	}
};

void drawPlayfield(int pfdx,int pfdy, std::vector<std::vector<int>>& nails, std::vector<stochasticBall>& balls,wchar_t fb[][40])
{
	int flipperHeight = pfdy / 2;

	int curxp0 = (pfdx / 2) - 1;
	int curxp1 = (pfdx / 2) + 1;

	// upper part

	for (int r = 0;r < flipperHeight;r++)
	{
		fb[r][curxp0]=L'█';
		fb[r][curxp1]=L'█';

		curxp0--;
		curxp1++;
	}

	// lower part

	for (int r = flipperHeight;r < flipperHeight*2;r++)
	{
		fb[r][curxp0] = L'█';
		fb[r][curxp1] = L'█';
	}

	// bottom line
	for (int x = curxp0;x <= curxp1;x++)
	{
		fb[(flipperHeight*2)-1][x]= L'█';
	}

	// nails
	for (int n = 0;n < nails.size();n++)
	{
		fb[nails[n][1]][nails[n][0]] = L'x';
	}

	// balls
	for (int b = 0;b < balls.size();b++)
	{
		balls[b].draw(fb);
	}
}

void day12bitblit(int dimx, int dimy, int posx, int posy, wchar_t fb[][40])
{
	for (int r = 0;r < dimy;r++)
	{
		consoleGotoxy(posx, posy + r);
		wchar_t line[40 + 1];
		line[40] = 0;
		for (int c = 0;c < dimx;c++)
		{
			line[c] = fb[r][c];
		}
		std::wcout << line;
	}
}

void day12()
{
	consoleHideCursor();
	setUnicodeConsole();
	clearScreen();

	srand((unsigned int)time(NULL));

	//

	const int pfdimx = 40;
	const int pfdimy = 32;

	wchar_t framebuffer[pfdimy][pfdimx];

	std::vector<stochasticBall> listOfBalls;
	std::vector<std::vector<int>> listOfNails;

	// create nails array
	const int numNails = 1 + 3 + 5 + 6 + 6;
	const int nailsRows = 5;

	int curNail = 0;
	int nailRow = 2;
	int nailPosx = pfdimx / 2;
	int nailIniDecx = nailPosx;
	while (curNail<numNails)
	{
		std::vector<int> curPoint;
		curPoint.push_back(nailPosx);
		curPoint.push_back(nailRow);
		listOfNails.push_back(curPoint);

		nailPosx += 2;
		curNail++;

		if ((curNail == 1) || (curNail == 3) || (curNail == 6) || (curNail == 10) || (curNail == 15))
		{
			nailRow += 3;
			nailIniDecx -= 1;
			nailPosx = nailIniDecx;
		}
	}

	const int maxBalls = 40;
	int counter = 0;
	bool goOut = false;
	while (!goOut)
	{
		for (stochasticBall& sb : listOfBalls)
		{
			sb.update(listOfNails, pfdimy,listOfBalls);
		}

		for (int r = 0;r < pfdimy;r++)
		{
			for (int c = 0;c < pfdimx;c++)
			{
				framebuffer[r][c] = L' ';
			}
		}

		drawPlayfield(pfdimx, pfdimy,listOfNails,listOfBalls,framebuffer);
		day12bitblit(pfdimx, pfdimy, 0, 0, framebuffer);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27) goOut = true;
		}

		counter++;
		if (counter == 5)
		{
			if (listOfBalls.size() < maxBalls)
			{
				stochasticBall b(pfdimx / 2, 0, (int)listOfBalls.size());
				listOfBalls.push_back(b);
			}
			counter = 0;
		}
	}

	consoleGotoxy(0, pfdimy + 1);
}
