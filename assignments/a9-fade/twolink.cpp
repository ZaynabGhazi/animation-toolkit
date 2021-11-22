#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include "atk/toolkit.h"
#include "atkui/skeleton_drawer.h"
#include "atkui/framework.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace atk;
using namespace glm;

class AIKSimple : public atkui::Framework
{
public:
  AIKSimple() : atkui::Framework(atkui::Perspective),
                mDrawer(),
                mGoalPosition()
  {
    mDrawer.showAxes = true;
    mDrawer.color = vec3(0.6, 1.0, 0.4);
  }

  virtual ~AIKSimple()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void setup()
  {
    Joint *shoulder = new Joint("Shoulder");
    mActor.addJoint(shoulder);
    shoulder->setLocalTranslation(vec3(0, 0, 0));

    Joint *elbow = new Joint("Elbow");
    mActor.addJoint(elbow, shoulder);
    elbow->setLocalTranslation(vec3(100, 0, 0));

    Joint *wrist = new Joint("Wrist");
    mActor.addJoint(wrist, elbow);
    wrist->setLocalTranslation(vec3(80, 0, 0));

    mActor.fk();
    mGoalPosition = wrist->getGlobalTranslation();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
#endif
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
  }

  void drawGui()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Demo Controls");
    ImGui::SetWindowSize(ImVec2(150, 150));
    ImGui::SetWindowPos(ImVec2(5, 5));
    ImGui::SliderFloat("X", &mGoalPosition[0], -500.0f, 500.0f);
    ImGui::SliderFloat("Y", &mGoalPosition[1], -500.0f, 500.0f);
    ImGui::SliderFloat("Z", &mGoalPosition[2], -500.0f, 500.0f);
    if (ImGui::Button("Reset"))
      reset();
    ImGui::End();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void reset()
  {
    for (int i = 0; i < mActor.getNumJoints(); i++)
    {
      mActor.getByID(i)->setLocalRotation(atk::IdentityQ);
    }
    mActor.fk();
    mGoalPosition = mActor.getByID(2)->getGlobalTranslation();
  }

  void draw()
  {
    mDrawer.draw(mActor, *this);
    drawFloor(2000, 20, 50);

    vec2 screenPos = worldToScreen(mGoalPosition);

    setColor(vec3(1.0, 0.0, 1.0));
    ortho(0, width(), 0, height(), -1000, 1000);
    renderer.lookAt(vec3(0, 0, 1), vec3(0), vec3(0, 1, 0));

    vec3 c = vec3(screenPos, 0);
    vec3 v1 = c + vec3(10, 0, 0);
    vec3 v2 = c - vec3(10, 0, 0);
    vec3 h1 = c + vec3(0, 10, 0);
    vec3 h2 = c - vec3(0, 10, 0);

    beginShader("unlit");
    drawCircle(c, 5.0f);
    drawLine(v1, v2);
    drawLine(h1, h2);
    endShader();

    // reset projection
    perspective(glm::radians(60.0f), width() / (float)height(), 10.0f, 2000.0f);
    renderer.lookAt(camera.position(), camera.look(), camera.up());

    drawGui();
    solveIKTwoLink(mActor, mGoalPosition);
  }

  void solveIKTwoLink(Skeleton &skeleton, const vec3 &goalPosition)
  {
    //we have:

    //p1_p2
    float l_1 = length(skeleton.getByName("Elbow")->getGlobalTranslation() - skeleton.getByName("Shoulder")->getGlobalTranslation());
    //p2_p3
    float l_2 = length(skeleton.getByName("Wrist")->getGlobalTranslation() - skeleton.getByName("Elbow")->getGlobalTranslation());
    //p1_p3 : assume goalPosition is with respect to p1
    float r = length(goalPosition);
    //solve for theta_2z
    //--solve for phi
    float cos_phi = (pow(r, 2) - pow(l_1, 2) - pow(l_2, 2)) / (-2 * l_1 * l_2);
    //clamp
    cos_phi > 1 ? cos_phi = 1 : cos_phi = (cos_phi < -1 ? -1 : cos_phi);
    float phi = acos(cos_phi);
    float theta_2z = phi - M_PI;
    //--rotate elbow
    skeleton.getByName("Elbow")->setLocalRotation(angleAxis(theta_2z, vec3(0, 0, 1)));
    //solve for theta_1z
    float sin_theta_1z = -(l_2)*sin(theta_2z) / r;
    //clamp
    sin_theta_1z > 1 ? sin_theta_1z = 1 : sin_theta_1z = (sin_theta_1z < -1 ? -1 : sin_theta_1z);
    float theta_1z = asin(sin_theta_1z);
    quat rot_theta = angleAxis(theta_1z, vec3(0, 0, 1));
    //solve full rotation as f(gamma,beta)
    float sin_gamma = goalPosition[1] / r;
    sin_gamma > 1 ? sin_gamma = 1 : sin_gamma = (sin_gamma < -1 ? -1 : sin_gamma);
    float gamma = asin(sin_gamma);
    quat rot_gamma = angleAxis(gamma, vec3(0, 0, 1));
    float beta = atan2(-1 * goalPosition[2], goalPosition[0]);
    quat rot_beta = angleAxis(beta, vec3(0, 1, 0));
    quat final_rot = rot_beta * rot_gamma * rot_theta;
    //--rotate shoulder
    skeleton.getByName("Shoulder")->setLocalRotation(final_rot);
    //update transforms
    skeleton.getRoot()->fk();
  }

private:
  atk::Skeleton mActor;
  atkui::SkeletonDrawer mDrawer;
  glm::vec3 mGoalPosition;
};

int main(int argc, char **argv)
{
  AIKSimple viewer;
  viewer.run();
  return 0;
}
