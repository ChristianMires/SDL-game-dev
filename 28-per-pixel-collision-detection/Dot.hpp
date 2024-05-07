/* the dot that will move around on the screen */
class Dot
{
	public:
		/* the dimensions of the dot */
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		/* maximum axis velocity of the dot */
		static const int DOT_VEL = 1;

		/* initializes the variables */
		Dot(int screen_width=0, int screen_height=0, int xpos=0, int ypos=0);

		/* takes key presses and adjusts the dot's velocity */
		void handleEvent(SDL_Event& e);

		/* moves the dot */
		void move(std::vector<SDL_Rect>& otherColliders);

		/* shows the dot on the screen */
		void render(LTexture *tex);

		/* gets the collision boxes */
		std::vector<SDL_Rect>& getColliders();

		/* box set collision detector */
		bool checkCollision(std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b);

	private:
		/* the screen height and width */
		int sh, sw;

		/* the X and Y offsets of the dot */
		int mPosX, mPosY;

		/* the velocity of the dot */
		int mVelX, mVelY;

		/* dot's collision boxes */
		std::vector<SDL_Rect> mColliders;

		/* moves the collision boxes relative to the dot's offset */
		void shiftColliders();
};

Dot::Dot(int screen_width, int screen_height, int xpos, int ypos)
{
	/* set the screen height and width */
	sw = screen_width;
	sh = screen_height;

	/* initializes the offsets */
	mPosX = xpos;
	mPosY = ypos;

	/* create the necessary SDL_Rects */
	mColliders.resize(11);

	/* initialize the velocity */
	mVelX = 0;
	mVelY = 0;

	/* initialize the collision boxes' width and height */
	mColliders[0].w = 6;
	mColliders[0].h = 1;

	mColliders[1].w = 10;
	mColliders[1].h = 1;

	mColliders[2].w = 14;
	mColliders[2].h = 1;

	mColliders[3].w = 16;
	mColliders[3].h = 2;

	mColliders[4].w = 18;
	mColliders[4].h = 2;

	mColliders[5].w = 20;
	mColliders[5].h = 6;

	mColliders[6].w = 18;
	mColliders[6].h = 2;

	mColliders[7].w = 16;
	mColliders[7].h = 2;

	mColliders[8].w = 14;
	mColliders[8].h = 1;

	mColliders[9].w = 10;
	mColliders[9].h = 1;

	mColliders[10].w = 6;
	mColliders[10].h = 1;

	/* initialize colliders relative to position */
	shiftColliders();
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

void Dot::move(std::vector<SDL_Rect>& otherColliders)
{
	/* move the dot left or right */
	mPosX += mVelX;
	shiftColliders();

	/* if the dot collided or went too far to the left or right */
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > sw) || checkCollision(mColliders, otherColliders)) {
		/* move back */
		mPosX -= mVelX;
		shiftColliders();
	}

	/* move the dot up or down */
	mPosY += mVelY;
	shiftColliders();

	/* if the dot went too far up or down */
	if ((mPosY < 0) || (mPosY + DOT_HEIGHT > sh) || checkCollision(mColliders, otherColliders)) {
		/* move back */
		mPosY -= mVelY;
		shiftColliders();
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

std::vector<SDL_Rect& Dot::getColliders()
{
	/* the row offset */

void Dot::shiftColliders()
{
	/* the row offset */
	int r = 0;

	/* go through the dot's collsion boxes */
	for (int set = 0; set < mColliders.size(); ++set) {
		/* center the collision box */
		mColliders[set].x = mPosX + (DOT_WIDTH - mColliders[set].w) / 2;

		/* set the collision box at its row offset */
		mColliders[set].y = mPosY + r;

		/* move the row offset down the height of the collision box */
		r += mColliders[set].h;
	}
}
