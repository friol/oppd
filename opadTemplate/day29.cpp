
//
// oppd
// friol2k22
//

#include "include/oppd.h"

void day29()
{
	std::vector<std::string> listOfTimezones;

	std::ifstream tzFile;
	tzFile.open("timezones.txt", std::ios::in);
	if (tzFile.is_open())
	{
		std::string tz;
		while (std::getline(tzFile, tz))
		{
			listOfTimezones.push_back(tz);
		}
	}
	else
	{
		std::cout << "Error: cannot open timezones.txt file." << std::endl;
		return;
	}

	//

	srand((unsigned int)time(NULL));
	int rndTz = rand() % listOfTimezones.size();

	httplib::Client cli("https://worldtimeapi.org");
	auto res = cli.Get("/api/timezone/"+listOfTimezones[rndTz]);

	if (res->status != 200)
	{
		std::cout << "Unable to reach worldtimeapi.org" << std::endl;
		return;
	}

	json tzResult = json::parse(res->body);
	std::string timeOffset = tzResult["utc_offset"];
	int iTimeOffset = stoi(timeOffset) - 1;
	//std::cout << stoi(offset) << std::endl;

	struct tm t;
	time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	localtime_s(&t, &now);

	//std::cout << iTimeOffset << std::endl;
	std::cout << "If the current hour is " << t.tm_hour << " what time is it in " << listOfTimezones[rndTz] << "?" << std::endl << ">";

	std::string answer;
	std::cin >> answer;

	int nowh = t.tm_hour;
	int thereh = stoi(answer);

	if (((thereh-nowh)%24) == iTimeOffset)
	{
		std::cout << "You're right!" << std::endl;
	}
	else
	{
		std::cout << "Wrong, sorry." << std::endl;
	}
}
