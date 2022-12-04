#ifndef HTREE_H
#define HTREE_H
#include "point.h"
#include <QFile>
#include "element.h"
#include <QDataStream>
#include <math.h>
#include <QDir>
#define MAXOFLINE 1000
class Htree
{
    Point* tree;
public:
    Htree();
    Point* gettree();
    bool create(QFile* f,QString &codpath);
    bool select(Point* tree,int upon,int &index1owest,int &indexlower);
    bool decode(QFile* f,QString &decpath);
    void str_makeup(char* src,char* &newcode,int newlen);
    int str_length(char* word);
    int str_search(char* src,char* sub);
    void str_cut(char* &src,int wordsize,int cutlen);
};

#endif // HTREE_H
