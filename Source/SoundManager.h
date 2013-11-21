#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H



#include "SDL.h"
#include "SDL_mixer.h"



class Sound_Manager {
public:
	static Sound_Manager * Get_Instance();
	static void Delete_Instance();

	Mix_Chunk * Load_Sound(const char *);
	void Load_Music(const char *);
	
	void Play_Sound(Mix_Chunk *);
	void Play_Music();
	void Stop_Music();
	
	Mix_Music * music_stream;

	bool mute;
	
private:
    Sound_Manager();
    ~Sound_Manager();

	static Sound_Manager * instance;
};



#endif
