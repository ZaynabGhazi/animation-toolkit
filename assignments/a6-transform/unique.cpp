#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using glm::vec3;

class Zaynab : public atkui::Framework
{
public:
    Zaynab() : atkui::Framework(atkui::Perspective) {}
    virtual ~Zaynab() {}

    virtual void setup()
    {
        setCameraEnabled(true);
        //lookAt(vec3(-300, 300, -300), vec3(0));
        background(vec3(0, (float)84 / 255, (float)119 / 255));

        Joint *root = new Joint("hip");
        root->setLocalTranslation(vec3(0, 150.0f*2, 0));
        _tentacle.addJoint(root);

        Joint *sternum = new Joint("sternum");
        root->setLocalTranslation(vec3(0, 45*2, 0));
        _tentacle.addJoint(sternum, root);

        Joint *lknee = new Joint("left_knee");
        lknee->setLocalTranslation(vec3(20*2, -30*2, 0));
        _tentacle.addJoint(lknee, root);

        Joint *rknee = new Joint("right_knee");
        rknee->setLocalTranslation(vec3(-20*2, -30*2, 0));
        _tentacle.addJoint(rknee, root);

        Joint *lleg = new Joint("left_leg");
        lleg->setLocalTranslation(vec3(0, -20*2, 0));
        _tentacle.addJoint(lleg, lknee);

        // todo: add more joints
        Joint *rleg = new Joint("right_leg");
        rleg->setLocalTranslation(vec3(0, -20*2, 0));
        _tentacle.addJoint(rleg, rknee);

        Joint *rshoulder = new Joint("right_shoulder");
        rshoulder->setLocalTranslation(vec3(-20*2, 0, 0));
        _tentacle.addJoint(rshoulder, sternum);

        Joint *lshoulder = new Joint("left_shoulder");
        lshoulder->setLocalTranslation(vec3(20*2, 0, 0));
        _tentacle.addJoint(lshoulder, sternum);

        Joint *larm = new Joint("left_arm");
        larm->setLocalTranslation(vec3(0, -10*2, 0));
        _tentacle.addJoint(larm, lshoulder);

        Joint *rarm = new Joint("right_arm");
        rarm->setLocalTranslation(vec3(0, -10*2, 0));
        _tentacle.addJoint(rarm, rshoulder);

        Joint *neck = new Joint("neck_line");
        neck->setLocalTranslation(vec3(0, 15*2, 0));
        _tentacle.addJoint(neck, sternum);

        _tentacle.fk(); // compute local2global transforms
    }

    virtual void scene()
    {
        setColor(vec3(0, (float)84 / 255, (float)119 / 255));
        drawCube(vec3(0, 0, 0), vec3(10 * width()));

        _tentacle.fk();
        setColor(vec3(1, 0, 0));

        for (int j = 0; j < _tentacle.getNumJoints(); j++)
        {
            if (_tentacle.getByID(j)->getParent() == NULL)
            {
                _tentacle.getByID(j)->setLocalTranslation(vec3(elapsedTime() * 10.0f, elapsedTime() * 10.0f, elapsedTime() * 10.0f));
                continue;
            }

            //animate tentacle joint
            _tentacle.getByID(j)->setLocalRotation(glm::angleAxis(sin((float)elapsedTime() + j), vec3(0, 0, 1)));

            Joint *parent = _tentacle.getByID(j)->getParent();
            Joint *child = _tentacle.getByID(j);
            vec3 globalParentPos = parent->getGlobalTranslation();
            vec3 globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 5);
        }
    }

protected:
    Skeleton _tentacle;
};

int main(int argc, char **argv)
{
    Zaynab viewer;
    viewer.run();
}
