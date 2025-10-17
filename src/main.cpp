#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
using namespace std;

struct Paddle
{
	int x, y;
	int w = 100, h = 100;
	int speed = 10;
	int score = 0;
};

struct Ball
{
	float x, y;
	int size = 10;
	float velocityX = 3, velocityY = 3;
};

enum GameStatePhases
{
	STOPPED = 1,
	RUNNING = 0,
	ENDED = -1
};

struct TextRender
{
	SDL_Texture *texture;
	SDL_Rect rect;
};

struct GameState
{
	GameStatePhases currentState = RUNNING;
};

void static DebugLogging(string message)
{
	cout << "[DEBUG]:  " << message << endl;
}

static TextRender createScoreTexture(SDL_Renderer *ren, TTF_Font *font, int score, int x, int y)
{
	TextRender result;
	char scoreText[32];
	snprintf(scoreText, sizeof(scoreText), "%d", score);
	SDL_Color textColor = {255, 255, 255, 255};
	SDL_Surface *surface = TTF_RenderText_Solid(font, scoreText, textColor);
	result.texture = SDL_CreateTextureFromSurface(ren, surface);
	int w = 50, h = 100;
	result.rect = {x, y, w, h};
	SDL_FreeSurface(surface);
	return result;
}

static TextRender GameOverText(SDL_Renderer *ren, TTF_Font *font, char *mesasge)
{
	TextRender result;
	SDL_Color textColor = {255, 255, 255, 255};
	SDL_Surface *surface = TTF_RenderText_Solid(font, mesasge, textColor);
	result.texture = SDL_CreateTextureFromSurface(ren, surface);
	result.rect = {550, 200, 200, 150};
	SDL_FreeSurface(surface);
	return result;
}

