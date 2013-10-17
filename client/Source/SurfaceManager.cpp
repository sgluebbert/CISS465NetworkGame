#include "SurfaceManager.h"

SurfaceManager * SurfaceManager::instance = NULL;
SurfaceManager * surface_manager = SurfaceManager::GetInstance();

SurfaceManager::SurfaceManager()
{
	if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (TTF_Init() != 0 )
	{
		printf("Unable to initialize TTF: %s\n", TTF_GetError());
		exit(1);
	}

	WINDOW_BOUNDING_BOX = DEFAULT_WINDOW_BOUNDING_BOX;

	// SDL_ShowCursor(false);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_WM_SetCaption("Not Named", NULL);
	surface = SDL_SetVideoMode ( WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE );// | SDL_FULLSCREEN);

	if ( surface == NULL ) 
	{
		std::cerr << "Unable to set " << WINDOW_BOUNDING_BOX.w << "x" << WINDOW_BOUNDING_BOX.h << " video: " << SDL_GetError() << std::endl;
		exit(1);
	}

	atexit(TTF_Quit);
	atexit(SDL_Quit);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glViewport(0, 0, WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);

	// Antialiasing on and amount
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 3);

	//Check for error
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		std::cerr << "Error initializing OpenGL: " << gluErrorString(error) << std::endl;
		exit(1);
	}

	Load();
}

SurfaceManager::~SurfaceManager()
{
}

void SurfaceManager::Load()
{
	fonts.init();

	test 					= SurfaceManager::LoadTexture("./Art/Untitled.bmp");
	bullet 					= SurfaceManager::LoadTexture("./Art/PPC_Shot.png");
	smoke 					= SurfaceManager::LoadTexture("./Art/smoke.png");
	ship 					= SurfaceManager::LoadTexture("./Art/ship01.png");
	background_main_menu 	= SurfaceManager::LoadTexture("./Art/Main_Menu.png");
	background_game 		= SurfaceManager::LoadTexture("./Art/Background.bmp");
	button 					= SurfaceManager::LoadTexture("./Art/Button.png");
	highlightedbutton 		= SurfaceManager::LoadTexture("./Art/HighlightedButton.png");
}

bool SurfaceManager::Resize(int new_width, int new_height)
{
	if (new_width < 1024)
		new_width = 1024;

	SDL_Surface *new_surface = SDL_SetVideoMode ( new_width, new_height, 32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE );

	if (new_surface == NULL)
	{
		std::cerr << "An error occured while trying to resize window: " << SDL_GetError() << std::endl;
		return false;
	}

	WINDOW_BOUNDING_BOX.w = new_width;
	WINDOW_BOUNDING_BOX.h = new_height;
	SDL_FreeSurface(surface);
	surface = new_surface;
	glViewport(0, 0, WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h);
	return true;
}

SurfaceManager * SurfaceManager::GetInstance()
{
	if (instance == NULL)
		instance = new SurfaceManager();
	
	return instance;
}

// void SurfaceManager::Delete_Instance()
// {
// 	delete instance;
// 	instance = NULL;
// }

GLuint SurfaceManager::LoadTexture(const char *c)
{
	SDL_Surface *surface;
	GLenum texture_format;
	GLint  nOfColors;
	GLuint texture = 0;

	if ((surface = IMG_Load(c)))
	{
		 // Check that the image's width is a power of 2
		if ( (surface->w & (surface->w - 1)) != 0 )
		{
			printf("warning: '%s' image's width is not a power of 2\n", c);
		}

		// Also check if the height is a power of 2
		if ( (surface->h & (surface->h - 1)) != 0 )
		{
			printf("warning: '%s' image's height is not a power of 2\n", c);
		}

		// get the number of channels in the SDL surface
		nOfColors = surface->format->BytesPerPixel;
		if (nOfColors == 4)     // contains an alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
			else
				texture_format = GL_BGRA;
		} 
		else if (nOfColors == 3)     // no alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
		}
		else
		{
			std::cout << "Warning: the image is not truecolor..  this will probably break\n";
			return -1;
		}

		
		// Have OpenGL generate a texture object handle for us
		glGenTextures(1, &texture);

		// Bind the texture object
		glBindTexture(GL_TEXTURE_2D, texture);

		// Set the texture's stretching properties
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

		SDL_FreeSurface(surface);
	} 
	else
	{
		std::cerr << "SDL could not load the image: " << SDL_GetError() << '\n';
		return -1;
	}

	if (texture == 0) std::cerr << "SDL could not load the image: " << c << '\n';
	return texture;
}

GLuint SurfaceManager::LoadTexture(SDL_Surface *surface, GLuint texture)
{
	GLenum texture_format;
	GLint  nOfColors;

	 // Check that the image's width is a power of 2
	if ( (surface->w & (surface->w - 1)) != 0 )
	{
		printf("warning: image's width is not a power of 2\n");
	}

	// Also check if the height is a power of 2
	if ( (surface->h & (surface->h - 1)) != 0 )
	{
		printf("warning: image's height is not a power of 2\n");
	}

	// get the number of channels in the SDL surface
	nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	} 
	else if (nOfColors == 3)     // no alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}
	else
	{
		std::cout << "Warning: the image is not truecolor..  this will probably break\n";
		return -1;
	}

	
	// Have OpenGL generate a texture object handle for us
	if (texture == 0)
		glGenTextures(1, &texture);

	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set the texture's stretching properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	if (texture == 0) std::cerr << "SDL could not load the image.\n";
	return texture;
}

