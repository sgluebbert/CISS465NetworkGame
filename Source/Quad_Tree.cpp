#include "Quad_Tree.h"



int Quad_Tree::MAX_DEPTH = 4;
int Quad_Tree::MAX_SIZE = 4;
Rect<double> Quad_Tree::MAX_BOUNDS = Rect<double>(0, 0, 2000, 2000);
Quad_Tree Quad_Tree::object_tree;




Quad_Tree::Quad_Tree() {
    bounds = MAX_BOUNDS;
    depth = 0;
    
    root = NULL;
    northeast = NULL;
    northwest = NULL;
    southeast = NULL;
    southwest = NULL;
}

Quad_Tree::Quad_Tree(Quad_Tree * parent, int ndepth, Rect<double> nbounds) {
    bounds = nbounds;
    depth = ndepth;
    
    root = parent;
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

bool Quad_Tree::Is_Within(Collidable * item) {
    Rect<double> temp;
    temp.x = item->bounding_volume.x;
    temp.y = item->bounding_volume.y;
    temp.w = item->bounding_volume.r;
    temp.h = item->bounding_volume.r;

    return bounds.Within(temp);
}

void Quad_Tree::Reinsert(Collidable * item) {
    if (root == NULL)
        Insert(item);
    
    root->Reinsert(item);
}

void Quad_Tree::Insert(Collidable * item) {
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

        std::deque<Collidable *> temp = items;
        items.clear();

        for (int i = temp.size() - 1; i >= 0; i--) {
            Insert(temp.back());
            temp.pop_back();
        }

        temp.clear();
    }
}

void Quad_Tree::Get_Possibles(std::deque<Collidable *> & possibles, Rect<double> & item) {
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
    northeast = new Quad_Tree(this, depth + 1, temp);
    
    temp.x = 0;
    temp.y = 0;
    northwest = new Quad_Tree(this, depth + 1, temp);
    
    temp.x = temp.w;
    temp.y = temp.h;
    southeast = new Quad_Tree(this, depth + 1, temp);
    
    temp.x = 0;
    temp.y = temp.h;
    southwest = new Quad_Tree(this, depth + 1, temp);
}

void Quad_Tree::Clear() {
    delete northeast;
    delete northwest;
    delete southeast;
    delete southwest;
    items.clear();
}



void Quad_Tree::Update() {
    for (int i = 0; i < items.size(); i++)
        if (!Is_Within(items[i])) {
            Collidable * temp = items[i];
            items.erase(items.begin() + i);
            Reinsert(temp);
        }

    northeast->Update();
    northwest->Update();
    southeast->Update();
    southwest->Update();
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