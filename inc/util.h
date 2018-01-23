/*
 * util.h
 *
 *  Created on: 19-jan.-2018
 *      Author: Guus Leenders
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include <stdint.h>
#include <stdbool.h>

bool StringStartsWith(char * str, char * seq);
char * StringToHexString(char * bin, unsigned int binsz, char **result );

#endif /* INC_UTIL_H_ */
