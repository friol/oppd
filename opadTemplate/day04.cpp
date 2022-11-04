//
// vu-meters
// oppd project
// friol 2o22
//

#include "include/oppd.h"

void drawVUMeter(int xpos, int ypos, int level,int* peakLevels, int chan)
{
	const int subBarHeight = 8;
	const int barHeight = 32;
	const int barWidth = 20;
	if (level > barHeight) level = barHeight;

	int vuRanges[4][2] = {
		{0,7},
		{8,15},
		{16,23},
		{24,31}
	};

	int colorz[16] = { 10,10,2,2,14,14,6,6,4,4,12,12,12,12 };

	wchar_t vuChars[4] = { L'░',L'▒',L'▓',L'█' };
	wchar_t nextVuChars[4] = { L'░',L'░',L'▒',L'▓'};

	wchar_t ditherPattern[subBarHeight][barWidth] = {
		{L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒'},
		{L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░'},
		{L'░',L'▒',L'░',L'░',L'░',L'▒',L'░',L'░',L'░',L'▒',L'░',L'░',L'░',L'▒',L'░',L'░',L'░',L'▒'},
		{L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░'},
		{L'░',L'▒',L'░',L'░',L'░',L'▒',L'░',L'░',L'░',L'▒',L'░',L'░',L'░',L'▒',L'░',L'░',L'░',L'▒'},
		{L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░',L'▒',L'░'},
		{L'▓',L'▒',L'▓',L'▓',L'▓',L'▒',L'▓',L'▓',L'▓',L'▒',L'▓',L'▓',L'▓',L'▒',L'▓',L'▓',L'▓',L'▒'},
		{L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█',L'█'}
	};

	// draw vu-meters
	int subBarCnt = 0;
	for (int r = 0;r < barHeight;r++)
	{
		wchar_t emptyVUchar = L' ';
		int inverseLevel = barHeight - level;
		consoleGotoxy(xpos, r+ypos);
		if (r < inverseLevel)
		{
			for (int c = 0;c < barWidth;c++)
			{
				std::wcout << emptyVUchar;
			}
		}
		else
		{
			int range = 0;
			bool found = false;
			wchar_t vuChar = L'.';
			wchar_t nextVuChar;
			while (!found)
			{
				if ((r >= vuRanges[range][0]) && (r <= vuRanges[range][1]))
				{
					found = true;
					vuChar = vuChars[range];
					nextVuChar = nextVuChars[range];
				}
				else range++;
			}

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, colorz[15-r/2]);
			for (int c = 0;c < barWidth;c++)
			{
				/*if (subBarCnt == 0) std::wcout << nextVuChar;
				else if ((subBarCnt!=0)&&(c % subBarCnt) == 0) std::wcout << nextVuChar;
				else*/ //std::wcout << vuChar;
				std::wcout << ditherPattern[subBarCnt][c];
			}
		}

		subBarCnt++;
		subBarCnt %= subBarHeight;
	}

	// peak levels
	if (level > peakLevels[chan])
	{
		peakLevels[chan] = level;
	}

	if (peakLevels[chan] == 0) return;

	resetConsole();

	int inversePeakLevel = barHeight - peakLevels[chan];
	consoleGotoxy(xpos, inversePeakLevel+ypos);
	for (int c = 0;c < barWidth-2;c++)
	{
		std::wcout << L'^';
	}

}

void day04usage(char** argv)
{
	std::wcout << "Usage:" << std::endl << argv[0] << " [module name]" << std::endl;
}

void day04(int argc, char** argv)
{
	std::string modName = "d:\\prova\\satellites.xm";
	if (argc == 2)
	{
		modName = argv[1];
	}
	else if (argc > 2)
	{
		day04usage(argv);
		return;
	}

	const int numChannels = 2;
	int peakLevels[numChannels] = { 0,0 };

	setUnicodeConsole();
	clearScreen();

	std::wcout << "Playing [" << modName.c_str() << "]" << std::endl;

	if (!BASS_Init(-1, 44100, 0, NULL, NULL))
	{
		std::wcout << "Error: cannot init BASS library." << std::endl;
		return;
	}

	int dx, dy;
	consoleGetDimensions(dx, dy);
	int vuposx = (dx - (20 * 2)) / 2;

	int peakMeterDelay = 1;
	int peakMeterCounter = 0;
	HMUSIC music;
	music = BASS_MusicLoad(FALSE, modName.c_str(), 0, 0, BASS_SAMPLE_FLOAT, 1);
	if (music)
	{
		BASS_ChannelPlay(music, FALSE);
		while (!_kbhit())
		{
			const int levelDivider = 320;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

			DWORD level = BASS_ChannelGetLevel(music);
			DWORD left = LOWORD(level), right = HIWORD(level);

			drawVUMeter(vuposx, 1, left / levelDivider, peakLevels, 0);
			drawVUMeter(vuposx+21, 1, right/ levelDivider, peakLevels, 1);

			peakMeterCounter++;
			if (peakMeterCounter == peakMeterDelay)
			{
				peakMeterCounter = 0;
				if (peakLevels[0] > 0) peakLevels[0]--;
				if (peakLevels[1] > 0) peakLevels[1]--;
			}
		}
	}

	consoleGotoxy(0, 34);
	BASS_Free();
}
