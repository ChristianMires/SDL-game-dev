//The mouse button

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT,
	BUTTON_SPRITE_MOUSE_OVER_MOTION,
    	BUTTON_SPRITE_MOUSE_DOWN,
    	BUTTON_SPRITE_MOUSE_UP,
    	BUTTON_SPRITE_TOTAL
};

class LButton
{
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		void handleEvent( SDL_Event* e );

		//Shows button sprite
		void render(SDL_Renderer* renderer, LTexture* texture );

	private:
		//Top left position
		SDL_Point mPosition;
		
		//Currently used global sprite
		LButtonSprite mCurrentSprite;

		//The button sprites
		SDL_Rect mSpriteClips[ BUTTON_SPRITE_TOTAL ];

};

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;

	for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
	{
		mSpriteClips[ i ].x = 0;
		mSpriteClips[ i ].y = i * 200;
		mSpriteClips[ i ].w = BUTTON_WIDTH;
		mSpriteClips[ i ].h = BUTTON_HEIGHT;
	}
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse is above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse is below the button
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
					break;
				
				case SDL_MOUSEBUTTONDOWN:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
					break;

				case SDL_MOUSEBUTTONUP:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
					break;
			}
		}
	}
}

void LButton::render( SDL_Renderer* renderer, LTexture* texture )
{
	//Show current button sprite
	texture->render( renderer, mPosition.x, mPosition.y, &mSpriteClips[ mCurrentSprite ] );
}
