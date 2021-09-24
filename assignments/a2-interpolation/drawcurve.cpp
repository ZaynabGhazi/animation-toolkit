#include <iostream>
#include "atkui/framework.h"
using namespace glm;

class DrawCubic : public atkui::Framework
{
public:
  DrawCubic() : atkui::Framework(atkui::Orthographic, 400, 400)
  {
  }
  vec3 vec_lerp(vec3 a, vec3 b, float t)
  {
    return a * (1 - t) + b * t;
  }
  virtual void setup()
  {
    int i = 0;
    for (float t = 0; t <= 1 && i < num_pts; t += (1 / (float)num_pts))
    {

      //bernstein
      bezier[i] = (float)pow((1 - t), 3) * B0 + 3 * t * (float)pow(1 - t, 2) * B1 + 3 * (float)pow(t, 2) * (1 - t) * B2 + (float)pow(t, 3) * B3;

      //casteljau

      vec3 q0 = vec_lerp(B0, B1, t);
      vec3 q1 = vec_lerp(B1, B2, t);
      vec3 q2 = vec_lerp(B2, B3, t);
      vec3 b0 = vec_lerp(q0, q1, t);
      vec3 b1 = vec_lerp(q1, q2, t);
      casteljau[i] = vec_lerp(b0, b1, t);
      i++;
    }
  }

  virtual void scene()
  {
    setColor(vec3(0, 0, 1));
    drawSphere(B0, 10);
    drawSphere(B3, 10);

    //draw bezier:

    for (int i = 0; i < num_pts; i++)
    {
      if (bernstein_flag)
        drawSphere(bezier[i], 2);
      else if (casteljau_flag)
        drawSphere(casteljau[i], 2);
    }

    setColor(vec3(1, 1, 0));
    drawSphere(B1, 10);
    drawSphere(B2, 10);
  }

  void keyUp(int key, int mod)
  {
    if (key == GLFW_KEY_1)
    {
      bernstein_flag = true;
      casteljau_flag = false;
    }
    else if (key == GLFW_KEY_2)
    {
      bernstein_flag = false;
      casteljau_flag = true;
    }
  }

private:
  vec3 B0 = vec3(100, 50, 0);
  vec3 B1 = vec3(150, 200, 0);
  vec3 B2 = vec3(250, 100, 0);
  vec3 B3 = vec3(300, 300, 0);
  const static int num_pts = 1000;
  vec3 bezier[num_pts];
  vec3 casteljau[num_pts];
  bool bernstein_flag = false;
  bool casteljau_flag = false;
};

int main(int argc, char **argv)
{
  DrawCubic viewer;
  viewer.run();
}
