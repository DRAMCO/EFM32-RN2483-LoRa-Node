/*
 * util.c
 *
 *  Created on: 19-jan.-2018
 *      Author: Guus Leenders
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool StringStartsWith(char * str, char * seq){
	uint8_t i;
	for(i=0; i<strlen(seq); i++){
		if(*(str+i) != *(seq+i)){
			return false;
		}
	}
	return true;
}
char * StringToHexString(char * bin, unsigned int binsz, char **result ){
	char     hex_str[]= "0123456789abcdef";
	unsigned int      i;

	if (!(*result = (char *)malloc(binsz * 2 + 1)))
	return (NULL);

	(*result)[binsz * 2] = 0;

	if (!binsz)
	return (NULL);

	for (i = 0; i < binsz; i++)
	{
	  (*result)[i * 2 + 0] = hex_str[(bin[i] >> 4) & 0x0F];
	  (*result)[i * 2 + 1] = hex_str[(bin[i]     ) & 0x0F];
	}
	return (*result);
}
