#include <iostream>
#include <tgmath.h>
#include "atkui/framework.h"
using namespace glm;

class Gradient : public atkui::Framework {
 public:
  Gradient() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup(){
      //draw the cubes
    float side_halflength = width()/(2*N);
    for (int i=0; i<N;i++){
      for(int j=0; j< N;j++){
        particles[i][j].position= vec3(side_halflength*(2*i+1) ,side_halflength*(2*j+1),0);  
        //interpolate color:
        float t_x = particles[i][j] .position.x / width();
        float t_y = particles[i][j] .position.y / height();
        vec3 c_0 = c_nw*(1-t_x)+c_ne*t_x;
        vec3 c_1 = c_sw*(1-t_x)+c_se*t_x;
        vec3 c = c_1 *(1-t_y)+c_0*t_y;
        particles[i][j].color = c;
      }
    }
  }

  virtual void scene() {
    for (int i=0; i<N;i++){
      for(int j=0; j< N;j++){
          setColor(particles[i][j].color);
          drawCube(particles[i][j].position,vec3(width()/N));  
           }
    }

  }

  private:
    struct Particle{
    vec3 position;
    vec3 color;
  };
  static const int N = 10;
  Particle particles[N][N];
  vec3 c_nw = vec3(1,1,0);
  vec3 c_ne = vec3(0,1,1);
  vec3 c_sw = vec3(1,0,0);
  vec3 c_se = vec3(1,0,1);
};

int main(int argc, char **argv) {
  Gradient viewer;
  viewer.run();
}
