#include "atk/toolkit.h"
#include <iostream>
using namespace atk;
using glm::vec3;

int main(int argc, char** argv)
{
   Skeleton skeleton;
   Motion motion;

   BVHReader reader;
   reader.load("../motions/Beta/idle.bvh", skeleton, motion);

   int numJoints = skeleton.getNumJoints(); 
   std::cout << "The number of joints is " << numJoints << std::endl;

   for (int i = 0; i < numJoints; i++)
   {
      std::string jointName = skeleton.getByID(i)->getName();
      std::cout << i << " " << jointName << std::endl;
   }

   Joint* root = skeleton.getRoot();
   std::cout << "The root joint name is: " << root->getName()<< std::endl;
   std::cout << "The root joint ID is: " <<root->getID()<< std::endl;
   std::cout << "The root joint's number of children is: " << root->getNumChildren() << std::endl;

   int rootNumChildren = root->getNumChildren();
   for (int i = 0; i < rootNumChildren; i++)
   {
      std::string childName = root->getChildAt(i)->getName();
      std::cout << childName << std::endl; 
   } 
   Joint *hand = skeleton.getByName("Beta:LeftHand");
   vec3 offset = hand->getLocalTranslation();
   std::cout << "The offset between the left hand and its parent is " << glm::to_string(offset) << std::endl;

   std::string handParentName = hand->getParent()->getName();
   std::cout << "The parent of the hand is " << handParentName << std::endl;
}
