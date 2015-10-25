#include"textureWrapper.h"
using namespace std;

const int SCR_WIDTH=640;
const int SCR_HEIGHT=360;

bool init();
bool loadMedia();
void close();

SDL_Rect idlesprites[3];
SDL_Rect walkingsprites[2];
SDL_Rect runesprites[5];
Wtexture mc_spritesheet,rune_spritesheet;

///////////////////////////////////////////////////////////////////////////////

bool init()
{
    //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
        cout << "could not initialize" << endl;
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "2" ) )
		{
			cout << "linear texture filtering not enabled" << endl;
		}

		//Create window
		window = SDL_CreateWindow( "Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_SHOWN);
		if( window == NULL )
		{
			cout << "window could not be created" << endl;
			success = false;
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( renderer == NULL )
			{
				cout << "could not create renderer or vsync is screwed up" << endl;
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					cout << "SDL_Image could not initialize" << endl;
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
    bool success=true;

    if(!mc_spritesheet.loadfromfile("source/textures/spritesheets/mc/spritesheet.png"))
    {
        cout << "couldn't load mc spritesheet" << endl;
    }
    else
    {
        //creates the space to render the texture
        //THIS IS NOT WHERE YOU RENDER THE SPRITE
        //THIS IS WHERE YOU SELECT WHAT AREA OF THE SHEET TO RENDER
        //JESUS CHRIST YOU'RE BAD

        //inhale
        idlesprites[0].x=0;     //x position of the spritesheet
        idlesprites[0].y=0;     //y position of the spritesheet
        idlesprites[0].w=64;    //width of the sprite
        idlesprites[0].h=64;    //height of the sprite

        //exhale
        idlesprites[1].x=64;
        idlesprites[1].y=0;
        idlesprites[1].w=64;
        idlesprites[1].h=64;

        //left leg
        walkingsprites[0].x=0;
        walkingsprites[0].y=64;
        walkingsprites[0].w=64;
        walkingsprites[0].h=64;

        //right leg
        walkingsprites[1].x=64;
        walkingsprites[1].y=64;
        walkingsprites[1].w=64;
        walkingsprites[1].h=64;
    }

    if(!rune_spritesheet.loadfromfile("source/textures/spritesheets/runes/spritesheet.png"))
    {
        cout << "couldn't load rune spritesheet" << endl;
    }
    else
    {
        runesprites[0].x=0;
        runesprites[0].y=0;
        runesprites[0].w=32;
        runesprites[0].h=32;
    }

    return success;
}

void close()
{
    //clear textures
    mc_spritesheet.free();

	//Destroy window
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		cout << "failed to initialize" << endl;
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			cout << "failed to load media" << endl;
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			int framebuffer=0;
			int spr_framebuffer=0;
			int mc_x_position=0; //the x-position of the character

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

				}

				//Clear screen
				SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
				SDL_RenderClear(renderer);

				const Uint8* current_key=SDL_GetKeyboardState(NULL);

                //walking to the right
				if(current_key[SDL_SCANCODE_RIGHT])
                {
                    SDL_Rect* current_clip=&walkingsprites[framebuffer/10];
                    mc_spritesheet.render(SCR_WIDTH/2,300,current_clip);

                    framebuffer++;
                    mc_x_position=mc_x_position+2;

                    if(framebuffer/10>=2)
                        framebuffer=0;
                }
                else //idle animation
                {
                    SDL_Rect* current_clip=&idlesprites[framebuffer/30];
                    mc_spritesheet.render(SCR_WIDTH/2,300,current_clip);

                    framebuffer++;

                    if(framebuffer/30>=2)
                        framebuffer=0;
                }

                if(mc_x_position>=40)
                {
                    if(spr_framebuffer<=40)
                    {
                        rune_spritesheet.render(SCR_WIDTH-mc_x_position,SCR_HEIGHT/2-100,&runesprites[0]);
                        spr_framebuffer++;
                    }
                    else if(spr_framebuffer>40)
                    {
                        rune_spritesheet.render(SCR_WIDTH-mc_x_position,SCR_HEIGHT/2-90,&runesprites[0]);
                        spr_framebuffer--;
                    }
                }

                SDL_RenderPresent(renderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
