#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define getlink() \
if (fgets(link, 4096, linkidx)) \
{ \
	char *p = link; \
	start = atoi(p); \
	while (1) \
	{ \
		if (*p == ',') \
			end = atoi(p+1); \
		else if (*p == ' ' && !plink) \
			plink = p+1; \
		else if (*p == '\n') \
		{ \
			*p = '\0'; \
			break; \
		} \
		p++; \
	} \
} \

int main(int argc, char *argv[])
{
	int n = 16; //number of bytes width;
	if(argc < 2 || n > 255 || n < 1){
		printf("Usage: htmlhex <file source> <links file(optional)>\n");
		exit(1);
	}

	FILE *src = fopen(argv[1], "rb");
	FILE *linkidx = fopen(argv[2], "rb");
	int start = 0;
	int end = 0;
	char link[4096];
	char old_buf[4096];
	char *plink = NULL;

	if (linkidx)
	{
		getlink()
		if (plink)
			strcpy(old_buf, plink);
	}
//	printf("%d %d %s\n", start, end, plink);
		
	char *BUFF = (char*)malloc(n+1);
	if(BUFF == NULL || src == NULL) exit(1);

	memset(BUFF, 0, n+1);
	unsigned char c;
	int i, j;
	unsigned int byte_count = 0;
	int byte_done = 0; //same as j but counts number of bytes until \n, not when = to n
	int started = 0;
	int _beg = -1;
	int _end = -1;

	//byte read
	printf("0000%04X  ", 0);
	for(i = n, j = 0; fread(&c, 1, 1, src) >= 1; i--){
		if(!i) {
			printf(" ");
			char* iter = BUFF;
			int cc = 0;
			int edge = 0;
			while(*iter)
			{
				if(cc == _beg)
				{
					printf("%s", old_buf);
					edge = 1;
				}
				else if (_beg == -1 && _end != -1 && !cc)
				{
					printf("%s", old_buf);
				}
				else if (cc == _end)
				{
					printf("</a>");
					edge = 0;
				}
				if(*iter == '<')
					printf("&lt;");
				else if(*iter == '>')
					printf("&gt;");
				else
					putc(*iter, stdout);
				cc++;
				iter++;
			}
			if (edge)
				printf("</a>");
			memset(BUFF, 0, n+1);
			putchar('\n');
			i = n; j = 0;
			started = 0;
			byte_count += n;
			printf("0000%04X  ", byte_count);
			_beg = -1;
			_end = -1;
			//print # bytes read
		}
		byte_done++;
		BUFF[j++] = (c >= '!' && c <= '~') ? c : ' '; //store character
		if (c == '\n')
		{
			byte_done = 0;	
			start = 0;
			end = 0;
			plink = 0;
			getlink()
		}
		char spc[2];
		spc[0] = i == n/2 ? ' ' : 0;
		spc[1] = '\0';
		//print byte
		if (plink)
		{
			if(byte_done == start || (byte_done > start && byte_done < end && i == n))
			{
				printf("%s%s%02X%s ", plink, spc, c, i == 1 ? "</a>" : "");
				started = 1;
				_beg = j-1;
				strcpy(old_buf, plink);
			}
			else if (byte_done == end || i == 1 && started)
			{
				printf("%s%s%02X</a> ", spc, i == n ? plink : "", c);
				started = 0;
				_end = j;
			}
			else
				printf("%s%02X ", spc, c);
		}
		else
			printf("%s%02X ", spc, c);
	}

	putchar('\n');
	fflush(stdin);
	free(BUFF);
	fclose(src);
	return 0;
}
