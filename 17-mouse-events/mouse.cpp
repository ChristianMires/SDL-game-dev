#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "LTexture.h"
#include "LButton.h"

//Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;

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

//The buttons
LButton gButtons[ TOTAL_BUTTONS ];

//Textures
LTexture gButtonsSpriteSheetTexture;

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

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load front alpha texture
	if( !gButtonsSpriteSheetTexture.loadFromFile( gRenderer, "textures/buttons.png" ) )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	else
	{
		gButtons[ 0 ].setPosition( 0, 0 );
		gButtons[ 1 ].setPosition( BUTTON_WIDTH, 0 );
		gButtons[ 2 ].setPosition( 0, BUTTON_HEIGHT );
		gButtons[ 3 ].setPosition( BUTTON_WIDTH, BUTTON_HEIGHT );
	}

	return success;
}

void close()
{
	//Free loaded images
	gButtonsSpriteSheetTexture.free();

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

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle key presses
					else if( e.type == SDL_KEYDOWN )
					{
						switch( e.key.keysym.sym)
						{
							//Alternate quit key
							case SDLK_q:
								quit = true;
								break;
						}
					}

					//Handle button events
					for( int i = 0; i < TOTAL_BUTTONS; ++i )
					{
						gButtons[ i ].handleEvent( &e );
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x5F, 0x5F, 0x5F, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render buttons
				for( int i = 0; i < TOTAL_BUTTONS; ++i )
				{
					gButtons[ i ].render( gRenderer, &gButtonsSpriteSheetTexture );
				}

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
