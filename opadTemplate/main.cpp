
//
// oppd project
// one program per day, November 2o22
// friol 2k22
//

#include "include/oppd.h"
#include "include/mazecell.h"

int main(int argc,char** argv)
{
    // seems it's the only way in windows to print unicode chars in the console
    int err=_setmode(_fileno(stdout), _O_U16TEXT);

    clearScreen();

    day01(argc,argv);

	return 0;
}
