#include "atkui/framework.h"
using namespace glm;

class ParticleCubic : public atkui::Framework {
 public:
  ParticleCubic() : atkui::Framework(atkui::Orthographic) {
  }

  vec3 curve(float t){
    return (float)pow((1-t),3)*B0+3*t*(float)pow(1-t,2)*B1+3*(float)pow(t,2)*(1-t)*B2+(float)pow(t,3)*B3;
  }

  void setup() {
    //create curve
    int i=0;
    for(float t=0; t <= 1 && i<num_pts; t+= (1/(float)num_pts)){
      bezier[i++] = curve(t);
    }
  }

  void scene() {
    setColor(vec3(1));
    drawSphere(curve(fmod(elapsedTime()/5,1)), 10);
    //draw curve
    setColor(vec3(1,0.8,0.1));
     for(int i=0; i < num_pts;i++){
          drawSphere(bezier[i],2);
    }
    
  }
  private:
  vec3 B0 = vec3(100,  50, 0);
  vec3 B1 = vec3(150, 200, 0);
  vec3 B2 = vec3(250, 100, 0);
  vec3 B3 = vec3(250, 250, 0);
  const static int num_pts= 1000;
  vec3 bezier[num_pts];
};

int main(int argc, char** argv) {
  ParticleCubic viewer;
  viewer.run();
}

