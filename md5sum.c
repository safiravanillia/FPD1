#include "CustomMD5Header.h"

void md5string(md5context_t * ctx, char * input);
void md5file(md5context_t * ctx, char * filename);

int main(int argc, char **argv){
    if(argc < 2 ||( strcmp(argv[1],"-s")!=0 && strcmp(argv[1],"-f")!=0)){
        printf(1,"Usage:\n");
        printf(1,"-s: digest string\n");
        printf(1,"-f: digest file\n");
        exit();
    }
    md5context_t ctx;
    if(strcmp(argv[1],"-s")==0){
        md5string(&ctx,argv[2]);
    }
    else{
	printf(1,"Erorr Input");	
	}
    exit();
}


void md5string(md5context_t * ctx, char * input){
    uint32 len = strlen(input);
    byte result[16];
    Init_Context(ctx);
    Update_Context(ctx, (byte*)input, len);
    Finalization(ctx,result);
    printmsg(result,16);
    printf(1,"\n");
}


