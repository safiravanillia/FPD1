#include "types.h"
#include "stat.h"
#include "user.h"  
int main(int argc, char *argv[])
{
  int i;

  if(argc == 1){
    printf(2, "Usage: rm files...\n"); 
    exit();
  }
  else if(argc == 2){
      printf(2, "Usage: rm folder...\n"); 
    exit();
  
  }

  for(i = 1; i < argc; i++){

	if (unlink(argv[2]) == 0){
		 printf(1, "berhasil mendelete file %s\n",argv[i]);}
	 else{
     		 printf(1, "gagal mendelete file %s\n",argv[i]);}
 
     
    } 
 exit();
}
