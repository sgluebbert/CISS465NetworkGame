#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H



#include "SDL.h"
#include "SDL_mixer.h"



class Sound_Manager {
public:
	static Sound_Manager * Get_Instance();
	static void Delete_Instance();

	Mix_Chunk * Load_Sound(const char *);
	void Load_Music(const char *);
	
	Mix_Music * Music_Stream;
	// Mix_Chunk * Laser_Sound;
	
	void Play_Sound(Mix_Chunk *);
	void Play_Music();
	void Stop_Music();

	bool mute;
	
private:
    Sound_Manager();
    ~Sound_Manager();

	static Sound_Manager * instance;
};



Sound_Manager * Sound_Manager::instance = NULL;



Sound_Manager::Sound_Manager() {
    // Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 512);
    mute = true;
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
}

Sound_Manager::~Sound_Manager() {
	Mix_HaltChannel(-1);
    Mix_CloseAudio();
	// Mix_FreeChunk(Laser_Sound);
	Mix_FreeMusic(Music_Stream);
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
	Music_Stream = Mix_LoadMUS(filename);
		
	if (Music_Stream == NULL) 
		printf("Error in Sound: Mix_LoadMUS returns NULL. %s\n", Mix_GetError());
}

void Sound_Manager::Play_Sound(Mix_Chunk * sample) {
	if (!mute)
    	Mix_PlayChannel(-1, sample, 0);
}

void Sound_Manager::Play_Music() {
	if (!mute)
    	Mix_PlayMusic(Music_Stream, -1);
}

void Sound_Manager::Stop_Music() {
    Mix_HaltMusic();
}



#endif
