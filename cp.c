/*#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

char buf[512];

void cat(int fd1, int fd2){
  int n;

  while((n = read(fd1, buf, sizeof(buf))) > 0) {
    write(1, buf, n);
  }
  if(n < 0){
    printf(1, "copy: read error\n");
    exit();
  }
}

int main(int argc, char *argv[]){
  int fd1,fd2,i;
  if(argc < 3){
    	printf(1, "copy: read error\n");
    	exit();
  }
  
  //fungsi cp -r nya masih copy biasa
  if(argc == 4 && strcmp(argv[1],"-R")==0){
  	if((fd1 = open(argv[2], O_RDONLY)) < 0){
    		printf(1, "copy: cannot open %s\n", argv[1]);
    		exit();
  	}
  	if ((fd2 = open(argv[3], O_CREATE | O_RDWR)) < 0) {
    		printf(1, "copy: error initializing file: %s\n", argv[2]);
    		exit();
  	}
  	cat(fd1,fd2);
	close(fd1);
  	close(fd2);
  	exit();
  } //cp biasa : copy 1/lebih file
  else if(argc>=3){
	for(i=1;i<argc-1;i++){
		if((fd1 = open(argv[i], O_RDONLY)) < 0){
    			printf(1, "copy: cannot open %s\n", argv[1]);
    			exit();
		}
		if ((fd2 = open(argv[argc], O_CREATE | O_RDWR)) < 0) {
    			printf(1, "copy: error initializing file: %s\n", argv[2]);
    			exit();
  		}
		cat(fd1,fd2);
		close(fd1);
  		close(fd2);
  	}
	exit();
  }
}*/

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

void cat(int fd1, int fd2){
  int n;

  while(( n=read(fd1,buf,sizeof(buf))) > 0){
		write(fd1,buf,n);
	}
  if(n < 0){
    printf(1, "copy: read error\n");
    exit();
  }
}

int main(int argc, char *argv[])
{
	int fd0,fd1;

	if(argc <= 2){
		printf(1, "Need 2 arguments\n");
		exit();
	}

	if((fd0 = open(argv[1], O_RDONLY)) < 0){
		printf(1,"cp: cant not open %s\n",argv[1]);
		exit();
	}

	if((fd1 = open(argv[2], O_CREATE|O_RDWR)) < 0){
		printf(1,"cp: cant not open %s\n",argv[2]);
		exit();
	}

	cat(fd0,fd1);
	close(fd0);
	close(fd1);
  exit();
}
