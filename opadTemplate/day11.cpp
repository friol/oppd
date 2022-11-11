//
// gosh, pong
// oppd, friol 2k22
//

#include "include/oppd.h"

#define MAXWIDTH 60

class paddle
{
private:
	int posx;
	int posy;

	const int padDimY = 5;
public:

	paddle(int px, int py)
	{
		posx = px;
		posy = py;
	}

	bool hits(int px, int py)
	{
		if ((px <= posx) && (py >= posy) && (py <= (posy + padDimY)))
		{
			return true;
		}

		return false;
	}

	void aiMove(int ballPosy, int maxy)
	{
		if (ballPosy > posy) this->moveDown(maxy);
		else if (ballPosy < posy) this->moveUp();
	}

	void moveUp()
	{
		if (posy > 1) posy--;
	}

	void moveDown(int maxy)
	{
		if ((posy < (maxy - 1 - padDimY))) posy++;
	}

	void draw(wchar_t fb[][MAXWIDTH])
	{
		for (int r = posy;r < posy + padDimY;r++)
		{
			fb[r][posx] = L'█';
		}
	}
};

class ball
{
private:
	int posx;
	int posy;
	int speedx;
	int speedy;
public:

	ball(int px, int py)
	{
		posx = px;
		posy = py;
		speedx = 1;
		speedy = 1;
	}

	void resetBall(int px,int py)
	{
		posx = px;
		posy = py;
		speedx = 1;
		speedy = 1;
	}

	int getPosX()
	{
		return posx;
	}

	int getPosY()
	{
		return posy;
	}

	void update(int fbdimx,int fbdimy,paddle& lp,paddle& rp,int& plscore,int& cpuscore)
	{
		posx += speedx;
		posy += speedy;

		if (posx <= 0)
		{
			if (lp.hits(posx, posy))
			{
				posx = 0;
				speedx = -speedx;
			}
			else
			{
				// increment player's score
				plscore++;
				this->resetBall(10, 10);
			}
		}
		if (posy < 0)
		{
			posy = 0;
			speedy = -speedy;
		}
		if (posx > (fbdimx - 1))
		{
			posx = fbdimx - 1;
			speedx = -speedx;
		}
		if (posy > (fbdimy - 1))
		{
			posy = fbdimy - 1;
			speedy = -speedy;
		}
	}

	void draw(wchar_t fb[][MAXWIDTH])
	{
		fb[posy][posx] = L'O';
	}
};

wchar_t numbers[10 * 5][3] = {
	{L'█',L'█',L'█'},
	{L'█',L' ',L'█'},
	{L'█',L' ',L'█'},
	{L'█',L' ',L'█'},
	{L'█',L'█',L'█'},
 	
	{L' ',L' ',L'█'},
	{L' ',L' ',L'█'},
	{L' ',L' ',L'█'},
	{L' ',L' ',L'█'},
	{L' ',L' ',L'█'},

	{L'█',L'█',L'█'},
	{L' ',L' ',L'█'},
	{L'█',L'█',L'█'},
	{L'█',L' ',L' '},
	{L'█',L'█',L'█'},

	{L'█',L'█',L'█'},
	{L' ',L' ',L'█'},
	{L'█',L'█',L'█'},
	{L' ',L' ',L'█'},
	{L'█',L'█',L'█'},

	{L'█',L' ',L'█'},
	{L'█',L' ',L'█'},
	{L'█',L'█',L'█'},
	{L' ',L' ',L'█'},
	{L' ',L' ',L'█'},

	{L'█',L'█',L'█'},
	{L'█',L' ',L' '},
	{L'█',L'█',L'█'},
	{L' ',L' ',L'█'},
	{L'█',L'█',L'█'},

	{L'█',L'█',L'█'},
	{L'█',L' ',L' '},
	{L'█',L'█',L'█'},
	{L'█',L' ',L'█'},
	{L'█',L'█',L'█'},

	{L'█',L'█',L'█'},
	{L' ',L' ',L'█'},
	{L' ',L' ',L'█'},
	{L' ',L' ',L'█'},
	{L' ',L' ',L'█'},

	{L'█',L'█',L'█'},
	{L'█',L' ',L'█'},
	{L'█',L'█',L'█'},
	{L'█',L' ',L'█'},
	{L'█',L'█',L'█'},

	{L'█',L'█',L'█'},
	{L'█',L' ',L'█'},
	{L'█',L'█',L'█'},
	{L' ',L' ',L'█'},
	{L' ',L' ',L'█'}

};

