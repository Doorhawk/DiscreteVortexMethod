#include "SolveWings.h"


SolveWings::SolveWings(vector<Wing>& wings):wings(wings),solverType(3),velocity(point(1,0)) {

}
void SolveWings::setAirflow(double _velocity, double angle) {
	velocity = { _velocity * cos(angle * acos(-1) / 180),_velocity * sin(angle * acos(-1) / 180) };
}
void SolveWings::solve() {
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
        for (int i = 0, mi = 0, sizei = wings.size(); i < sizei; i++) {
            x[mi] = 0;
            for (int k = 0, sizek = wings[i].panels.size(); k < sizek; k++, mi++) {
            }
            x[mi] = 0;
            mi++;
        }

        for (int i = 0, mi = 0, sizei = wings.size(); i < sizei; i++) {
            double G = 0;
            for (int k = 0, sizek = wings[i].panels.size(); k < sizek; k++, mi++) {
                G-=wings[i].panels[k].intensity = x[mi];
            }
            mi++;
            wings[i].Gamma = G;
            wings[i].Cy = G * 2 / velocity.abs() / 1;
        }
    }


    
}
void SolveWings::setParametrs(int n, double m, double p, double t) {

    for (auto& w : wings)
        w.setWingPatametrs(n, m, p, t);

}
void SolveWings::setAngle(double angle) {
    for (auto& w : wings)
        w.setWingAngle(angle);
}
point SolveWings::getVelocity(point x) {
    double Vx = 0;
    double Vy = 0;
    for (int i = 0, sizei = wings.size(); i < sizei; i++) {
        for (int k = 0, sizek = wings[i].panels.size(); k < sizek; k++) {
            double r = (x - wings[i].panels[k].vortex).abs();
            Vx += wings[i].panels[k].intensity / 2 / acos(-1) * (-x.y + wings[i].panels[k].vortex.y) / (r * r);
            Vy += wings[i].panels[k].intensity / 2 / acos(-1) * (x.x - wings[i].panels[k].vortex.x) / (r * r);
        }
    }
    return { Vx + velocity.x,Vy + velocity.y };
}
vector<vector<double>> SolveWings::getInfluenceMatrix() {
    int sizeMatrix = 0;
    for (auto& w : wings)
        sizeMatrix += w.panels.size()+1;

    vector<vector<double>> matrix(sizeMatrix, vector<double>(sizeMatrix,0.0));


    int mi = 0, mj = 0;
    for (int i = 0, sizei = wings.size(); i < sizei; i++) {
        for (int k = 0,sizek = wings[i].panels.size(); k < sizek; k++,mi++) {
            mj = 0;
            for (int j = 0, sizej = wings.size(); j < sizej; j++) {
                for (int l = 0, sizel = wings[j].panels.size(); l < sizel; l++,mj++) {

                    double r = (wings[i].panels[k].control - wings[j].panels[l].vortex).abs();
                    double unx = (wings[i].panels[k].control.y - wings[j].panels[l].vortex.y) * wings[i].panels[k].norm.x;
                    double vny = (wings[i].panels[k].control.x - wings[j].panels[l].vortex.x) * wings[i].panels[k].norm.y;

                    matrix[mi][mj] = 1 / (2 * acos(-1) * r) * (unx - vny) / r;
                }
                matrix[mi][mj] = matrix[mi][mj- wings[j].panels.size()];
                mj++;
            }
        }
    }

    

    for (int i = 0, kot = 0, sizei = wings.size(); i < sizei; i++) {
        
        matrix[mi][kot] = matrix[mi][kot + wings[i].panels.size()] = 1;
        kot += wings[i].panels.size() + 1;
        mi++;
    }




    return matrix;
}
vector<double> SolveWings::getRightPart() {
    int sizeB = 0;
    for (auto& w : wings)
        sizeB += w.panels.size()+1;
    vector<double> rhs(sizeB,0);

    int mi = 0;
    for (int i = 0, sizei = wings.size(); i < sizei; i++) {
        for (int k = 0, sizek = wings[i].panels.size(); k < sizek; k++, mi++) {
            rhs[mi] = wings[i].panels[k].norm.x * velocity.x + wings[i].panels[k].norm.y * velocity.y;
        }
    }


    return rhs;
}