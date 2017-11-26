#include "types.h"
#include "stat.h"
#include "user.h"  
int main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    printf(2, "Usage: rm files...\n"); 
    exit();
  }

  for(i = 1; i < argc; i++){
	if (unlink(argv[i]) == 0){
		 printf(1, "berhasil mendelete file %s\n",argv[i]);}
	 else{
     		 printf(1, "gagal mendelete file %s\n",argv[i]);}
 
     
    } 
 exit();
}
