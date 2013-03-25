#ifndef BCURVE_H
#define BCURVE_H
#include "vector.h"
#include "common.h"
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

class Bcurve
{
public:
    Bcurve();
    ~Bcurve(){}
    static void computeBezier4points( vector<Vector3>&points, const Vector3& a,const Vector3& b,const Vector3& c,const Vector3& d,const float& t);
//    vector<Vector3> getPoints(){
//        return this->points;
//    }
    //Vector3 computeBezier5points(Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 e, float t);

    static void computeBezier5points( vector<Vector3>&points, const Vector3& a,const Vector3& b,const
                                      Vector3& c,const Vector3& d, const Vector3& e,const float& t);

private:
    vector<Vector3> points;
};

#endif // BCURVE_H
