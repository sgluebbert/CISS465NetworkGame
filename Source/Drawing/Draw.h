#ifndef DRAW_H
#define DRAW_H



#include "../Math.h"
#include "Texture.h"



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



static const Color RED(1, 0, 0, 1);
static const Color BLUE(0, 0, 1, 1);
static const Color GREEN(0, 1, 0, 1);
static const Color WHITE(1, 1, 1, 1);
static const Color BLACK(0, 0, 0, 1);
static const Color GRAY(.5, .5, .5, 1);

static const Color LIGHT_BLUE(0, 0, 1, 0.5);
static const Color LIGHT_RED(1, 0, 0, 0.5);
static const Color LIGHT_GREEN(0, 1, 0, 0.5);
static const Color LIGHT_GRAY(.5, .5, .5, 0.5);

static const Color ARMOR_COLOR(0, 0.5, 0, 1);
static const Color HEALTH_COLOR(0.5, 0.5, 0, 1);
static const Color SHIELD_COLOR(0, 0.5, 0.5, 1);
static const Color POWER_COLOR(0.5, 0.25, 0, 1);

static const Color RADAR_BORDER(0.545, 0.545, 0);
static const Color WEAPONBAY_BORDER(0.73, 1, 0.94);
static const Color WEAPONBAY_BODY(0.671, 0.858, 0.439);
static const Color WEAPONCD(0, 1, 0.1, .4);


static const int SMOOTH_FACTOR = 100;



void DrawLine(float x1, float y1, float x2, float y2, const Color * color = NULL);
void DrawRect(float x1, float y1, float x2, float y2, bool fill, const Color * color = NULL);
void DrawPolygon(float x, float y, float r, int s, double a, bool fill, const Color * color = NULL);
void DrawCircle(float x, float y, float r, bool fill, const Color * color = NULL);
void DrawCircleSection(float x, float y, float r, double sa, double ea, bool fill, const Color * color = NULL);



#endif
