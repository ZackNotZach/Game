#ifndef WTEXTURE
#define WTEXTURE

#include<iostream>
#include<SDL.h>
#include<SDL_image.h>

using namespace std;

SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;

class Wtexture
{
public:

    //constructor, destructor, deallocator
    Wtexture();
    ~Wtexture();
    void free();

    //loads image
    bool loadfromfile(const string path);

    //renders texture at a point
    void render(int x, int y, SDL_Rect* clip=NULL);

    //gets image dimensions
    int getwidth();
    int getheight();

    SDL_Texture* texture;
    int width;
    int height;
};

//constructor
Wtexture::Wtexture()
{
    width=0;
    height=0;

    texture=NULL;
}

//destructor
Wtexture::~Wtexture()
{
    free();
}

//deallocator
void Wtexture::free()
{
    if(texture!=NULL)
    {
        SDL_DestroyTexture(texture);
        texture=NULL;
        width=0;
        height=0;
    }
}

bool Wtexture::loadfromfile(const string path)
{
    //remove preexisting texture
    free();
    SDL_Texture* newtexture=NULL;

    SDL_Surface* loadedsurface=IMG_Load(path.c_str());
    if(loadedsurface==NULL)
    {
        cout << "could not load texture located at " << path << "." << endl;
    }
    else
    {
        newtexture=SDL_CreateTextureFromSurface(renderer,loadedsurface);
        if(newtexture==NULL)
        {
            cout << "could not create texture from surface at " << path << "." << endl;
        }
        else
        {
            //image dimensions
            width=loadedsurface->w;
            height=loadedsurface->h;
        }
    }

    SDL_FreeSurface(loadedsurface);

    texture=newtexture;
    return texture!=NULL;
}

void Wtexture::render(int x, int y, SDL_Rect* clip)
{
   //render at a specific location
   SDL_Rect quad={x,y,width,height}; //quad is the destination rectangle--where you want to render the image

   if(clip!=NULL) //if you don't want to render the whole spritesheet
   {
       quad.w=clip->w;
       quad.h=clip->h;
   }

   SDL_RenderCopy(renderer,texture,clip,&quad);
}

int Wtexture::getheight()
{
    return height;
}

int Wtexture::getwidth()
{
    return width;
}

#endif // WTEXTURE
