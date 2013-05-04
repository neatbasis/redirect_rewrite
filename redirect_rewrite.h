/*
 * redirect_rewrite.h
 *
 *  Created on: May 4, 2013
 *  Author: Sebastian Mäki
 */

#ifndef REDIRECT_REWRITE_H_
#define REDIRECT_REWRITE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <syslog.h>
#include "matcher.h"


int main(int argc, char **argv);
void compile_patterns();
char* match(char* url);
char* getParam(char*, regex_t param);

#endif /* REDIRECT_REWRITE_H_ */
