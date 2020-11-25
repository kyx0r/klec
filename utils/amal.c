
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define p(...) {fprintf(stdout, __VA_ARGS__); \
fprintf(stdout, "\n");} \

#define pa(fp, ...) \
if(fp) \
{ \
fprintf(fp , __VA_ARGS__); \
fprintf(fp, "\n"); \
} else { \
p(__VA_ARGS__); \
} \

#define MAXPATH 20
static char *rblock;
static int crblock;
static int rblocksize;
static int *roffs;
static int croffs;
static int roffssize;
/* opts */
static int syspath;

/* Size of each input chunk to be
   read and allocate for. */
#ifndef  READALL_CHUNK
#define  READALL_CHUNK  262144
#endif

#define  READALL_OK          0  /* Success */
#define  READALL_INVALID    -1  /* Invalid parameters */
#define  READALL_ERROR      -2  /* Stream error */
#define  READALL_TOOMUCH    -3  /* Too much input */
#define  READALL_NOMEM      -4  /* Out of memory */

/* This function returns one of the READALL_ constants above.
   If the return value is zero == READALL_OK, then:
   (*dataptr) points to a dynamically allocated buffer, with
   (*sizeptr) chars read from the file.
   The buffer is allocated for one extra char, which is NUL,
   and automatically appended after the data.
   Initial values of (*dataptr) and (*sizeptr) are ignored.
 */
int readall(FILE *in, char **dataptr, size_t *sizeptr)
{
	char  *data = NULL, *temp;
	size_t size = 0;
	size_t used = 0;
	size_t n;

	/* None of the parameters can be NULL. */
	if (in == NULL || dataptr == NULL || sizeptr == NULL)
		return READALL_INVALID;

	/* A read error already occurred? */
	if (ferror(in))
		return READALL_ERROR;

	while (1) {

		if (used + READALL_CHUNK + 1 > size) {
			size = used + READALL_CHUNK + 1;

			/* Overflow check. Some ANSI C compilers
			   may optimize this away, though. */
			if (size <= used) {
				free(data);
				return READALL_TOOMUCH;
			}

			temp = realloc(data, size);
			if (temp == NULL) {
				free(data);
				return READALL_NOMEM;
			}
			data = temp;
		}

		n = fread(data + used, 1, READALL_CHUNK, in);
		if (n == 0)
			break;

		used += n;
	}

	if (ferror(in)) {
		free(data);
		return READALL_ERROR;
	}

	temp = realloc(data, used + 1);
	if (temp == NULL) {
		free(data);
		return READALL_NOMEM;
	}
	data = temp;
	data[used] = '\0';

	*dataptr = data;
	*sizeptr = used;

	return READALL_OK;
}

char *strstr(const char *haystack, const char *needle)
{
	if (!*needle) // Empty needle.
		return (char *) haystack;

	const char    needle_first  = *needle;

	// Runs strchr() on the first section of the haystack as it has a lower
	// algorithmic complexity for discarding the first non-matching characters.
	haystack = strchr(haystack, needle_first);
	if (!haystack) // First character of needle is not in the haystack.
		return NULL;

	// First characters of haystack and needle are the same now. Both are
	// guaranteed to be at least one character long.
	// Now computes the sum of the first needle_len characters of haystack
	// minus the sum of characters values of needle.

	const char   *i_haystack    = haystack + 1
		,   *i_needle      = needle   + 1;

	unsigned int  sums_diff     = *haystack;
	int identical     = 1;

	while (*i_haystack && *i_needle) {
		sums_diff += *i_haystack;
		sums_diff -= *i_needle;
		identical &= *i_haystack++ == *i_needle++;
	}

	// i_haystack now references the (needle_len + 1)-th character.

	if (*i_needle) // haystack is smaller than needle.
		return NULL;
	else if (identical)
		return (char *) haystack;

	size_t        needle_len    = i_needle - needle;
	size_t        needle_len_1  = needle_len - 1;

	// Loops for the remaining of the haystack, updating the sum iteratively.
	const char   *sub_start;
	for (sub_start = haystack; *i_haystack; i_haystack++) {
		sums_diff -= *sub_start++;
		sums_diff += *i_haystack;

		// Since the sum of the characters is already known to be equal at that
		// point, it is enough to check just needle_len-1 characters for
		// equality.
		if (
				sums_diff == 0
				&& needle_first == *sub_start // Avoids some calls to memcmp.
				&& memcmp(sub_start, needle, needle_len_1) == 0
		   )
			return (char *) sub_start;
	}

	return NULL;
}

