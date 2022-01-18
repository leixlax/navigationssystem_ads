#pragma once
#ifndef K7SCAN_H
#define K7SCAN_H

#include <stdlib.h>
#include <iostream>
#pragma warning(disable:4786)
#include <string>
#include <map>
#include <deque>

using namespace std;

#define	Getc(s)			getc(s)
#define	Ungetc(c)		{ungetc(c,IP_Input); ugetflag=1;}

int comp_string_location(string);
int read_txt_file();

#endif