#include <math.h>

class RgbLight
{
    private:
        byte pin_r;
        byte pin_g;
        byte pin_b;

        byte r_ = 255;
        byte g_ = 255;
        byte b_ = 255;

        bool on_ = false;

        void update()
        {
            if(on_)
            {
                analogWrite(pin_r, r_);
                analogWrite(pin_g, g_);
                analogWrite(pin_b, b_);
            }
            else
            {
                analogWrite(pin_r, 0);
                analogWrite(pin_g, 0);
                analogWrite(pin_b, 0);
            }
            
        }

    public:
        RgbLight(byte pin_R, byte pin_G, byte pin_B)
            :pin_r(pin_R), pin_g(pin_G), pin_b(pin_B)
        {
            pinMode(pin_r, OUTPUT);
            pinMode(pin_g, OUTPUT);
            pinMode(pin_b, OUTPUT);

        }
        
        inline void off()
        {
            on_ = false;
            update();
        }

        inline void on()
        {
           on_ = true;
           update(); 
        }

        bool getState(byte* color = nullptr)
        {
            if(color != nullptr)
            {
                color[0] = r_;
                color[1] = g_;
                color[2] = b_;
            }
            return on_;
        }

        void setColor(byte r, byte g, byte b, bool turn_on = true)
        {
           r_ = r;
           g_ = g;
           b_ = b;
           if(turn_on) on();
        }

        /* set color in HSV mode
            h,s,v - floats from 0.0 to 1.0
            limit_mixed_brightness - limits the value for mixed colors 
                (so that eg. cyan isn't brighter than blue)
            turn_on - update color/turn on the light immediately
        */
        void setHsv(float h, float s, float v, bool limit_mixed_brightness = false, bool turn_on = true)
        {
            // h,s,v need to be from [0.0, 1.0]
            h = (h > 1.0) ? 1.0 : h;
            h = (h < 0.0) ? 0.0 : h;
            s = (s > 1.0) ? 1.0 : s;
            s = (s < 0.0) ? 0.0 : s;
            v = (v > 1.0) ? 1.0 : v;
            v = (v < 0.0) ? 0.0 : v;
            
            // Algorithm from https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB_alternative
            float kR = fmod(5.0 + h*6, 6);
            float kG = fmod(3.0 + h*6, 6);
            float kB = fmod(1.0 + h*6, 6);

            float rf = v - v*s*max(0, min(1, min(kR, 4-kR)));
            float gf = v - v*s*max(0, min(1, min(kG, 4-kG)));
            float bf = v - v*s*max(0, min(1, min(kB, 4-kB)));

            if(limit_mixed_brightness && (rf+gf+bf)>v)
            {
                rf = v*rf/(rf+gf+bf);
                gf = v*gf/(rf+gf+bf);
                bf = v*bf/(rf+gf+bf);
            }

            setColor(rf*255, gf*255, bf*255);
        }

};
