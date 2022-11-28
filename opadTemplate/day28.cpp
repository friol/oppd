
//
// oppd goes exoteric
// friol 2k22
//

#include "include/oppd.h"

void drawVertline(int h)
{
	const int paddingLeft = 4;
	const int halfSize = 4;

	for (int r = 0;r < h;r++)
	{
		for (int c = 0;c <=(paddingLeft + halfSize);c++)
		{
			if (c < (paddingLeft + halfSize))
			{
				std::wcout << " ";
			}
			else std::wcout << L"█";
		}
		std::wcout << std::endl;
	}

}

void drawLine()
{
	const int paddingLeft = 4;
	const int lineSize = 8;

	for (int c = 0;c <= (paddingLeft + lineSize);c++)
	{
		if (c < paddingLeft) std::wcout << " ";
		else std::wcout << L"█";
	}
	std::wcout << std::endl;
}

void drawCircle()
{
	const int paddingLeft = 4;
	const int circleDiameter = 8;

	for (int r = 0;r <=circleDiameter;r++)
	{
		for (int c = 0;c < paddingLeft;c++)
		{
			std::wcout << " ";
		}

		for (int c = paddingLeft;c <= (paddingLeft + circleDiameter);c++)
		{
			float dx = c - (paddingLeft + circleDiameter / 2);
			float dy = r - (circleDiameter / 2);
			float dist = sqrtf((dx * dx) + (dy * dy));

			float delta = 1.11;
			if ( (dist < ((circleDiameter / 2)+delta)) && (dist >= ((circleDiameter / 2))) )
			{
				std::wcout << L"█";
			}
			else
			{
				std::wcout << " ";
			}
		}

		std::wcout << std::endl;
	}

}

void drawTriangle()
{
	const int paddingLeft = 4;
	const int triangleSide = 8;

	for (int c = 0;c <=(paddingLeft + triangleSide);c++)
	{
		if (c < paddingLeft)
		{
			std::wcout << " ";
		}
		else
		{
			std::wcout << L"█";
		}
	}

	std::wcout << std::endl;
	int posl = paddingLeft;
	int posr = paddingLeft + triangleSide;

	bool done = false;
	while (!done)
	{
		for (int c = 0;c <= (paddingLeft + triangleSide);c++)
		{
			if ((c == posl)||(c==posr))
			{
				std::wcout << L"█";
			}
			else
			{
				std::wcout << " ";
			}
		}
		std::wcout << std::endl;

		posl++;
		posr--;

		if (posl > posr) done = true;
	}
}

void drawSquare()
{
	const int paddingLeft = 4;
	const int squareSide = 8;

	for (int r = 0;r < squareSide;r++)
	{
		if (r == 0)
		{
			for (int c = 0;c <=(paddingLeft + squareSide);c++)
			{
				if (c < paddingLeft) std::wcout << " ";
				else std::wcout << L"█";
			}
		}
		else if (r < (squareSide - 1))
		{
			for (int c = 0;c <=(paddingLeft + squareSide);c++)
			{
				if (c==paddingLeft) std::wcout << L"█";
				else if (c==(paddingLeft+squareSide)) std::wcout << L"█";
				else std::wcout << " ";
			}
		}
		else
		{
			for (int c = 0;c <=(paddingLeft + squareSide);c++)
			{
				if (c < paddingLeft) std::wcout << " ";
				else std::wcout << L"█";
			}
		}

		std::wcout << std::endl;
	}

}

void day28()
{
	setUnicodeConsole();
	clearScreen();
	consoleGotoxy(0, 0);

	std::vector<std::function<void()>> drawingFuncs;
	drawingFuncs.emplace_back(drawCircle);
	drawingFuncs.emplace_back(drawTriangle);
	drawingFuncs.emplace_back(drawSquare);
	drawingFuncs.emplace_back(drawLine);

	//

	srand((unsigned int)time(NULL));

	drawVertline(3);
	const int numSymbols = 4;
	for (int s = 0;s < numSymbols;s++)
	{
		int r = rand() % numSymbols;

		drawingFuncs[r]();
		if (s<(numSymbols-1)) std::wcout << std::endl;
	}
	drawVertline(3);
}
