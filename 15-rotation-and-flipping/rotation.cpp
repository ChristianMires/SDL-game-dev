#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
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

//Walking animation
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];

//Textures
LTexture gSpriteSheetTexture;
LTexture gBackgroundTexture;

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
	if( !gSpriteSheetTexture.loadFromFile( gRenderer, "textures/spritesheet.png" ) )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	else
	{
		//Set standard alpha blending
		gSpriteSheetTexture.setBlendMode( SDL_BLENDMODE_BLEND );

		//Set sprite clips
		gSpriteClips[ 0 ].x = 0;
		gSpriteClips[ 0 ].y = 0;
		gSpriteClips[ 0 ].w = 64;
		gSpriteClips[ 0 ].h = 205;

		gSpriteClips[ 1 ].x = 64;
		gSpriteClips[ 1 ].y = 0;
		gSpriteClips[ 1 ].w = 64;
		gSpriteClips[ 1 ].h = 205;

		gSpriteClips[ 2 ].x = 128;
		gSpriteClips[ 2 ].y = 0;
		gSpriteClips[ 2 ].w = 64;
		gSpriteClips[ 2 ].h = 205;

		gSpriteClips[ 3 ].x = 192;
		gSpriteClips[ 3 ].y = 0;
		gSpriteClips[ 3 ].w = 64;
		gSpriteClips[ 3 ].h = 205;

	}

	if( !gBackgroundTexture.loadFromFile( gRenderer, "textures/background.png" ) )
	{
		printf( "Failed to load background image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();
	gBackgroundTexture.free();

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

			//Angle of rotation
			double degrees = 0;

			//Flip type
			SDL_RendererFlip flipType = SDL_FLIP_NONE;

			//Current animation frame
			int frame = 0;

			//Modulation components
			Uint8 r = 255;
			Uint8 g = 255;
			Uint8 b = 255;
			Uint8 a = 255;

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
					else if( e.type == SDL_KEYDOWN )
					{
						switch( e.key.keysym.sym)
						{
							//Alternate quit key
							case SDLK_q:
								quit = true;
								break;

							//Increase red
							case SDLK_r:
								r += 32;
								break;

							//Increase green
							case SDLK_f:
								g += 32;
								break;
							
							//Increase blue
							case SDLK_v:
								b += 32;
								break;

							//Decrease red
							case SDLK_t:
								r -= 32;
								break;

							//Decrease green
							case SDLK_g:
								g -= 32;
								break;

							//Decrease blue
							case SDLK_b:
								b -= 32;
								break;

							//Increase alpha
							case SDLK_a:
								if( a + 32 > 255 )
									a = 255;
								else
									a += 32;
								break;

							//Decrease alpha
							case SDLK_z:
								if( a - 32 < 0 )
									a = 0;
								else
									a -= 32;
								break;

							//Rotate clockwise
							case SDLK_p:
								degrees += 60;
								break;

							//Rotate counter-clockwise
							case SDLK_o:
								degrees -= 60;

							//Face right
							case SDLK_RIGHT:
								flipType = SDL_FLIP_HORIZONTAL;
								break;

							//Face left
							case SDLK_LEFT:
								flipType = SDL_FLIP_NONE;
								break;

							//Flip vertical
							case SDLK_UP:
								flipType = SDL_FLIP_VERTICAL;
								break;
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBackgroundTexture.render( 0, 0 );

				//Modulate and render texture
				gSpriteSheetTexture.setColor( r, g, b );
				gSpriteSheetTexture.setAlpha( a );

				SDL_Rect* currentClip = &gSpriteClips[ frame / 4 ];
				gSpriteSheetTexture.render( ( SCREEN_WIDTH - currentClip->w ) / 2 , ( SCREEN_HEIGHT - currentClip->h ) / 2 , currentClip, degrees, NULL, flipType );

				//Update screen
				SDL_RenderPresent( gRenderer );

				//Go to next frame
				++frame;

				//Cycle animation
				if( frame / 4 >= WALKING_ANIMATION_FRAMES )
				{
					frame = 0;
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
