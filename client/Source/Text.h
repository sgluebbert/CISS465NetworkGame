#ifndef TEXT_H
#define TEXT_H

#include "Draw.h"

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
	void Draw(float x, float y);

	GLuint texture;
	TTF_Font *font;
	Color color;

	unsigned short width, height;
};

void DrawText(float x, float y, const char *c, TTF_Font *font, const Color *color);

#endif