char* strtok_r(char *s, const char *delim, char **save_ptr)
{
	char *end;
	if (s == NULL)
		s = *save_ptr;
	if (*s == '\0')
	{
		*save_ptr = s;
		return NULL;
	}
	/* Scan leading delimiters.  */
	s += strspn (s, delim);
	if (*s == '\0')
	{
		*save_ptr = s;
		return NULL;
	}
	/* Find the end of the token.  */
	end = s + strcspn (s, delim);
	if (*end == '\0')
	{
		*save_ptr = end;
		return s;
	}
	/* Terminate the token and make *SAVE_PTR point past it.  */
	*end = '\0';
	*save_ptr = end + 1;
	return s;
}

char* findch(char *str, char ch)
{
	while(*str)
	{
		if(*str == ch)
		{
			return str;
		}
		str++;
	}
	return NULL;
}

char* efindch(char *str, char ch)
{
	char *t = findch(str, ch);
	assert(t);
	return t;
}

FILE *fopenpath(char *name, char **path)
{
	FILE* f = fopen(name, "rb");
	int len, len1;
	char** path1 = path;
	if(!f)
	{
		len1 = strlen(name);
		while(*path)
		{
			len = strlen(*path);
			char onepath[len + len1];
			memcpy(onepath, *path, len);
			memcpy(onepath+len, name, len1);
			onepath[len+len1] = '\0';
			f = fopen(onepath, "rb");
			if(f)
			{
				break;
			}
			path++;	
		}
	}
	if(!f)
	{
		p("warn: %s not found. tried:", name);
		while(*path1)
		{
			len = strlen(*path1);
			char onepath[len + len1];
			memcpy(onepath, *path1, len);
			memcpy(onepath+len, name, len1);
			onepath[len+len1] = '\0';
			p("%s", onepath);
			path1++;	
		}
	}
	return f;
}

size_t tstrlen(char* s)
{
	register const char* i;
	for(i=s; *i && *i != ' ' && *i != '\t'; ++i);
	return (i-s);
}

void pop_redundancy()
{
	if(!croffs)
		return;
	croffs--;
	char *laststr = rblock + roffs[croffs];
	int len = strlen(laststr);
	crblock -= len;
}

int check_redundancy(char *name)
{
	for(int i = 0; i < croffs; i++)
	{
		if(strcmp(rblock + roffs[i], name) == 0)
		{
			//p("%s", name);
			return 1;
		}
	}
	return 0;
}

void build_redundancy(char *name)
{
	int len = tstrlen(name)+1;
	char* block; 
	while(crblock+len >= rblocksize)
	{
		rblocksize *= 2;
		rblock = realloc(rblock, rblocksize);
		assert(rblock);
	}
	while(croffs*sizeof(int) >= roffssize)
	{
		roffssize *= 2;
		roffs = realloc(roffs, roffssize);
		assert(roffs);
	}
	assert(croffs != -1);
	block = rblock + crblock;
	memcpy(block, name, len-1);
	block[len] = '\0';
	roffs[croffs] = crblock;
	crblock += len;
	croffs++;
}

//check for anything other than space or tab 
//to the left and right starting from "from"
int check_indent(char* from, char* begin, char* end)
{
	for(char* i = from+1; i < end; i++)
	{
		if(*i != ' ' && *i != '\t')
			return 1;
	}
	for(char* i = from; i > begin; i--)
	{
		if(*i != ' ' && *i != '\t')
			return 1;
	}
	return 0;
}

//return the last continuous occurance of ' ' or '\t'
char *get_lastindent(char* str)
{
	assert(*str == ' ' || *str == '\t');
	while(*str)
	{
		if(*str == ' ' || *str == '\t')
		{
			str++;
		} else {
			break;
		}
	}
	return str;
}

