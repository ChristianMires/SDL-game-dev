#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "LTexture.hpp"
#include "LTimer.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Textures
LTexture gPromptTextTexture;
LTexture gTimeTextTexture;

//Fonts
TTF_Font* gFont = NULL;


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}


		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); 
			if( gRenderer == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	//Initialize SDL_ttf
	if( TTF_Init() < 0 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont( "ttf/lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Set text color as black
		SDL_Color textColor = { 0, 0, 0, 255 };

		//Load prompt texture
		if( !gPromptTextTexture.loadFromRenderedText( gRenderer, "Press Enter to Reset Start Time.", gFont, textColor ) )
		{
			printf( "Unable to render prompt texture!\n" );
			success = false;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	gPromptTextTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 255 };

			//The application timer
			LTimer timer;

			//In memory text stream
			std::stringstream timeText;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
						quit = true;

					//Handle key presses
					else if( e.type == SDL_KEYDOWN ) 
					{
						//Start/stop
						if( e.key.keysym.sym == SDLK_s )
						{
							if( timer.isStarted() )
							{
								timer.stop();
							}
							else
							{
								timer.start();
							}
						}
						//Pause/unpause
						else if( e.key.keysym.sym == SDLK_p )
						{
							if( timer.isPaused() )
							{
								timer.unpause();
							}
							else
							{
								timer.pause();
							}
						}
					}
				}

				//Set text to be rendered
				timeText.str( "" );
				timeText << "Seconds since start time " << ( timer.getTicks() / 1000.f );

				//Render text
				if( !gTimeTextTexture.loadFromRenderedText( gRenderer, timeText.str().c_str(), gFont, textColor ) )
				{
					printf( "Unable to render time texture!\n" );
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x5F, 0x5F, 0x5F, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render current texture
				gPromptTextTexture.render( gRenderer, ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
				gTimeTextTexture.render( gRenderer, ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gPromptTextTexture.getHeight() ) / 2 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
