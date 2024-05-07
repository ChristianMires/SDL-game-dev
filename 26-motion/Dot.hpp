/* the dot that will move around on the screen */
class Dot
{
	public:
		/* the dimensions of the dot */
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		/* maximum axis velocity of the dot */
		static const int DOT_VEL = 10;

		/* initializes the variables */
		Dot(int screen_height, int screen_width);

		/* takes key presses and adjusts the dot's velocity */
		void handleEvent(SDL_Event& e);

		/* moves the dot */
		void move();

		/* shows the dot on the screen */
		void render(LTexture *tex);

	private:
		/* the screen height and width */
		int sh, sw;

		/* the X and Y offsets of the dot */
		int mPosX, mPosY;

		/* the velocity of the dot */
		int mVelX, mVelY;
};

Dot::Dot(int screen_height, int screen_width)
{
	/* set the screen height and width */
	sh = screen_height;
	sw = screen_width;

	/* initializes the offsets */
	mPosX = 0;
	mPosY = 0;

	/* initialize the velocity */
	mVelX = 0;
	mVelY = 0;
}

void Dot::handleEvent(SDL_Event& e)
{
	/* if a key was pressed */
	if (e.type == SDL_KEYDOWN & e.key.repeat == 0) {
		/* adjust the velocity */
		switch (e.key.keysym.sym) {
			case SDLK_UP: mVelY -= DOT_VEL; break;
			case SDLK_DOWN: mVelY += DOT_VEL; break;
			case SDLK_LEFT: mVelX -= DOT_VEL; break;
			case SDLK_RIGHT: mVelX += DOT_VEL; break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		/* adjust the velocity */
		switch (e.key.keysym.sym) {
			case SDLK_UP: mVelY += DOT_VEL; break;
			case SDLK_DOWN: mVelY -= DOT_VEL; break;
			case SDLK_LEFT: mVelX += DOT_VEL; break;
			case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}
	}
}

void Dot::move()
{
	/* move the dot left or right */
	mPosX += mVelX;

	/* if the dot went too far to the left or right */
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > sw)) {
		/* move back */
		mPosX -= mVelX;
	}

	/* move the dot up or down */
	mPosY += mVelY;

	/* if the dot went too far up or down */
	if ((mPosY < 0) || (mPosY + DOT_HEIGHT > sh)) {
		/* move back */
		mPosY -= mVelY;
	}
}

void Dot::render(LTexture *tex)
{
	/* show the dot */
	tex->render(mPosX, mPosY);
}
