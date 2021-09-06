#include "atkui/framework.h"
#include <unistd.h>

using glm::vec3;

class Sphere3D : public atkui::Framework
{
public:
  Sphere3D() : atkui::Framework(atkui::Perspective)
  {
  }

private:
  vec3 position = vec3(0, 0, 0);
  vec3 direction;
  float radius = 50.0;
  float velocity = 50;
  bool isSliding = false;

  virtual void scene()
  {
    setColor(vec3(0, 1, 0));
    if (isSliding)
    {
      position += direction * dt() * velocity;
    }
    drawSphere(position, radius);
  }

  virtual void keyUp(int key, int mods)
  {
    if (key == GLFW_KEY_SPACE)
    {
      isSliding = true;
      direction = agl::randomUnitVector();
    }
    if (key == GLFW_KEY_R)
    {
      //reset to origin
      isSliding = false;
      position = vec3(0, 0, 0);
    }
  }
};

int main(int argc, char **argv)
{
  Sphere3D viewer;
  viewer.run();
  return 0;
}
