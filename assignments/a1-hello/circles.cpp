#include "atkui/framework.h"
#include "math.h"
using namespace glm;
class Circles : public atkui::Framework
{

public:
   Circles() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
      center[0] = width()/2;
      center[1] = height()/2;
       for(int i = 0; i < numLayers; i++){
         radius[i] = 15 +20 * i;
         color[i] = pallet.operator[](drand48()*pallet.size());
         for(int j = 0; j < numCircles; j++){
               float theta = M_PI * 10 * j / 180;
               circles[i][j] = vec2(center[0]+radius[i]*cos(theta),center[1]+radius[i]*sin(theta));
         }
      }
   }

   virtual void scene() {
      //draw the circles in initial positions
      for(int i = 0; i < numLayers; i++){
         setColor(color[i]);
         int direction = pow(-1,i);
         for(int j = 0; j < numCircles; j++){
               //compute circular movement:
               float theta = atan2(circles[i][j].y-center[1],circles[i][j].x-center[0])+theta_rate*dt()*direction;
               circles[i][j] = vec2(center[0]+radius[i]*cos(theta),center[1]+radius[i]*sin(theta));
               drawSphere(vec3(circles[i][j].y,circles[i][j].x,0),10);
         }
      }
   }


private:
static const int numLayers = 15;
static const int numCircles = 36;
float theta_rate = 0.2;
int center[2];
vec2 circles[numLayers][numCircles];
vec3 color[numLayers];
int radius[numLayers];
//https://www.color-hex.com/color-palette/82379
std::vector<vec3> pallet =
   {
      vec3(0,165,227)/255.0f,
      vec3(141,215,191)/255.0f,
      vec3(255,150,197)/255.0f,
      vec3(255,87,104)/255.0f,
      vec3(255,162,58)/255.0f
   };

};

int main(int argc, char** argv)
{
   Circles viewer;
   viewer.run();
   return 0;
}
