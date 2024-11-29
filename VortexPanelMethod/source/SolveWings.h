#pragma once
#include "Wing.h"
using namespace std;

class SolveWings {
	
public:
	int solverType;
	point velocity;
	SolveWings(vector<Wing>& wings);
	void setAirflow(double _velocity, double angle);
	void solve();
	point getVelocity(point x);   
	void setParametrs(int n, double m, double p, double t);
	void changeParametrs(int dn, double dm, double dp, double dt,double dangle,point dpos, int wingnum);
	void setAngle(double angle);
	vector<vector<double>> getInfluenceMatrix();
	vector<double> getRightPart();

	vector<Wing> wings;
private:
	
	

};