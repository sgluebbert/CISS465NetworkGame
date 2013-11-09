#include "Progress_Bar.h"



Progress_Bar::Progress_Bar() {
	back_color 		= Color(0.00, 0.00, 0.00);
	border_color	= Color(1.00, 0.00, 0.00);
	normal_color 	= Color(0.50, 0.50, 0.50);

	progress = 1.0;
}


void Progress_Bar::Set_Rect(int x_coord, int y_coord, int width, int height, int border_width) {
	border_rect.x = x_coord;
	border_rect.y = y_coord;
	border_rect.w = width;
	border_rect.h = height;
	bar_rect.x = border_rect.x + border_width;
	bar_rect.y = border_rect.y + border_width;
	bar_rect.w = border_rect.w - 2 * border_width;
	bar_rect.h = border_rect.h - 2 * border_width;
}



void Progress_Bar::Notify(double pFlag) {
	progress = pFlag;
}

void Progress_Bar::Update(double dt) {

}

void Progress_Bar::Draw() {
	DrawRect(border_rect.x, border_rect.y,
			 border_rect.x + border_rect.w, border_rect.y + border_rect.h,
			 true, &border_color);
	DrawRect(bar_rect.x, bar_rect.y,
			 bar_rect.x + bar_rect.w, bar_rect.y + bar_rect.h,
			 true, &back_color);
	DrawRect(bar_rect.x, bar_rect.y,
			 bar_rect.x + (bar_rect.w * progress), bar_rect.y + bar_rect.h,
			 true, &normal_color);
}



Health_Bar::Health_Bar() {
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


void Health_Bar::Set_Color(Color color) {
	normal_color = color;
	damage_color = Color(color.r * 0.5, color.g * 0.5, color.b * 0.5);
	repair_color = Color(color.r * 1.5, color.g * 1.5, color.b * 1.5);
}



void Health_Bar::Notify(double pFlag) {
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

void Health_Bar::Update(double dt) {
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

void Health_Bar::Draw() {
	DrawRect(border_rect.x, border_rect.y,
			 border_rect.x + border_rect.w, border_rect.y + border_rect.h,
			 true, &border_color);
	DrawRect(bar_rect.x, bar_rect.y,
			 bar_rect.x + bar_rect.w, bar_rect.y + bar_rect.h,
			 true, &back_color);

	if (state == DAMAGE_HEALTH_STATE)
		DrawRect(bar_rect.x, bar_rect.y,
				 bar_rect.x + (bar_rect.w * progress), bar_rect.y + bar_rect.h,
				 true, &damage_color);
	else if (state == REPAIR_HEALTH_STATE)
		DrawRect(bar_rect.x, bar_rect.y,
				 bar_rect.x + (bar_rect.w * progress), bar_rect.y + bar_rect.h,
				 true, &repair_color);

	DrawRect(bar_rect.x, bar_rect.y,
			 bar_rect.x + (bar_rect.w * difference), bar_rect.y + bar_rect.h,
			 true, &normal_color);
}