#include <random>
#include <functional>
#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "agl/renderer.h"
#include "devil.h"

using namespace atk;
using namespace glm;

class Thriller : public atkui::Framework
{
public:
   Thriller() : atkui::Framework(atkui::Perspective) {}
   virtual ~Thriller() {}

   virtual void setup()
   {
      BVHReader reader;
      reader.load("../motions/Warrok/WarrokThriller.bvh", _skeleton, _motion);

      //create 12 devils
      for (int i = 0; i < 12; i++)
      {
         int offset = (i%3)*200;
         vec3 position = vec3(i*100+offset,0,offset+random()%100);
         vec3 color = vec3(drand48(),drand48(),drand48());
         float size = 0.7+drand48()*0.6;
         _devil = Devil(position, color, size);
         _devils[i] = _devil;
      }
   }

   virtual void scene()
   {
      if (!_paused)
         _motion.update(_skeleton, elapsedTime());
      for(int i=0; i < 12;i++){
         _devils[i].draw(_skeleton, *this);
      }
   }

   virtual void keyUp(int key, int mods)
   {
      if (key == 'P')
         _paused = !_paused;
   }

protected:
   Motion _motion;
   Skeleton _skeleton;
   bool _paused = false;
   Devil _devil;
   Devil _devils[12];
};

int main(int argc, char **argv)
{
   srand(time(NULL));
   Thriller viewer;
   viewer.run();
}