void include_recurse(FILE *infile, FILE *outfile, char **paths)
{
	char *inbuf;
	char *tok; 
	FILE *nextfp;
	size_t size;

	char *quote1;
	char *quote2;
	char *less;
	char *more;
	char *hash;
	char *include;
	char *ifndef;
	char *ifdef;
	char *_if;
	char *define;
	char *endif;
	char *_else;
	char *elif;
	char *error;
	char *undef;
	char *saveptr;
	char *content;
	unsigned int skipblock = 0;

	int ret = readall(infile, &inbuf, &size);
	fclose(infile);
	if(ret != 0)
	{
		p("read failure with %d", ret);
		return;
	}
	tok = strtok_r(inbuf, "\n\r", &saveptr);

	while(tok)
	{
		hash = strstr(tok, "#"); 
		if(hash)
		{
			ifdef = strstr(tok, "ifdef");
			if(ifdef)
			{
				if(check_indent(hash, tok, ifdef))
					goto next_tok;
				content = get_lastindent(ifdef+5);		
				if(!check_redundancy(content))
				{
					skipblock++;
				}
				goto next_tok;	
			}
			ifndef = strstr(tok, "ifndef");
			if(ifndef)
			{
				if(check_indent(hash, tok, ifndef))
					goto next_tok;
				content = get_lastindent(ifndef+6);		
				if(check_redundancy(content) || skipblock)
				{
					skipblock++;
				}
				goto next_tok;	
			}
			elif = strstr(tok, "elif");
			if(elif)
			{
				goto next_tok;	
			}
			_else = strstr(tok, "else");
			if(_else)
			{
				goto next_tok;	
			}
			error = strstr(tok, "error");
			if(error)
			{
				goto next_tok;	
			}
			undef = strstr(tok, "undef");
			if(undef)
			{
				goto next_tok;	
			}
			_if = strstr(tok, "if");
			if(_if && 
					((*(_if-1) == ' ' || 
					  *(_if-1) == '\t'||
					  *(_if-1) == '#') && 
					  *(_if+2) == ' ' || 
					  *(_if+2) == '\t'))
			{
				if(check_indent(hash, tok, _if))
					goto next_tok;
				if(skipblock)
				{
					skipblock++;
				}
				goto next_tok;	
			}
			//p("tok: %s %d", tok, skipblock);
			if(skipblock)
			{
				endif = strstr(tok, "endif");
				if(endif)
				{
					if(check_indent(hash, tok, endif))
						goto next_tok;
					skipblock--;
					goto skip_tok;	
				}
				goto next_tok;	
			}
			define = strstr(tok, "define");
			if(define)
			{
				if(check_indent(hash, tok, define))
					goto next_tok;
				content = get_lastindent(define+6);		
				build_redundancy(content);
				goto next_tok;	
			}
			include = strstr(tok, "include");
			if(include)
			{
				if(check_indent(hash, tok, include))
					goto next_tok;
				quote1 = findch(include+7, '"');		
				if(!quote1)
				{
					if(syspath)
					{
						less = efindch(include+7, '<');
						more = efindch(less, '>');
						pa(outfile, "// %s", tok);
						*more = '\0';
						nextfp = fopenpath(&less[1], paths);
						*more = '>';
						if(!nextfp)
							goto next_tok;
						include_recurse(nextfp, outfile, paths);
						goto skip_tok;
					}
					goto next_tok;
				}
				quote2 = efindch(&quote1[1], '"');		
				pa(outfile, "// %s", tok);
				*quote2= '\0';
				nextfp = fopenpath(&quote1[1], paths);
				*quote2= '"';
				if(!nextfp)
					goto next_tok;
				include_recurse(nextfp, outfile, paths);
				goto skip_tok;
			}
		} 
next_tok:;
	 if(skipblock)
		 goto skip_tok;
	 pa(outfile, "%s", tok);
skip_tok:;
	 tok = strtok_r(NULL, "\n\r", &saveptr);
	}
	free(inbuf);
	assert(skipblock == 0);
}

void help()
{
	printf(
		"Usage: amal [OPTION]... [FILE]...\n"
		"Create amalgimation from C source code\n"
		"\n"
		"  -o <filename>  output file\n"
		"  -i <filename>  input filename\n"
		"  -I <paths>     add include search path\n"
		"  -s <>          use system includes <>\n"
		"  -h             display this help message\n"
	      );
	exit(0);
}

int main(int argc, char** argv)
{
	char **arg = argv + 1;
	char **arg_end = argv + argc;
	char **argin;

	/* defaults */
	char *soutfile = NULL;
	char *sinfile = NULL;
	FILE *outfile = NULL;
	FILE *infile = NULL;
	char *paths[MAXPATH] = {0};

	/* handle options */
	while (arg != arg_end && (*arg)[0] == '-') {
		switch ((*arg)[1]) {
			case 'h':
				help();
				break;
			case 'o':
				arg++;
				if(arg == arg_end)
					p("expected filename after option -o");
				soutfile = *arg;
				break;
			case 'i':
				arg++;
				if(arg == arg_end)
					p("expected filename after option -i");
				sinfile = *arg;
				break;
			case 's':
				syspath = 1;
				break;
			case 'I':
				arg++;
				argin = arg;
				while (arg != arg_end && (*arg)[0] != '-') {
					if((arg - argin) > MAXPATH-1)
					{
						p("warn: MAXPATH exceeded");
						break;
					}
					paths[arg - argin] = *arg;
					arg++;
				}
				arg--;
				break;
			default:
				p("invalid option %s", *arg);
				break;
		}
		arg++;
	}
	if(soutfile)
	{
		outfile = fopen(soutfile, "w+");		
		assert(outfile);
	} 
	if(!sinfile)
	{
		help();
	}

	rblocksize = 100 * sizeof(char);
	rblock = malloc(rblocksize);
	roffssize = 10 * sizeof(int);
	roffs = malloc(roffssize);
	//build_redundancy("_TCC_H");

	infile = fopen(sinfile, "rb");		
	include_recurse(infile, outfile, paths);
	for(int i = 0; i < croffs; i++)
	{
//		p("b %s", rblock+roffs[i]);
	}
	return 0;
}
