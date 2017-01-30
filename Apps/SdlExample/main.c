#include <SDL.h>
#include <stdio.h>
/*
Start app with --dfb:system=FBDev options
This example assumes the reference file system is in SdlBase.
Change it accordingly to your setup
*/
/*
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		else
		{
			screenSurface = SDL_GetWindowSurface( window );
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0x00, 0x00 ) );
			SDL_UpdateWindowSurface( window );
			SDL_Delay( 20000 );
		}
	}
	SDL_DestroyWindow( window );
	SDL_Quit();
	return 0;
}
*/

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#define WIDTH 800
#define HEIGHT 480
#define IMG_PATH1 "exit1.png"
#define IMG_PATH2 "exit2.png"
#define IMG_PATH3 "exit3.png"
#define IMG_PATH4 "exit4.png"

SDL_Texture *img1 = NULL;
SDL_Texture *img2 = NULL;
SDL_Texture *img3 = NULL;
SDL_Texture *img4 = NULL;

int main (int argc, char *argv[]) {

	// variable declarations
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	int w, h; // texture width & height
	int img_ptr = 0 , events = 0;

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return 1;

	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("Image Loading", 0, 0, WIDTH, HEIGHT, 0);
    //win = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,  WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	// load our image
	img1 = IMG_LoadTexture(renderer, IMG_PATH1);
	img2 = IMG_LoadTexture(renderer, IMG_PATH2);
	img3 = IMG_LoadTexture(renderer, IMG_PATH3);
	img4 = IMG_LoadTexture(renderer, IMG_PATH4);

	//SDL_QueryTexture(img, NULL, NULL, &w, &h); // get the width and height of the texture
	// put the location where we want the texture to be drawn into a rectangle
	// I'm also scaling the texture 2x simply by setting the width and height
	//SDL_Rect texr; texr.x = WIDTH/4; texr.y = HEIGHT/4; texr.w = w*4; texr.h = h*4;
	//SDL_Rect texr; texr.x = WIDTH; texr.y = HEIGHT; texr.w = w; texr.h = h;

	// main loop
	while (1) {

		// event handling
		/*
		SDL_Event e;
        events = 0;
		if ( SDL_PollEvent(&e) ) {

			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
            events++;
            SDL_Delay( 500 );
		}

        if ( events != 0 )
        {
        */
            SDL_RenderClear(renderer);
            // copy the texture to the rendering context
            //SDL_RenderCopy(renderer, img, NULL, &texr);
            switch ( img_ptr )
            {
                case 0 : SDL_RenderCopy(renderer, img1, NULL, NULL); break;
                case 1 : SDL_RenderCopy(renderer, img2, NULL, NULL); break;
                case 2 : SDL_RenderCopy(renderer, img3, NULL, NULL); break;
                case 3 : SDL_RenderCopy(renderer, img4, NULL, NULL); break;
            }
            // flip the backbuffer
            // this means that everything that we prepared behind the screens is actually shown
            SDL_RenderPresent(renderer);
            img_ptr++;
            img_ptr &= 3;
            SDL_Delay( 1000 );
            /*
        }
		// clear the screen
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
