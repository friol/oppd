//
// oppd 2o22
// friol
// the jar
//

#include "include/oppd.h"

void drawJar(std::vector<std::vector<int>>& lofBeans)
{
	consoleGotoxy(0, 0);

	std::cout << "           xxxx" << std::endl;
	std::cout << "           x  x" << std::endl;
	std::cout << "         xxx  xxx" << std::endl;
	std::cout << "   xxxxxx        xxxxxx  " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << " xx                    xx " << std::endl;
	std::cout << "  x                    x " << std::endl;
	std::cout << "   xxxxxxxxxxxxxxxxxxxx " << std::endl;

	for (std::vector<int>& bean : lofBeans)
	{
		consoleGotoxy(bean[0] + 3, bean[1] + 4);
		std::cout << "*";
	}
}

void gener8beans(std::vector<std::vector<int>>& lofBeans,int numBeans)
{
	const int beansDimX = 20;
	const int beansDimY = 13;

	// generate numBeans unique beans
	int totBeans = 0;
	while (totBeans < numBeans)
	{
		int bx = rand() % beansDimX;
		int by = rand() % beansDimY;
		std::vector<int> newBean;
		newBean.push_back(bx);
		newBean.push_back(by);

		bool found = false;
		for (std::vector<int>& bean : lofBeans)
		{
			if ((bean[0] == bx) && (bean[1] == by))
			{
				found = true;
			}
		}

		if (found == false)
		{
			lofBeans.push_back(newBean);
			totBeans++;
		}
	}


}

void day14usage(char** argv)
{
	std::cout << "Usage:" << std::endl << argv[0] << " [easy|normal|hard]" << std::endl;
}

void day14(int argc,char** argv)
{
	// min num beans, max num beans, time to display

	int skillTable[3][3] = {
		{10,20,300},
		{20,40,160},
		{30,50,50}
	};

	int skillLevel = 1; // 0 easy, 1 normal, 2 hard

	if (argc == 2)
	{
		if (!strcmp(argv[1], "easy"))
		{
			skillLevel = 0;
		}
		else if (!strcmp(argv[1], "normal"))
		{
		}
		else if (!strcmp(argv[1], "hard"))
		{
			skillLevel = 2;
		}
		else
		{
			day14usage(argv);
		}
	}

	srand((unsigned int)time(NULL));

	int numBeans = skillTable[skillLevel][0]+(rand()% skillTable[skillLevel][1]);
	std::vector<std::vector<int>> listOfBeans;

	gener8beans(listOfBeans, numBeans);

	clearScreen();
	consoleHideCursor();

	drawJar(listOfBeans);
	int timed = skillTable[skillLevel][2];
	while (timed > 0)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		timed--;
	}

	// ok clear screen and ask
	clearScreen();

	consoleGotoxy(0, 20);
	std::cout << "How many beans were in the jar? (enter a number)" << std::endl << ">";
	std::string answer;
	std::cin >> answer;

	int iAnswer = 0;

	try
	{
		iAnswer = std::stoi(answer);
	}
	catch (const std::invalid_argument& e) 
	{
		std::cout << "Error: [" << e.what() << "]" << std::endl;
		return;
	}

	if (iAnswer == numBeans)
	{
		std::cout << "That's the right answer!" << std::endl;
	}
	else
	{
		std::cout << "Sorry, the jar contained " << numBeans << " beans." << std::endl;
	}

	drawJar(listOfBeans);
	consoleGotoxy(0, 23);
}
