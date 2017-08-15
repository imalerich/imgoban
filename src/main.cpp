#include <stdio.h>
#include <stdlib.h>
#include <adelie2d/adelie.h>

#include "constants.h"
#include "goban.h"

#define PI 3.14159265f
#define SHIT 30

unsigned SCREEN_W = 800;
unsigned SCREEN_H = 600;
Ad_Scene_p scene;
Goban_p g;

bool update();
bool render();
Ad_GameNode_p createBoardNode(Ad_Scene_p scene);

int main(int argc, const char ** argv) {
    if (!ad_init(SCREEN_W, SCREEN_H)) {
	return 1;
    }

    scene = make_shared<Ad_Scene>(SCREEN_W, SCREEN_H);
    g = make_shared<Goban>(scene, 19);

    ad_set_frame_rate(SHIT);
    return ad_run(update, render);
}

bool update() { 
    return true; 
}

bool render() {
    scene->render(al_map_rgb(32, 32, 32));
    g->renderLabels();
    return true;
}
