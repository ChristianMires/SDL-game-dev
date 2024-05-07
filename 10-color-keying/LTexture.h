//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallcoates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( SDL_Renderer* renderer, std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		SDL_Renderer* mRenderer;

		//Image dimensions
		int mWidth;
		int mHeight;
};

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mRenderer = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( SDL_Renderer* renderer, std::string path )
{
	//Get rid of preexisting texture
	free();

	//Set renderer for process
	mRenderer = renderer;

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Lod image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( mRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mRenderer = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render( int x, int y )
{
	//Set renering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy( mRenderer, mTexture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}
