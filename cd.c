#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    printf(2, "Usage: change current directory (not possible trough program though, this is just a dud)\n");
    exit();
  }

  if(chdir(argv[1]) < 0){
    printf(2, "cd: %s failed to change directory\n", argv[i]);
  }

  exit();
}
