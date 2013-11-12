#ifndef	CHAT_FEED_H
#define CHAT_FEED_H



#include <deque>
#include <string>
#include <sstream>

#include "Rect.h"
#include "TextureManager.h"
#include "Timer.h"



struct Feed_Message {
	Feed_Message(const std::string &, float);

	std::string message;
	Timer timer;
};



class Chat_Feed {
public:
	Chat_Feed();

	void Player_Joined(const std::string &);
	void Player_Disconnected(const std::string &);
	void Player_Died(const std::string &);
	void Player_Killed(const std::string &, const std::string &);

	void Update(double);
	void Draw();

private:
	Rect<int> bounding_box;
	Rect<int> display_bounding_box;
	std::deque<Feed_Message> messages;

	static const float MESSAGE_LIFESPAN;
	static const int MAX_NUMBER_OF_MESSAGES;
	static const int FONT_HEIGHT;
	static const int BORDER_WIDTH;
	static const int X_OFFSET;
	static const int Y_OFFSET;

	void Message_Cleanup();
};



#endif