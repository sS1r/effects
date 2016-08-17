//Hohtopallo on monikäyttöinen piirtofunktio, jota voidaan käyttää vaikutusalueen mallintamiseen
//Pikselien "intensiteetti" määräytyy funktion y = 1/x^2 mukaan, jolloin saadaan samanlainen
//intensiteetin muutos, kuin esim. sähkökentällä tai äänellä.

//Sisältää:


//-ETÄISYYSFUNKTIO
//-RADIAANIEN JA ASTEIDEN VÄLISET MUUTOSFUNKTIOT
//-KANTALUOKKA HIUKKASILLE, JOSTA AALTO, FYYSINEN HIUKKANEN JA HOHTOPALLO PERIYTYVÄT
//-"AALTOLUOKKA"
//-TULI-LUOKKA TULIEFEKTILLE, KOOSTUU MONESTA "AALLOSTA"
//-HIUKKASLUOKKA
//-SUORA TEXTUURIN LUONTIFUNKTIO HOHTOPALLOLLE
//-YKSINKERTAISEN PYÖREÄN HIUKKASEN  TEKSTUURIN LUONTIFUNKTIO
//-HÄIVYTYSFUNKTIO


#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>

#include "effects.h"

int t_counter = 0; //texture counter

//Etäisyydenmittaukseen apufunktio
float effects::get_distance(SDL_Point p1, SDL_Point p2)
{
    int dist_x = p2.x - p1.x;
    int dist_y = p2.y - p1.y;

    float distance = SDL_sqrt(dist_x*dist_x + dist_y*dist_y);

    return distance;
}

double effects::deg2rad(float deg) //muuta asteet radiaaneiksi
{
    return (deg/360)*2*3.14159265358979;
}

double effects::rad2deg(float rad) //muuta radiaanit asteiksi
{
    return rad/(2*3.14159265358979)*360;
}

