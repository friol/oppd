//
// oppd
// robots&teleports
// player is a robot
// friol 2k22
//

#include "include/oppd.h"

class powerpill
{
private:
public:

	int posx, posy;
	bool eaten;

	powerpill(int px, int py)
	{
		this->posx = px;
		this->posy = py;
		this->eaten = false;
	}

	void draw(int x, int y)
	{
		consoleGotoxy(this->posx + x, this->posy + y);
		std::wcout << L"*";
	}
};

class robot
{
private:
public:

	int posx, posy;

	robot(int px, int py)
	{
		this->posx = px;
		this->posy = py;
	}

	void draw(int x,int y)
	{
		consoleGotoxy(this->posx+x, this->posy+y);
		std::wcout << L"☺";
	}

	void move(int pfdimx, int pfdimy, int dir, int x,int y)
	{
		if ((dir == 0) && (this->posy > (y)))
		{
			consoleGotoxy(this->posx+x, this->posy+y);
			std::wcout << L" ";
			this->posy--;
		}
		else if ((dir == 3) && (this->posx > (x+1)))
		{
			consoleGotoxy(this->posx + x, this->posy + y);
			std::wcout << L" ";
			this->posx--;
		}
		else if ((dir == 1) && (this->posx < (pfdimx - 1-x)))
		{
			consoleGotoxy(this->posx + x, this->posy + y);
			std::wcout << L" ";
			this->posx++;
		}
		else if ((dir == 2) && (this->posy < (pfdimy - 1-y)))
		{
			consoleGotoxy(this->posx + x, this->posy + y);
			std::wcout << L" ";
			this->posy++;
		}
	}

	void teleport(int pfx, int pfy, std::vector<std::vector<int>>& listOfKillers, std::vector<powerpill>& listOfPowerpills,int x,int y)
	{
		consoleGotoxy(this->posx+x, this->posy+y);
		std::wcout << L" ";

		bool safelyLanded = true;
		do
		{
			int npx = 1+rand() % (pfx - 2);
			int npy = 1+rand() % (pfy - 2);

			for (powerpill& pp : listOfPowerpills)
			{
				if (!pp.eaten)
				{
					if ((pp.posx == npx) && (pp.posy == npy))
					{
						safelyLanded = false;
					}
				}
			}

			for (std::vector<int>& kk : listOfKillers)
			{
				if ((kk[0] == npx) && (kk[1] == npy))
				{
					safelyLanded = false;
				}
			}

			if (safelyLanded)
			{
				this->posx = npx;
				this->posy = npy;
			}
		} while (!safelyLanded);
	}
};

class killer
{
private:
public:

	int posx, posy;

	killer(int px, int py)
	{
		this->posx = px;
		this->posy = py;
	}

	void draw(int x,int y)
	{
		consoleGotoxy(this->posx+x, this->posy+y);
		std::wcout << L"@";
	}

	void move(robot& player,int x,int y)
	{
		consoleGotoxy(this->posx+x, this->posy+y);
		std::wcout << L" ";

		if (this->posx < player.posx) this->posx++;
		if (this->posy < player.posy) this->posy++;

		if (this->posx > player.posx) this->posx--;
		if (this->posy > player.posy) this->posy--;
	}
};

void drawPlayfield(int dimx, int dimy,int px,int py)
{
	for (int r = 0;r < dimy;r++)
	{
		consoleGotoxy(px, py + r);
		for (int c = 0;c < dimx;c++)
		{
			if ((r == 0)|| (r == (dimy - 1))|| (c == 0)|| (c == (dimx - 1)))
			{
				std::wcout << L"▓";
			}
			else
			{
				std::wcout << L" ";
			}
		}
		std::wcout << std::endl;
	}
}


//
//
//

void initPowerpills(int pfx, int pfy, robot& player,int npp,std::vector<powerpill>& ppvec)
{
	int numPlaced=0;
	while (numPlaced<npp)
	{
		int nx = 1+(rand() % (pfx - 2));
		int ny = 1+(rand() % (pfy - 2));

		if ((nx != player.posx) && (ny != player.posy))
		{
			powerpill pp(nx, ny);
			ppvec.push_back(pp);
			numPlaced++;
		}
	}
}

