#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "Name           Type  Index    Size\n");
    if(st.ino<10) printf(1, "%s File       %d      %d\n", fmtname(path), st.ino, st.size);
    else printf(1, "%s File    %d     %d\n", fmtname(path), st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    printf(1, "Name           Type         Index    Size\n");
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      if(st.type==1){
        if(st.ino<10) printf(1, "%s Directory      %d      %d\n", fmtname(buf), st.ino, st.size);
    	else printf(1, "%s Directory      %d     %d\n", fmtname(buf), st.ino, st.size);
      }
      else if(st.type==2){
	     if(st.ino<10) printf(1, "%s File           %d      %d\n", fmtname(buf), st.ino, st.size);
    	     else printf(1, "%s File           %d     %d\n", fmtname(buf), st.ino, st.size);
      }
      else if(st.type==3){
	     if(st.ino<10) printf(1, "%s Device        %d      %d\n", fmtname(buf), st.ino, st.size);
    	     else printf(1, "%s Device         %d     %d\n", fmtname(buf), st.ino, st.size);
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit();
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit();
}
