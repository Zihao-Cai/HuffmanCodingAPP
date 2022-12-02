#ifndef HTREE_H
#define HTREE_H
#include "point.h"
#include <QFile>
#include "element.h"
#include <QDataStream>
#include <math.h>
#define MAXLINE 100
class Htree
{
    Point* tree;
public:
    Htree();
    Point* gettree();
    bool create(QFile* f);
    bool select(Point* tree,int upon,int &index1owest,int &indexlower);
    bool decode(QFile* f);
    char* str_makeup(char* &src);
};

#endif // HTREE_H
