#include "atkui/framework.h"

using namespace glm;
class Eyes : public atkui::Framework
{
  public:
    Eyes() : atkui::Framework(atkui::Orthographic) {
    }
  private:
    int largeRadius = 150;
    int smallRadius = 50;
    float width_ratio = (1.0/8.0)*width();
    float height_ratio = 0.5*height();

    virtual void scene() {
      //draw white eye space
      setColor(vec3(255,255,255));
      drawSphere(vec3(2*width_ratio,height_ratio,0),largeRadius);
      drawSphere(vec3(6*width_ratio,height_ratio,0),largeRadius);
      //draw black eye balls
      setColor(vec3(0,0,0));
      //update eye movement
      float leye_x = 50.0f * sin(elapsedTime()) + 2*width_ratio;
      float reye_x = 50.0f * sin(elapsedTime()) + 6*width_ratio;
      drawSphere(vec3(reye_x,height_ratio,200),smallRadius);
      drawSphere(vec3(leye_x,height_ratio,200),smallRadius);
    }
};

int main(int argc, char** argv) {
  Eyes viewer;
  viewer.run();
  return 0;
}
