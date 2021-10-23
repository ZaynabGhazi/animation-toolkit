#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atkmath/vector3.h"

#define DEBUG 0
namespace atkmath
{

	Quaternion Quaternion::Slerp(const Quaternion &q0, const Quaternion &q1, double t)
	{
		// TODO
		return Quaternion(1, 0, 0, 0);
	}

	void Quaternion::toAxisAngle(Vector3 &axis, double &angleRad) const
	{
		//compute angle
		angleRad = 2 * acos(this->mW);
		Vector3 v = Vector3(this->mX, this->mY, this->mZ);
		if (sqrt(this->mX * this->mX + this->mY * this->mY + this->mZ * this->mZ) == 0)
			axis = Vector3(1, 0, 0);
		else
			axis = v / sqrt(this->mX * this->mX + this->mY * this->mY + this->mZ * this->mZ);
		axis.normalize();
	}

	void Quaternion::fromAxisAngle(const Vector3 &axis, double angleRad)
	{
		this->mW = cos(angleRad / 2.0);
		this->mX = sin(angleRad / 2.0) * axis[0];
		this->mY = sin(angleRad / 2.0) * axis[1];
		this->mZ = sin(angleRad / 2.0) * axis[2];
		normalize();
	}

	Matrix3 Quaternion::toMatrix() const
	{
		Matrix3 result;
		//diagonal
		result[0][0] = 1 - (double)2 * (pow(this->mY, 2) + pow(this->mZ, 2));
		result[1][1] = 1 - 2.0 * (pow(this->mX, 2) + pow(this->mZ, 2));
		result[2][2] = 1 - 2.0 * (pow(this->mY, 2) + pow(this->mX, 2));
		//other entries
		result[0][1] = 2 * (this->mX * this->mY - this->mW * this->mZ);
		result[0][2] = 2 * (this->mX * this->mZ + this->mW * this->mY);
		result[1][0] = 2 * (this->mX * this->mY + this->mW * this->mZ);
		result[1][2] = 2 * (this->mY * this->mZ - this->mW * this->mX);
		result[2][0] = 2 * (this->mX * this->mZ - this->mW * this->mY);
		result[2][1] = 2 * (this->mY * this->mZ + this->mW * this->mX);
		return result;
	}

	void Quaternion::fromMatrix(const Matrix3 &rot)
	{
		//find max
		double max = 0;
		double w_2 = ((double)1) / 4.0 * (rot[0][0] + rot[1][1] + rot[2][2] + 1);
		double x_2 = ((double)1) / 4.0 * (rot[0][0] - rot[1][1] - rot[2][2] + 1);
		max = std::max(w_2, x_2);
		double y_2 = ((double)1) / 4.0 * (-1 * rot[0][0] + rot[1][1] - rot[2][2] + 1);
		max = std::max(max, y_2);
		double z_2 = ((double)1) / 4.0 * (-1 * rot[0][0] - rot[1][1] + rot[2][2] + 1);
		max = std::max(max, z_2);
		if (max == w_2)
		{
			//compute w
			this->mW = sqrt(w_2);
			if (DEBUG)
				std::cout << " w2 " << w_2 << std::endl;
			//compute x
			this->mX = ((double)1) / (4.0 * this->mW) * (rot[2][1] - rot[1][2]);
			if (DEBUG)
				std::cout << " x " << this->mX << std::endl;

			//compute y
			this->mY = ((double)1) / (4.0 * this->mW) * (rot[0][2] - rot[2][0]);
			if (DEBUG)
				std::cout << " y " << this->mY << std::endl;

			//compute z
			this->mZ = ((double)1) / (4.0 * this->mW) * (rot[1][0] - rot[0][1]);
			if (DEBUG)
				std::cout << " z " << this->mZ << std::endl;
		}
		else if (max == x_2)
		{
			//compute X
			this->mX = sqrt(x_2);
			//compute W
			this->mW = ((double)1) / (4.0 * this->mX) * (rot[3][2] - rot[2][3]);
			//compute y
			this->mY = ((double)1) / (4.0 * this->mW) * (rot[1][3] - rot[3][1]);
			//compute z
			this->mZ = ((double)1) / (4.0 * this->mX) * (rot[1][3] - rot[3][1]);
		}
		else if (max == y_2)
		{
			//compute y
			this->mY = sqrt(y_2);
			//compute x
			this->mX = ((double)1) / (4.0 * this->mY) * (rot[1][0] + rot[0][1]);
			//compute w
			this->mW = ((double)1) / (4.0 * this->mY) * (rot[0][2] - rot[2][0]);
			//compute z
			this->mZ = ((double)1) / (4.0 * this->mY) * (rot[1][2] - rot[2][1]);
		}
		//case z_2 is max
		else
		{
			//compute z
			this->mZ = sqrt(z_2);
			//compute x
			this->mX = ((double)1) / (4.0 * this->mZ) * (rot[0][2] + rot[2][0]);
			//compute y
			this->mY = ((double)1) / (4.0 * this->mZ) * (rot[1][2] - rot[2][1]);
			//compute w
			this->mW = ((double)1) / (4.0 * this->mX) * (rot[0][2] - rot[2][0]);
		}
		normalize();
	}

}