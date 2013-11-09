#include "Progress_Circle.h"



Progress_Circle::Progress_Circle() {
	Progress_Display();
	start_angle = 0.0;
}


void Progress_Circle::Set_Circle(int x_coord, int y_coord, int radius, int border_width, int center_width, double angle = 0) {
	border_circle.x = progress_circle.x = center_circle.x = x_coord;
	border_circle.y = progress_circle.y = center_circle.y = y_coord;
	border_circle.r = radius;
	progress_circle.r = radius - border_width;
	center_circle.r = center_width;

	if (angle > 359.0)
		start_angle = 359.0;
	else if (angle < 0.0)
		start_angle = 0.0;
	else
		start_angle = angle;
}



void Progress_Circle::Notify(double pFlag) {
	Progress_Display::Notify(pFlag);
}

void Progress_Circle::Update(double dt) {

}

void Progress_Circle::Draw() {
	DrawCircle(border_circle.x, border_circle.y, border_circle.r, true, &border_color);
	DrawCircle(progress_circle.x, progress_circle.y, progress_circle.r, true, &back_color);
	DrawCircleSection(progress_circle.x, progress_circle.y, progress_circle.r, start_angle, 360.0 * progress, true, &normal_color);
	DrawCircle(center_circle.x, center_circle.y, center_circle.r, true, &border_color);
}



Alignment_Circle::Alignment_Circle() {
	back_color 		= Color(0.00, 0.00, 0.00);
	border_color	= Color(1.00, 0.00, 0.00);
	lhs_color		= Color(0.25, 0.25, 0.25);
	normal_color 	= Color(0.50, 0.50, 0.50);
	rhs_color 		= Color(0.75, 0.75, 0.75);

	progress = 0.0;

	start_angle = 0.0;
	midangle = 180.0;
}


void Alignment_Circle::Set_Circle(int x_coord, int y_coord, int radius, int border_width, int center_width, double angle = 0) {
	Progress_Circle::Set_Circle(x_coord, y_coord, radius, border_width, center_width, angle);

	midangle = start_angle + 180.0;

	if (midangle > 359)
		midangle -= 359;
	else if (midangle < 0)
		midangle += 359;
}

void Alignment_Circle::Notify(double pFlag) {
	if (pFlag < -1.0)
		pFlag = -1.0;
	if (pFlag > 1.0)
		pFlag = 1.0;

	progress = pFlag;
}

void Alignment_Circle::Update(double dt) {

}

void Alignment_Circle::Draw() {
	DrawCircle(border_circle.x, border_circle.y, border_circle.r, true, &border_color);
	DrawCircle(progress_circle.x, progress_circle.y, progress_circle.r, true, &back_color);

	if (progress < 0)
		DrawCircleSection(progress_circle.x, progress_circle.y, progress_circle.r, start_angle, midangle + 180 * progress, true, &lhs_color);
	else
		DrawCircleSection(progress_circle.x, progress_circle.y, progress_circle.r, midangle, midangle + 180 * progress, true, &rhs_color);

	DrawCircle(center_circle.x, center_circle.y, center_circle.r, true, &border_color);
}



Health_Circle::Health_Circle() {
	back_color 		= Color(0.00, 0.00, 0.00);
	border_color	= Color(1.00, 1.00, 1.00);
	damage_color	= Color(0.25, 0.25, 0.25);
	normal_color 	= Color(0.50, 0.50, 0.50);
	repair_color 	= Color(0.75, 0.75, 0.75);

	progress = difference = 1.0;

	state = NORMAL_HEALTH_STATE;
	damage_equalization_rate = 0.1;
	repair_equalization_rate = 0.2;
}


void Health_Circle::Set_Color(Color color) {
	normal_color = color;
	damage_color = Color(color.r * 0.5, color.g * 0.5, color.b * 0.5);
	repair_color = Color(color.r * 1.5, color.g * 1.5, color.b * 1.5);
}



void Health_Circle::Notify(double pFlag) {
	if (pFlag > 1.0)
		pFlag = 1.0;
	if (pFlag < 0.0)
		pFlag = 0.0;

	difference = pFlag - difference;

	if (difference == 0) {
		difference = pFlag;
		return;
	}
    else if (difference < 0)
        state = DAMAGE_HEALTH_STATE;
    else// if (difference > 0)
        state = REPAIR_HEALTH_STATE;

    difference = pFlag;
}

void Health_Circle::Update(double dt) {
	switch(state) {
	case NORMAL_HEALTH_STATE:
		break;
	case DAMAGE_HEALTH_STATE:
		progress -= damage_equalization_rate * dt;

		if (progress <= difference) {
			state = NORMAL_HEALTH_STATE;
			progress = difference;
		}

		break;
	case REPAIR_HEALTH_STATE:
		progress += repair_equalization_rate * dt;

		if (progress >= difference) {
			state = NORMAL_HEALTH_STATE;
			progress = difference;
		}

		break;
	}
}

void Health_Circle::Draw() {
	DrawCircle(border_circle.x, border_circle.y, border_circle.r, true, &border_color);
	DrawCircle(progress_circle.x, progress_circle.y, progress_circle.r, true, &back_color);

	if (state == DAMAGE_HEALTH_STATE)
		DrawCircleSection(progress_circle.x, progress_circle.y, progress_circle.r, start_angle, 360.0 * progress, true, &damage_color);
	else if (state == REPAIR_HEALTH_STATE)
		DrawCircleSection(progress_circle.x, progress_circle.y, progress_circle.r, start_angle, 360.0 * progress, true, &repair_color);

	DrawCircleSection(progress_circle.x, progress_circle.y, progress_circle.r, start_angle, 360.0 * difference, true, &normal_color);
	DrawCircle(center_circle.x, center_circle.y, center_circle.r, true, &border_color);
}