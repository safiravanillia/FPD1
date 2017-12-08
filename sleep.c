#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
	int convert,hasil;
	convert = atoi(argv[1]);
	hasil = convert*10000/147;
	printf(1,"wait fot %d seconds ->",convert);
	sleep(hasil);
	printf(1,"wait is over\n");
	exit();
}
