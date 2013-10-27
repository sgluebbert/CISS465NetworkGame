#ifndef TEXT_H
#define TEXT_H

#include "Draw.h"

struct Text
{
	Text()
		: texture(NULL)
	{}
	~Text()
	{
		if (texture != NULL)
			delete texture;
	}

	Text& operator=( Text rhs )
	{
		texture = rhs.texture;
		font = rhs.font; color = rhs.color;
		return *this;
	}
	Text(const char *c, TTF_Font *font, Color color);

	void Reload(const char *c);
	void Draw(float x, float y);

	TTF_Font *font;
	Color color;

	unsigned short width, height;

private:
	Texture *texture;
	Texture * LoadText(const char *c, TTF_Font *font, Color color_, unsigned short &width, unsigned short &height, GLuint texture = 0);
};

void DrawText(float x, float y, const char *c, TTF_Font *font, const Color *color);

#endif