#include "types.h"
#include "stat.h"
#include "user.h"    
int main(int argc, char *argv[])
{ 


  int i ; 
  if(argc == 1 && strcmp(argv[0],"rm")==0 ){
    printf(2, "Usage: rm files... %s\n",argv[0]); 
    exit();
  }
  else if(argc == 2 && strcmp(argv[0],"rm")==0 && strcmp(argv[1],"-r")==0){
      printf(2, "Usage: rm -r folder...\n"); 
    exit();
  
  }
 
  for(i = 1; i < argc; i++){



	if (unlink(argv[i]) == 0){
		 printf(1, "berhasil mendelete file %s\n",argv[i]);}
	  
 
     
    } 
 exit();
}
