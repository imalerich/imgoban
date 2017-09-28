#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "config.h"

Ad_Scene_p scene;
Game_p g;

bool update();
bool render();
Ad_GameNode_p createBoardNode(Ad_Scene_p scene);

int main(int argc, const char ** argv) {
	Config c("../sample.cfg");
    scene = c.create_scene();
	if (scene == nullptr) { exit(1); }
	g = c.create_game(scene);

    return ad_run(update, render);
}

bool update() { 
    g->update();
    return true; 
}

bool render() {
    scene->render(al_map_rgb(32, 32, 32));
    g->render();
    return true;
}
