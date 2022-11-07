//
// oppd
// skylines
// friol 2o22
//

#include "include/oppd.h"

void drawPlane()
{
	int condimix, condimy;
	consoleGetDimensions(condimix, condimy);

	int planex= rand() % condimix;

	consoleGotoxy(planex, 0);
	std::wcout << L"   //";
	consoleGotoxy(planex, 1);
	std::wcout << L"<=[]====7";
	consoleGotoxy(planex, 2);
	std::wcout << L"   \\\\";
}

void drawSkyScraper()
{
	const int numPossibleChars = 5;
	wchar_t possibleChars[numPossibleChars] = {L'═',L'█',L'░',L'▒',L'▓'};

	const int ssBaseLineY = 20;
	int ssMaxHeight = 16;
	int ssMinHeight = 4;
	int ssMaxWidth = 6;
	int ssMinWidth = 2;

	int condimix, condimy;
	consoleGetDimensions(condimix, condimy);

	int ssx = rand() % condimix;
	int sswidth = rand()%ssMaxWidth;
	if (sswidth < ssMinWidth) sswidth = ssMinWidth;
	int ssheight = rand()%ssMaxHeight;
	if (ssheight < ssMinHeight) ssheight = ssMinHeight;

	int ssCharNum = rand() % numPossibleChars;
	wchar_t ssChar = possibleChars[ssCharNum];
	for (int r = ssBaseLineY;r > (ssBaseLineY - ssheight);r--)
	{
		for (int c = ssx;c < (ssx + sswidth);c++)
		{
			consoleGotoxy(c, r);
			std::wcout << ssChar;
		}
	}

	// antenna
	bool hasAntenna = ((rand() % 100) > 50) ? true : false;
	if (hasAntenna)
	{
		for (int r = (ssBaseLineY - ssheight);r> (ssBaseLineY - ssheight-2);r--)
		{
			consoleGotoxy(ssx,r);
			char ch = consoleGetCharAt(ssx, r);
			if (ch==' ') std::wcout << L'|';
		}
	}

	// super-antenna
	bool hasSuperAntenna = ((rand() % 100) > 95) ? true : false;
	if (hasAntenna&&hasSuperAntenna)
	{
		consoleGotoxy(ssx, (ssBaseLineY - ssheight - 2));
		char ch = consoleGetCharAt(ssx, (ssBaseLineY - ssheight - 2));
		if (ch == ' ') std::wcout << L'0';
		consoleGotoxy(ssx, (ssBaseLineY - ssheight - 3));
		ch = consoleGetCharAt(ssx, (ssBaseLineY - ssheight - 3));
		if (ch == ' ') std::wcout << L'|';
	}

}

void day07()
{
	setUnicodeConsole();
	clearScreen();
	srand((unsigned int)time(NULL));

	const int maxSkyscrapers = 110;

	for (int ss = 0;ss < maxSkyscrapers;ss++)
	{
		drawSkyScraper();
	}
	drawPlane();

	consoleGotoxy(0, 22);

	while (!_kbhit())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
