#include "Text.h"

Text::Text(const char *c, TTF_Font *font_, Color color_)
{
	font = font_;
	color = color_;
	texture = LoadText(c, font, color, width, height);
}

void Text::Reload(const char *c)
{
	GLuint oldId = 0;
	if (texture != NULL)
	{
		oldId = texture->GetTexture();
		delete texture;
	}
	texture = LoadText(c, font, color, width, height, oldId);
}

void Text::Draw(float x, float y)
{
	glColor4f(color.r, color.g, color.b, color.a);
	texture->DrawAtRect(x, y);
}

Texture * Text::LoadText(const char *c, TTF_Font *font, Color color_, unsigned short &width, unsigned short &height, GLuint texture)
{
	SDL_Color color = {color_.r * 255, color_.g * 255, color_.b * 255, color_.a * 255};
	SDL_Surface *surface = TTF_RenderText_Blended(font, c, color);
	width = surface->w;
	height = surface->h;
	Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
		on the endianness (byte order) of the machine */
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	unsigned short w, h, offset = 16;
	w = surface->w;
	h = surface->h;

	while (w > offset) {offset *= 2;}
	w = offset;
	offset = 16;
	while (h > offset) offset *= 2;
	h = offset;

	SDL_Surface *bound = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(bound, NULL, SDL_MapRGBA(bound->format, 0, 0, 0, 0) );
	SDL_SetAlpha(surface, 0, SDL_ALPHA_OPAQUE);
	SDL_BlitSurface(surface, NULL, bound, NULL);

	Texture *t = new Texture(bound, texture);

	SDL_FreeSurface(surface);
	SDL_FreeSurface(bound);
	return t;
}

void DrawText(float x, float y, const char *c, TTF_Font *font, const Color *color)
{
	if (color != NULL)
		glColor4f(color->r, color->g, color->b, color->a);
	if (color == NULL)
		color = &WHITE;
	Text temp(c, font, *color);
	temp.Draw(x, y);
}
