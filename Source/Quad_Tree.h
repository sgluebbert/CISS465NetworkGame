#ifndef QUAD_TREE_H
#define QUAD_TREE_H



#include "Collidable.h"
#include "Rect.h"



class Quad_Tree {
public:
    Quad_Tree();
    Quad_Tree(Quad_Tree *, int, Rect<double>);
    ~Quad_Tree();

    bool Is_Within(Collidable *);
    void Insert(Collidable *);
    void Reinsert(Collidable *);
    void Get_Possibles(std::deque<Collidable *> &, Rect<double> &);

    void Subdivide();
    void Clear();
    
    void Update();
    void Draw();

    static Quad_Tree object_tree;

private:
    static int MAX_DEPTH;
    static int MAX_SIZE;
    static Rect<double> MAX_BOUNDS;

    Rect<double> bounds;
    int depth;
    
    Quad_Tree * root;
    Quad_Tree * northeast;
    Quad_Tree * northwest;
    Quad_Tree * southeast;
    Quad_Tree * southwest;

    std::deque<Collidable *> items;
};



#endif
