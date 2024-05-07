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
		void move(SDL_Rect& wall);

		/* shows the dot on the screen */
		void render(LTexture *tex);

		/* box collision detector */
		bool checkCollision(SDL_Rect a, SDL_Rect b);

	private:
		/* the screen height and width */
		int sh, sw;

		/* the X and Y offsets of the dot */
		int mPosX, mPosY;

		/* the velocity of the dot */
		int mVelX, mVelY;

		/* Dot's collision box */
		SDL_Rect mCollider;
};

Dot::Dot(int screen_height, int screen_width)
{
	/* set the screen height and width */
	sh = screen_height;
	sw = screen_width;

	/* initializes the offsets */
	mPosX = 0;
	mPosY = 0;

	/* set collision box dimension */
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;

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

void Dot::move(SDL_Rect& wall)
{
	/* move the dot left or right */
	mPosX += mVelX;
	mCollider.x = mPosX;

	/* if the dot collided or went too far to the left or right */
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > sw) || checkCollision(mCollider, wall)) {
		/* move back */
		mPosX -= mVelX;
		mCollider.x = mPosX;
	}

	/* move the dot up or down */
	mPosY += mVelY;
	mCollider.y = mPosY;

	/* if the dot went too far up or down */
	if ((mPosY < 0) || (mPosY + DOT_HEIGHT > sh) || checkCollision(mCollider, wall)) {
		/* move back */
		mPosY -= mVelY;
		mCollider.y = mPosY;
	}
}

void Dot::render(LTexture *tex)
{
	/* show the dot */
	tex->render(mPosX, mPosY);
}

bool Dot::checkCollision(SDL_Rect a, SDL_Rect b)
{
//	
//	/* the sides of the rectangles */
//	int leftA, leftB;
//	int rightA, rightB;
//	int topA, topB;
//	int bottomA, bottomB;
//
//	/* calculate the sides of rect A */
//	leftA = a.x;
//	rightA = a.x + a.w;
//	topA = a.y;
//	bottomA = a.y + a.h;
//
//	/* calculate the sides of rect B */
//	leftB = b.x;
//	rightB = b.x + b.w;
//	topB = b.y;
//	bottomB = b.y + b.h;
//
//	/* if any of the sides from A are outside of B */
//	if (bottomA <= topB) return false;
//	if (topA >= bottomB) return false;
//	if (rightA <= leftB) return false;
//	if (leftA >= rightB) return false;
	if (SDL_HasIntersection(&a, &b)) return true;

	/* if none of the sides from A are outside B */
	return false;
}
