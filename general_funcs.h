#ifndef PARTNER_H
#define PARTNER_H
#include "partner.h"
#endif

bool writeCSV(const char* filename, Partner* p_mas, int size);
int fillMas(const char* filename, Partner* p_mas);
bool searchCheck(Partner multi[], Partner linear[], Partner binary[], Partner redblack[], Partner hash[], long size);
bool sortCheck(Partner std[], Partner shake[], Partner quick[], Partner merge[], long size);