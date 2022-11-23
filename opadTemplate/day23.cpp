//
// oppd - random quote game
// friol 2k22
//

#include "include/oppd.h"

const std::string whiteSpaces(" \f\n\r\t\v");

void trim(std::string& str, const std::string& trimChars = whiteSpaces);


void day23()
{
	// download quote from https://zenquotes.io/api/random

	httplib::Client cli("https://zenquotes.io");
	auto res = cli.Get("/api/random");

	if (res->status != 200)
	{
		std::cout << "Unable to reach zenquotes.io" << std::endl;
		return;
	}

	json quote = json::parse(res->body);

	std::string sentence = quote[0]["q"];
	std::string author = quote[0]["a"];
	trim(sentence);

	std::cout << sentence << std::endl << std::endl;


	// propose 3 random authors from storage

	std::vector<std::string> listOfAuthors;
	std::ifstream authorsFile;
	authorsFile.open("authors.txt", std::ios::in);

	if (authorsFile.is_open())
	{
		std::string oneAuthor;
		while (std::getline(authorsFile, oneAuthor))
		{
			listOfAuthors.push_back(oneAuthor);
		}
	}
	else
	{
		std::cout << "Error opening authors.txt file." << std::endl;
		return;
	}

	srand((unsigned int)time(NULL));
	int rand1 = rand() % (listOfAuthors.size());
	int rand2 = (rand1 + 5000) % listOfAuthors.size();

	std::vector<std::string> choices;
	choices.push_back(listOfAuthors[rand1]);
	choices.push_back(listOfAuthors[rand2]);
	choices.push_back(author);
	std::random_shuffle(std::begin(choices), std::end(choices));

	std::cout << "Who is the author of the sentence?" << std::endl;
	std::cout << "1) " << choices[0] << std::endl;
	std::cout << "2) " << choices[1] << std::endl;
	std::cout << "3) " << choices[2] << std::endl;

	bool validAnswer = false;
	while (!validAnswer)
	{
		std::string answer;
		std::cout << ">";
		std::getline(std::cin, answer);

		if ((answer != "1") && (answer != "2") && (answer != "3"))
		{
			std::cout << "Please answer with 1, 2 or 3." << std::endl;
		}
		else
		{
			validAnswer = true;
			int ianswer = stoi(answer);

			if (choices[ianswer - 1] == author)
			{
				std::cout << "That's correct!" << std::endl;
			}
			else
			{
				std::cout << "No, the original author was " << author << std::endl;
			}
		}
	}
}
