#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include "atk/toolkit.h"
#include <iostream>
#include <cmath>

using namespace glm;
using namespace std;
using namespace atk;

class WASDSimple : public atkui::Framework
{
public:
  WASDSimple() : atkui::Framework(atkui::Perspective) {}

  virtual void setup()
  {
    BVHReader reader;
    reader.load("../motions/Beta/walking.bvh", _skeleton, _walk);
    _drawer.showAxes = true;
    globalPos = _skeleton.getByName("Beta:Head")->getGlobalTranslation();
    globalLookPos = _skeleton.getByName("Beta:Spine1")->getGlobalTranslation();

    desired_speed = 0.67;
    position = vec3(0);
  } 

  virtual void scene()
  {
    update();
    _drawer.draw(_skeleton, *this);

    // draw heading
    vec3 p = _skeleton.getRoot()->getGlobalTranslation();
    p[1] = 10; // set height close above the floor

    setColor(vec3(0, 1, 1));
    push();
    translate(p);
    rotate(_heading, vec3(0, 1, 0));
    translate(vec3(0, 0, 25));
    scale(vec3(10, 10, 50));
    drawCylinder(vec3(0), 1.0f);
    pop();
  }

  virtual void update()
  {
    _walk.update(_skeleton, elapsedTime());

    Pose current = _skeleton.getPose();
    current.rootPos = position + vec3(0,current.rootPos[1],0);
    current.jointRots[0] *= glm::angleAxis(_heading, vec3(0, 1, 0));
    _skeleton.setPose(current);

    // TODO: Override the default camera to follow the character
    vec3 trig_delta = vec3(desired_speed*sin(_heading), 0, desired_speed*cos(_heading));
    position += trig_delta;
    globalPos += trig_delta;
    globalLookPos += trig_delta;
    lookAt(globalPos+vec3(-230*sin(_heading),280,-200*cos(_heading)) /*adjust camera position*/, globalLookPos, vec3(0, 1, 0));

    // update heading when key is down
    if (keyIsDown('D'))
      _heading -= 0.05;
    if (keyIsDown('A'))
      _heading += 0.05;
  }

protected:
  float _heading;
  vec3 globalPos;
  vec3 globalLookPos;
  float desired_speed;
  vec3 position;
  Motion _walk;
  Skeleton _skeleton;
  atkui::SkeletonDrawer _drawer;
};

int main(int argc, char **argv)
{
  WASDSimple viewer;
  viewer.run();
}
