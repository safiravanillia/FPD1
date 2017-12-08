#include "CustomMD5Header.h"

void md5string(md5context_t * ctx, char * input);
void md5file(md5context_t * ctx, char * filename);

void printmsg(unsigned char *msg,unsigned int len)
{
	unsigned int i;
	const char base16[16] = {
		'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	for(i=0;i<len;i++)
	{
		printf("%c",base16[(((int)msg[i]&0xf0)>>4)]);
		printf("%c",base16[(((int)msg[i]&0xf))]);
	}
	printf("\n");
}

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
    else if(strcmp(argv[1],"-f")==0){
        md5file(&ctx,argv[2]);
    }
    exit();
}


void md5string(md5context_t * ctx, char * input){
    uint32 len = strlen(input);
    byte result[16];
    Init_Context(ctx);
    Update_Context(ctx, (byte*)input, len);
    Finalization(ctx,result);
    int i;
	printmsg(result,3);
    //for(i=0; i<16; i++)
    //   printf(1,"%2.2x",result[i]);
    //printf(1,"\n");
}

void md5file(md5context_t * ctx, char * filename){
    int fd;
    uint32 r;
    byte buff[1024];
    byte result[16];
    Init_Context(ctx);
    if((fd = open(filename, O_RDONLY)) < 0){
        printf(2,"File not found\n");
        exit();
    }
    while ((r = read(fd, buff, sizeof(buff))) > 0) {
        Update_Context(ctx, (byte*)buff, r);
    }
    Finalization(ctx,result);
    close(fd);
    int i;
    for(i=0; i<16; i++)
        printf(1,"%2.2x",result[i]);
    printf(1,"\n");
}
