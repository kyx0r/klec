#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int n = (argc == 3) ? atoi(argv[2]) : 16;
    if(argc < 2 || n > 255 || n < 1){
        printf("Usage: hexview <file source> [# bytes to display max 255 bytes]\n");
        exit(1);
    }

    FILE *src = fopen(argv[1], "rb");
    char *BUFF = (char*)malloc(n+500);
    if(BUFF == NULL || src == NULL) exit(1);

    memset(BUFF, 0, n+500);
    unsigned char c;
    int i, j;
    unsigned int byte_count = 0;

    //byte read
    printf("<span class='ws'>0000</span>%04X<span class='ws'> ", 0); 
    for(i = n, j = 0; fread(&c, 1, 1, src) >= 1; i--){
        if(!i){
            printf("</span> ");
	    char* iter = BUFF;
	    while(*iter)
	    {
		    if(*iter == '<')
		    {
			    printf("&lt;");
		    } else if(*iter == '>') {
			    printf("&gt;");
		    } else {
			    putc(*iter, stdout);	
		    }
		    iter++;
	    }
            memset(BUFF, 0, n+500);
            putchar('\n');
            i = n; j = 0;
            byte_count += n;
            printf("<span class='ws'>0000</span>%04X<span class='ws'> ", byte_count); 
	    //print # bytes read
        }
        BUFF[j++] = (c >= '!' && c <= '~') ? c : ' '; //store character
	if(i == 8 || i == n)
	{
		printf("</span> %02X<span class='ws'> ", c); //print byte
	} else {
		printf("</span>%02X<span class='ws'> ", c); //print byte
	}
    }

    putchar('\n');
    fflush(stdin);
    free(BUFF);
    fclose(src);
    return 0;
}