void initKillers(int pfx, int pfy, robot& player, int nofk, std::vector<killer>& listOfKillers, std::vector<powerpill>& listOfPowerpills)
{
	int playerMinDistance = 4;
	int numPlaced = 0;
	while (numPlaced < nofk)
	{
		int nx = 1 + (rand() % (pfx - 2));
		int ny = 1 + (rand() % (pfy - 2));

		int distx = (int)fabs(nx - player.posx);
		int disty = (int)fabs(ny - player.posy);

		if ((distx>playerMinDistance) && (disty>playerMinDistance))
		{
			killer nk(nx, ny);
			listOfKillers.push_back(nk);
			numPlaced++;
		}
	}
}

void day20()
{
	const int playfieldDimx = 80;
	const int playfieldDimy = 24;

	int numberOfKillers = 3;
	int numberOfPowerpills = 3;
	int numberOfTeleports = 3;

	robot player(10, 10);
	std::vector<killer> listOfKillers;
	std::vector<powerpill> listOfPowerpills;

	initPowerpills(playfieldDimx, playfieldDimy, player,numberOfPowerpills,listOfPowerpills);
	initKillers(playfieldDimx, playfieldDimy, player,numberOfKillers,listOfKillers,listOfPowerpills);

	//

	setUnicodeConsole();
	clearScreen();
	consoleHideCursor();

	drawPlayfield(playfieldDimx, playfieldDimy,0,1);

	bool goout = false;
	bool winCondition = false;
	while ((!goout)&&(!winCondition))
	{
		int nppeaten = 0;
		for (powerpill& pp : listOfPowerpills)
		{
			if (pp.eaten) nppeaten++;
		}

		consoleGotoxy(0, 0);
		std::wcout << " Fugue&Teleports - Remaining Teleports: " << numberOfTeleports << "          Number of pills remaining: " << (numberOfPowerpills-nppeaten);
		for (powerpill& pp : listOfPowerpills)
		{
			if (!pp.eaten) pp.draw(0,1);
		}
		for (killer& kk : listOfKillers)
		{
			kk.draw(0,1);
		}
		player.draw(0,1);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (_kbhit())
		{
			bool playerMoved = false;
			char ch = _getch();
			if (ch == 27)
			{
				goout = true;
			}
			else if (ch == 72) // up
			{
				player.move(playfieldDimx, playfieldDimy, 0,0,1);
				playerMoved = true;
			}
			else if (ch == 77) // right
			{
				player.move(playfieldDimx, playfieldDimy, 1, 0, 1);
				playerMoved = true;
			}
			else if (ch == 80) // down
			{
				player.move(playfieldDimx, playfieldDimy, 2, 0, 1);
				playerMoved = true;
			}
			else if (ch == 75) // left
			{
				player.move(playfieldDimx, playfieldDimy, 3, 0, 1);
				playerMoved = true;
			}
			else if (ch == 32) // teleport!
			{
				if (numberOfTeleports > 0)
				{
					std::vector<std::vector<int>> lofKillers;
					for (killer& kk : listOfKillers)
					{
						std::vector<int> posk;
						posk.push_back(kk.posx);
						posk.push_back(kk.posy);
					}

					player.teleport(playfieldDimx, playfieldDimy, lofKillers, listOfPowerpills,0,1);

					numberOfTeleports--;
				}
			}

			if (playerMoved)
			{
				for (killer& kk : listOfKillers)
				{
					kk.move(player,0,1);
				}

				// check for death
				for (killer& kk : listOfKillers)
				{
					if ((player.posx == kk.posx) && (player.posy == kk.posy))
					{
						consoleGotoxy(0, playfieldDimy + 1);
						std::wcout << "You have been catched by a killer." << std::endl;
						goout = true;
					}
				}

				// check for powerpill eaten
				for (powerpill& pp : listOfPowerpills)
				{
					if ((player.posx == pp.posx) && (player.posy == pp.posy))
					{
						pp.eaten = true;
					}
				}

				// check for win condition
				winCondition = true;
				for (powerpill& pp : listOfPowerpills)
				{
					if (!pp.eaten) winCondition = false;
				}
			}
		}
	}

	if (winCondition)
	{
		int nppeaten = 0;
		for (powerpill& pp : listOfPowerpills)
		{
			if (pp.eaten) nppeaten++;
		}
		consoleGotoxy(0, 0);
		std::wcout << " Fugue&Teleports - Remaining Teleports: " << numberOfTeleports << "          Number of pills remaining: " << (numberOfPowerpills - nppeaten);
		player.draw(0, 1);
		consoleGotoxy(0, playfieldDimy + 1);
		std::wcout << "Robots WIN!";
	}

	consoleGotoxy(0, playfieldDimy + 2);
	consoleShowCursor();
}
