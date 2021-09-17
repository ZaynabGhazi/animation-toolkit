#include "atkui/framework.h"

using namespace glm;

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
      center[0] = width()/2;
        center[1] = height()/2;
       for(int i = 0; i < numLayers; i++){
         radius[i] = 15 +20 * i;
         for(int j = 0; j < numCircles; j++){
               float theta = M_PI * 10 * j / 180;
               circles[i][j] = vec2(center[0]+radius[i]*cos(theta),center[1]+radius[i]*sin(theta));
         }
      }
  }

  virtual void scene() {
      //draw the circles in initial positions
      for(int i = 0; i < numLayers; i++){
         for(int j = 0; j < numCircles; j++){
                setColor(pallet.operator[](j%pallet.size()));
               //compute spiral movement:
               float theta = atan2(circles[i][j].y-center[1],circles[i][j].x-center[0])+theta_rate*dt();
               circles[i][j] = vec2(center[0]+radius[i]*cos(theta),center[1]+radius[i]*sin(theta));
                float angle = 0.1 * j + 0.6 *(i+1)*elapsedTime();
                float x = center[0]+(radius[i] + 10 * angle) * cos(angle);
                float y = center[1]+(radius[i] + 10 * angle) * sin(angle);
               drawSphere(vec3(x,y,0),10);
         }
      }
  }

private:
//You can add more layers to the spiral by changing the number below:
static const int numLayers = 1;
static const int numCircles = 4000;
float theta_rate = 0.2;
int center[2];
vec2 circles[numLayers][numCircles];
int radius[numLayers];
//https://www.color-hex.com/ [neon pallet]
std::vector<vec3> pallet =
   {
      vec3(254,0,0)/255.0f,
      vec3(253,254,2)/255.0f,
      vec3(11,255,1)/255.0f,
      vec3(1,30,254)/255.0f,
      vec3(254,0,246)/255.0f
   };
};

int main(int argc, char** argv) {
  Unique viewer;
  viewer.run();
  return 0;
}


