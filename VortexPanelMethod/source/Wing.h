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
    point velocity;         // flow velocity
    double Gamma;           // circulation value
    double Cy;              // dimensionless lift
    double wingAngel;       
    int solverType;          
    

    Wing();
    Wing(int n, double m, double p, double t);
    void setAirflow(double _velocity, double angle);                // angle in gardus
    void setWingAngle(double angle);                                // in gardus
    void setWingPatametrs(int n, double m, double p, double t);     // m p t -> NACA0000, n*2 - number of panels
    void solve();                                                   
        
    point getVelocity(point x);                                     // return velocity at point
    vector<point> getPressure(bool up);                             // return array of pressure ​​on the upper or lower surface
private:
   
    vector<vector<double>> getInfluenceMatrix();            
    vector<double> getRightPart();                          
    double NACAwing(double x, double m, double p, double t, bool up);     // wing shape function for upper and lower surface
    vector<point> getWingCoord(int n, double m, double p, double t);      // return array of wing points
};