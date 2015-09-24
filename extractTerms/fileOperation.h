//
// Created by yueg on 8/24/15.
//

#ifndef FINDWORD_FILEOPERATION_H
#define FINDWORD_FILEOPERATION_H
#include <string>
using namespace std;
bool writeRes2File(const char *filename, string str);
char * readInputFromFile(const char *filename);
#endif //FINDWORD_FILEOPERATION_H
