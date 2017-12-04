#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void move(int *from, int *to)
{
  // Unlink whatever is there
  unlink(to);	
  
  // Link old to new
  if (link(from, to)!=0) {
    fprintf(stderr, "Unable to link %s to %s\n", from, to);
    exit(1);
  }

  // Now try to unlink the old one
  if (unlink(from)!=0) {
    fprintf(stderr, "Unable to unlink %s\n", from);
    exit(1);
  }
}

int main (int argc, char *argv[])
{
  struct stat sb;

if (argc < 3)
	{
    		printf(1, "mv: read error\n");
		exit();
  	}
  // See if the last argument is a directory
  if ((stat(argv[argc-1], &sb)==0) && ((sb.st_mode & S_IFMT) == S_IFDIR)) {

    // Yes, link each argument into that directory
    for (int i= 1; i < argc-1; i++) {

      // Append the from file's name to the directory name
      strcpy(fulldest, argv[argc-1]);
      strcat(fulldest, "/");
      strcat(fulldest, argv[i]);
      move(argv[i], fulldest);
    }
  } else {
    // We should only have two files
    if (argc != 3) {
      fprintf(stderr, "Usage: mv file1 file, or mv file file ... dir\n");
      exit(1);
    }
    move(argv[1], argv[2]);
  }
  exit();	
}
