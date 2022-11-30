#ifndef POINT_H
#define POINT_H


class Point
{
    Point* lchild;
    Point* rchild;
    Point* parent;
    char data;
    int weight;
public:
    Point();
    int getweight();
    char getdata();
    Point* getlchild();
    Point* getrchild();
    Point* getparent();
    bool setdata(char ch);
    bool setlchild(Point* l);
    bool setrchild(Point* r);
    bool setparent(Point* prt);
    bool setweight(int w);
};

#endif // POINT_H