//Creates a glow ball texture
SDL_Texture* effects::create_glow_ball(SDL_Renderer* renderer, int radius, int r, int g, int b, int dim)
{
    float relative_dist = 0;
    float distance = 0;
    int alpha = 0;

    SDL_Point p = {0, 0};
    SDL_Point p_mid = {radius, radius};
    SDL_Texture* texture = NULL;

    if(dim < 0)
    {
        dim = 4.0;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, radius*2, radius*2);
    if(!texture)
    {
        throw SDL_GetError();
    }

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    //HUOM!!! JOS RENDERBLENDMODE ON PÄÄLLÄ, HOHTOPALLON LAATU HEIKKENEE

    for(int y = 0; y < 2*radius; y++)
    {
        for(int x = 0; x < 2*radius; x++)
        {
            p.x = x;
            p.y = y;

            distance = get_distance(p, p_mid);

            if(distance <= radius)
            {
                relative_dist = distance/radius*dim + 0.5;
                alpha = 255/(relative_dist*relative_dist)/4;
            }
            else
            {
                alpha = 0;
            }

            SDL_SetRenderDrawColor(renderer, r, g, b, alpha);

            if(SDL_RenderDrawPoint(renderer, x, y) != 0)
            {
                throw SDL_GetError();
            }
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    t_counter++;

    return texture;
}

//Creates a simple round, solid particle texture
SDL_Texture* effects::create_particle(SDL_Renderer* renderer, int r, int g, int b, int a, int radius)
{
    SDL_Texture* texture = NULL;

    SDL_Point p = {0, 0};
    SDL_Point p_mid = {radius, radius};

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, radius*2, radius*2);

    if(!texture)
    {
        throw SDL_GetError();
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    for(int y = 0; y < 2*radius; y++)
    {
        for(int x = 0; x < 2*radius; x++)
        {
            p.x = x;
            p.y = y;

            if(get_distance(p, p_mid) <= radius)
            {
                if(SDL_RenderDrawPoint(renderer, x, y) != 0)
                {
                    throw SDL_GetError();
                }
            }
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    t_counter++;

    return texture;
}

int effects::fade(SDL_Texture* texture, int alpha)
{
    SDL_BlendMode blend;
    Uint8 alpha_now = 0;

    //Get the current alpha and blend mode
    SDL_GetTextureBlendMode(texture, &blend);
    SDL_GetTextureAlphaMod(texture, &alpha_now);

    //Set the correct blend mode if necessary
    if(blend != SDL_BLENDMODE_BLEND)
    {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    }

    //Set the new alpha value and limit it if necessary
    alpha += alpha_now;

    if(alpha < 0)
    {
        alpha = 0;
    }

    if(alpha > 255)
    {
        alpha = 255;
    }

    if(SDL_SetTextureAlphaMod(texture, alpha) != 0)
    {
        throw SDL_GetError();
    }

    //Return the new alpha
    return alpha;
}

/* *****************************

**** PARTICLE BASE CLASS METHODS ****

***************************** */

int effects::particle::p_counter = 0;

void effects::particle::set_texture(SDL_Texture* _texture)
{
    int w, h;

    if(_texture)
    {
        texture = _texture;

        SDL_QueryTexture(texture, 0, 0, &w, &h);

        rect.x = get_middle().x - w / 2;
        rect.y = get_middle().y - h / 2;
        rect.w = w;
        rect.h = h;
    }
}

void effects::particle::free_texture()
{
    if(texture)
    {
        SDL_DestroyTexture(texture);
    }
}

void effects::particle::draw(SDL_Renderer* renderer)
{
    if(SDL_RenderCopy(renderer, texture, NULL, &rect) != 0)
    {
        throw SDL_GetError();
    }
}

void effects::particle::move(int mid_x, int mid_y)
{
    middle_point.x = mid_x;
    middle_point.y = mid_y;
    rect.x = mid_x - rect.w / 2;
    rect.y = mid_y - rect.h / 2;
}

void effects::particle::move_rect(int x, int y)
{
    rect.x = x;
    rect.y = y;
    middle_point.x = x + rect.w / 2;
    middle_point.y = y + rect.h / 2;
}


SDL_Rect effects::particle::get_rect()
{
    return rect;
}

SDL_Point effects::particle::get_middle()
{
    return middle_point;
}

/* *****************************

**** WAVE CLASS METHODS ****

***************************** */

SDL_Point effects::wave::get_start()
{
    SDL_Point p = {start_x, start_y};
    return p;
}

void effects::wave::move_start(int x, int y)
{
    int diff_x = x - start_x;
    int diff_y = y - start_y;

    move(get_middle().x + diff_x, get_middle().y + diff_y);

    start_x = x;
    start_y = y;
}

void effects::wave::set_amp(int _amp)
{
    amp = _amp;
}
void effects::wave::set_freq(int _freq)
{
    freq = _freq;
}
void effects::wave::set_spd(int _spd)
{
    spd = _spd;
}
void effects::wave::set_phase(int _phase)
{
    phase = _phase;
}

void effects::wave::handle_wave()
{
    int move_x = 0;
    int move_y = 0;

    if(dir == up or dir == down)
    {
        move_x = start_x + SDL_sin(deg2rad(angle + phase))*amp;
        move_y = (dir == up ? get_middle().y - spd : get_middle().y + spd);

        if((dir == up ? start_y - get_middle().y : get_middle().y - start_y) >= height)
        {
            move_x = start_x;
            move_y = start_y;
            angle = 0;
        }
    }
    else if(dir == left or dir == right)
    {
        move_y = start_y + SDL_sin(deg2rad(angle + phase))*amp;
        move_x = (dir == left ? get_middle().x - spd : get_middle().x + spd);

        if((dir == left ? start_x - get_middle().x : get_middle().x - start_x) >= height)
        {
            move_x = start_x;
            move_y = start_y;
            angle = 0;
        }
    }

    move(move_x, move_y);
    angle += freq;
}

/* *****************************

**** PHYSICAL PARTICLE CLASS METHODS ****

***************************** */


void effects::physical_particle::handle_particle()
{
    speed_x += acceleration_x;
    speed_y += acceleration_y;

    rect.x += speed_x;
    rect.y += speed_y;
}

void effects::physical_particle::set_acceleration(float _acc_x, float _acc_y)
{
    acceleration_x = _acc_x;
    acceleration_y = _acc_y;
}

void effects::physical_particle::apply_force(float force_x, float force_y)
{
    acceleration_x = force_x / mass;
    acceleration_y = force_y / mass;
}

int effects::physical_particle::get_mass()
{
    return mass;
}
