#include "Draw.h"

void InitWindow()
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
	window_surface = SDL_SetVideoMode ( WINDOW_BOUNDING_BOX.w, WINDOW_BOUNDING_BOX.h, 32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE );// | SDL_FULLSCREEN);

	if ( window_surface == NULL ) 
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
}

GLuint LoadTexture(const char *c)
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

GLuint LoadTexture(SDL_Surface *surface, GLuint texture)
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

GLuint LoadText(const char *c, TTF_Font *font, Color color_, unsigned short &width, unsigned short &height, GLuint texture)
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

void DrawImageRect(GLuint texture, float x, float y, int w, int h)
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

void DrawImage(GLuint texture, float x, float y, float angle, float scale)
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

void DrawImage(GLuint texture, float x, float y, float angle)
{
	DrawImage(texture, x, y, angle, 1);
}

void DrawImage(GLuint texture, float x, float y)
{
	DrawImage(texture, x, y, 0, 1);
}

void DrawLine(float x1, float y1, float x2, float y2, const Color *color)
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

void DrawRect(float x1, float y1, float x2, float y2, bool fill, const Color *color)
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
