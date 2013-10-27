#include "Draw.h"

void DrawLine(float x1, float y1, float x2, float y2, const Color *color)
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 0, -1, 1 );
	glTranslatef(0, 0, 0);
	glRotatef(0, 0, 0, 1);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (color != NULL)
		glColor4f(color->r, color->g, color->b, color->a);

	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}

void DrawRect(float x1, float y1, float x2, float y2, bool fill, const Color *color)
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 0, -1, 1 );
	glTranslatef(0, 0, 0);
	glRotatef(0, 0, 0, 1);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);

	if (!fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (color != NULL)
		glColor4f(color->r, color->g, color->b, color->a);

	glBegin(GL_QUADS);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
	glEnd();

}
