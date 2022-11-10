//
// oppd
// cpu tester
// friol's 2o22
//

#include "include/oppd.h"

void day10usage(char** argv)
{
	std::cout << "Usage:" << std::endl << argv[0] << " [numberOfTests]" << std::endl;
}

void day10(int argc, char** argv)
{
	int numberOfTests=10;

	if (argc == 2)
	{
		numberOfTests = atoi(argv[1]);
	}
	else if (argc > 2)
	{
		day10usage(argv);
		return;
	}

}
