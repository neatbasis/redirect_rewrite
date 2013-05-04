/*
 * redirect_rewrite.c
 *
 *  Created on: Apr 15, 2013
 *  Author: Sebastian Mäki
 */

#include "redirect_rewrite.h"
#include "urldecode.h"

/* The default buffer size is the maximum path portion size
 * that IE can handle, so we should never have to realloc */
#define BUF_SIZE 2048
/* In case we do have to realloc, do it in increments of 512 bytes*/
#define INCR_SIZE 512

//#define DEBUG

unsigned int allocatedSize  = BUF_SIZE;
char *content;

matcher matchers[] = {
		{
			"//.*.google.[^/]+/url", /* The regexp pattern to find a match */
			"url", /* The query parameter which holds the destination url */
			{{NULL},{NULL}},
			0
		},
		{
			"//.*.facebook.com/l.php",
			"u",
			{{NULL},{NULL}},
			0
		},
/* Add your own matchers in here */

};

#define NUM_MATCHERS sizeof matchers / sizeof matchers[0]

int main(void) {
	compile_patterns();
	/* Allocate some memory */
	content = (char*) malloc(sizeof(char) * BUF_SIZE);
	memset (content,0,sizeof(char) * BUF_SIZE);
	/* Check if the memory couldn't be allocated; if it's the case, handle the problem as appropriate. */
	if (NULL == content) {
#ifdef DEBUG
		perror("Could not allocate memory");
#endif
		return EXIT_FAILURE;
	}

	do{
		/* Read line into contents */
		if(readIn() == -1)
			return EXIT_FAILURE;

		/* Grab the text up to the space character */
		char* channel = strtok (content," ");
		if(channel != NULL){
			/* Grab more text up to the next space character
			 * and try to get a redirect url match */
			char* original_url = strtok (content," ");
			if(NULL == original_url){
				printf("%s \n", channel);
				continue;
			}
			char* url = match(original_url);
			if(NULL != url){
				printf("%s 302:%s\n", channel, url);
				continue;
			}
		}
		printf("%s \n", channel);
	}while(is_ready(fileno(stdin)));

	return EXIT_FAILURE;
}

unsigned int readIn(){

	int c;
	unsigned int n = 0;
	unsigned int localAllocatedSize, oldSize;
	localAllocatedSize = allocatedSize;

	memset (content,0,sizeof(char) * localAllocatedSize);
	while(1) {
		if(n==localAllocatedSize){
			oldSize = localAllocatedSize;
			localAllocatedSize += INCR_SIZE;
			content = (char*) realloc(content, sizeof(char) * localAllocatedSize);
			if (NULL == content) {
				break;
			}
			if(oldSize<localAllocatedSize)
				memset (content+oldSize,0,sizeof(char) * INCR_SIZE);
		}

		c = fgetc(stdin);

		if (c != '\n'){
			content[n] = c;
			n++;
		}else{
			allocatedSize = localAllocatedSize;
			return n;
		}
	}
	allocatedSize = localAllocatedSize;
#ifdef DEBUG
	perror("Could not allocate memory");
#endif
	return -1;
}

void compile_patterns(){

	unsigned int valid_matchers = 0;
	unsigned int x=0;
	while(x < NUM_MATCHERS){

		char pattern[strlen(matchers[x].parameter) + 15];
		sprintf(pattern, "[?,&]%s=([^&]+)", matchers[x].parameter);

		//query and redirect url matching
		matchers[x].valid = !regcomp(&matchers[x].regex.url, pattern, REG_EXTENDED)
				&& !regcomp(&matchers[x].regex.redirect, matchers[x].pattern, REG_EXTENDED);

		//count valid matchers
		if(matchers[x].valid)
			valid_matchers++;

		x++;
	}
}

char* match(char *url){
	char *b;
	unsigned int x=0;
	while(x < NUM_MATCHERS){
		if( !(regexec(&matchers[x].regex.redirect, url, 0, NULL, 0)) ){
			b = (char *)getParam(url, matchers[x].regex.url);
			return b;
		}
		x++;
	}
	return NULL;
}

char* getParam(char *url, regex_t prm){
	unsigned int maxGroups = 2;
	regmatch_t cm[maxGroups];
	char * decoded;
	decoded = NULL;

	if(!regexec(&prm, url, maxGroups, cm, 0)){
		  if (cm[1].rm_so != (size_t)-1){
			  char sourceCopy[strlen(url) + 1];
			  strcpy(sourceCopy, url);
			  sourceCopy[cm[1].rm_eo] = 0;
			  decoded = url_decode(sourceCopy + cm[1].rm_so);
		  }
	}
	return decoded;
}

unsigned int is_ready(int fd) {
    fd_set fdset;
    struct timeval timeout;
    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
    //int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
    return select(fd+1, &fdset, NULL, NULL, &timeout) == 1 ? 1 : 0;
}

