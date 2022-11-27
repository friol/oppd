//
// ed-sheeraner
// oppd 2k22
// friol
//

#include "include/oppd.h"

std::string pickRandomVerse(int songNum, json& theLyrics)
{
	std::string curLine="";

	std::string song = theLyrics["lyricsarr"][songNum];
	
	std::string delim = "\n";
	std::vector<std::string> verseArr;
	auto start = 0U;
	auto end = song.find(delim);
	while (end != std::string::npos)
	{
		verseArr.push_back(song.substr(start, end - start));
		start = (unsigned int)(end + delim.length());
		end = song.find(delim, start);
	}

	bool found = false;

	while (!found)
	{
		int rnd = rand() % (verseArr.size());
		if (verseArr[rnd].find("[") == std::string::npos)
		{
			curLine = verseArr[rnd];
			found = true;
		}
	}

	return curLine;
}

void day26()
{
	json theLyrics;

	// read lyrics file
	std::ifstream lyricsFile;
	lyricsFile.open("esLyrics.json", std::ios::in);

	if (lyricsFile.is_open())
	{
		std::stringstream buffer;
		buffer << lyricsFile.rdbuf();
		std::string fileInMem = buffer.str();

		theLyrics = json::parse(fileInMem);
	}
	else
	{
		std::cout << "Error: cannot open esLyrics.json file." << std::endl;
		return;
	}

	// create a "new" song

	srand((unsigned int)time(NULL));

	int numSongs=(int)theLyrics["lyricsarr"].size();

	int verse = 0;
	int songLen = 10;
	while (verse < songLen)
	{
		int randSong = rand() % numSongs;
		std::string curVerse = pickRandomVerse(randSong,theLyrics);
		std::cout << curVerse << std::endl;
		verse++;
	}

	std::cout << std::endl;
}
