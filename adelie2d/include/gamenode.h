#ifndef GAME_NODE_H
#define GAME_NODE_H

#include <allegro5/allegro.h>
#include <vector>
#include <memory>
#include <iostream>

#include "math/rect.h"
#include "math/vector2.h"
#include "camera.h"

using namespace std;

using namespace std;

class Ad_GameNode;

typedef shared_ptr<Ad_GameNode> Ad_GameNode_p;
typedef vector<Ad_GameNode_p> Ad_NodeArray;

class Ad_GameNode {
public:
	Ad_GameNode(const Ad_Rect &bounds);
	Ad_GameNode(const char * filename, const Ad_Vector2 &pos);
	~Ad_GameNode();

	/**
	 * \fn void load_bitmap(const char * filename)
	 * \brief Loads the bitmap for this node.
	 * This function will not resize the node.
	 * \param filename Name of the file to load as this nodes bitmap.
	 */
	void load_bitmap(const char * filename);

	/**
	 * \fn void set_bitmap(ALLEGRO_BITMAP * bitmap)
	 * \brief Sets the bitmap for this node.
	 * This function will free the current bitmap if necessary.
	 * \param bitmap The bitmap to use for this node.
	 */
	void set_bitmap(ALLEGRO_BITMAP * bitmap);

	/**
	 * \fn void remove_from_parent(Ad_GameNode * child)
	 * \brief Removes the child from it's parent.
	 * This function will fail if this node is the root
	 * of the scene and has no parent.
	 */
	void remove_from_parent();

	/**
	 * \fn void add_child(Ad_GameNode_p child)
	 * \brief Adds a child node to this node.
	 * \param child The child node to add to the input parent.
	 * \param parent The parent we will add the child node to.
	 */
	void add_child(Ad_GameNode_p child);

	/**
	 * \fn void render_to_camera(Ad_Camera_p camera, double globalx, double globaly)
	 * \brief Draws the input node and all of its children to the display.
	 * \param camera Camera to render this node to.
	 * \param globalx Global x position of this nodes parent.
	 * \param globaly Global y position of this nodes parent.
	 */
	void render_to_camera(Ad_Camera_p camera, double globalx, double globaly);

	/**
	 * \fn void move_node(Ad_Vector2 vel)
	 * \brief Moves this node by the given velocity.
	 * This method will move the node by velocity relative
	 * to the time passed since the last frame.
	 * \param vel The velocity at which to move this node.
	 */
	void move_node(Ad_Vector2 vel);

	/**
	 * \fn inline void set_pos(Ad_Vector2 pos)
	 * \brief sets the nodes position
	 * \param pos Position to set the node to
	 */
	inline void set_pos(Ad_Vector2 pos) {
		if (!parent) {
			cerr << "error - cannot move the root node" << endl;
			return;
		}

		bounds = Ad_Rect(pos.x, pos.y, bounds.get_size().x, bounds.get_size().y);
	}

	/**
	 * \fn inline Ad_Rect get_bounds() const
	 * \brief Returns the bounds of this node.
	 * \return The bounds of this node.
	 */
	inline Ad_Rect get_bounds() const {
		return bounds;
	}

private:
	Ad_NodeArray children;
	Ad_GameNode * parent;
	ALLEGRO_BITMAP * bitmap;

	Ad_Rect bounds;
};

#endif
