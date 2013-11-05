#ifndef QUAD_TREE_H
#define QUAD_TREE_H



#include <deque>
#include <iostream>

#include "Entity.h"
#include "Rect.h"
#include "System.h"



class Quad_Tree {
public:
    Quad_Tree();
    Quad_Tree(int, Rect<double>);
    ~Quad_Tree();

    bool Is_Within(Entity *);
    void Insert(Entity *);
    void Get_Possibles(std::deque<Entity *> &, Rect<double> &);

    void Subdivide();
    void Clear();
    
    void Update(double);
    void Draw();

    friend std::ostream & operator<<(std::ostream &, const Quad_Tree &);

private:
    static int MAX_DEPTH;
    static int MAX_SIZE;
    static Rect<double> MAX_BOUNDS;

    Rect<double> bounds;
    int depth;
    
    Quad_Tree * northeast;
    Quad_Tree * northwest;
    Quad_Tree * southeast;
    Quad_Tree * southwest;

    std::deque<Entity *> items;
};



#endif
