#include "SoundManager.h"
#include <iostream>



Sound_Manager * Sound_Manager::instance = NULL;



Sound_Manager::Sound_Manager() {
    mute = false;

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    	fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
    	printf("Mix_OpenAudio: %s\n", Mix_GetError());

	atexit(Mix_Quit);

    music_stream = NULL;
}

Sound_Manager::~Sound_Manager() {
    Mix_FreeMusic(music_stream);
	Mix_HaltChannel(-1);
    Mix_CloseAudio();
}

Sound_Manager * Sound_Manager::Get_Instance() {
	if (instance == NULL)
		instance = new Sound_Manager();
		
	return instance;
}

void Sound_Manager::Delete_Instance() {
	delete instance;
	instance = NULL;
}

Mix_Chunk * Sound_Manager::Load_Sound(const char * filename) {
	Mix_Chunk * sample = Mix_LoadWAV(filename);
		
	if (sample == NULL) 
		printf("Error in Sound: Mix_LoadWAV returns NULL. %s\n", Mix_GetError());
    
    return sample;
}

void Sound_Manager::Load_Music(const char * filename) {
	if (!music_stream) 
		Mix_FreeMusic(music_stream);

	music_stream = Mix_LoadMUS(filename);
		
	if (!music_stream) 
		printf("Error in Sound: Mix_LoadMUS returns NULL. %s\n", Mix_GetError());
}

void Sound_Manager::Play_Sound(Mix_Chunk * sample) {
	if (!mute && sample != NULL)
    	Mix_PlayChannel(-1, sample, 0);
}

void Sound_Manager::Play_Music() {
	if (!mute && music_stream)
    	if (Mix_PlayMusic(music_stream, -1) == -1)
			printf("Mix_PlayMusic: %s\n", Mix_GetError());
}

void Sound_Manager::Stop_Music() {
    Mix_HaltMusic();
}