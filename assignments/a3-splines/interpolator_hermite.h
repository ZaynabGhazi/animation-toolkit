#ifndef _interpolator_hermite_
#define _interpolator_hermite_

#include <string>
#include <vector>
#include <Eigen/Dense>
#include "interpolator.h"

//--------------------------------
// Hermite
//--------------------------------

class InterpolatorHermite : public Interpolator
{
public:
    InterpolatorHermite() : Interpolator("Hermite"), mClampDir(glm::vec3(1, 0, 0)) {}

    virtual glm::vec3 interpolate(int segment, double u) const
    {
        u = (float)u;
        float H0 = 1 - 3 * pow(u, 2) + 2 * pow(u, 3);
        float H1 = u * pow(1 - u, 2);
        float H2 = -pow(u, 2) + pow(u, 3);
        float H3 = 3 * pow(u, 2) - 2 * pow(u, 3);
        return H0 * mCtrlPoints.at(2 * segment) + H1 * mCtrlPoints.at(2 * segment + 1) + H2 * mCtrlPoints.at(2 * (segment + 1) + 1) + H3 * (mCtrlPoints.at(2 * (segment + 1)));
    }

    virtual void computeControlPoints(const std::vector<glm::vec3> &keys)
    {
        mCtrlPoints.clear();
        //solve matrix equation
        Eigen::MatrixXd A(keys.size(), keys.size());
        Eigen::MatrixXd p(keys.size(), 2);
        Eigen::MatrixXd pPrime(keys.size(), 2); // slopes for each control point
        //fill A
        for (int i = 1; i < keys.size() - 1; i++)
        {
            for (int j = 0; j < keys.size(); j++)
            {
                if (j == i - 1)
                    A(i, j) = 1;
                else if (j == i)
                    A(i, j) = 4;
                else if (j == i + 1)
                    A(i, j) = 1;
                else
                    A(i, j) = 0;
            }
        }
        //if clamped
        if (isClamped())
        {
            A(0, 0) = 1;
            //p(0, 0) = 0.821429;
            //p(0, 1) = 2.250000;
            p(0, 0) = 1;
            p(0, 1) = 0;
            for (int j = 1; j < keys.size(); j++)
            {
                A(0, j) = 0;
            }
            A(keys.size() - 1, keys.size() - 1) = 1;
            //p(keys.size() - 1, 0) = 1.678571;
            //p(keys.size() - 1, 1) = -3.750000;
            p(keys.size() - 1, 0) = 1;
            p(keys.size() - 1, 1) = 0;
            for (int j = 0; j < keys.size() - 1; j++)
            {
                A(keys.size() - 1, j) = 0;
            }
        }
        //if natural
        else
        {
            A(0, 0) = 2;
            A(0, 1) = 1;
            A(keys.size() - 1, keys.size() - 2) = 1;
            A(keys.size() - 1, keys.size() - 1) = 2;
            for (int i = 2; i < keys.size(); i++)
                A(0, i) = 0;
            for (int j = 0; j < keys.size() - 2; j++)
                A(keys.size() - 1, j) = 0;

            p(0, 0) = (3.0f * (keys.at(1) - keys.at(0)))[0];
            p(0, 1) = (3.0f * (keys.at(1) - keys.at(0)))[1];
            p(keys.size() - 1, 0) = (3.0f * (keys.at(keys.size() - 1) - keys.at(keys.size() - 2)))[0];
            p(keys.size() - 1, 1) = (3.0f * (keys.at(keys.size() - 1) - keys.at(keys.size() - 2)))[1];
        }
        for (int i = 1; i < keys.size() - 1; i++)
        {
            p(i, 0) = (3.0f * (keys.at(i + 1) - keys.at(i - 1)))[0];
            p(i, 1) = (3.0f * (keys.at(i + 1) - keys.at(i - 1)))[1];
        }
        /*std::cout << "Here is A:\n"
                  << A << std::endl;
        std::cout << "Here is P:\n"
                  << p << std::endl;
        std::cout << "Here is the vector v:\n"
                  << A.inverse() << std::endl;

        std::cout << "Here is the vector p':\n"
                  << pPrime << std::endl;*/
        pPrime = A.inverse() * p;
        for (int i = 0; i < keys.size(); i++)
        {
            mCtrlPoints.push_back(keys.at(i));
            mCtrlPoints.push_back(glm::vec3(pPrime(i, 0), pPrime(i, 1), 0));
        }
    }

    void setClamped(bool c) { mIsClamped = c; }
    bool isClamped() const { return mIsClamped; }

    void setClampedDirection(const glm::vec3 &v) { mClampDir = v; }
    glm::vec3 getClampDirection() const { return mClampDir; }

private:
    bool mIsClamped = true;
    glm::vec3 mClampDir;
};

#endif
