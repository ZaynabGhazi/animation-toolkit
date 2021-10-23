#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include "atkui/framework.h"

namespace atkmath
{

   Vector3 Matrix3::toEulerAnglesXYZ() const
   {

      float theta = 0.0;
      float beta = 0.0;
      float alpha = 0.0;
      if (this->m13 != 1 && this->m13 != -1)
      {
         theta = asin(this->m13);
         beta = (-1) * atan2(this->m12, this->m11);
         alpha = (-1) * atan2(this->m23, this->m33);
      }
      else if (this->m13 == 1)
      {
         beta = atan2(this->m21, (-1) * this->m31);
         theta = 0;
         theta = M_PI / 2;
      }
      else
      {
         alpha = 0;
         beta = atan2(this->m21, this->m31);
         theta = (M_PI / 2) * (-1);
      }
      return Vector3(alpha, theta, beta);
   }

   Vector3 Matrix3::toEulerAnglesXZY() const
   {
      float theta = 0.0;
      float beta = 0.0;
      float alpha = 0.0;
      if (this->m12 != 1 && this->m12 != -1)
      {
         beta = (-1) * asin(this->m12);
         theta = atan2(this->m13, this->m11);
         alpha = atan2(this->m32, this->m22);
      }
      else if (this->m12 == -1)
      {
         alpha = atan2(this->m31, this->m21);
         theta = 0;
         beta = M_PI / 2;
      }
      else
      {
         alpha = atan2((-1) * this->m31, (-1) * this->m21);
         theta = 0;
         beta = (M_PI / 2) * (-1);
      }
      return Vector3(alpha, theta, beta);
   }

   Vector3 Matrix3::toEulerAnglesYXZ() const
   {
      float theta = 0.0;
      float beta = 0.0;
      float alpha = 0.0;
      if (this->m23 != 1 && this->m23 != -1)
      {
         alpha = (-1) * asin(this->m23);
         theta = atan2(this->m13, this->m33);
         beta = atan2(this->m21, this->m22);
      }
      else if (this->m23 == -1)
      {
         beta = atan2(this->m31, this->m11);
         theta = 0;
         alpha = M_PI / 2;
      }
      else
      {
         beta = atan2((-1) * this->m31, this->m11);
         theta = 0;
         alpha = (M_PI / 2) * (-1);
      }
      return Vector3(alpha, theta, beta);
   }

   Vector3 Matrix3::toEulerAnglesYZX() const
   {
      float theta = 0.0;
      float beta = 0.0;
      float alpha = 0.0;
      if (this->m21 != 1 && this->m21 != -1)
      {
         beta = asin(this->m21);
         theta = (-1) * atan2(this->m31, this->m11);
         alpha = (-1) * atan2(this->m23, this->m22);
      }
      else if (this->m21 == -1)
      {
         alpha = atan2(this->m32, this->m12);
         theta = 0;
         beta = (M_PI / 2) * (-1);
      }
      else
      {
         alpha = atan2(this->m32, (-1) * this->m12);
         theta = 0;
         beta = M_PI / 2;
      }
      return Vector3(alpha, theta, beta);
   }

   Vector3 Matrix3::toEulerAnglesZXY() const
   {
      float theta = 0.0;
      float beta = 0.0;
      float alpha = 0.0;
      if (this->m32 != 1 && this->m32 != -1)
      {
         alpha = asin(this->m32);
         theta = (-1) * atan2(this->m31, this->m33);
         beta = (-1) * atan2(this->m12, this->m22);
      }
      else if (this->m32 == -1)
      {
         beta = atan2(this->m21, this->m11);
         theta = 0;
         alpha = (M_PI / 2) * (-1);
      }
      else
      {
         beta = atan2(this->m21, this->m11);
         theta = 0;
         alpha = M_PI / 2;
      }
      return Vector3(alpha, theta, beta);
   }

   Vector3 Matrix3::toEulerAnglesZYX() const
   {
      float theta = 0.0;
      float beta = 0.0;
      float alpha = 0.0;
      if (this->m31 != 1 && this->m31 != -1)
      {
         theta = asin(-1 * this->m31);
         beta = atan2(this->m21, this->m11);
         alpha = atan2(this->m32, this->m33);
      }
      else if (this->m31 == 1)
      {
         beta = 0;
         alpha = atan2((-1) * this->m12, (-1) * this->m13);
         theta = (M_PI / 2) * (-1);
      }
      else
      {
         beta = 0;
         alpha = atan2(this->m12, this->m13);
         theta = M_PI / 2;
      }
      return Vector3(alpha, theta, beta);
   }

