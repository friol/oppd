//
// oppd - banners
// friol 2k22
//

#include "include/oppd.h"

#define DAY18FONTHEIGHT 6

std::string day18font[DAY18FONTHEIGHT] = {
	{"  **   *****   ****  *****  ****** ******  ****  *    * *      * *    * *      *    * *    *  ****  *****   ****  *****   ****  ***** *    * *    * *    * *    * *   * ******       "},
	{" *  *  *    * *    * *    * *      *      *    * *    * *      * *   *  *      **  ** **   * *    * *    * *    * *    * *        *   *    * *    * *    *  *  *   * *      *        "},
	{"*    * *****  *      *    * *****  *****  *      ****** *      * ****   *      * ** * * *  * *    * *    * *    * *    *  ****    *   *    * *    * *    *   **     *      *         "},
	{"****** *    * *      *    * *      *      *  *** *    * *      * *  *   *      *    * *  * * *    * *****  *  * * *****       *   *   *    * *    * * ** *   **     *     *          "},
	{"*    * *    * *    * *    * *      *      *    * *    * * *    * *   *  *      *    * *   ** *    * *      *   *  *   *  *    *   *   *    *  *  *  **  **  *  *    *    *           "},
	{"*    * *****   ****  *****  ****** *       ****  *    * *  ****  *    * ****** *    * *    *  ****  *       *** * *    *  ****    *    ****    **   *    * *    *   *   ******       "}
};

int letterLen[27] = { 6,6,6,6,6,6,6,6,1,6,6,6,6,6,6,6,6,6,6,5,6,6,6,6,6,6,6 };

void printBanner(std::string s)
{
	const int bannerMaxLen = 20;
	if (s.size() > bannerMaxLen)
	{
		std::cout << "Unfortunately, max printable banner size is " << bannerMaxLen << std::endl;
		return;
	}

	for (char& c : s)
	{
		// only chars a-z and spaces are allowed
		if (((int(c)!=32)&&(int(c) < 97)) || (int(c) >= (97 + 26)))
		{
			std::cout << "Unfortunately, only a-z letters and spaces are allowed" << std::endl;
			return;
		}
	}

	int xpos = 0;
	for (char& c : s)
	{
		int charPos;
		if (int(c) == 32)
		{
			charPos = 26;
		}
		else charPos=int(c) - 97;

		int pos = 0;
		int subStart = 0;
		while (pos < charPos)
		{
			subStart += letterLen[pos]+1;
			pos++;
		}
		
		int subEnd = subStart + letterLen[pos];

		for (int r = 0;r < DAY18FONTHEIGHT;r++)
		{
			consoleGotoxy(xpos, r);
			std::string s2 = day18font[r].substr(subStart, letterLen[pos]);
			std::cout << s2;
		}
		xpos += letterLen[pos]+1;
	}
}

void day18usage(char** argv)
{
	std::cout << "Usage:" << std::endl << argv[0] << " <banner text>" << std::endl;
}

void day18(int argc, char** argv)
{
	if (argc != 2)
	{
		day18usage(argv);
		return;
	}

	//std::string stringToPrint = "azeta letters";
	std::string stringToPrint = argv[1];

	clearScreen();
	printBanner(stringToPrint);
	consoleGotoxy(0, 6);
}
