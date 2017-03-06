#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#define WIDTH 800
#define HEIGHT 480
#define IMG_PATH0 "ks00.png"
#define IMG_PATH1 "ks01.png"
#define IMG_PATH2 "ks02.png"
#define IMG_PATH3 "ks03.png"
#define IMG_PATH4 "ks04.png"
#define IMG_PATH5 "ks05.png"
#define IMG_PATH6 "ks06.png"

SDL_Texture *img0 = NULL;
SDL_Texture *img1 = NULL;
SDL_Texture *img2 = NULL;
SDL_Texture *img3 = NULL;
SDL_Texture *img4 = NULL;
SDL_Texture *img5 = NULL;
SDL_Texture *img6 = NULL;

//#define AUTO_LOOP

int main (int argc, char *argv[])
{
SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;
int img_ptr = 0 , events = 0;
FILE *fp;
char    video_string[32];
int  width,height;
int auto_loop = 0, loop_delay=1;
SDL_Event e;

    if (argc >= 3)
    {
        printf("%s\n",argv[1]);
        if ( strcmp("loop",argv[1]) == 0 )
        {
            auto_loop = 1;
            loop_delay = atoi(argv[2]);
            printf("Loop mode\n");
        }
    }
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;
    fp = fopen("/sys/class/graphics/fb0/mode","r");
    if ( !fp )
        return -1;
    fread(video_string,1,32,fp);
    fclose(fp);
    if (strstr(video_string,"800x480"))
    {
        width = 800;
        height = 480;
    }
    if (strstr(video_string,"800x600"))
    {
        width = 800;
        height = 600;
    }
    if (strstr(video_string,"1280x800"))
    {
        width = 1280;
        height = 800;
    }
	win = SDL_CreateWindow("Image Loading", 0, 0, width, height, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	img0 = IMG_LoadTexture(renderer, IMG_PATH0);
	img1 = IMG_LoadTexture(renderer, IMG_PATH1);
	img2 = IMG_LoadTexture(renderer, IMG_PATH2);
	img3 = IMG_LoadTexture(renderer, IMG_PATH3);
	img4 = IMG_LoadTexture(renderer, IMG_PATH4);
	img5 = IMG_LoadTexture(renderer, IMG_PATH5);
	img6 = IMG_LoadTexture(renderer, IMG_PATH6);

    SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, img6, NULL, NULL);
    SDL_RenderPresent(renderer);

	while (1)
	{
        if ( auto_loop == 1)
        {
            SDL_Delay( loop_delay * 1000 );
            SDL_RenderClear(renderer);
            switch ( img_ptr )
            {
                case 0 : SDL_RenderCopy(renderer, img1, NULL, NULL); break;
                case 1 : SDL_RenderCopy(renderer, img2, NULL, NULL); break;
                case 2 : SDL_RenderCopy(renderer, img3, NULL, NULL); break;
                case 3 : SDL_RenderCopy(renderer, img4, NULL, NULL); break;
                case 4 : SDL_RenderCopy(renderer, img5, NULL, NULL); break;
                case 5 : SDL_RenderCopy(renderer, img6, NULL, NULL); break;
            }
            SDL_RenderPresent(renderer);
            img_ptr++;
            if ( img_ptr > 5 )
                img_ptr = 0;
        }
        else
        {
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

            if ( events != 0 )
            {
                SDL_RenderClear(renderer);
                switch ( img_ptr )
                {
                    case 0 : SDL_RenderCopy(renderer, img1, NULL, NULL); break;
                    case 1 : SDL_RenderCopy(renderer, img2, NULL, NULL); break;
                    case 2 : SDL_RenderCopy(renderer, img3, NULL, NULL); break;
                    case 3 : SDL_RenderCopy(renderer, img4, NULL, NULL); break;
                    case 4 : SDL_RenderCopy(renderer, img5, NULL, NULL); break;
                    case 5 : SDL_RenderCopy(renderer, img6, NULL, NULL); break;
                }
                SDL_RenderPresent(renderer);
                img_ptr++;
                if ( img_ptr > 5 )
                    img_ptr = 0;
                SDL_Delay( 10 );
                events = 0;
            }
        }
	}
	SDL_DestroyTexture(img1);
	SDL_DestroyTexture(img2);
	SDL_DestroyTexture(img3);
	SDL_DestroyTexture(img4);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	return 0;
}
