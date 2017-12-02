#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{

  if(argc < 2){
    printf(2, "Usage: changes current working directory...\n");
    exit();
  }

  if(chdir(argv[argc+1])<0){
    printf(2, "cannot cd %s\n", argv[argc]);
  }

  exit();
}
