#include "atkui/framework.h"
#include <cmath>        // std::abs

using namespace glm;

class Look : public atkui::Framework {
 public:
  Look() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    _mouseX = width() * 0.5;
    _mouseY = height() * 0.5;
  }

  virtual void scene() {
    if (track_mouse) mouseMove(mousePosition().x,mousePosition().y);
    vec3 target = vec3(_mouseX, _mouseY, 0);
    setColor(vec3(1,0,0));
    drawSphere(target, 5);

    //eyes:
     //draw white eye space
    setColor(vec3(255,255,255));
    drawSphere(vec3(2*width_ratio,height_ratio,0),largeRadius);
    drawSphere(vec3(6*width_ratio,height_ratio,0),largeRadius);      
    //draw black eye balls
     setColor(vec3(0,0,0));
     //update eye movement
     float theta1 = atan2(target.y-height_ratio,target.x-2*width_ratio);
     float theta2 = atan2(target.y-height_ratio,target.x-6*width_ratio);

    float leye_x = movement_radius* cos(theta1) + 2*width_ratio;
    float leye_y = movement_radius* sin(theta1) + height_ratio;
    float reye_x = movement_radius* cos(theta2) + 6*width_ratio;
    float reye_y = movement_radius* sin(theta2) + height_ratio;
    drawSphere(vec3(reye_x,reye_y,200),smallRadius);
    drawSphere(vec3(leye_x,leye_y,200),smallRadius);
  }

  void mouseMove(int x, int y) {
    _mouseX = x;
    _mouseY = height() - y;
  }

  virtual void mouseDown(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT){
      track_mouse = true;
    }
  }
  virtual void mouseUp(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT){
      track_mouse = false;
    }
  }


 private:
  int _mouseX;
  int _mouseY;
  bool track_mouse = false;
  int largeRadius = 150;
  int smallRadius = 50;
  float width_ratio = (1.0/8.0)*width();
  float height_ratio = 0.5*height();
  int movement_radius = 25;
};

int main(int argc, char** argv) {
  Look viewer;
  viewer.run();
  return 0;
}
