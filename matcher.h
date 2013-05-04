/*
 * matcher.h
 *
 *  Created on: Apr 15, 2013
 *  Author: Sebastian Mäki
 */

#ifndef MATCHER_H_
#define MATCHER_H_

typedef struct {
	regex_t redirect;
	regex_t url;
}regex_matcher;

typedef struct {
	char *pattern;
	char *parameter;
	regex_matcher regex;
	unsigned int valid;
}matcher;

#endif /* MATCHER_H_ */
