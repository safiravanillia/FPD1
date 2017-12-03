#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc,char *argv[])
{
	int i;
	int inputfile;
	for(i = 1;i<=argc;i++)
	{
		inputfile = open(argv[i],O_CREATE | O_RDWR);
		close(inputfile);
	}
	exit();
}
