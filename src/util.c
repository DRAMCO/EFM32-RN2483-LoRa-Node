/*
 * util.c
 *
 *  Created on: 19-jan.-2018
 *      Author: Guus Leenders
 */

#include <stdint.h>
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
