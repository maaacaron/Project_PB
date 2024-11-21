#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


void make_twoway_pipe()         //파이프 이름 겹치지 않도록 A조와 B조 구분 필요
{
    unlink("./battlefifo1");
    unlink("./battlefifo2");

    if(mkfifo("./battlefifo1", 0666) == -1)
    {
        perror("mkfifo");
        exit(1);
    }
    if(mkfifo("./battlefifo2", 0666) == -1)
    {
        perror("mkfifo");
        exit(1);
    }
}


int main()
{
    int fd1, fd2, n;



}