#include <stdio.h>
#include <stdlib.h>
#include <adelie2d/adelie.h>

#include "player/roboplayer.h"
#include "player/userplayer.h"
#include "constants.h"
#include "game.h"

#define SHIT 30

unsigned SCREEN_W = 1366;
unsigned SCREEN_H = 768;
Ad_Scene_p scene;
Game_p g;

bool update();
bool render();
Ad_GameNode_p createBoardNode(Ad_Scene_p scene);

int main(int argc, const char ** argv) {
    if (!ad_init(SCREEN_W, SCREEN_H)) {
	return 1;
    }

    scene = make_shared<Ad_Scene>(SCREEN_W, SCREEN_H);
    g = make_shared<Game>(scene, 9, 6.5f);

	// attach to bots to play against each other
	g->attach_player(make_shared<RoboPlayer>(g->size, g->komi), SLATE);
	// g->attach_player(make_shared<RoboPlayer>(g->size, g->komi), SHELL);
	// g->attach_player(make_shared<UserPlayer>(g->board, g->size), SLATE);
	g->attach_player(make_shared<UserPlayer>(g->board, g->size), SHELL);

    ad_set_frame_rate(SHIT);
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
