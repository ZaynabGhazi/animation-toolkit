#include "atkui/framework.h"

using namespace glm;

class Particles : public atkui::Framework
{
 public:
  Particles() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    for(int i=0; i<250; i++){
      particles[i].position = vec2(rand()%((int)width()),rand()%((int)height()));
      particles[i].velocity = 20+rand()%70;
      particles[i].color = vec3(drand48(),drand48(),drand48());
    }
  }

  virtual void scene() {
    for (int i=0; i < 250; i++) {
      setColor(particles[i].color);
      particles[i].position.x += particles[i].velocity*dt();
      particles[i].position.y += particles[i].velocity*dt();
      if (particles[i].position.x > width()) particles[i].position.x = 0;
      if (particles[i].position.y > height()) particles[i].position.y =  0;
      drawSphere(vec3(particles[i].position.x,particles[i].position.y,0),radius);
    }
  }

  struct Particle{
    vec2 position;
    float velocity;
    vec3 color;
  };

  private:
    Particle particles[250];
    int radius = 10;
};

int main(int argc, char** argv) {
  Particles viewer;
  viewer.run();
  return 0;
}

