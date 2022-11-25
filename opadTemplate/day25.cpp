
//
// oppd day 25: fireworks!!!
// friol 2k22
//

#include "include/oppd.h"

enum fireworkState { rising, exploding };

#define d25pfdimx 120

class petal
{
private:
public:
	int px, py;
	int vx, vy;
};

class firework
{
private:

	fireworkState state;
	int px, py;
	int vy;
	std::vector<petal> petals;
	int numStepsToExplosion;
	bool gone = false;

	const int numPetals = 12;

public:

	firework(int x, int pfx, int pfy)
	{
		this->state = rising;
		this->px = x;
		this->py = pfy;
		this->vy = -3;

		this->numStepsToExplosion = 15 + (rand() % 5);
	}

	void update()
	{
		if (this->state == rising)
		{
			this->py += this->vy;

			if ((this->numStepsToExplosion % 4) == 0)
			{
				this->vy += 1;
			}

			this->numStepsToExplosion--;
			if (this->numStepsToExplosion == 0)
			{
				this->state = exploding;

				float angle = 0;
				float magnitudo = 6.0;
				for (int p = 0;p < numPetals;p++)
				{
					petal pet;
					pet.px = this->px;
					pet.py = this->py;

					pet.vx = (int)(magnitudo * sin(angle));
					pet.vy = (int)(magnitudo * cos(angle));

					angle += (float)((2.0 * 3.1415) / ((float)this->numPetals));
					this->petals.push_back(pet);
				}
			}
		}
		else if (this->state == exploding)
		{
			for (int p = 0;p < this->petals.size();p++)
			{
				this->petals[p].px += this->petals[p].vx;
				this->petals[p].py += (this->petals[p].vy/2);

				this->petals[p].vy += 1;
			}
		}
	}

	void draw(char fb[][d25pfdimx],int fbdimy)
	{
		if (this->state == rising)
		{
			if ((this->px >= 0) && (this->px < d25pfdimx) && (this->py >= 0) && (this->py < fbdimy))
			{
				fb[this->py][this->px] = '.';
			}
		}
		else if (this->state == exploding)
		{
			if (this->gone) return;

			bool drawn = false;
			for (petal& p : this->petals)
			{
				if ((p.px >= 0) && (p.px < d25pfdimx) && (p.py > 0) && (p.py < fbdimy))
				{
					fb[p.py][p.px] = 'o';
					drawn = true;
				}
			}

			if (drawn == false)
			{
				this->gone = true;
			}
		}
	}

};

void day25bitblit(int dimx, int dimy, int posx, int posy, char fb[][d25pfdimx])
{
	for (int r = 0;r < dimy;r++)
	{
		consoleGotoxy(posx, posy + r);
		char line[d25pfdimx+1];
		line[d25pfdimx] = 0;
		for (int c = 0;c < dimx;c++)
		{
			line[c] = fb[r][c];
		}
		std::cout << line;
	}
}

void day25()
{
	consoleGotoxy(0, 0);
	consoleHideCursor();
	clearScreen();

	const int pfDimx = d25pfdimx;
	const int pfDimy = 30;
	char framebuffer[pfDimy][pfDimx];

	std::vector<firework> fwArr;

	// time between 2 fireworks
	const int fwSpace = 12;
	int time2nextfw = fwSpace;

	firework fw1(rand()%pfDimx, pfDimx, pfDimy);
	fwArr.push_back(fw1);

	bool goout = false;
	while (!goout)
	{
		time2nextfw--;
		if (time2nextfw == 0)
		{
			time2nextfw = fwSpace;
			firework fw1(rand() % pfDimx, pfDimx, pfDimy);
			fwArr.push_back(fw1);
		}

		// clear
		for (int r = 0;r < pfDimy;r++)
		{
			for (int x = 0;x < pfDimx;x++)
			{
				framebuffer[r][x] = ' ';
			}
		}

		for (firework& fw : fwArr)
		{
			fw.update();
			fw.draw(framebuffer,pfDimy);
		}

		day25bitblit(pfDimx, pfDimy, 0, 0, framebuffer);

		std::this_thread::sleep_for(std::chrono::milliseconds(30));

		if (_kbhit() && (!goout))
		{
			char ch = _getch();
			if (ch == 27)
			{
				goout = true;
			}
		}
	}

	consoleGotoxy(0, 20);
	consoleShowCursor();
}
