#include "Map.h"



const int Map::X_COORD = 0;
const int Map::Y_COORD = 1;
const float Map::DEFAULT_SECTION_SPACING = 1000.0;



Map::Map(int seed, float scale)
	: SEED(seed) {
	srand(SEED);

	if (scale < 0.5)
		map_scale = 0.5;
	else if (scale > 2.5)
		map_scale = 2.5;
	else
		map_scale = scale;

	max_players_per_team = int(6 * map_scale);
	min_players_per_team = 1;
	number_of_planets = rand() % 7 + 1;

	map_bounds.x = (number_of_planets / 2) * map_scale * -1000.0 - 500.0 * map_scale;
	map_bounds.y = map_scale * -1000.0;
	map_bounds.w = number_of_planets * map_scale * 1000.0;
	map_bounds.h = map_scale * 2000.0;

	int spawn_points = 3 * map_scale;
	for (int i = 0; i < spawn_points; ++i)
	{
		red_spawn_points.push_back(SpawnPoint(-map_bounds.w / 2 + 100 + rand() % 200, map_bounds.y + 100 + rand() % (map_bounds.h - 200)));
		blue_spawn_points.push_back(SpawnPoint(map_bounds.w / 2 - 100 - rand() % 200, map_bounds.y + 100 + rand() % (map_bounds.h - 200)));
	}

	srand(time(NULL));
}

Map::~Map() {
	Planet::Clear_Planets();

	for (int i = 0; i < pickups.size(); i++)
		delete pickups[i];
}

void Map::Update(double dt) {
}


void Map::Draw() {
}

void Map::DrawLobbyPreview(int x, int y, int w, int h) {
	// Draws a mini map for use in the lobby.  Should put everything in the rectangle given.
	// We could center it within the rectangle if the rectangle is not the correct aspect ratio.
	srand(SEED);

    TextureManager *textureManager = TextureManager::GetInstance();

	float ar = (float)w / (map_bounds.w);
	float ws = ar * w;
	float hs = ar * h;
	if (ws < hs)
	{
		ws *= 1 / hs;
		hs = 1;
	}
	else
	{
		hs *= 1 / ws;
		ws = 1;
	}

	float scaleHeight = (hs * (h - y)) / map_bounds.h;
	float scaleWidth = (ws * (w - x)) / map_bounds.w;

	Rect<int> preview_bounds(x, y, map_bounds.w * scaleWidth, map_bounds.h * scaleHeight);
	DrawRect(preview_bounds.x, preview_bounds.y, preview_bounds.w + x, preview_bounds.h + y, false, &GRAY);

	glColor4f(1, 0, 0, 1);
	for (int i = 0; i < red_spawn_points.size(); ++i)
		textureManager->spawn_point_icon->DrawCentered(preview_bounds.x + preview_bounds.w / 2 + red_spawn_points[i].x / 2 * scaleWidth, preview_bounds.y + preview_bounds.h / 2 + red_spawn_points[i].y * scaleHeight, 0, 6);

	glColor4f(0, 0, 1, 1);
	for (int i = 0; i < blue_spawn_points.size(); ++i)
		textureManager->spawn_point_icon->DrawCentered(preview_bounds.x + preview_bounds.w / 2 + blue_spawn_points[i].x / 2 * scaleWidth, preview_bounds.y + preview_bounds.h / 2 + blue_spawn_points[i].y * scaleHeight, 0, 6);

	glColor4f(1, 1, 1, 1);
	Planet::Preview_Planets(number_of_planets, map_scale * ar, preview_bounds.x + preview_bounds.w / 2, preview_bounds.y + preview_bounds.h / 2, scaleWidth);
	srand(time(NULL));
}

void Map::Generate_Map() {
	srand(SEED);
	Planet::Generate_Planets(number_of_planets, map_scale);

	for (int i = -1; i < 2; i += 2) {
		Pickup_Factory * temp = new Pickup_Factory(0, i * 250);

		temp->Set_Pickup(new Collectible());

		pickups.push_back(temp);
	}
	
	srand(time(NULL));
}