#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include "Panel.h"
#include "Point.h"
#include "solver.h"
class Wing {
public:
	vector<Panel> panels;  
    double Gamma;           // circulation value
    double Cy;              // dimensionless lift
    double n;
    double m,p,t;
    double wingAngel; 
    point position;
    
    Wing();
    Wing(int n, double m, double p, double t,double angle,point pos);            
    void setWingAngle(double angle);  // in gardus
    void setWingPosition(point pos);
    void setN(int n);
    void changeParametrs(int dn, double dm, double dp, double dt, double dangle, point dpos);
    void setGeometryParametrs(double m, double p, double t);
    void updateGeometry();     // m p t -> NACA0000, n*2 - number of panels
    
                                                                      // return velocity at point
    vector<point> getPressure(point vel ,bool up);                             // return array of pressure ​​on the upper or lower surface
private:
                           
    double NACAwing(double x, bool up);     // wing shape function for upper and lower surface
    vector<point> getWingCoord();      // return array of wing points
};