   void Matrix3::fromEulerAnglesXYZ(const Vector3 &angleRad)
   {
      float alpha = angleRad[0];
      float theta = angleRad[1];
      float beta = angleRad[2];
      Matrix3 Rx(1, 0, 0,
                 0, cos(alpha), -1 * sin(alpha),
                 0, sin(alpha), cos(alpha));

      Matrix3 Ry(cos(theta), 0, sin(theta),
                 0, 1, 0,
                 -1 * sin(theta), 0, cos(theta));
      Matrix3 Rz(cos(beta), -1 * sin(beta), 0,
                 sin(beta), cos(beta), 0,
                 0, 0, 1);
      Matrix3 m = Rx * Ry;
      *this = m * Rz;
   }

   void Matrix3::fromEulerAnglesXZY(const Vector3 &angleRad)
   {
      float alpha = angleRad[0];
      float theta = angleRad[1];
      float beta = angleRad[2];
      Matrix3 Rx(1, 0, 0,
                 0, cos(alpha), -1 * sin(alpha),
                 0, sin(alpha), cos(alpha));

      Matrix3 Ry(cos(theta), 0, sin(theta),
                 0, 1, 0,
                 -1 * sin(theta), 0, cos(theta));
      Matrix3 Rz(cos(beta), -1 * sin(beta), 0,
                 sin(beta), cos(beta), 0,
                 0, 0, 1);
      Matrix3 m = Rx * Rz;
      *this = m * Ry;
   }

   void Matrix3::fromEulerAnglesYXZ(const Vector3 &angleRad)
   {
      float alpha = angleRad[0];
      float theta = angleRad[1];
      float beta = angleRad[2];
      Matrix3 Rx(1, 0, 0,
                 0, cos(alpha), -1 * sin(alpha),
                 0, sin(alpha), cos(alpha));

      Matrix3 Ry(cos(theta), 0, sin(theta),
                 0, 1, 0,
                 -1 * sin(theta), 0, cos(theta));
      Matrix3 Rz(cos(beta), -1 * sin(beta), 0,
                 sin(beta), cos(beta), 0,
                 0, 0, 1);
      Matrix3 m = Ry * Rx;
      *this = m * Rz;
   }

   void Matrix3::fromEulerAnglesYZX(const Vector3 &angleRad)
   {
      float alpha = angleRad[0];
      float theta = angleRad[1];
      float beta = angleRad[2];
      Matrix3 Rx(1, 0, 0,
                 0, cos(alpha), -1 * sin(alpha),
                 0, sin(alpha), cos(alpha));

      Matrix3 Ry(cos(theta), 0, sin(theta),
                 0, 1, 0,
                 -1 * sin(theta), 0, cos(theta));
      Matrix3 Rz(cos(beta), -1 * sin(beta), 0,
                 sin(beta), cos(beta), 0,
                 0, 0, 1);
      Matrix3 m = Ry * Rz;
      *this = m * Rx;
   }

   void Matrix3::fromEulerAnglesZXY(const Vector3 &angleRad)
   {
      float alpha = angleRad[0];
      float theta = angleRad[1];
      float beta = angleRad[2];
      Matrix3 Rx(1, 0, 0,
                 0, cos(alpha), -1 * sin(alpha),
                 0, sin(alpha), cos(alpha));

      Matrix3 Ry(cos(theta), 0, sin(theta),
                 0, 1, 0,
                 -1 * sin(theta), 0, cos(theta));
      Matrix3 Rz(cos(beta), -1 * sin(beta), 0,
                 sin(beta), cos(beta), 0,
                 0, 0, 1);
      Matrix3 m = Rz * Rx;
      *this = m * Ry;
   }

   void Matrix3::fromEulerAnglesZYX(const Vector3 &angleRad)
   {
      float alpha = angleRad[0];
      float theta = angleRad[1];
      float beta = angleRad[2];
      Matrix3 Rx(1, 0, 0,
                 0, cos(alpha), -1 * sin(alpha),
                 0, sin(alpha), cos(alpha));

      Matrix3 Ry(cos(theta), 0, sin(theta),
                 0, 1, 0,
                 -1 * sin(theta), 0, cos(theta));
      Matrix3 Rz(cos(beta), -1 * sin(beta), 0,
                 sin(beta), cos(beta), 0,
                 0, 0, 1);
      Matrix3 m = Rz * Ry;
      *this = m * Rx;
   }

   void Matrix3::toAxisAngle(Vector3 &axis, double &angleRad) const
   {
      Quaternion q;
      q.fromMatrix(*this);
      q.toAxisAngle(axis, angleRad);
   }

   void Matrix3::fromAxisAngle(const Vector3 &axis, double angleRad)
   {
      Quaternion q;
      q.fromAxisAngle(axis, angleRad);
      //convert quaternion to matrix
      *this = q.toMatrix();
   }

}