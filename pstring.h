#ifndef _PSTRING_
#define _PSTRING_

#include "global.h"
#include "pfont.h"

#include <stdio.h>
#include <string>

char *copy_string(char *destination,const char *source);
Letter GetLetter(char ch);
int GetCharWidth(char ch);
int GetStringWidth(std::string str, int kerning = DEFAULT_KERNING);

#endif