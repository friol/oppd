//
// another day, another oppd
// instant-tombola
// friol 2o22
//

#include "include/oppd.h"

class numberStatus
{
private:

public:
	int number;
	int status; // 0=not extracted, 1=extracted

	numberStatus(int n)
	{
		this->number = n;
		this->status = 0;
	}
};

class cartel
{
private:
	
	std::vector<std::vector<numberStatus>> numbers;

public:

	cartel()
	{
		// every cartel has 3 lines of 5 random numbers
		// each number is unique (from 1 to 90)
		// each row is increasing from left to right

		std::vector<int> allNumbers;
		for (int i = 1;i <=90;i++)
		{
			allNumbers.push_back(i);
		}

		random_shuffle(std::begin(allNumbers), std::end(allNumbers));

		std::vector<int> fifteenNumbers;
		for (int i = 0;i < 15;i++)
		{
			fifteenNumbers.push_back(allNumbers[i]);
		}

		sort(std::begin(fifteenNumbers), std::end(fifteenNumbers));

		int pos = 0;
		std::vector<numberStatus> newRow1;
		std::vector<numberStatus> newRow2;
		std::vector<numberStatus> newRow3;
		for (int c = 0;c < 5;c++)
		{
			numberStatus ns1(fifteenNumbers[pos]);
			newRow1.push_back(ns1); pos++;
			numberStatus ns2(fifteenNumbers[pos]);
			newRow2.push_back(ns2); pos++;
			numberStatus ns3(fifteenNumbers[pos]);
			newRow3.push_back(ns3); pos++;
		}

		numbers.push_back(newRow1);
		numbers.push_back(newRow2);
		numbers.push_back(newRow3);
	}

	void printCartel()
	{
		for (int r = 0;r < 3;r++)
		{
			for (int c = 0;c < 5;c++)
			{
				std::cout << numbers[r][c].number << " ";
			}
			std::cout << std::endl;
		}
	}
};

class tabellone
{
private:

	std::vector<std::vector<numberStatus>> numbers;

public:

	tabellone()
	{
		int num = 1;
		for (int r = 0;r < 9;r++)
		{
			std::vector<numberStatus> newRow;
			for (int c = 0;c < 10;c++)
			{
				newRow.push_back(num);
				num++;
			}
			numbers.push_back(newRow);
		}
	}
};

//
//
//

void day17()
{
	srand((unsigned int)time(NULL));

	const int numOtherPlayers = 4;
	cartel playerCartel;
	std::vector<cartel> otherCartels;
	for (int p = 0;p < numOtherPlayers;p++)
	{
		cartel newCartel;
		otherCartels.push_back(newCartel);
	}

	tabellone tabb;

	// start the simulation



}
