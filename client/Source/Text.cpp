#include "Text.h"

Text::Text(const char *c, TTF_Font *font_, Color color_)
{
	font = font_;
	color = color_;
	texture = LoadText(c, font, color, width, height);
}

Text::~Text()
{
	if (texture == 0) return;
	glDeleteTextures(1, &texture);
}

void Text::Reload(const char *c)
{
	texture = LoadText(c, font, color, width, height, texture);
}

void Text::Draw(float x, float y)
{
	glColor4f(color.r, color.g, color.b, color.a);
	DrawImageRect(texture, x, y);
}

void DrawText(float x, float y, const char *c, TTF_Font *font, const Color *color)
{
	if (color != NULL)
		glColor4f(color->r, color->g, color->b, color->a);
	if (color == NULL)
		color = &WHITE;
	Text temp(c, font, *color);
	DrawImageRect(temp.texture, x, y);
}
