#ifndef	CHAT_WINDOW_H
#define CHAT_WINDOW_H



#include <queue>
#include <string>
#include <sstream>

#include "System.h"



class Chat_Window {
public:
	Chat_Window();
	~Chat_Window();

	//Message Functions
	void Player_Joined(int);
	void Player_Disconnected(int);
	void Player_Died(int);

	void Draw();

private:
	SDL_Rect bounding_box;
	SDL_Rect chat_bounding_box;
	SDL_Surface * surface;
	std::queue<std::string> messages;

	static const int MAX_NUMBER_OF_MESSAGES;
	static const int FONT_HEIGHT;
	static const int X_OFFSET;

	void Notify();
	void Message_Cleanup();
};



const int Chat_Window::MAX_NUMBER_OF_MESSAGES = 10;
const int Chat_Window::FONT_HEIGHT = 10;
const int Chat_Window::X_OFFSET = 646;



Chat_Window::Chat_Window() {
	bounding_box.x = 0;
	bounding_box.y = 0;
	bounding_box.w = 154;
	bounding_box.h = 104;
	chat_bounding_box.x = bounding_box.x + 2;
	chat_bounding_box.y = bounding_box.y + 2;
	chat_bounding_box.w = bounding_box.w - 2 * 2;
	chat_bounding_box.h = bounding_box.h - 2 * 2;

	// surface = SDL_CreateRGBSurface(SDL_HWSURFACE, bounding_box.w, bounding_box.h, 32, 0, 0, 0, 0);
}

Chat_Window::~Chat_Window() {
	while(!messages.empty())
		messages.pop();
}

//Could make this more functional and have one real message function that just replaces wildcards with appropriate things
void Chat_Window::Player_Joined(int player_team) {
	std::stringstream temp;
	temp << "Player " << player_team << " has joined.";

	messages.push(temp.str());

	Message_Cleanup();
	Notify();
}

void Chat_Window::Player_Disconnected(int player_team) {
	std::stringstream temp;
	temp << "Player " << player_team << " has disconnected.";

	messages.push(temp.str());

	Message_Cleanup();
	Notify();
}

void Chat_Window::Player_Died(int player_team) {
	std::stringstream temp;
	temp << "Player " << player_team << " has died.";

	messages.push(temp.str());

	Message_Cleanup();
	Notify();
}

void Chat_Window::Message_Cleanup() {
	while (messages.size() > MAX_NUMBER_OF_MESSAGES)
		messages.pop();
}

void Chat_Window::Notify() {
	// SDL_FillRect(surface, &bounding_box, 0x808080);
	// SDL_FillRect(surface, &chat_bounding_box, 0x000000);

	// std::queue<std::string> temp_queue = messages;
	// SDL_Rect temp_rect;
	// temp_rect.x = 2;
	// temp_rect.y = 2;

	// while (!temp_queue.empty()) {
	// 	// SDL_BlitSurface(font_manager->Render(font_manager->chat_window_font, temp_queue.front(), GRAY), NULL, surface, &temp_rect);
	// 	temp_queue.pop();
	// 	temp_rect.y += FONT_HEIGHT;
	// }
}

void Chat_Window::Draw() {
	bounding_box.x += X_OFFSET;

	DrawRect(bounding_box.x, bounding_box.y, bounding_box.x + bounding_box.w, bounding_box.y + bounding_box.h, true, &BLACK);

	std::queue<std::string> temp_queue = messages;
	SDL_Rect temp_rect;
	temp_rect.x = 2;
	temp_rect.y = 2;

	while (!temp_queue.empty()) {
		DrawText(bounding_box.x + temp_rect.x, bounding_box.y + temp_rect.y, temp_queue.front().c_str(), SurfaceManager::GetInstance()->fonts.font_FreeMono_10, &WHITE);
		temp_queue.pop();
		temp_rect.y += FONT_HEIGHT;
	}

	DrawRect(bounding_box.x, bounding_box.y, bounding_box.x + bounding_box.w, bounding_box.y + bounding_box.h, false, &WHITE);

	bounding_box.x -= X_OFFSET;
}



#endif