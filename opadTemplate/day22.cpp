//
// oppd
// day22
// friol
// 2k22
//

#include "include/oppd.h"

bool hasAllowedChars(std::string sentence)
{
	std::string allowedChars = "abcdefghijklmnopqrstuvwxyz0123456789";
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

}

void day22()
{
	bool goout = false;
	while (!goout)
	{
		std::string sentence;

		std::cout << "Enter sentence to encode:" << std::endl << ">";
		std::cin >> sentence;

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
				}
			}
		}
	}
}
