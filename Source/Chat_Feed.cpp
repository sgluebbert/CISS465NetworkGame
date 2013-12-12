#include "Chat_Feed.h"



Feed_Message::Feed_Message(const std::string & copy, float interval) {
	message = copy;
	timer.Set_Interval(interval);
	timer.Start();
}



const float Chat_Feed::MESSAGE_LIFESPAN = 10.0;
const int Chat_Feed::MAX_NUMBER_OF_MESSAGES = 10;
const int Chat_Feed::FONT_HEIGHT = 10;
const int Chat_Feed::BORDER_WIDTH = 2;
const int Chat_Feed::X_OFFSET = 646;
const int Chat_Feed::Y_OFFSET = 0;



Chat_Feed::Chat_Feed() {
	bounding_box.x = 0;
	bounding_box.y = 0;
	bounding_box.w = 154;
	bounding_box.h = 104;
	display_bounding_box.x = bounding_box.x + Chat_Feed::BORDER_WIDTH;
	display_bounding_box.y = bounding_box.y + Chat_Feed::BORDER_WIDTH;
	display_bounding_box.w = bounding_box.w - 2 * Chat_Feed::BORDER_WIDTH;
	display_bounding_box.h = bounding_box.h - 2 * Chat_Feed::BORDER_WIDTH;
}

//Could make this more functional and have one real message function that just replaces wildcards with appropriate things
void Chat_Feed::Player_Joined(const std::string & callsign) {
	std::stringstream stream;

	stream << callsign << " has joined.";

	Feed_Message temp(stream.str(), Chat_Feed::MESSAGE_LIFESPAN);

	messages.push_back(temp);

	Message_Cleanup();
}

void Chat_Feed::Player_Disconnected(const std::string & callsign) {
	std::stringstream stream;

	stream << callsign << " has disconnected.";

	Feed_Message temp(stream.str(), Chat_Feed::MESSAGE_LIFESPAN);

	messages.push_back(temp);

	Message_Cleanup();
}

void Chat_Feed::Player_Died(const std::string & callsign) {
	std::stringstream stream;

	stream << callsign << " has died.";

	Feed_Message temp(stream.str(), Chat_Feed::MESSAGE_LIFESPAN);

	messages.push_back(temp);

	Message_Cleanup();
}

void Chat_Feed::Player_Killed(const std::string & lhs, const std::string & rhs) {
	std::stringstream stream;

	stream << lhs << " has killed " << rhs << '!';

	Feed_Message temp(stream.str(), Chat_Feed::MESSAGE_LIFESPAN);

	messages.push_back(temp);

	Message_Cleanup();
}

void Chat_Feed::Message_Cleanup() {
	while (messages.size() > MAX_NUMBER_OF_MESSAGES)
		messages.pop_front();

	while (!messages.empty() && messages.front().timer.Ended())
		messages.pop_front();
}

void Chat_Feed::Update(double dt) {
	for (int i = 0; i < messages.size(); i++)
		messages[i].timer.Update(dt);

	Message_Cleanup();
}

void Chat_Feed::Draw() {
	bounding_box.x += X_OFFSET;
	bounding_box.y += Y_OFFSET;

	Rect<int> temp_rect;
	temp_rect.x = Chat_Feed::BORDER_WIDTH;
	temp_rect.y = Chat_Feed::BORDER_WIDTH;
	Color temp_color(1.0, 1.0, 1.0);

	Text text(" ", TextureManager::GetInstance()->fonts.font_FreeMono_10, temp_color);
	for (int i = messages.size() - 1; i >= 0; i--) {
		text.color.a = 1.0 - messages[i].timer.Get_Progress() * messages[i].timer.Get_Progress();
		text.Reload(messages[i].message.c_str());
		text.Draw(bounding_box.x + bounding_box.w - text.width, bounding_box.y + temp_rect.y);
		temp_rect.y += FONT_HEIGHT;
	}

	bounding_box.x -= X_OFFSET;
	bounding_box.y -= Y_OFFSET;
}