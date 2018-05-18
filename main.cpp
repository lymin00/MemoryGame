#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include "game.h"
#undef main

LTexture Game;

void LTexture::loadMedia(std::string path,int x, int y, int w, int h)
{
    gTexture =  Game.loadFromFile(path);
	SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
    SDL_RenderCopy(gRenderer, gTexture, NULL, &dest);
}

void LTexture::loadText(std::string time)
{
	gFont = TTF_OpenFont( "font.ttf", 28 );
	if( gFont != NULL )
	{
		SDL_Color textColor = { 255, 255, 255 };
		if( !Game.loadFromRenderedText( time, textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
	}
}

void LTexture::close()
{
    Mix_FreeChunk( gSound );
	gSound = NULL;
	Mix_FreeMusic( gMusic );
	gMusic = NULL;
	Game.free();
	TTF_CloseFont( gFont );
	gFont = NULL;
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void LTexture::renderPicture()
{
    clearScreen();
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    loadMedia("theme_game.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    std::string time = "";
    duration = 30 - duration;
    if (duration >= 0)
    {
        if (countPic == 12)
        {
            clearScreen();
            loadMedia("win.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            SDL_RenderPresent( gRenderer );
            SDL_Delay(2000);
            quit = true;
            return;
        }
    }
    if (duration == 0)
    {
        time = "00:00";
        loadText(time);
        Game.render(395,28);
        clearScreen();
        loadMedia("lose.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        SDL_RenderPresent( gRenderer );
        SDL_Delay(2000);
        quit = true;
        return;
    }
    else
    {
        while (duration != 0)
        {
            time = char(duration % 10 + 48) + time;
            duration /= 10;
        }
        time = "00:" + time;
    }
    loadText(time);
    Game.render(395,28);
    for (int i = 0; i < 12; i++)
    {
        if (pathPictureRender[i] != "")
        {
            loadMedia(pathPictureRender[i], posx[i], posy[i], pictureSize, pictureSize);
        }
    }
    for (int i = 0; i < quantityOfPictureIsOpen; i++)
    {
        int number = picturePos[i];
        loadMedia(pathPic[number], posx[number], posy[number], pictureSize, pictureSize);
    }
    if (checkOpen)
    {
        loadMedia("00.png", posx[picturePos[0]], posy[picturePos[0]], pictureSize, pictureSize);
        loadMedia("00.png", posx[picturePos[1]], posy[picturePos[1]], pictureSize, pictureSize);
    }
    SDL_RenderPresent( gRenderer );
}

int main(int argc, const char * argv[])
{
    srand(time(NULL));
    Game.pathPicture();
    for (int i = 0; i < 12; i++)
        Game.pathPictureRender[i] = "";
    Game.init();
    Game.clearScreen();
    Game.loadMedia("start.png", 0, 0, Game.SCREEN_WIDTH, Game.SCREEN_HEIGHT);
    SDL_RenderPresent( Game.gRenderer );
    Game.gMusic = Mix_LoadMUS( "mainMusic.wav" );
    Mix_PlayMusic( Game.gMusic, -1 );
    bool qu = false;
    while (!qu)
    while( SDL_PollEvent( &Game.event ) != 0 )
    {
        if( Game.event.type == SDL_QUIT )
        {
            qu = true;
        }
        else
        {
            if (!Game.played && (Game.event.type == SDL_MOUSEMOTION))
            {
                if ((Game.event.button.x>=290 && Game.event.button.x<=510) && (Game.event.button.y >= 220 && Game.event.button.y <= 320))
                {
                    Game.clearScreen();
                    Game.loadMedia("start1.png", 0, 0, Game.SCREEN_WIDTH, Game.SCREEN_HEIGHT);
                    SDL_RenderPresent( Game.gRenderer );
                }
                else
                {
                    Game.clearScreen();
                    Game.loadMedia("start.png", 0, 0, Game.SCREEN_WIDTH, Game.SCREEN_HEIGHT);
                    SDL_RenderPresent( Game.gRenderer );
                }
            }
            if (!Game.played  && (Game.event.type == SDL_MOUSEBUTTONDOWN) &&
                (Game.event.button.x>=290 && Game.event.button.x<=510) && (Game.event.button.y >= 220 && Game.event.button.y <= 320))
            {
                Game.loadSound("playSound.wav");
                Game.start = std::clock();
                Game.played = true;
                Game.renderPicture();
                qu = true;
            }
        }
    }
    while( !Game.quit )
    {
        Game.renderPicture();
        while( SDL_PollEvent( &Game.event ) != 0 )
        {
            if( Game.event.type == SDL_QUIT )
            {
                Game.quit = true;
            }
            else
            {
                if (Game.played && (Game.event.type == SDL_MOUSEBUTTONDOWN))
                {

                    Game.mouseX = Game.event.button.x;
                    Game.mouseY = Game.event.button.y;
                    Game.openPicture();
                }
            }
        }
    }
    Game.close();
	return 0;
}
