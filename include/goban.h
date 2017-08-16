#ifndef GOBAN_H
#define GOBAN_H

#include <memory>
#include <adelie2d/adelie.h>

using namespace std;

class Goban;
typedef shared_ptr<Goban> Goban_p;

class Goban {
public:
    Goban(Ad_Scene_p Scene, unsigned BoardSize);

    void renderLabels();
    unsigned getGridSize();
    unsigned getBoardSize();

    Ad_GameNode_p node;
    Ad_GameNode_p grid;

private:
    ALLEGRO_FONT * font;

    const Ad_Scene_p scene;
    const unsigned size;

    void addToScene();
    void addGridToBoard();
    void addStarsToBoard();
};

#endif
