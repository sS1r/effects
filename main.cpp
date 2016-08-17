#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>
#include "effects.h"

extern int t_counter;

int main(int argc, char *argv[])
{
    try
    {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        SDL_Texture* alfa_test = NULL;
        SDL_Rect dst = {300, 300, 200, 200};

        SDL_Event e;

        bool q = false;
        bool f = false;
        bool copy1 = false;


        //Test glowballs
        effects::particle b1(300, 300);
        effects::particle b2(500, 600);
        effects::particle b3(600, 600);
        effects::particle b4(100, 200);

        //Test fires
        effects::fire fire1(60, 600, 400, 90, 200);
        effects::fire fire2(30, 300, 500, 30, 100);
        effects::fire fire3(100, 100, 200, 400, 60, effects::left, false);

        //Test wave particles
        effects::wave p1(300, 800, 800, 0, 70, 10, 12);
        effects::wave p2(350, 800, 800, 30, 40, 30, 17);
        effects::wave p3(350, 800, 800, 20, 100, 10, 15);
        effects::wave p4(300, 800, 800, 180);
        effects::wave p5(350, 800, 800, 70, 50, 50, 23);
        effects::wave p6(320, 800, 800, 60, 40, 20, 20);
        effects::wave p7(390, 800, 800, 100, 70, 15, 13);
        effects::wave p8(300, 800, 800, 190, 80, 30, 27);

        //Test physical particles
        effects::physical_particle P1(300, 200, 30);


        //Init SDL
        if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            throw SDL_GetError();
        }

        window = SDL_CreateWindow("CLUSTERFUCK 3000", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, 0);
        if(window == NULL)
        {
            throw SDL_GetError();
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(renderer == NULL)
        {
            throw SDL_GetError();
        }

        SDL_Surface* temp = IMG_Load("test.png");
        if(temp)
        {
            alfa_test = SDL_CreateTextureFromSurface(renderer, temp);
            SDL_FreeSurface(temp);
        }
        else
        {
            throw SDL_GetError();
        }

        effects::Fire_Init(renderer);

        b1.set_texture(effects::create_glow_ball(renderer, 500, 200, 50, 150));
        b2.set_texture(effects::create_glow_ball(renderer, 100, 50, 50, 40));
        b3.set_texture(effects::create_glow_ball(renderer, 50, 255, 255, 255));
        b4.set_texture(effects::create_glow_ball(renderer, 200, 240, 0, 180, 30));

        fire1.create_fire(renderer);
        fire2.create_fire(renderer, 10, 5, true);
        fire3.create_fire(renderer, 7, 15);

        p1.set_texture(effects::create_glow_ball(renderer, 30, 154, 130, 20));
        p2.set_texture(effects::create_glow_ball(renderer, 30, 230, 20, 100));
        p3.set_texture(effects::create_glow_ball(renderer, 20, 20, 250, 50));
        p4.set_texture(effects::create_glow_ball(renderer, 20, 200, 80, 154));
        p5.set_texture(effects::create_glow_ball(renderer, 20, 230, 140, 20));
        p6.set_texture(effects::create_glow_ball(renderer, 20, 60, 80, 240));
        p7.set_texture(effects::create_glow_ball(renderer, 10, 130, 250, 70));
        p8.set_texture(effects::create_glow_ball(renderer, 10, 150, 180, 230));

        P1.set_texture(effects::create_particle(renderer, 30, 10, 100, 255, 10));
        P1.speed_x = 5;

        std::cout << ">> Alustus valmis\n" << std::endl;

        while(!q)
        {
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT)
                {
                    q = true;
                }
                if(e.type == SDL_KEYDOWN)
                {
                    if(e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        q = true;
                    }
                    if(e.key.keysym.sym == SDLK_f)
                    {
                        f = true;
                    }

                    if(e.key.keysym.sym == SDLK_1)
                    {
                        copy1 = true;
                    }
                }

                if(e.type == SDL_KEYUP)
                {
                    if(e.key.keysym.sym == SDLK_1)
                    {
                        copy1 = false;
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, NULL);

            b1.draw(renderer);
            b2.draw(renderer);
            b3.draw(renderer);
            b4.draw(renderer);


            fire1.handle_fire();
            fire2.handle_fire();
            fire3.handle_fire();

            fire1.draw_fire(renderer);
            fire2.draw_fire(renderer);
            fire3.draw_fire(renderer);

            if(copy1)
            {
                fire2.draw_copy(renderer, 300, 300);
                fire2.draw_copy(renderer, 100, 500);
                fire2.draw_copy(renderer, 500, 500);
                fire2.draw_copy(renderer, 300, 700);

                fire2.draw_copy(renderer, 400, 400);
                fire2.draw_copy(renderer, 400, 600);
                fire2.draw_copy(renderer, 200, 600);
                fire2.draw_copy(renderer, 200, 400);
            }


            p1.handle_wave();
            p2.handle_wave();
            p3.handle_wave();
            p4.handle_wave();
            p5.handle_wave();
            p6.handle_wave();
            p7.handle_wave();
            p8.handle_wave();

            p1.draw(renderer);
            p2.draw(renderer);
            p3.draw(renderer);
            p4.draw(renderer);
            p5.draw(renderer);
            p6.draw(renderer);
            p7.draw(renderer);
            p8.draw(renderer);


            //"Force fields"
            if(P1.get_rect().x < 300)
            {
                P1.apply_force(70, -80);
            }
            if(P1.get_rect().x > 500)
            {
                P1.apply_force(-160, 50);
            }
            if(P1.get_rect().y < 300)
            {
                P1.apply_force(-120, 560);
            }
            if(P1.get_rect().y > 500)
            {
                P1.apply_force(100, -100);
            }

            P1.handle_particle();

            //"Walls"
            if(P1.get_rect().x <= 0)
            {
                P1.speed_x = -P1.speed_x*0.7;
                P1.move_rect(0, P1.get_rect().y);
            }
            if(P1.get_rect().x + P1.get_rect().w >= 800)
            {
                P1.speed_x = -P1.speed_x*0.7;
                P1.move_rect(800 - P1.get_rect().w, P1.get_rect().y);
            }
            if(P1.get_rect().y <= 0)
            {
                P1.speed_y = -P1.speed_y*0.7;
                P1.move_rect(P1.get_rect().x, 0);
            }
            if(P1.get_rect().y + P1.get_rect().h >= 800)
            {
                P1.speed_y = -P1.speed_y*0.7;
                P1.move_rect(P1.get_rect().x, 800 - P1.get_rect().h);
            }

            if(f)
            {
                if(effects::fade(alfa_test, -4) == 0)
                {
                    f = false;
                }
            }

            SDL_RenderCopy(renderer, alfa_test, NULL, &dst);

            P1.draw(renderer);

            SDL_Delay(25);

            SDL_RenderPresent(renderer);
        }

        //Deallocate memory

        effects::Fire_Destroy();

        b1.free_texture();
        b2.free_texture();
        b3.free_texture();
        b4.free_texture();


        p1.free_texture();
        p2.free_texture();
        p3.free_texture();
        p4.free_texture();
        p5.free_texture();
        p6.free_texture();
        p7.free_texture();
        p8.free_texture();

        P1.free_texture();

        SDL_DestroyTexture(alfa_test);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        std::cout << ">> Hiukkasia luotiin " << b1.p_counter << " kpl, ja tekstuureja " <<
        t_counter << " kpl." << std::endl;
    }

    //Error handling
    catch(const char* error)
    {
        std::cout << ">> Virhe: " << error << std::endl;
    }

    SDL_Quit();
    return 0;
}
