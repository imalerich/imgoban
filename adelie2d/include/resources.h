#ifndef RESOURCES_H
#define RESOURCES_H

#include <unordered_map>
#include <string>
#include <allegro5/allegro.h>

using namespace std;

class Ad_Resources {
public:
	/**
	 * \fn Ad_Resources & shared()
	 * \brief Returns the singleton pointer to this class.
	 * \return Shared pointer to this class.
	 */
	static Ad_Resources & shared() {
		static Ad_Resources s;
		return s;
	}

	/**
	 * \fn void delete_resources()
	 * \brief Free's all allegro assets registered to Ad_Resources.
	 */
	void delete_resources();

	/**
	 * \fn ALLEGRO_BITMAP register_vitmap(string key, ALLEGRO_BITMAP * bitmap)
	 * \brief registers the input bitmap to the given key
	 * Any request to load a bitmap of the given key will return 
	 * the input bitmap instead once it is registered.
	 * This bitmap is guaranteed to then be released on destruction of
	 * Ad_Resources.
	 * \param key The key to associate the bitmap with.
	 * \param bitmap The bitmap to be registered.
	 * \return The input 'bitmap' parameter.
	 */
	ALLEGRO_BITMAP * register_bitmap(string key, ALLEGRO_BITMAP * bitmap);

	/**
	 * \fn ALLEGRO_BITMAP * get_bitmap(string key)
	 * \brief This method loads a bitmap with the given filename, then registers it.
	 * Registration of the bitmap is done with the register_bitmap(...) function.
	 * The loaded bitmap is then returned from this function.
	 * If the given filename is already registered, the associated
	 * bitmap will be returned instead.
	 * \param key The key to associate the bitmap with, or the filename if it doesn't exist.
	 * \return The bitmap associated with the given key.
	 */
	ALLEGRO_BITMAP * get_bitmap(string key);

	/**
	 * \fn bool does_bitmap_exist(string key);
	 * \brief Determines whether or not a bitmap is available with the given key.
	 * \param key Key to check for a bitmap with.
	 * \return Whether or not the input key is associated with a bitmap.
	 */
	bool does_bitmap_exist(string key);

private:
	Ad_Resources() { };

	unordered_map<string, ALLEGRO_BITMAP*> allegro_bitmaps;

	// do not allow copying of the singleton
	Ad_Resources(Ad_Resources const&) = delete;
	void operator=(Ad_Resources const&) = delete;
};

#endif
