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
	enum winStatus { nothing = 0, ambo = 1, terno = 2, quaterna = 3, cinquina = 4 };
	winStatus ws;

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

	bool checkNinARow(int row, int howMany)
	{
		int numInARow = 0;
		for (int c = 0;c < 5;c++)
		{
			if (numbers[row][c].status == 1) numInARow++;
		}

		if (numInARow >= howMany) return true;

		return false;
	}

	void placeNumberAndCheck(int n, bool& isAmbo, bool& isTerno, bool& isQuaterna, bool& isCinquina, bool& isTombola)
	{
		bool placed = false;
		int curRow = -1;
		for (int r = 0;r < 3;r++)
		{
			for (int c = 0;c < 5;c++)
			{
				if (numbers[r][c].number == n)
				{
					numbers[r][c].status = 1;
					placed = true;
					curRow = r;
				}
			}
		}

		if (placed)
		{
			if (ws == nothing)
			{
				isAmbo = false;
				if (this->checkNinARow(curRow,2))
				{
					ws = ambo;
					isAmbo = true;
				}
				return;
			}
			else if (ws == ambo)
			{
				isTerno= false;
				if (this->checkNinARow(curRow, 3))
				{
					ws = terno;
					isTerno = true;
				}
				return;
			}
			else if (ws == terno)
			{
				isQuaterna= false;
				if (this->checkNinARow(curRow, 4))
				{
					ws = quaterna;
					isQuaterna= true;
				}
				return;
			}
			else if (ws == quaterna)
			{
				isCinquina= false;
				if (this->checkNinARow(curRow, 5))
				{
					ws = cinquina;
					isCinquina= true;
				}
				return;
			}
			else if (ws == cinquina)
			{
				isTombola = true;
				for (int r = 0;r < 3;r++)
				{
					for (int c = 0;c < 5;c++)
					{
						if (numbers[r][c].status == 0) isTombola = false;
					}
				}
				return;
			}
		}
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
	enum winStatus { nothing=0, ambo=1, terno=2, quaterna=3, cinquina=4 };
	winStatus ws;

public:

	tabellone()
	{
		ws = nothing;

		int num = 1;
		for (int r = 0;r < 9;r++)
		{
			std::vector<numberStatus> newRow;
			for (int c = 0;c < 10;c++)
			{
				numberStatus ns(num);
				newRow.push_back(ns);
				num++;
			}
			numbers.push_back(newRow);
		}
	}

	bool checkNinARow(int numRow, int numCol, int cartel, int n)
	{
		int numInARow = 0;
		if (cartel == 0)
		{
			for (int c = 0;c < 5;c++)
			{
				if (numbers[numRow][c].status == 1) numInARow++;
			}
		}
		else
		{
			for (int c = 5;c < 10;c++)
			{
				if (numbers[numRow][c].status == 1) numInARow++;
			}
		}

		if (numInARow >= n) return true;
		else return false;
	}

	bool checkForTombola(int r, int c)
	{
		for (int row = r;row < (r+3);row++)
		{
			for (int col = c;col < (c+5);col++)
			{
				if (numbers[row][col].status == 0) return false;
			}
		}

		return true;
	}

	void placeNumberAndCheck(int n,bool& isAmbo,bool& isTerno,bool& isQuaterna,bool& isCinquina,bool& isTombola)
	{
		numbers[(n - 1) / 10][(n - 1) % 10].status = 1;
		int numRow = (n - 1) / 10;
		int numCol = (n - 1) % 10;
		int cartel = (n <= 5) ? 0 : 1;

		if (ws == nothing)
		{
			isAmbo = false;
			if (checkNinARow(numRow, numCol, cartel, 2))
			{
				ws = ambo;
				isAmbo = true;
				return;
			}
		}
		else if (ws == ambo)
		{
			isTerno = false;
			if (checkNinARow(numRow, numCol, cartel, 3))
			{
				ws = terno;
				isTerno = true;
				return;
			}
		}
		else if (ws == terno)
		{
			isQuaterna = false;
			if (checkNinARow(numRow, numCol, cartel, 4))
			{
				ws = quaterna;
				isQuaterna = true;
				return;
			}
		}
		else if (ws == quaterna)
		{
			isCinquina= false;
			if (checkNinARow(numRow, numCol, cartel, 5))
			{
				ws = cinquina;
				isCinquina= true;
				return;
			}
		}
		else if (ws == cinquina)
		{
			isTombola = false;
			if (
				(checkForTombola(0, 0))||
				(checkForTombola(3, 0))||
				(checkForTombola(6, 0))||
				(checkForTombola(0, 5))||
				(checkForTombola(3, 5))||
				(checkForTombola(6, 5))
				)
			{
				isTombola = true;
				return;
			}
		}
	}

	void print()
	{
		int num = 0;
		for (int r = 0;r < 9;r++)
		{
			for (int c = 0;c < 10;c++)
			{
				if (numbers[r][c].status == 0)
				{
					std::cout << std::setw(2) << numbers[r][c].number << " ";
				}
				else
				{
					std::cout << "XX ";
				}
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}
};

//
//
//

void day17()
{
	srand((unsigned int)time(NULL));

	const int numPlayers=5; // 0 is you, the others are NPC
	std::vector<cartel> cartelList;
	for (int p = 0;p < numPlayers;p++)
	{
		cartel newCartel;
		cartelList.push_back(newCartel);
	}

	tabellone tabb;

	// start the simulation

	std::vector<int> backpack;
	for (int n = 1;n < 90;n++)
	{
		backpack.push_back(n);
	}
	random_shuffle(std::begin(backpack), std::end(backpack));

	int bppos = 0;
	int winStatus = 0; // 0=nothing,1=ambo,etc.
	bool winCondition = false;
	while (!winCondition)
	{
		// extract a number
		int extractedNum = backpack[backpack.size() - 1];
		backpack.pop_back();

		std::cout << "Number " << std::to_string(extractedNum) << " was extracted" << std::endl;

		// place number on the tabellone and check if there's an ambo, terno, etc. or if there's a win condition (tombola)
		bool isAmbo = false;
		bool isTerno = false;
		bool isQuaterna = false;
		bool isCinquina = false;
		bool isTombola = false;
		tabb.placeNumberAndCheck(extractedNum, isAmbo, isTerno, isQuaterna, isCinquina, isTombola);

		if ((winStatus == 0) && (isAmbo))
		{
			std::cout << "Tabellone scores an ambo!" << std::endl;
			winStatus = 1;
		}
		else if ((winStatus==1)&&(isTerno))
		{
			std::cout << "Tabellone scores a terno!" << std::endl;
			winStatus = 2;
		}
		else if ((winStatus == 2) && (isQuaterna))
		{
			std::cout << "Tabellone scores a quaterna!" << std::endl;
			winStatus = 3;
		}
		else if ((winStatus==3)&&(isCinquina))
		{
			std::cout << "Tabellone scores a cinquina!" << std::endl;
			winStatus = 4;
		}
		else if ((winStatus==4)&&(isTombola))
		{
			std::cout << "Tabellone scores TOMBOLA!" << std::endl; 
			winCondition = true;
			tabb.print();
		}

		// place number on each cartel, see if there's an ambo, etc. or if it's a win condition 
		for (int cart = 0;cart < numPlayers;cart++)
		{
			bool cartIsAmbo = false;
			bool cartIsTerno = false;
			bool cartIsQuaterna = false;
			bool cartIsCinquina = false;
			bool cartIsTombola = false;
			cartelList[cart].placeNumberAndCheck(extractedNum, cartIsAmbo, cartIsTerno, cartIsQuaterna, cartIsCinquina, cartIsTombola);

			if ((!isAmbo) && (!isTerno) && (!isQuaterna) && (!isCinquina) && (!isTombola))
			{
				std::string who = "Player " + std::to_string(cart);
				if (cart == 0) who = "Your cartel";
				if ((winStatus==0)&&(cartIsAmbo))
				{
					std::cout << who << " scores an ambo!" << std::endl;
					cartelList[cart].printCartel();
					winStatus = 1;
				}
				else if ((winStatus==1)&&(cartIsTerno))
				{
					std::cout << who << " scores a terno!" << std::endl;
					cartelList[cart].printCartel();
					winStatus = 2;
				}
				else if ((winStatus==2)&&(cartIsQuaterna))
				{
					std::cout << who << " scores a quaterna!" << std::endl;
					cartelList[cart].printCartel();
					winStatus = 3;
				}
				else if ((winStatus==3)&&(cartIsCinquina))
				{
					std::cout << who << " scores a cinquina!" << std::endl;
					cartelList[cart].printCartel();
					winStatus = 4;
				}
				else if ((winStatus == 4) && (cartIsTombola))
				{
					std::cout << who << " scores TOMBOLA!" << std::endl;
					cartelList[cart].printCartel();
					winCondition=true;
				}
			}
		}

		//

		if (backpack.size()==0)
		{
			// but this should not happen
			winCondition = true;
		}
	}
}
