#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL/SDL_opengl.h"
#include <iostream>

const SDL_Rect DEFAULT_WINDOW_BOUNDING_BOX = {0, 0, 800, 600};
extern SDL_Rect WINDOW_BOUNDING_BOX;

struct Texture
{
	Texture()
		: id(0)
	{}
	Texture(const char *);
	Texture(SDL_Surface *surface, GLuint texture = 0);
	~Texture();

	GLuint GetTexture() const { return id; }

	void DrawAtRect(float x, float y, int w = 0, int h = 0);

	void DrawCentered(float x, float y);
	void DrawCentered(float x, float y, float angle);
	void DrawCentered(float x, float y, float angle, float scale);

	GLuint id;
private:
	GLuint LoadTexture(const char *);
	GLuint LoadTexture(SDL_Surface *surface, GLuint texture);
};


static SDL_Surface *window_surface;
void InitWindow();

#endif