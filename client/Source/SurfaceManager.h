#ifndef SURFACE_MANAGER_H
#define SURFACE_MANAGER_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL/SDL_opengl.h"
#include <iostream>

static const SDL_Rect DEFAULT_WINDOW_BOUNDING_BOX = {0, 0, 800, 600};
static SDL_Rect WINDOW_BOUNDING_BOX;

struct Color
{
	Color(float r_, float g_, float b_, float a_ = 1)
		: r(r_), g(g_), b(b_), a(a_)
		{
			if (r > 1)	r = 1;
			if (g > 1)	g = 1;
			if (b > 1)	b = 1;
			if (a > 1)	a = 1;
			if (r < 0)	r = 0;
			if (g < 0)	g = 0;
			if (b < 0)	b = 0;
			if (a < 0)	a = 0;
		}
	Color()
		: r(1), g(1), b(1), a(1)
		{}
	Color(const Color &c)
		: r(c.r), g(c.g), b(c.b), a(c.a)
		{}

	void set(float r_, float g_, float b_, float a_ = 1)
	{
		r = r_; g = g_; b = b_; a = a_;
	}

	Color operator- (float n)
	{
		return Color(r - n, g - n, b - n);
	}

	Color operator+ (float n)
	{
		return Color(r + n, g + n, b + n);
	}

	Color operator+= (float n)
	{
		return Color(r + n, g + n, b + n);
	}

	float r, g, b, a;
};

const Color RED(1, 0, 0, 1);
const Color GREEN(0, 1, 0, 1);
const Color WHITE(1, 1, 1, 1);
const Color BLACK(0, 0, 0, 1);
const Color GRAY(.5, .5, .5, 1);

struct Fonts
{
	Fonts();
	~Fonts();
	void init();

	TTF_Font *font_FreeMono_10;
	TTF_Font *font_FreeMono_12;
	TTF_Font *font_FreeMono_14;
	TTF_Font *font_FreeMono_16;
	TTF_Font *font_FreeMono_18;
	TTF_Font *font_FreeMono_20;

	TTF_Font *font_Impact_12;
	TTF_Font *font_Impact_14;
	TTF_Font *font_Impact_16;
	TTF_Font *font_Impact_18;
	TTF_Font *font_Impact_20;
};

class SurfaceManager {
public:
	static SurfaceManager * GetInstance();

	GLuint test;
	GLuint bullet;
	GLuint smoke;
	GLuint ship;
	GLuint background_main_menu;
	GLuint background_game;
	GLuint button;
	GLuint highlightedbutton;
	
	void Load();
	bool Resize(int, int);

	static void Clear();
	static void Flip();

	static GLuint LoadTexture(const char *);
	static GLuint LoadTexture(SDL_Surface *surface, GLuint texture);
	static GLuint LoadText(const char *c, TTF_Font *font, Color color_, unsigned short &width, unsigned short &height, GLuint texture = 0);

	static void DrawImageRect(GLuint, float x, float y, int w = 0, int h = 0);
	static void DrawImage(GLuint, float x, float y);
	static void DrawImage(GLuint, float x, float y, float angle);
	static void DrawImage(GLuint, float x, float y, float angle, float scale);

	static void DrawLine(float x1, float y1, float x2, float y2, const Color *color = NULL);
	static void DrawRect(float x1, float y1, float x2, float y2, bool fill, const Color *color = NULL);

	static void DrawText(float x, float y, const char *c, TTF_Font *font, const Color *color = NULL);

	Fonts fonts;

private:
	SurfaceManager();
	~SurfaceManager();

	static SurfaceManager * instance;
	SDL_Surface * surface;
};

struct Text
{
	Text(){texture = 0;}
	Text& operator=( Text rhs )
	{
		texture = rhs.texture;
		rhs.texture = 0; font = rhs.font; color = rhs.color;
		return *this;
	}
	Text(const char *c, TTF_Font *font, Color color);
	~Text();

	void Reload(const char *c);

	GLuint texture;
	TTF_Font *font;
	Color color;

	unsigned short width, height;
};

extern SurfaceManager * surface_manager;

#endif
