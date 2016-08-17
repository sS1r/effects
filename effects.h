#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED

#include <vector>

#include <SDL.h>



namespace effects
{
    enum direction
    {
        up, down, left, right
    };

    //Etäisyydenmittaukseen apufunktio
    float get_distance(SDL_Point p1, SDL_Point p2);

    //muuta asteet radiaaneiksi
    double deg2rad(float deg);

    //muuta radiaanit asteiksi
    double rad2deg(float rad);

    //Creates a glow ball surface
    SDL_Texture* create_glow_ball(SDL_Renderer* renderer, int radius, int r = 255, int g = 255, int b = 255, int dim = 4.0);

    //Creates a simple round particle without glow
    SDL_Texture* create_particle(SDL_Renderer* renderer, int r, int g, int b, int a, int radius);

    //Fades texture
    //Returns the current alpha
    int fade(SDL_Texture* texture, int alpha);

    //Creates the fire particle textures
    void Fire_Init(SDL_Renderer* renderer);

    //Destroys the fire texture cache
    void Fire_Destroy();

    //Particle base class
    class particle
    {
        protected:

            SDL_Texture* texture;
            SDL_Point middle_point;
            SDL_Rect rect;

        public:

            static int p_counter; //particle counter

            void set_texture(SDL_Texture* texture);
            void free_texture();

            void draw(SDL_Renderer* renderer);

            void move(int mid_x, int mid_y); //moves from the middle
            void move_rect(int x, int y); //moves from the top left corner of the texture

            SDL_Rect get_rect();
            SDL_Point get_middle();

            particle(int mid_x, int mid_y): texture(NULL)
            {
                middle_point.x = mid_x;
                middle_point.y = mid_y;

                rect.x = mid_x;
                rect.y = mid_y;
                rect.w = 0;
                rect.h = 0;

                p_counter++;
            }

            ~particle()
            {

            }
    };

    class wave: public particle
    {
        private:

            int phase;
            int amp;
            int freq;
            int spd;

            int start_x;
            int start_y;

            direction dir;

            int height;

            int angle;

        public:

            SDL_Point get_start();

            void move_start(int x, int y);

            void handle_wave();

            void set_amp(int _amp);
            void set_freq(int _freq);
            void set_spd(int _spd);
            void set_phase(int _phase);

            wave(int mid_x, int mid_y, int _height, int _phase = 0, int _amp = 70, int _freq = 20, int _spd = 10, direction _dir = up):
                particle(mid_x, mid_y), phase(_phase), amp(_amp), freq(_freq), spd(_spd),
                start_x(mid_x), start_y(mid_y), dir(_dir), height(_height)
            {

            }
            ~wave()
            {

            }
    };

    class physical_particle: public particle
    {
        private:

            int mass;

            float acceleration_x, acceleration_y; //pixels per refresh^2

        public:

            float speed_x, speed_y; //pixels per refresh

            void handle_particle();

            void set_acceleration(float _acc_x, float _acc_y);

            void apply_force(float force_x, float force_y);

            int get_mass();

            physical_particle(int mid_x, int mid_y, int _mass): particle(mid_x, mid_y), mass(_mass),
                    acceleration_x(0), acceleration_y(0), speed_x(0), speed_y(0)
            {

            }

            ~physical_particle()
            {

            }
    };



    class fire
    {
        private:
            std::vector<wave> particles;

            particle middle;

            direction dir;

            int quantity;

            bool mid;

        public:

            SDL_Point center; //The middle of the fire source (same location as the middle glow ball)

            SDL_Rect rect; //Rect limiting the fire

            //Creates the fire effect
            //If the fire is small, call small = true for smaller fire particles
            void create_fire(SDL_Renderer* renderer, int base_freq = 10, int base_speed = 5, bool small = false);

            void handle_fire();

            void move(int mid_x, int mid_y);

            void draw_fire(SDL_Renderer* renderer);

            //Draws a copy of the fire to (x,y)
            void draw_copy(SDL_Renderer* renderer, int x, int y);


            fire(int _q, int x, int y, int w, int h, direction _dir = up, bool _mid = true):
                middle(0, 0), dir(_dir), quantity(_q), mid(_mid)
            {
                rect.x = x;
                rect.y = y;
                rect.w = w;
                rect.h = h;

                switch(_dir)
                {
                    case up:
                        center.x = x;
                        center.y = y + h - h/5;
                        break;
                    case down:
                        center.x = x;
                        center.y = y + h/5;
                        break;
                    case left:
                        center.x = x + w - w/5;
                        center.y = y;
                        break;
                    case right:
                        center.x = x + w/5;
                        center.y = y;
                        break;
                    default:
                        break;
                }

                middle.move(center.x, center.y);
            }

            ~fire()
            {
                middle.free_texture();
            }
    };
}

#endif //EFFECTS_H_INCLUDED
