//
// typing game
// friol, oppd 2022
//

#include "include/oppd.h"


void day03()
{
	// get a random quote from https://zenquotes.io/api/random

	// HTTPS
	httplib::Client cli("https://zenquotes.io");
	auto res = cli.Get("/api/random");

	if (res->status != 200)
	{
		std::cout << "Unable to reach zenquotes.io" << std::endl;
	}

	json quote = json::parse(res->body);
	std::cout << quote << std::endl;


}
