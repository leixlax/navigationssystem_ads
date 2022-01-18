#pragma once
#ifndef DIJKSTRA_H
#define DIJKSTRA_H
//define city names for the dijkstra algorithm so you can write the names into the dijkstra2() and dijkstra3() function
#define KARLSRUHE 0 
#define STUTTGART 1
#define MANNHEIM  2
#define PFORZHEIM 3
#define RASTATT   4
#define BADENBADEN 5
#define FREIBURG 6
#define OFFENBURG 7
#define KEHL 8
#define FREUDENSTADT 9
#define REUTLINGEN 10 
#define TUEBINGEN 11
#define ULM 12
#define LOERRACH 13
#define KONSTANZ 14

#define ANZAHL 15
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

void dijkstra_input(int, int);

#endif