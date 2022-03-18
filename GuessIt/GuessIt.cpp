#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <SDL_ttf.h>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Renderer* screen = NULL;
TTF_Font* myFont = NULL;

SDL_Event event;
std::string output2 = "make a guess";
std::string output1 = "0";
std::string output3 = "100";
int secretNumber = 0;
bool gameOver = false;
bool quit = false;
bool correctScreen = 0;
bool newGame = true;
int playerGuess = 0;
std::string tooLarge = "Too large";
std::string tooSmall = "Too small";
std::string correct = "Correct";
SDL_Color color = { 255, 251, 38 };
int textWidth;
int textHeight;
int digit;
bool playAgainScreen = 0;
int score = 100;

void init();
int getRandomNumber();
void handleInput();
int compare();
bool playerGuessing = 1;
void showOutput();
void showInput();

int main(int argc, char* args[])
{
	srand(time(NULL));
	secretNumber = getRandomNumber();
	init();
	while (!quit)
	{
		handleInput();
		SDL_SetRenderDrawColor(screen, 0, 0, 0, 0);
		SDL_RenderClear(screen);
		showInput();
		SDL_RenderPresent(screen);
	}
	return 0;
}

void init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}

	window = SDL_CreateWindow("GuessIt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}

	if (!TTF_Init()) 
	{
		std::cout << SDL_GetError() << std::endl;
	}

	screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (screen == NULL) {
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}

	myFont = TTF_OpenFont("8-BIT-WONDER.ttf", 24);
}

void handleInput() {
	while (SDL_PollEvent(&event) != 0) {
		if (!gameOver) {
			//score = 100;
			digit = -1;
			if (event.type == SDL_KEYDOWN) {
				if (playerGuessing) {
					switch (event.key.keysym.sym) {
					case SDLK_0:
						digit = 0;
						break;
					case SDLK_1:
						digit = 1;
						break;
					case SDLK_2:
						digit = 2;
						break;
					case SDLK_3:
						digit = 3;
						break;
					case SDLK_4:
						digit = 4;
						break;
					case SDLK_5:
						digit = 5;
						break;
					case SDLK_6:
						digit = 6;
						break;
					case SDLK_7:
						digit = 7;
						break;
					case SDLK_8:
						digit = 8;
						break;
					case SDLK_9:
						digit = 9;
						break;
					case SDLK_BACKSPACE:
						playerGuess /= 10;
						output1 = std::to_string(playerGuess);
						break;
					case SDLK_RETURN:
						playerGuessing = false;
						if (compare() == 0) {
							output2 = correct;
							correctScreen = 1;
							gameOver = 1;
						}
						else if (compare() < 0) {
							output2 = tooSmall;
							score--;
							output3 = std::to_string(score);
						}
						else {
							output2 = tooLarge;
							score--;
							output3 = std::to_string(score);
						}
					}
					if (digit != -1) {
						playerGuess = playerGuess * 10 + digit;
						output1 = std::to_string(playerGuess);
						output2 = "Make a guess";
					}
				}

				else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
					playerGuessing = true;
					output1 = "0";
					playerGuess = 0;
					output2 = "Make a guess";
				}
			}
			else if (event.type == SDL_QUIT) {
				gameOver = true;
				quit = true;
			}
		}
		else {
			if (correctScreen) {
				if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
					correctScreen = 0;
				}
			}
			else {
				output1 = "PLAY AGAIN";
				output2 = "Y       N";
				if (event.type == SDL_KEYDOWN) {
					if (event.key.keysym.sym == SDLK_y) {
						gameOver = false;
						output1 = "0";
						output2 = "make a guess";
						output3 = "100";
						playerGuess = 0;
						playerGuessing = 1;
						score = 100;
						newGame = true;
						secretNumber = getRandomNumber();
					}
					else if (event.key.keysym.sym == SDLK_n) {
						gameOver = true;
						quit = true;
					}
				}
				else if (event.type == SDL_QUIT) {
					gameOver = true;
					quit = true;
				}
			}
		}
	}

}

int getRandomNumber() {
	return rand() % 100 + 1;
}

int compare() {
	return playerGuess - secretNumber;
}

void printToScreen(std::string output, int h) {
	if (myFont == NULL) {
		std::cout << TTF_GetError() << std::endl;
		return;
	}
	
	SDL_Surface* LoadSurface = TTF_RenderText_Solid(myFont, output.c_str(), color);
	if (LoadSurface == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		return;
	}
	textHeight = LoadSurface->h;
	textWidth = LoadSurface->w;

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(screen, LoadSurface);
	if (newTexture == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		return;
	}
	SDL_FreeSurface(LoadSurface);
	SDL_Rect renderQuad = { SCREEN_WIDTH / 2 - textWidth / 2, h , textWidth, textHeight };
	SDL_RenderCopy(screen, newTexture, NULL, &renderQuad);
}

void showInput()
{
	printToScreen("SCORE", 350);
	printToScreen(output1, 150);
	printToScreen(output2, 100);
	printToScreen(output3, 400);
}