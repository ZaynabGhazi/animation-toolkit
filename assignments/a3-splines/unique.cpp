#include "atkui/framework.h"
#include "interpolator_hermite.h"
#include "interpolator_catmullrom.h"

using namespace glm;
using namespace std;

/* draws a curve between a source and a target determined by the user and a sphere traversing it*/
class Unique : public atkui::Framework

{
public:
   Unique() : atkui::Framework(atkui::Orthographic)
   {
   }
   virtual void setup()
   {
      //hermite interpolation for random movement of the particles
      //catmullrom interpolator for color of particles
      std::vector<glm::vec3> keys;
      for (int i = 0; i < 100; i++)
      {
         keys.push_back(vec3(rand() % ((int)width()), rand() % ((int)height()), 0));
      }
      interpol.computeControlPoints(keys);
      std::vector<glm::vec3> keys_;
      for (int i = 0; i < 100; i++)
      {
         keys_.push_back(vec3(drand48(), drand48(), drand48()));
      }
      interpol_.computeControlPoints(keys_);
      for (int i = 0; i < 250; i++)
      {
         particles[i].position = vec2(rand() % ((int)width()), rand() % ((int)height()));
         particles[i].velocity = 20 + rand() % 30;
      }
   }

   virtual void scene()
   {
      for (int i = 0; i < 250; i++)
      {
         setColor(interpol_.interpolate(rand() % 99, dt()));
         particles[i].position.x += particles[i].velocity * dt();
         particles[i].position.y += particles[i].velocity * dt();
         if (particles[i].position.x > width())
            particles[i].position.x = 0;
         if (particles[i].position.y > height())
            particles[i].position.y = 0;
         vec3 p = vec3(particles[i].position.x, particles[i].position.y, 0);
         drawSphere(p+interpol.interpolate(rand() % 99, drand48()), radius);
      }
   }

   struct Particle
   {
      vec2 position;
      float velocity;
   };

private:
   Particle particles[250];
   int radius = 10;
   InterpolatorHermite interpol;
   InterpolatorCatmullRom interpol_;
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}