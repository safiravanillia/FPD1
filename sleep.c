#include "types.h"
#include "stat.h"
#include "user.h"

int main(int n, char *chr[]){
	int p,m;
	p = atoi(chr[1]);
	m = p*10000/147;
	printf(1,"wait for %d seconds -> ",p);
	sleep(m);
	printf(1,"wait is over\n");
	exit();
}
