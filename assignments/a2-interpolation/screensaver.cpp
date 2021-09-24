#include "atkui/framework.h"
#include <tgmath.h>

using namespace glm;
using namespace std;

class Screensaver : public atkui::Framework
{
public:
  //Utility functions:

  //generate curve point using control points:
  vec3 curve(float t, vec3 B0, vec3 B1, vec3 B2, vec3 B3)
  {
    return (float)pow((1 - t), 3) * B0 + 3 * t * (float)pow(1 - t, 2) * B1 + 3 * (float)pow(t, 2) * (1 - t) * B2 + (float)pow(t, 3) * B3;
  }

  //generate random vector in window scope:
  vec3 vector()
  {
    return vec3(rand() % (int)width(), rand() % (int)height(), 0);
  }

  //linearly interpolate:
  vec3 vec_lerp(vec3 a, vec3 b, float t)
  {
    return a * (1 - t) + b * t;
  }

  Screensaver() : atkui::Framework(atkui::Orthographic)
  {
  }

  void setup()
  {
    //init curve 1 and curve2
    int i = 0;
    vec3 B0 = vector();
    vec3 B1 = vector();
    vec3 B2 = vector();
    vec3 B3 = vector();
    vec3 C0 = vector();
    vec3 C1 = vector();
    vec3 C2 = vector();
    vec3 C3 = vector();
    curve1_color = vec3(drand48(), drand48(), drand48());
    curve2_color = vec3(drand48(), drand48(), drand48());
    for (float t = 0; t <= 1 && i < num_pts; t += (1 / (float)num_pts))
    {
      curve1[i] = curve(t, B0, B1, B2, B3);
      curve2[i] = curve(t, C0, C1, C2, C3);
      i++;
    };
  }

  void scene()
  {
    //draw curve1 and curve2
    setColor(vec3(1, 0.8, 0.1));
    for (int i = 0; i < num_pts; i++)
    {
      setColor(curve1_color);
      drawSphere(curve1[i], 0.7);
      setColor(curve2_color);
      drawSphere(curve2[i], 0.7);
    }

    //draw trails
    list<Curve>::iterator k;
    for (k = trails.begin(); k != trails.end(); k++)
    {
      setColor(k->color);
      //draw curve
      for (int i = 0; i < num_pts; i++)
      {
        drawSphere(k->points[i], 0.7);
      }
    }

    //interpolate
    interpol_color = vec_lerp(curve1_color, curve2_color, t);
    setColor(interpol_color);
    for (int i = 0; i < num_pts; i++)
    {
      interpol_curve[i] = vec_lerp(curve1[i], curve2[i], t);
      drawSphere(interpol_curve[i], 0.7);
    }
    t += 0.05;

    //save current interpolation
    if (elapsedTime() - tau >= save_clock)
    {
      tau = elapsedTime();
      struct Curve trail;
      for (int j = 0; j < num_pts; j++)
        trail.points[j] = interpol_curve[j];
      trail.color = interpol_color;
      if (trails.size() == num_curves)
      {
        trails.pop_front();
      }
      trails.push_back(trail);
    }

    if (t > 1)
    {
      t = 0;
      //generate new curve and store curve2 in curve1
      int j = 0;
      vec3 B0 = vector();
      vec3 B1 = vector();
      vec3 B2 = vector();
      vec3 B3 = vector();
      curve1_color = curve2_color;
      curve2_color = vec3(drand48(), drand48(), drand48());
      for (float T = 0; T <= 1 && j < num_pts; T += (1 / (float)num_pts))
      {
        curve1[j] = curve2[j];
        curve2[j] = curve(T, B0, B1, B2, B3);
        j++;
      }
    }
  }

private:
  const static int num_pts = 1000;
  const static int num_curves = 50;
  vec3 curve1[num_pts];
  vec3 curve1_color;
  vec3 curve2[num_pts];
  vec3 curve2_color;
  vec3 interpol_curve[num_pts];
  vec3 interpol_color;
  float t = 0;
  float tau = 0;
  float save_clock = 0.1;
  struct Curve
  {
    vec3 points[num_pts];
    vec3 color;
  };
  list<Curve> trails;
};

int main(int argc, char **argv)
{
  Screensaver viewer;
  viewer.run();
}
