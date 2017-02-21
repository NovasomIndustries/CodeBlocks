#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#define WIDTH 1280
#define HEIGHT 800
#define IMG_PATH1 "exit1.png"
#define IMG_PATH2 "exit2.png"
#define IMG_PATH3 "exit3.png"
#define IMG_PATH4 "exit4.png"
#define IMG_PATH5 "exit5.png"
#define IMG_PATH6 "exit6.png"

SDL_Texture *img1 = NULL;
SDL_Texture *img2 = NULL;
SDL_Texture *img3 = NULL;
SDL_Texture *img4 = NULL;
SDL_Texture *img5 = NULL;
SDL_Texture *img6 = NULL;

int main (int argc, char *argv[])
{
SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;
int img_ptr = 0 , events = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return 1;

	win = SDL_CreateWindow("Image Loading", 0, 0, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	img1 = IMG_LoadTexture(renderer, IMG_PATH1);
	img2 = IMG_LoadTexture(renderer, IMG_PATH2);
	img3 = IMG_LoadTexture(renderer, IMG_PATH3);
	img4 = IMG_LoadTexture(renderer, IMG_PATH4);
	img5 = IMG_LoadTexture(renderer, IMG_PATH5);
	img6 = IMG_LoadTexture(renderer, IMG_PATH6);

    SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, img5, NULL, NULL);
    SDL_RenderPresent(renderer);
	while (1)
	{
	/*
		SDL_Event e;
        events = 0;
		if ( SDL_PollEvent(&e) )
		{
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
            else if (e.type == SDL_MOUSEBUTTONDOWN )
            {
                events++;
                printf("Event %d\n",e.type);
            }
		}
        SDL_Delay( 1000 );
        events++;
        SDL_RenderCopy(renderer, img6, NULL, NULL);

        if ( events != 0 )
        {
            SDL_RenderClear(renderer);
            switch ( img_ptr )
            {
                case 0 : SDL_RenderCopy(renderer, img5, NULL, NULL); break;
                case 1 : SDL_RenderCopy(renderer, img5, NULL, NULL); break;
                case 2 : SDL_RenderCopy(renderer, img5, NULL, NULL); break;
                case 3 : SDL_RenderCopy(renderer, img5, NULL, NULL); break;
            }
            SDL_RenderPresent(renderer);
            img_ptr++;
            img_ptr &= 3;
            SDL_Delay( 10 );
            events = 0;
        }
        */
	}
	SDL_DestroyTexture(img1);
	SDL_DestroyTexture(img2);
	SDL_DestroyTexture(img3);
	SDL_DestroyTexture(img4);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	return 0;
}
