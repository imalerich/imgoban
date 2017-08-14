#ifndef CAMERA_H
#define CAMERA_H

#include <allegro5/allegro.h>
#include <memory>
using namespace std;

class Ad_Camera;

typedef shared_ptr<Ad_Camera> Ad_Camera_p;

class Ad_Camera {
public:
	Ad_Camera();

	/**
	 * \fn set_as_render_target()
	 * \brief Tells allegro to render graphics to this camera.
	 */
	void set_as_render_target();

	/**
	 * \fn void present_to_screen()
	 * \brief Presents the camera to the display.
	 */
	void present_to_display();

	double xpos;
	double ypos;
	double zoom;
	double rot;

private:
	ALLEGRO_BITMAP * display_buffer;
};

#endif