GLuint SurfaceManager::LoadText(const char *c, TTF_Font *font, Color color_, unsigned short &width, unsigned short &height, GLuint texture)
{
	SDL_Color color = {color_.r * 255, color_.g * 255, color_.b * 255, color_.a * 255};
	SDL_Surface *surface = TTF_RenderText_Blended(font, c, color);
	width = surface->w;
	height = surface->h;
	Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
		on the endianness (byte order) of the machine */
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	unsigned short w, h, offset = 16;
	w = surface->w;
	h = surface->h;

	while (w > offset) {offset *= 2;}
	w = offset;
	offset = 16;
	while (h > offset) offset *= 2;
	h = offset;

	SDL_Surface *bound = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(bound, NULL, SDL_MapRGBA(bound->format, 0, 0, 0, 0) );
	SDL_SetAlpha(surface, 0, SDL_ALPHA_OPAQUE);
	SDL_BlitSurface(surface, NULL, bound, NULL);

	texture = LoadTexture(bound, texture);

	SDL_FreeSurface(surface);
	SDL_FreeSurface(bound);
	return texture;
}

void SurfaceManager::Clear()
{
	glClear( GL_COLOR_BUFFER_BIT );
	glLoadIdentity();
}

void SurfaceManager::Flip()
{
	SDL_GL_SwapBuffers();
}

void SurfaceManager::DrawImageRect(GLuint texture, float x, float y, int w, int h)
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 0, -1, 1 );
	glTranslatef(x, y, 0);
	glRotatef(0, 0, 0, 1);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);

	if (w == 0)
	{
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_QUADS);
		glTexCoord2i( 0, 0 );
		glVertex2f( 0, 0);
		glTexCoord2i( 1, 0 );
		glVertex2f( w, 0);
		glTexCoord2i( 1, 1 );
		glVertex2f( w, h);
		glTexCoord2i( 0, 1 );
		glVertex2f( 0, h);
	glEnd();
}

void SurfaceManager::DrawImage(GLuint texture, float x, float y, float angle, float scale)
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 0, -1, 1 );
	glTranslatef(x, y, 0);
	glRotatef(angle, 0, 0, 1);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_QUADS);
		glTexCoord2i( 0, 0 );
		glVertex2f( -scale, -scale);
		glTexCoord2i( 1, 0 );
		glVertex2f( scale, -scale);
		glTexCoord2i( 1, 1 );
		glVertex2f( scale, scale);
		glTexCoord2i( 0, 1 );
		glVertex2f( -scale, scale);
	glEnd();
}

void SurfaceManager::DrawImage(GLuint texture, float x, float y, float angle)
{
	DrawImage(texture, x, y, angle, 1);
}

void SurfaceManager::DrawImage(GLuint texture, float x, float y)
{
	DrawImage(texture, x, y, 0, 1);
}

void SurfaceManager::DrawLine(float x1, float y1, float x2, float y2, const Color *color)
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

void SurfaceManager::DrawRect(float x1, float y1, float x2, float y2, bool fill, const Color *color)
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

void SurfaceManager::DrawText(float x, float y, const char *c, TTF_Font *font, const Color *color)
{
	if (color != NULL)
		glColor4f(color->r, color->g, color->b, color->a);
	if (color == NULL)
		color = &WHITE;
	Text temp(c, font, *color);
	DrawImageRect(temp.texture, x, y);
}

Fonts::Fonts()
{
}

Fonts::~Fonts()
{
	// TTF_CloseFont(font_FreeMono_12);
	// std::cout << "killed fonts\n";
}

void Fonts::init()
{
	font_FreeMono_10 = TTF_OpenFont("Font/FreeMono.ttf", 10);
	if(!font_FreeMono_10) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_FreeMono_12 = TTF_OpenFont("Font/FreeMono.ttf", 12);
	if(!font_FreeMono_12) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_FreeMono_14 = TTF_OpenFont("Font/FreeMono.ttf", 14);
	if(!font_FreeMono_14) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_FreeMono_16 = TTF_OpenFont("Font/FreeMono.ttf", 16);
	if(!font_FreeMono_16) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_FreeMono_18 = TTF_OpenFont("Font/FreeMono.ttf", 18);
	if(!font_FreeMono_18) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_FreeMono_20 = TTF_OpenFont("Font/FreeMono.ttf", 20);
	if(!font_FreeMono_20) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_12 = TTF_OpenFont("Font/impact.ttf", 12);
	if(!font_Impact_12) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_14 = TTF_OpenFont("Font/impact.ttf", 14);
	if(!font_Impact_14) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_16 = TTF_OpenFont("Font/impact.ttf", 16);
	if(!font_Impact_16) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_18 = TTF_OpenFont("Font/impact.ttf", 18);
	if(!font_Impact_18) printf("TTF_OpenFont: %s\n", TTF_GetError());

	font_Impact_20 = TTF_OpenFont("Font/impact.ttf", 20);
	if(!font_Impact_20) printf("TTF_OpenFont: %s\n", TTF_GetError());
}

Text::Text(const char *c, TTF_Font *font_, Color color_)
{
	font = font_;
	color = color_;
	texture = SurfaceManager::LoadText(c, font, color, width, height);
}

Text::~Text()
{
	if (texture == 0) return;
	glDeleteTextures(1, &texture);
}

void Text::Reload(const char *c)
{
	texture = SurfaceManager::LoadText(c, font, color, width, height, texture);
}
