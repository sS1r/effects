#include <stdlib.h>
#include <time.h>

#include "effects.h"

const int FIRE_PARTICLES = 50;
static SDL_Texture* fire_texture_cache[FIRE_PARTICLES];
static SDL_Texture* fire_texture_cache_small[FIRE_PARTICLES];

//Creates *FIRE_PARTICLES* fire particle textures
//Increase the number for more variety, but the function becomes slower
void effects::Fire_Init(SDL_Renderer* renderer)
{
    srand(time(NULL) + SDL_GetTicks());

    for(int i = 0;i < FIRE_PARTICLES;i++)
    {
        int radius = 40 + rand()%10;
        int red = 200 + rand()%55;
        int green = 20 + rand()%60;
        int blue = rand()%40;

        fire_texture_cache[i] = effects::create_glow_ball(renderer, radius, red, green, blue, 3.0);
    }

    for(int i = 0;i < FIRE_PARTICLES;i++)
    {
        int radius = 20 + rand()%10;
        int red = 200 + rand()%55;
        int green = 20 + rand()%60;
        int blue = rand()%40;

        fire_texture_cache_small[i] = effects::create_glow_ball(renderer, radius, red, green, blue, 3.0);
    }
}

//Destroy fire particles
//Call this at the end of program if you call Fire_Init()
void effects::Fire_Destroy()
{
    for(int i = 0;i < FIRE_PARTICLES;i++)
    {
        if(fire_texture_cache[i])
        {
            SDL_DestroyTexture(fire_texture_cache[i]);
        }
    }

    for(int i = 0;i < FIRE_PARTICLES;i++)
    {
        if(fire_texture_cache_small[i])
        {
            SDL_DestroyTexture(fire_texture_cache_small[i]);
        }
    }
}


void effects::fire::create_fire(SDL_Renderer* renderer, int base_freq, int base_speed, bool small)
{
    for(int i = 0;i < quantity;i++)
    {
        int x = 0, y = 0, height = 0, amp = 0;

        if(dir == up or dir == down)
        {
            x = center.x - rect.w / 14 + rand()%(rect.w / 7);
            y = rect.y + (dir == up ? rect.h : 0);
            height = rect.h / 5 + rand()%(rect.h * 4 / 5);
            amp = rect.w * 5 / 28 + rand()%(rect.w / 4);
        }
        else if(dir == left or dir == right)
        {
            x = rect.x + (dir == left ? rect.w : 0);
            y = center.y - rect.h / 14 + rand()%(rect.h / 7);
            height = rect.w / 5 + rand()%(rect.w * 4 / 5);
            amp = rect.h * 5 / 28 + rand()%(rect.h / 4);
        }

        int phase = rand()%360;
        int freq = base_freq + rand()%(base_freq*2);
        int speed = base_speed + rand()%base_speed;

        wave particle(x, y, height, phase, amp, freq, speed, dir);
        particles.push_back(particle);
    }

    for(int i = 0;i < quantity;i++)
    {
        if(!small)
        {
            particles[i].set_texture(fire_texture_cache[rand()%(FIRE_PARTICLES - 1)]);
        }
        else
        {
            particles[i].set_texture(fire_texture_cache_small[rand()%(FIRE_PARTICLES - 1)]);
        }
    }

    middle.set_texture(effects::create_glow_ball(renderer, quantity*3, 230, 70, 20, 3.5));
}

//Handles "fire logic". Call this before each frame refresh
void effects::fire::handle_fire()
{
    for(unsigned int i = 0;i < particles.size();i++)
    {
        particles[i].handle_wave();
    }
}

//Moves the fire's location to (x, y)
void effects::fire::move(int x, int y)
{
    int diff_x = x - rect.x;
    int diff_y = y - rect.y;

    rect.x = x;
    rect.y = y;
    center.x += diff_x;
    center.y += diff_y;

    middle.move(center.x, center.y);

    for(unsigned int i = 0;i < particles.size();i++)
    {
        particles[i].move_start(particles[i].get_start().x + diff_x, particles[i].get_start().y + diff_y);
    }
}

//Renderes the fire
void effects::fire::draw_fire(SDL_Renderer* renderer)
{
    if(mid)
    {
        middle.draw(renderer);
    }
    for(unsigned int i = 0;i < particles.size();i++)
    {
        particles[i].draw(renderer);
    }
}

//Draws a copy of the fire to location (x, y)
void effects::fire::draw_copy(SDL_Renderer* renderer, int x, int y)
{
    SDL_Rect orig_rect = rect;

    move(x, y);
    draw_fire(renderer);

    move(orig_rect.x, orig_rect.y);
}
