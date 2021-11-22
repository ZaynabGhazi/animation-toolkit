#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <stdio.h>
#include <string.h>

using namespace atk;
using namespace atkui;
using namespace glm;

class ACrossfadeViewer : public atkui::Framework
{
public:
  ACrossfadeViewer() : atkui::Framework(atkui::Perspective) {}
  virtual ~ACrossfadeViewer() {}

  void loadMotion1(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion1_);
  }

  void loadMotion2(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion2_);
  }

  void crossfade(int numBlendFrames)
  {
    assert(motion1_.getNumKeys() > 0);
    assert(motion2_.getNumKeys() > 0);
    assert(numBlendFrames > 0);
    assert(numBlendFrames <= motion1_.getNumKeys());
    assert(numBlendFrames <= motion2_.getNumKeys());

    int start1 = motion1_.getNumKeys() - numBlendFrames;
    int start2 = 0;

    //fix speed
    blend_.setFramerate(motion1_.getFramerate());
    for (int i = 0; i < start1; i++)
    {
      blend_.appendKey(motion1_.getKey(i));
    }
    Pose initial = motion1_.getKey(motion1_.getNumKeys() - 1);
    align(initial, motion2_);
    //Blend
    for (int i = 0; i < numBlendFrames; i++)
    {
      Pose pose_1 = motion1_.getKey(start1 + i);
      Pose pose_2 = motion2_.getKey(start2 + i);
      Pose blended = Pose::Lerp(pose_1, pose_2, (float)i / (numBlendFrames - 1));
      blend_.appendKey(blended);
    }
    for (int i = numBlendFrames; i < motion2_.getNumKeys(); i++)
    {
      blend_.appendKey(motion2_.getKey(i));
    }
  }
  void align(Pose &initial, Motion &motion)
  {
    float heading = glm::eulerAngles(initial.jointRots[0]).y;
    Pose first = motion.getKey(0);
    Transform shift(angleAxis(0.0f, vec3(1, 0, 0)), -first.rootPos);
    Transform rotate(angleAxis(heading, vec3(0, 1, 0)), initial.rootPos);

    for (int i = 0; i < motion.getNumKeys(); i++)
    {
      Pose pose = motion.getKey(i);
      Transform current(pose.jointRots[0], pose.rootPos);
      Transform final = rotate * shift * current;
      pose.jointRots[0] = final.r();
      pose.rootPos = final.t();
      motion.editKey(i, pose);
    }
  }

  void save(const std::string &filename)
  {
    BVHWriter writer;
    writer.save(filename, skeleton_, blend_);
  }

  void scene()
  {
    blend_.update(skeleton_, elapsedTime());
    drawer_.draw(skeleton_, *this);
  }

private:
  Skeleton skeleton_;
  SkeletonDrawer drawer_;
  Motion motion1_;
  Motion motion2_;
  Motion blend_;
};

std::string PruneName(const std::string &name)
{
  int idx1 = name.find_last_of("/");
  int idx2 = name.find_last_of(".");
  return name.substr(idx1 + 1, idx2 - idx1 - 1);
}

int main(int argc, char **argv)
{
  std::string motion1 = "../motions/Beta/walking.bvh";
  std::string motion2 = "../motions/Beta/jump.bvh";
  int numFrames = 10;

  try
  {
    for (int i = 0; i < argc; i++)
    {
      if (strncmp(argv[i], "-m1", 3) == 0)
        motion1 = argv[i + 1];
      else if (strncmp(argv[i], "-m2", 3) == 0)
        motion2 = argv[i + 1];
      else if (strncmp(argv[i], "-nframes", 7) == 0)
        numFrames = std::stoi(argv[i + 1]);
    }
  }
  catch (const std::exception &e)
  {
    std::cout << "usage: crossfade -m1 <path> -m2 <path> -nframes <num frames>\n";
  }

  ACrossfadeViewer viewer;
  viewer.loadMotion1(motion1);
  viewer.loadMotion2(motion2);
  viewer.crossfade(numFrames);

  std::string saveName = "blend-" + PruneName(motion1) + PruneName(motion2);
  saveName += "-" + std::to_string(numFrames) + ".bvh";
  viewer.save(saveName);

  viewer.run();
  return 0;
}
