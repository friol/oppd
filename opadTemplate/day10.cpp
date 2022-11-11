//
// oppd
// cpu tester
// friol's 2o22
//

#include "include/oppd.h"

void performCPUtest(int numberOfTests)
{
	for (int t = 0; t < numberOfTests; t++)
	{
		// fibonacci test
		int fiboMax = 32;
		int fiboNum = (rand() % fiboMax);
		if (fiboNum < 20) fiboNum = 20;

		//std::cout << "Fibo num is " << fiboNum << std::endl;

		int fibonm2 = 0;
		int fibonm1 = 1;
		int newFibo = 1;
		for (int n = 2; n < fiboNum; n++)
		{
			newFibo = fibonm1 + fibonm2;
			fibonm2 = fibonm1;
			fibonm1 = newFibo;
		}

		//std::cout << fiboNum << " " << newFibo << std::endl;
	}
}

void day10usage(char** argv)
{
	std::cout << "Usage:" << std::endl << argv[0] << " [numberOfTests]" << std::endl;
}

void day10(int argc, char** argv)
{
	long testMultiplier = 4096;
	long numberOfTests=4096;

	if (argc == 2)
	{
		numberOfTests = atoi(argv[1]);
	}
	else if (argc > 2)
	{
		day10usage(argv);
		return;
	}

	// perform cpu test

	srand((unsigned int)time(NULL));

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	performCPUtest(numberOfTests*testMultiplier);

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	float delta = (float)std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	float numOfTestsNormalized = ((float)numberOfTests) / 4096.0;

	std::cout << "Milliseconds elapsed: " << delta << std::endl;
	std::cout << "This computer oppd index is " << (delta / 229.0)/numOfTestsNormalized << " (higher than 1: better)" << std::endl;
}
