#include <unistd.h>
#include <string.h>
#include <stdio.h>

void p(char *buff, char *header)
{
    char pBuffer[0x1000];

    puts(header);
    read(0, pBuffer, 0x1000);
    *(strchr(pBuffer, 0xa)) = '\0';
    strncpy(buff, pBuffer, 0x14);
}

void pp(char *buff)
{
    char firstBuffer[20];
    char secondBuffer[20];

    p(firstBuffer, " - ");
    p(secondBuffer, " - ");

    strcpy(buff, firstBuffer);
    int len  = strlen(buff);
    buff[len] = ' ';
    buff[len + 1] = 0x0;
    strcat(buff, secondBuffer);
}

int main(int argc, char **argv)
{
    char buff[42];

    pp(buff);
    puts(buff);
    return 0x0;
}
