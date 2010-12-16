#ifndef _DIRECTIONS_H_
#define _DIRECTIONS_H_

#include "Point.h"

#include <vector>
using namespace std;

// Donats una normal i un nombre de direccions, retorna un vector amb direccions sobre l'hemisferi
// definit per la normal. Per tant, tots els vectors del resultat formen amb la normal un angle no superior a PI/2 
void randomDirections(Vector normal, int numberOfDirections, vector<Vector>& result);

// donat un enter entre 0 i 319, retorna una direccio sobre l'esfera de Gauss (com a vectors unitari)
Vector directions320(int i); 

#endif