#include "atkui/framework.h"
#include "atkmath/matrix3.h"
#include <Eigen/Dense>
#include <math.h>

#define X 0
#define Y 1
#define Z 3
using namespace glm;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Orthographic)
   {
   }

   virtual void setup()
   {
      for (int i = 0; i < 360; i++)
      {
         //draw rose with 8 petals
         float r = size * sin(M_PI * 8 * i / 180);
         float x = r * cos(M_PI * i / 180);
         float y = r * sin(M_PI * i / 180);
         spheres[i] = vec3(x + width() / 2, y + height() / 2, 0);
      }
   }

   virtual void scene()
   {
      //rotate over x
      euler_angles[X] += 0.09 * dt();
      if (euler_angles[X] == 360)
         euler_angles[X] = 0;
      rot_matrix.fromEulerAnglesXYZ(euler_angles * M_PI / 180);
      Eigen::MatrixXd R(3, 3);
      R(0, 0) = rot_matrix[0][0];
      R(0, 1) = rot_matrix[0][1];
      R(0, 2) = rot_matrix[0][2];
      R(1, 0) = rot_matrix[1][0];
      R(1, 1) = rot_matrix[1][1];
      R(1, 2) = rot_matrix[1][2];
      R(2, 0) = rot_matrix[2][0];
      R(2, 1) = rot_matrix[2][1];
      R(2, 2) = rot_matrix[2][2];
      vec3 color = pallet.operator[](drand48()*pallet.size());

      for (int i = 0; i < 360; i++)
      {
         Eigen::MatrixXd P(3, 1);
         P(0, 0) = spheres[i][0];
         P(1, 0) = spheres[i][1];
         P(2, 0) = spheres[i][2];
         Eigen::MatrixXd result = R * P;
         spheres[i][0] = result(0, 0);
         spheres[i][1] = result(1, 0);
         spheres[i][2] = result(2, 0);
         setColor(color);
         drawSphere(vec3(spheres[i][0], spheres[i][1] + height() / 2 - 50, 0), 5);
      }
   }

private:
   int size = 50;
   vec3 spheres[360];
   atkmath::Matrix3 rot_matrix;
   atkmath::Vector3 euler_angles = atkmath::Vector3(0, 0, 0);
   //https://www.color-hex.com/color-palette/82379
   std::vector<vec3> pallet =
       {
           vec3(0, 165, 227) / 255.0f,
           vec3(141, 215, 191) / 255.0f,
           vec3(255, 150, 197) / 255.0f,
           vec3(255, 87, 104) / 255.0f,
           vec3(255, 162, 58) / 255.0f};
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}