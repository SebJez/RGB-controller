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

    void setColor(byte r, byte g, byte b)
    {
       r_ = r;
       g_ = g;
       b_ = b;
       on();
    }

    /* set color in HSV mode
      h,s,v - floats from 0.0 to 1.0
      normalize_brightness - limits the value for mixed colors 
        (so that eg. cyan isn't brighter than blue)
    */
    void setHsv(float h, float s, float v, bool normalize_brightness = false)
    {
      // h,s,v need to be from [0.0, 1.0]
      //wraparound h
      h = modff(h,nullptr);
      if(h < 0) h+=1.0;

      //limit s and v
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

      if(normalize_brightness && (rf+gf+bf)>v)
      {
        rf = v*rf/(rf+gf+bf);
        gf = v*gf/(rf+gf+bf);
        bf = v*bf/(rf+gf+bf);
      }

      setColor(rf*255, gf*255, bf*255);
    }

    void rainbow(float brightness, float time_ms)
    {
      for(int h=0; h<100; h+=1) 
      {
        setHsv(h/100.0,1,0.33*brightness,true);
        delay(time_ms/100.0); 
      }
    }

    void transitionHsv(float h1, float s1, float v1,
                float h2, float s2, float v2,
                float time_ms, bool normalize_brightness = false)
    {
      setHsv(h1,s1,v1,normalize_brightness);
      
      //find shorter way around the color circle
      float deltah = h2 - h1;
      if(deltah > 0.5) deltah -= 1.0;
      else if(deltah < -0.5) deltah += 1.0;

      float dh =  deltah / 100.0;
      float ds = (s2-s1) / 100.0;
      float dv = (v2-v1) / 100.0;


      for (int i = 0; i < 100; i++)
      {
        setHsv(h1+i*dh, s1+i*ds, v1+i*dv, normalize_brightness);
      }
      
    }

    void rgbToHsv(byte r, byte g, byte b, float* return_table)
    {
      float h,s,v,c;
      v = max(max(r,g),b);
      c = v - (min(min(r,g),b));
      s = (v>0) ? c/v : 0;
      if (c<=0)
      {
        h = 0;
      }
      else if (v == r)
      {
        h = 0 + (g-b)/c;
        h /= 6.0;
      }
      else if (v == g)
      {
        h = 2 + (b-r)/c;
        h /= 6.0;
      }
      else
      {
        h = 4 + (r-g)/c;
        h /= 6.0;
      }

      return_table[0] = h;
      return_table[1] = s;
      return_table[2] = v;
      
    }

    void transitionRgb(byte r1, byte g1, byte b1,
                       byte r2, byte g2, byte b2,
                       float time_ms, float normalize_brightness = false)
    {
      float hsv1[3];
      float hsv2[3];
      rgbToHsv(r1,g1,b1,hsv1);
      rgbToHsv(r2,g2,b2,hsv2);

      transitionHsv(hsv1[0],hsv1[1],hsv1[2],hsv2[0],hsv2[1],hsv2[2],
                    time_ms,normalize_brightness);
    }



};