int main(int argc, char **args)
{
	GameState game;
	SDL_Window *window = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		cout << "Press any key to continue..." << endl;
		SDL_Delay(2000);
		return 1;
	}

	TTF_Init();

	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);

	if (window == NULL || !window)
	{
		cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
		SDL_Delay(2000);
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL)
	{
		cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
		SDL_Delay(2000);
		return 1;
	}

	const char *fontPath = "../assets/bytebounce/ByteBounce.ttf";
	TTF_Font *font = TTF_OpenFont(fontPath, 28);

	if (!font)
	{
		cout << "Failed to load font! TTF_Error: " << TTF_GetError() << endl;
		cout << "The path for the fonts not found \n"
			 << fontPath << endl;
		return 1;
	}

	SDL_Event event;
	Paddle leftPaddlePosition = {0, 310, 10, 100};
	Paddle rightPaddlePosition = {1270, 310, 10, 100};
	Ball ball = {640, 360};

	while (game.currentState == RUNNING || game.currentState == ENDED)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				game.currentState = STOPPED;
			}
		};

		TextRender leftScore = createScoreTexture(ren, font, leftPaddlePosition.score, 200, 50);
		TextRender rightScore = createScoreTexture(ren, font, rightPaddlePosition.score, 1000, 50);
		TextRender gameOver = GameOverText(ren, font, "");

		if (leftPaddlePosition.score > rightPaddlePosition.score && game.currentState == ENDED)
		{
			SDL_DestroyTexture(gameOver.texture);
			gameOver = GameOverText(ren, font, "Left Won");
		}
		else if (leftPaddlePosition.score < rightPaddlePosition.score && game.currentState == ENDED)
		{
			SDL_DestroyTexture(gameOver.texture);
			gameOver = GameOverText(ren, font, "Right Won");
		}

		SDL_SetRenderDrawColor(ren, 30, 30, 30, 255);
		SDL_RenderClear(ren);

		SDL_RenderCopy(ren, leftScore.texture, NULL, &leftScore.rect);
		SDL_RenderCopy(ren, rightScore.texture, NULL, &rightScore.rect);
		SDL_RenderCopy(ren, gameOver.texture, NULL, &gameOver.rect);

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);

		if (keystate[SDL_SCANCODE_Q] || keystate[SDL_SCANCODE_ESCAPE])
		{
			DebugLogging("Quit key pressed, exiting game");
			game.currentState = STOPPED;
		}

		if (game.currentState == RUNNING)
		{
			if (leftPaddlePosition.score == 2)
			{

				game.currentState = ENDED;
			}

			if (rightPaddlePosition.score == 2)
			{
				SDL_RenderCopy(ren, gameOver.texture, NULL, &gameOver.rect);
				game.currentState = ENDED;
			}

			if (keystate[SDL_SCANCODE_W] && leftPaddlePosition.y > 0)
			{
				leftPaddlePosition.y -= leftPaddlePosition.speed;
			}

			if (keystate[SDL_SCANCODE_S] && leftPaddlePosition.y + leftPaddlePosition.h < 720)
			{
				leftPaddlePosition.y += leftPaddlePosition.speed;
			}

			if (keystate[SDL_SCANCODE_UP] && rightPaddlePosition.y > 0)
			{
				rightPaddlePosition.y -= rightPaddlePosition.speed;
			}

			if (keystate[SDL_SCANCODE_DOWN] && rightPaddlePosition.y + rightPaddlePosition.h < 720)
			{
				rightPaddlePosition.y += rightPaddlePosition.speed;
			}

			if (ball.velocityX >= 100 || ball.velocityX <= -100)
			{
				ball.velocityX = 3;
			}

			ball.x += ball.velocityX;
			ball.y += ball.velocityY;

			// this is when the ball touches the top and bottom borders
			if (ball.y <= 0 || ball.y + ball.size >= 720)
			{
				ball.velocityY *= -1;
			}

			if (ball.x <= leftPaddlePosition.x + leftPaddlePosition.w &&
				ball.y + ball.size >= leftPaddlePosition.y &&
				ball.y <= leftPaddlePosition.y + leftPaddlePosition.h)
			{
				ball.velocityX -= 0.5;
				ball.velocityX *= -1;
				ball.x = leftPaddlePosition.x + leftPaddlePosition.w;
			}

			if (ball.x + ball.size >= rightPaddlePosition.x &&
				ball.y + ball.size >= rightPaddlePosition.y &&
				ball.y <= rightPaddlePosition.y + rightPaddlePosition.h)
			{
				ball.velocityX += 0.5;
				ball.velocityX *= -1;
				ball.x = rightPaddlePosition.x - ball.size;
			}

			// this is when the ball touches the left and right borders
			if (ball.x < 0)
			{
				ball.x = 640;
				ball.y = 360;
				ball.velocityX = -ball.velocityX;
				rightPaddlePosition.score += 1;
				SDL_DestroyTexture(leftScore.texture);
				leftScore = createScoreTexture(ren, font, leftPaddlePosition.score, 200, 50);
				DebugLogging("Ball went out on the left side");
			}

			if (ball.x > 1280)
			{
				ball.x = 640;
				ball.y = 360;
				ball.velocityX = -ball.velocityX;
				leftPaddlePosition.score += 1;
				SDL_DestroyTexture(leftScore.texture);
				leftScore = createScoreTexture(ren, font, leftPaddlePosition.score, 200, 50);
				DebugLogging("Ball went out on the right side");
			}
		}

		SDL_Rect leftRect = {leftPaddlePosition.x, leftPaddlePosition.y, leftPaddlePosition.w, leftPaddlePosition.h};
		SDL_Rect rightRect = {rightPaddlePosition.x, rightPaddlePosition.y, rightPaddlePosition.w, rightPaddlePosition.h};
		SDL_Rect ballRect = {(int)ball.x, (int)ball.y, ball.size, ball.size};

		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderFillRect(ren, &leftRect);
		SDL_RenderFillRect(ren, &rightRect);
		SDL_RenderFillRect(ren, &ballRect);
		SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
		for (int i = 0; i < 720; i += 20)
			SDL_RenderDrawLine(ren, 640, i, 640, i + 10);
		SDL_DestroyTexture(leftScore.texture);
		SDL_DestroyTexture(rightScore.texture);
		SDL_DestroyTexture(gameOver.texture);
		SDL_RenderPresent(ren);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
