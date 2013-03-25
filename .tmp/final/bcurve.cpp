#include "bcurve.h"
#include <assert.h>

Bcurve::Bcurve()
{
}
void Bcurve::computeBezier4points( vector<Vector3>&points, const Vector3& a,const Vector3& b,const Vector3& c,const Vector3& d,const float& t){


    Vector3 ab,bc,cd,abbc,bccd,dest;
    ab = Vector3::lerp(a,b,t);
    bc = Vector3::lerp(b,c,t);
    cd = Vector3::lerp(c,d,t);
    abbc = Vector3::lerp(ab,bc,t);
    bccd = Vector3::lerp(bc,cd,t);
    dest = Vector3::lerp(abbc,bccd,t);
    points.push_back(dest);
//    cout<<dest<<endl;
//    cout<<"push into vector?"<<points.size()<<endl;
}


void Bcurve::computeBezier5points( vector<Vector3>&points, const Vector3& a,const Vector3& b,const Vector3& c,const Vector3& d, const Vector3& e,const float& t){

    float p = 1.f -t;
    Vector3 dest;
    dest = pow(p,4)*a+4*t*pow(p,3)*b+6*t*t*pow(p,2)*c+4*p*pow(t,3)*d+pow(t,4)*e;
//    Vector3 ab,bc,cd,de,abbc,bccd,cdde, abbcbccd, bccdcdde, dest;

//    ab = Vector3::lerp(a,b,t);
//    bc = Vector3::lerp(b,c,t);
//    cd = Vector3::lerp(c,d,t);
//    de = Vector3::lerp(d,e,t);
//    abbc = Vector3::lerp(ab,bc,t);
//    bccd = Vector3::lerp(bc,cd,t);
//    cdde = Vector3::lerp(cd,de,t);
//    abbcbccd = Vector3::lerp(abbc,bccd,t);
//    bccdcdde = Vector3::lerp(bccd,cdde,t);

//    dest = Vector3::lerp(abbcbccd,bccdcdde,t);

    points.push_back(dest);
//    cout<<dest<<endl;
//    cout<<"push into vector?"<<points.size()<<endl;
}
