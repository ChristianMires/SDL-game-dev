#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "LTexture.h"

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
LTexture gPressTexture;
LTexture gUpTexture;
LTexture gDownTexture;
LTexture gLeftTexture;
LTexture gRightTexture;

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

	//Load press texture
	if( !gPressTexture.loadFromFile( gRenderer, "textures/press.png" ) )
	{
		printf( "Failed to load press texture!\n" );
		success = false;
	}

	//Load up texture
	if( !gUpTexture.loadFromFile( gRenderer, "textures/up.png" ) )
	{
		printf( "Failed to load up texture!\n" );
		success = false;
	}

	//Load down texture
	if( !gDownTexture.loadFromFile( gRenderer, "textures/down.png" ) )
	{
		printf( "Failed to load down texture!\n" );
		success = false;
	}

	//Load left texture
	if( !gLeftTexture.loadFromFile( gRenderer, "textures/left.png" ) )
	{
		printf( "Failed to load left texture!\n" );
		success = false;
	}
	
	//Load right texture
	if( !gRightTexture.loadFromFile( gRenderer, "textures/right.png" ) )
	{
		printf( "Failed to load right texture!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gPressTexture.free();
 	gUpTexture.free();
 	gDownTexture.free();
 	gLeftTexture.free();
 	gRightTexture.free();

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

			//Current rendered texture
			LTexture* currentTexture = NULL;

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
				}

				//Set texture based on current keystate
				const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
				if( currentKeyStates[ SDL_SCANCODE_Q ] )
				{
					quit = true;
				}
				if( currentKeyStates[ SDL_SCANCODE_UP ] )
				{
					currentTexture = &gUpTexture;
				}
				else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
				{
					currentTexture = &gDownTexture;
				}
				else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
				{
					currentTexture = &gLeftTexture;
				}
				else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
				{
					currentTexture = &gRightTexture;
				}
				else
				{
					currentTexture = &gPressTexture;
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x5F, 0x5F, 0x5F, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render current texture
				currentTexture->render( gRenderer );


				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
