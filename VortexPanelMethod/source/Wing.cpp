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

Wing::Wing(int n, double m, double p, double t) {
    solverType = 3;
    vector<point> vec = getWingCoord(n, m, p, t);
    for (size_t i = 0, size = vec.size() - 1; i < size; i++) {
        panels.push_back(Panel(vec[i], vec[i + 1]));
    }
    panels.push_back(Panel(vec[vec.size() - 1], vec[0]));
    velocity = { 1,0 };
    Gamma = 0;
    Cy = 0;
}
Wing::Wing() {
    double m = 0;
    double p = 0;
    double t = 12;
    int n = 101;
    solverType = 3;
    vector<point> vec = getWingCoord(n, m, p, t);
    for (size_t i = 0, size = vec.size() - 1; i < size; i++) {
        panels.push_back(Panel(vec[i], vec[i + 1]));
    }
    panels.push_back(Panel(vec[vec.size() - 1], vec[0]));
    velocity = { 1,0 };
    Gamma = 0;
    Cy = 0;
}

void Wing::setAirflow(double _velocity, double angle) {
        velocity = { _velocity * cos(angle * acos(-1) / 180),_velocity * sin(angle * acos(-1) / 180) };
}
void Wing::setWingAngle(double angle) {
        wingAngel = angle;
}
void Wing::setWingPatametrs(int n, double m, double p, double t) {
    if (n <= 0) {
        n = 1;
    }
    panels.clear();
    vector<point> vec = getWingCoord(n, m, p, t);
    for (size_t i = 0, size = vec.size() - 1; i < size; i++) {
        panels.push_back(Panel(vec[i], vec[i + 1]));
    }
    panels.push_back(Panel(vec[vec.size() - 1], vec[0]));
}
void Wing::solve() {
    vector<vector<double>> matA = getInfluenceMatrix();
    vector<double> vecB = getRightPart();
    vector<double> x;
    switch (solverType)
    {
    case 1:
        x = Solver::GaussianMethod(matA, vecB);
        break;
    case 2:
        x = Solver::LUdecomposition(matA, vecB);
        break;
    case 3:
        x = Solver::QRdecomposition(matA, vecB);
        break;
    default:
        break;
    }
    if (x.size()) {
        x[0] = 0;
        Gamma = 0;
        for (size_t i = 0; i < panels.size(); i++) {
            panels[i].intensity = x[i];
            Gamma -= panels[i].intensity;
        }
    }
    Cy = Gamma * 2 / velocity.abs() / 1;
   
}

point Wing::getVelocity(point x) {
    double Vx = 0;
    double Vy = 0;
    size_t size = panels.size();
    for (size_t i = 0; i < size; i++) {
        double r = (x - panels[i].vortex).abs();
        Vx += panels[i].intensity / 2 / acos(-1) * (-x.y + panels[i].vortex.y) / (r * r);
        Vy += panels[i].intensity / 2 / acos(-1) * (x.x - panels[i].vortex.x) / (r * r);
    }
    return { Vx + velocity.x,Vy + velocity.y };
}
vector<point> Wing::getPressure(bool up) {
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
vector<vector<double>> Wing::getInfluenceMatrix() {
    size_t size = panels.size();
    vector<vector<double>> matrix(size + 1);
    for (size_t i = 0; i < size; i++) {
        matrix[i].resize(size + 1);
        for (int j = 0; j < size; j++) {
            double r = (panels[i].control - panels[j].vortex).abs();
            double unx = (panels[i].control.y - panels[j].vortex.y) * panels[i].norm.x;
            double vny = (panels[i].control.x - panels[j].vortex.x) * panels[i].norm.y;
            matrix[i][j] = 1 / (2 * acos(-1) * r) * (unx - vny) / r;
        }
        matrix[i][size] = matrix[i][0];
    }
    matrix[size].resize(size + 1);
    matrix[size][0] = 1;
    matrix[size][size] = 1;
    return matrix;
}
vector<double> Wing::getRightPart() {
    size_t size = panels.size();
    vector<double> rhs(size + 1);
    for (size_t i = 0; i < size; i++) {
        rhs[i] = panels[i].norm.x * velocity.x + panels[i].norm.y * velocity.y;
    }
    rhs[size] = 0;
    return rhs;
}
double Wing::NACAwing(double x, double _m, double _p, double _t, bool up) {
    double m = _m / 100.;
    double p = _p / 10.;
    double t = _t / 100.;
    if (x <= 0)
        return 0;
    if (x >= 1)
        return 0;
    double yt = 5 * t * (0.298222773 * sqrt(x) - 0.127125232 * x - 0.357907906 * x * x + 0.291984971 * x * x * x - 0.105174606 * x * x * x * x);
    double yc = 0;
    if (x >= 0 && x <= p)
        yc = m / p / p * (2 * p * x - x * x);
    else
        yc = m / (1 - p) / (1 - p) * ((1 - 2 * p) + 2 * p * x - x * x);
    if (up)
        return yc + yt;
    else
        return yc - yt;

}
vector<point> Wing::getWingCoord(int n, double m, double p, double t) {
    vector<point> vec;
    double x = 0;
    for (int i = n; i >= 0; i--) {
        x = (1 - cos(i * acos(-1) / n)) / 2; 
        //vec.push_back({ x,oval(x,true) });
        vec.push_back({ x,NACAwing(x, m, p, t, true) });
    }
    for (int i = 1; i < n; i++) {
        x = (1 - cos(i * acos(-1) / n)) / 2;
        //vec.push_back({ x,oval(x,false) }); // draw circle
        vec.push_back({ x,NACAwing(x, m, p, t, false) });
    }
    // geometry rotation
    if (wingAngel != 0) {
        double rad = -wingAngel * acos(-1) / 180;
        for (auto& v : vec) {
            v = { v.x * cos(rad) - v.y * sin(rad),v.y * cos(rad) + v.x * sin(rad) };
        }
    }
    return vec;
}

