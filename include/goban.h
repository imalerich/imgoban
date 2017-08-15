#ifndef GOBAN_H
#define GOBAN_H

#include <adelie2d/adelie.h>

class Goban;

typedef shared_ptr<Goban> Goban_p;

class Goban {
public:
    Goban(Ad_Scene_p Scene, unsigned BoardSize);

    void renderLabels();
    unsigned getGridSize();
    unsigned getBoardSize();

private:
    ALLEGRO_FONT * font;

    const Ad_Scene_p scene;
    Ad_GameNode_p node;
    Ad_GameNode_p grid;
    const unsigned size;

    void addToScene();
    void addGridToBoard();
    void addStarsToBoard();
    void randInit();
};

#endif
