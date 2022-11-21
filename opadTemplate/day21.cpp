//
// capital cities
// now also an oppd program
// friol 2k22
//

#include "include/oppd.h"

void day21()
{
	std::map<std::string, std::string> capcityMap;

	// load capital city file
	std::ifstream capcityFile;
	capcityFile.open("country-capitals.csv", std::ios::in);

	int numCaps = 0;
	if (capcityFile.is_open())
	{
		std::string aline;
		while (std::getline(capcityFile, aline))
		{
			//std::cout << aline << std::endl;
			std::string stateName = aline.substr(0, aline.find(','));
			stateName.replace(stateName.find("\""), 1, "");
			stateName.replace(stateName.find("\""), 1, "");


			std::string capName = aline.substr(aline.find(',')+1);
			capName.replace(capName.find("\""), 1, "");
			capName.replace(capName.find("\""), 1, "");

			//std::cout << stateName << " " << capName << std::endl;

			numCaps++;
			capcityMap[stateName] = capName;
		}
	}
	else
	{
		std::cout << "Error: missing country-capitals.csv file." << std::endl;
		return;
	}

	//

	srand((unsigned int)time(NULL));

	bool goout = false;
	int totalQuestions = 0;
	int rightAnswers = 0;
	while (!goout)
	{
		totalQuestions++;

		int randomCap = rand() % numCaps;

		std::map<std::string, std::string>::iterator it = capcityMap.begin();
		for (int i = 0;i < randomCap;i++)
		{
			it++;
		}
		std::string stateName = it->first;
		std::string capName = it->second;

		std::string answer;
		std::cout << "Which is the capital of " << stateName << "?" << std::endl << ">";
		std::getline(std::cin, answer);

		std::transform(answer.begin(), answer.end(), answer.begin(), [](unsigned char c) { return std::tolower(c); });

		if ((answer == "quit") || (answer == "exit") || (answer == "q"))
		{
			goout = true;
		}
		else
		{
			std::string capLowa = capName;
			std::transform(capLowa.begin(), capLowa.end(), capLowa.begin(), [](unsigned char c) { return std::tolower(c); });

			if (answer == capLowa)
			{
				std::cout << "That's right!" << std::endl << std::endl;
				rightAnswers++;
			}
			else
			{
				std::cout << "No, the capital of " << stateName << " is " << capName << std::endl << std::endl;
			}
		}
	}

	std::cout << "You got " << rightAnswers << " answers right out of " << totalQuestions << " questions." << std::endl;
}
