#include "Quad_Tree.h"



int Quad_Tree::MAX_DEPTH = 4;
int Quad_Tree::MAX_SIZE = 4;
Rect<double> Quad_Tree::MAX_BOUNDS = Rect<double>(0, 0, 800, 600);




Quad_Tree::Quad_Tree() {
    bounds = MAX_BOUNDS;
    depth = 0;
    
    northeast = NULL;
    northwest = NULL;
    southeast = NULL;
    southwest = NULL;
}

Quad_Tree::Quad_Tree(int ndepth, Rect<double> nbounds) {
    bounds = nbounds;
    depth = ndepth;
    
    northeast = NULL;
    northwest = NULL;
    southeast = NULL;
    southwest = NULL;
}

Quad_Tree::~Quad_Tree() {
    delete northeast;
    delete northwest;
    delete southeast;
    delete southwest;
    items.clear();
}

bool Quad_Tree::Is_Within(Entity * item) {
    Rect<double> temp;
    temp.x = item->x;
    temp.y = item->y;
    temp.w = item->w;
    temp.h = item->h;

    return bounds.Within(temp);
}

void Quad_Tree::Insert(Entity * item) {
    if (northeast == NULL)
        items.push_back(item);
    else if (northeast->Is_Within(item))
        northeast->Insert(item);
    else if (northwest->Is_Within(item))
        northwest->Insert(item);
    else if (southeast->Is_Within(item))
        southeast->Insert(item);
    else if (southwest->Is_Within(item))
        southwest->Insert(item);
    else
        items.push_back(item);

    if (items.size() > MAX_SIZE && northeast == NULL) {
        Subdivide();

        std::deque<Entity *> temp = items;
        items.clear();

        for (int i = temp.size() - 1; i >= 0; i--) {
            Insert(temp.back());
            temp.pop_back();
        }

        temp.clear();
    }
}

void Quad_Tree::Get_Possibles(std::deque<Entity *> & possibles, Rect<double> & item) {
    if (northeast == NULL) {
        for (int i = 0; i < items.size(); i++)
            possibles.push_back(items[i]);
    }
    else if (northeast->bounds.Within(item))
        northeast->Get_Possibles(possibles, item);
    else if (northwest->bounds.Within(item))
        northwest->Get_Possibles(possibles, item);
    else if (southeast->bounds.Within(item))
        southeast->Get_Possibles(possibles, item);
    else if (southwest->bounds.Within(item))
        southwest->Get_Possibles(possibles, item);
    else {
        for (int i = 0; i < items.size(); i++)
            possibles.push_back(items[i]);

        northeast->Get_Possibles(possibles, item);
        northwest->Get_Possibles(possibles, item);
        southeast->Get_Possibles(possibles, item);
        southwest->Get_Possibles(possibles, item);
    }
}

void Quad_Tree::Subdivide() {
    if (depth > MAX_DEPTH)
        return;

    Rect<double> temp;
    temp.w = bounds.w / 2.0;
    temp.h = bounds.h / 2.0;
    
    temp.x = temp.w;
    temp.y = 0;
    northeast = new Quad_Tree(depth + 1, temp);
    
    temp.x = 0;
    temp.y = 0;
    northwest = new Quad_Tree(depth + 1, temp);
    
    temp.x = temp.w;
    temp.y = temp.h;
    southeast = new Quad_Tree(depth + 1, temp);
    
    temp.x = 0;
    temp.y = temp.h;
    southwest = new Quad_Tree(depth + 1, temp);
}

void Quad_Tree::Clear() {
    delete northeast;
    delete northwest;
    delete southeast;
    delete southwest;
    items.clear();
}



void Quad_Tree::Update(double dt) {
}

void Quad_Tree::Draw() {
    Rect<double> temp;

    temp.x = bounds.x;
    temp.y = bounds.y;
    temp.w = bounds.w;
    temp.h = 1;
    //SDL_FillRect(WINDOW, &temp, 0x808080);

    temp.w = 1;
    temp.h = bounds.h;
    //SDL_FillRect(WINDOW, &temp, 0x808080);

    temp.x = bounds.w - 1;
    temp.h = bounds.h;
    //SDL_FillRect(WINDOW, &temp, 0x808080);

    temp.x = bounds.x;
    temp.y = bounds.h - 1;
    temp.w = bounds.w;
    temp.h = 1;
    //SDL_FillRect(WINDOW, &temp, 0x808080);
    
    if (northeast != NULL) {
        northeast->Draw();
        northwest->Draw();
        southeast->Draw();
        southwest->Draw();
    }
}



std::ostream & operator<<(std::ostream & cout, const Quad_Tree & rhs) {
    for (int t = 0; t < rhs.depth; t++)
        std::cout << '\t';
    std::cout << "Quad Tree::Depth " << rhs.depth << std::endl;

    for (int t = 0; t < rhs.depth; t++)
        std::cout << '\t';
    std::cout << "Position: [" << rhs.bounds.x << ", " << rhs.bounds.y << ']' << std::endl;

    for (int t = 0; t < rhs.depth; t++)
        std::cout << '\t';
    std::cout << "Bounds: [" << rhs.bounds.w << ", " << rhs.bounds.h << ']' << std::endl;

    for (int t = 0; t < rhs.depth; t++)
        std::cout << '\t';
    std::cout << "Items:" << std::endl;

    for (int i = 0; i < rhs.items.size(); i++) {
        for (int t = 0; t < rhs.depth; t++)
            std::cout << '\t';

        std::cout << rhs.items[i] << std::endl;
    }

    if (rhs.northeast == NULL)
        return cout;

    std::cout << *rhs.northeast << std::endl;
    std::cout << *rhs.northwest << std::endl;
    std::cout << *rhs.southeast << std::endl;
    std::cout << *rhs.southwest << std::endl;

    return cout;
}