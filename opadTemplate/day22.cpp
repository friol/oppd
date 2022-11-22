//
// this oppd program will be transmitted in morse code
// day22
// friol 2k22
//

#include "include/oppd.h"

bool hasAllowedChars(std::string sentence)
{
	std::string allowedChars = "abcdefghijklmnopqrstuvwxyz0123456789 ";
	for (char& c : sentence)
	{
		bool found = false;
		for (char& ac : allowedChars)
		{
			if (ac == c) found = true;
		}

		if (!found) return false;
	}

	return true;
}

std::string morseEncode(std::string sentence)
{
	std::string morseCodes[] = {
		".-", // a
		".---",
		"-.-.",
		"-..",
		".",
		"..-.",
		"--.",
		"....",
		"..",
		".---",
		"-.-",
		".-..",
		"--",
		"-.",
		"---",
		".--.",
		"--.-",
		".-.",
		"...",
		"-",
		"..-",
		"...-",
		".--",
		"-..-",
		"-.--",
		"--..",// z
		"-----", // 0
		".----",
		"..---",
		"...--",
		"....-",
		".....",
		"-....",
		"--...",
		"---..",
		"----." // 9
	};

	std::string encodedString = "";
	std::string charsArray = "abcdefghijklmnopqrstuvwxyz0123456789";
	for (char& c : sentence)
	{
		if (c == ' ')
		{
			encodedString += "  ";
		}
		else
		{
			int pos = 0;
			for (char& ac : charsArray)
			{
				if (ac == c)
				{
					encodedString += morseCodes[pos] + " ";
				}
				pos++;
			}
		}
	}

	return encodedString;
}

void day22()
{
	bool goout = false;
	while (!goout)
	{
		std::string sentence;

		std::cout << std::endl << "Enter sentence to encode:" << std::endl << ">";
		std::getline(std::cin, sentence);

		if ((sentence == "quit") || (sentence == "exit"))
		{
			goout = true;
		}
		else
		{
			// validate sentence
			if (sentence.size() == 0)
			{
				std::cout << "Error: sentence should be at least 1 character long." << std::endl;
			}
			else
			{
				if (!hasAllowedChars(sentence))
				{
					std::cout << "Error: sentence shoud contain only letters (a-z) and numbers (0-9)." << std::endl;
				}
				else
				{
					std::string encodedString = morseEncode(sentence);
					std::cout << "String in morse code is: " << encodedString << std::endl;
				}
			}
		}
	}
}
