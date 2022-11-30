#include "point.h"

Point::Point()
{
    lchild = nullptr;
    rchild = nullptr;
    parent = nullptr;
    data = '\0';
    weight = 0;
}

char Point::getdata(){
    return data;
}

bool Point::setdata(char ch){
    if((this->data = ch)) return true;
    return false;
}


Point* Point::getlchild(){
    return lchild;
}

bool Point::setlchild(Point *l){
    if((this->lchild = l))  return true;
    return false;
}

Point* Point::getrchild(){
    return rchild;
}

bool Point::setrchild(Point *r){
    if((this->rchild = r))  return true;
    return false;
}

Point* Point::getparent(){
    return parent;
}

bool Point::setparent(Point *prt){
    if((this->parent = prt))    return true;
    return false;
}

int Point::getweight(){
    return weight;
}

bool Point::setweight(int w){
    if((this->weight = w))  return true;
    return false;
}



