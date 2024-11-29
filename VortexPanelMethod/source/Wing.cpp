#include "Wing.h"
using namespace std;
using namespace std::chrono;

double oval(double x, bool up) {
    // returns circle coordinates
    if (x <= 0)
        return 0;
    if (x >= 1)
        return 0;

    if (up)
        return sqrt(1 - (2 * x - 1) * (2 * x - 1)) / 2;// +sin(30 * (x - 1)) / 20;
    else
        return -sqrt(1 - (2 * x - 1) * (2 * x - 1)) / 2;// +sin(30 * (x - 1)) / 20;
}

Wing::Wing(int n, double m, double p, double t,double angle, point pos):position(pos),n(n),m(m),p(p),t(t),wingAngel(angle){
    
    updateGeometry();
    Gamma = 0;
    Cy = 0;
}
Wing::Wing():position(point(0,0)){
    m = 0;
    p = 0;
    t = 12;
    n = 101;
    wingAngel = 0;
    updateGeometry();
    Gamma = 0;
    Cy = 0;
}


void Wing::setWingAngle(double angle) {
    wingAngel = angle;
}
void Wing::setWingPosition(point pos) {
    position = pos;
}
void Wing::setN(int _n) {
    if (n <= 0) 
        n = 1;
    else 
        n = _n;
}
void Wing::setGeometryParametrs(double _m, double _p, double _t) {
    m = _m;
    p = _p;
    t = _t;
}
void Wing::changeParametrs(int dn, double dm, double dp, double dt, double dangle, point dpos) {
    n += dn;
    if (n <= 0)
        n = 1;
    m += dm;
    p += dp;
    t += dt;
    wingAngel += dangle;
    position = position + dpos;
}
void Wing::updateGeometry() {
    panels.clear();
    vector<point> vec = getWingCoord();
    for (size_t i = 0, size = vec.size() - 1; i < size; i++) {
        panels.push_back(Panel(vec[i], vec[i + 1]));
    }
    panels.push_back(Panel(vec[vec.size() - 1], vec[0]));
}


vector<point> Wing::getPressure(point velocity,bool up) {
    vector<point> vec(panels.size() / 2 - 1);
    if (up) {
        for (size_t i = 0, size = panels.size() / 2 - 1; i < size; i++) {
            double l = (panels[i].vortex - panels[i + 1].vortex).abs();
            vec[i] = { panels[i].control.x,
                1 - ((panels[i].intensity + panels[i + 1].intensity) / 2 / l / velocity.abs())
                  * ((panels[i].intensity + panels[i + 1].intensity) / 2 / l / velocity.abs()) };
        }
    }
    else {
        for (size_t i = panels.size() / 2, size = panels.size() - 1; i < size; i++) {
            double l = (panels[i].vortex - panels[i + 1].vortex).abs();
            vec[i - panels.size() / 2] = { panels[i].control.x,
                1 - ((panels[i].intensity + panels[i + 1].intensity) / 2 / l / velocity.abs())
                  * ((panels[i].intensity + panels[i + 1].intensity) / 2 / l / velocity.abs()) };
        }
    }
    return vec;
}


double Wing::NACAwing(double x, bool up) {
    double _m = m / 100.;
    double _p = p / 10.;
    double _t = t / 100.;
    if (x <= 0)
        return 0;
    if (x >= 1)
        return 0;
    double yt = 5 * _t * (0.298222773 * sqrt(x) - 0.127125232 * x - 0.357907906 * x * x + 0.291984971 * x * x * x - 0.105174606 * x * x * x * x);
    double yc = 0;
    if (x >= 0 && x <= _p)
        yc =_m / _p / _p * (2 * _p * x - x * x);
    else
        yc = _m / (1 - _p) / (1 - _p) * ((1 - 2 * _p) + 2 * _p * x - x * x);
    if (up)
        return yc + yt;
    else
        return yc - yt;

}
vector<point> Wing::getWingCoord() {
    vector<point> vec;
    double x = 0;
    for (int i = n; i >= 0; i--) {
        x = (1 - cos(i * acos(-1) / n)) / 2; 
        //vec.push_back({ x,oval(x,true) });
        vec.push_back({ x,NACAwing(x, true) });
    }
    for (int i = 1; i < n; i++) {
        x = (1 - cos(i * acos(-1) / n)) / 2;
        //vec.push_back({ x,oval(x,false) }); // draw circle
        vec.push_back({ x,NACAwing(x, false) });
    }
    // geometry rotation
    if (wingAngel != 0) {
        double rad = -wingAngel * acos(-1) / 180;
        for (auto& v : vec) {
            v = { v.x * cos(rad) - v.y * sin(rad),v.y * cos(rad) + v.x * sin(rad) };
        }
    }
    //vec = { {1,1},{0,1},{0,0},{1,0} };

    for (auto& v : vec) {
        v = v+position;
    }
    return vec;
}

