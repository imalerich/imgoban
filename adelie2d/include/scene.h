#ifndef SCENE_H
#define SCENE_H

#include <memory>
using namespace std;

#include "camera.h"
#include "gamenode.h"
#include "math/rect.h"
#include "colors.h"

class Ad_Scene;

typedef shared_ptr<Ad_Scene> Ad_Scene_p;

class Ad_Scene {
public:
	Ad_Scene(unsigned width, unsigned height);
	~Ad_Scene();

	/**
	 * \fn void ad_render(ALLEGRO_COLOR clear_color) const
	 * \brief renders the screen to the primary display
	 * param clear_color initial color to clear the screen to.
	 */
	void render(ALLEGRO_COLOR clear_color = BLACK) const;

	/**
	 * \fn inline Ad_GameNode_p * get_root() const
	 * \brief Returns the root node of this scene.
	 * \return The root node for the scene.
	 */
	inline Ad_GameNode_p get_root() const {
		return root;
	}

	/**
	 * \fn inline Ad_Camera_p * get_camera() const
	 * \brief Returns the camera for this scene.
	 * \return The camera for this scene.
	 */
	inline Ad_Camera_p get_camera() const {
		return camera;
	}

	/**
	 * \fn inline Ad_Rect get_bounds() const
	 * \brief Returns the bounds of this scene.
	 * This method is equivalent to self.get_root()->get_bounds().
	 * \return The bounds of this scenes root node.
	 */
	inline Ad_Rect get_bounds() const {
		return root->get_bounds();
	}

private:
	Ad_Camera_p camera;
	Ad_GameNode_p root;
};

#endif
