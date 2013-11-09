#include "Draw.h"

void DrawLine(float x1, float y1, float x2, float y2, const Color * color)
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

void DrawRect(float x1, float y1, float x2, float y2, bool fill, const Color * color)
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

void DrawPolygon(float x, float y, float r, int s, double a, bool fill, const Color * color) {
    int loop_iter = (s - 1) / 2;
    double div_angle = 2 * PI / s;

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
    
    glBegin(GL_TRIANGLE_STRIP);
    
    glVertex2f(x, y + r);//sin(0) = 0 and cos(0) = 1
    
    for (int i = 0; i <= loop_iter; i++) {
        double lha = a - div_angle * i;
        double rha = a + div_angle * i;
        
        glVertex2f(x + sin(lha) * r, y + cos(lha) * r);
        glVertex2f(x + sin(rha) * r, y + cos(rha) * r);
    }
    
    if (s % 2 == 0) {
        double angle = a + div_angle * (s / 2);
        glVertex2f(x + sin(angle) * r, y + cos(angle) * r);
    }
    
    glEnd();
}

void DrawCircle(float x, float y, float r, bool fill, const Color * color) {
    DrawPolygon(x, y, r, SMOOTH_FACTOR, 0, fill, color);
}

void DrawCircleSection(float x, float y, float r, double sa, double ea, bool fill, const Color * color) {
    sa *= (PI / 180);
    ea *= (PI / 180);
    double div_angle = (ea - sa) / SMOOTH_FACTOR;

    if (div_angle <= 0)
    	return;

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
    
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y);
    
    for (double i = sa; i <= ea; i += div_angle)
        glVertex2f(x - sin(i) * r, y - cos(i) * r);
    
    glEnd();
}