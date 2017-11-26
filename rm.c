#include "types.h"
#include "stat.h"
#include "user.h"
#include "stdio.h"   
#include "stdlib.h"
 
int main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    printf(2, "Usage: rm files...\n"); 
    exit();
  }

  for(i = 1; i < argc; i++){
	if (remove(argv[i]) == 0){
		 printf("berhasil mendelete");}
	 else{
     		 printf("gagal mendelete");}
 
      break;
    } 
 
}