void renderPlayfield(wchar_t fb[][MAXWIDTH],ball& b,paddle& lp,paddle& rp,int fbdimx,int fbdimy,int plScore,int cpuScore)
{
	// upper band
	for (int c = 0;c < fbdimx;c++)
	{
		fb[0][c] = L'█';
	}

	// lower band
	for (int c = 0;c < fbdimx;c++)
	{
		fb[fbdimy-1][c] = L'█';
	}

	// central dotted line
	for (int r = 1;r < (fbdimy - 1);r += 2)
	{
		fb[r][(int)(fbdimx/2)] = L'█';
	}

	// ball
	b.draw(fb);

	// paddles
	lp.draw(fb);
	rp.draw(fb);

	// score 0-9
	int scposx = 10;
	int scposy = 2;
	for (int r = 0;r < 5;r++)
	{
		for (int c = 0;c < 3;c++)
		{
			fb[r + scposy][c + scposx] = numbers[(plScore * 5)+r][c];
		}
	}

	scposx = 47;
	scposy = 2;
	for (int r = 0;r < 5;r++)
	{
		for (int c = 0;c < 3;c++)
		{
			fb[r + scposy][c + scposx] = numbers[(cpuScore * 5) + r][c];
		}
	}
}

void day11bitblit(int dimx, int dimy, int posx, int posy, wchar_t fb[][MAXWIDTH])
{
	for (int r = 0;r < dimy;r++)
	{
		consoleGotoxy(posx, posy + r);
		wchar_t line[MAXWIDTH+1];
		line[MAXWIDTH] = 0;
		for (int c = 0;c < dimx;c++)
		{
			line[c] = fb[r][c];
		}
		std::wcout << line;
	}
}

void day11()
{
	consoleHideCursor();
	setUnicodeConsole();
	clearScreen();

	const int fbdimx = MAXWIDTH;
	const int fbdimy = 24;
	wchar_t framebuffer[fbdimy][fbdimx];

	ball theBall(10, 10);
	paddle lPaddle(0, 12);
	paddle rPaddle(fbdimx-1, 14);

	int scorePlayer = 0;
	int scoreCPU = 0;
	bool goOut = false;
	bool win = false;
	while ((!goOut) && (!win))
	{
		for (int r = 0;r < fbdimy;r++)
		{
			for (int x = 0;x < fbdimx;x++)
			{
				framebuffer[r][x] = L' ';
			}
		}

		theBall.update(fbdimx,fbdimy,lPaddle,rPaddle,scorePlayer,scoreCPU);
		rPaddle.aiMove(theBall.getPosY(), fbdimy);

		renderPlayfield(framebuffer, theBall, lPaddle, rPaddle, fbdimx, fbdimy,scorePlayer,scoreCPU);
		day11bitblit(fbdimx, fbdimy, 1, 1, framebuffer);

		if (scorePlayer == 9)
		{
			goOut = true;
			win = true;
		}

		if (scoreCPU == 9)
		{
			goOut = true;
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (_kbhit())
		{
			int ch = _getch();
			if (ch == 27) goOut = true;
			else if (ch == 72) // up
			{
				lPaddle.moveUp();
			}
			else if (ch == 80) // down
			{
				lPaddle.moveDown(fbdimy);
			}
		}
	}

	consoleGotoxy(0, fbdimy+1);
}
