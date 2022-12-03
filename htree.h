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
    void str_makeup(char* &src);
    int str_length(char* word);
    int str_search(char* src,char* sub);
    void str_cut(char* &src,int wordsize,int cutlen);
};

#endif // HTREE_H
