#pragma once
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;

class Solver {
public:
    static vector<double> GaussianMethod(vector<vector<double>>& matA, vector<double>& vecB) {
        cout <<"Start solving: ";
        size_t n = matA.size();
        if (n != vecB.size()) {
            throw std::invalid_argument("matrix Error");
        }
        for (int i = 0; i < n; ++i) {
            if (n > 10 && i % (n / 10) == 0) {
                cout << i / (n / 10)*10 <<"% ";
            }
            int maxRow = i;
            for (int k = i + 1; k < n; ++k) {
                if (std::abs(matA[k][i]) > std::abs(matA[maxRow][i])) {
                    maxRow = k;
                }
            }
            std::swap(matA[i], matA[maxRow]);
            std::swap(vecB[i], vecB[maxRow]);
            if (std::abs(matA[i][i]) < 1e-20) {
                cout << endl << "matrix ERROR" << endl;
            }
            for (int k = i + 1; k < n; ++k) {
                double coeff = matA[k][i] / matA[i][i];
                for (int j = i; j < n; ++j) {
                    matA[k][j] -= coeff * matA[i][j];
                }
                vecB[k] -= coeff * vecB[i];
            }
        }
        vector<double> x(n);
        for (int i = n - 1; i >= 0; --i) {
            x[i] = vecB[i] / matA[i][i];

            for (int j = i - 1; j >= 0; --j) {
                vecB[j] -= matA[j][i] * x[i];
            }
        }
        cout << endl;
        return x;
    }
    static vector<double> QRdecomposition(vector<vector<double>>& matA, vector<double>& vecB) {
        cout << endl << "Start solving" << endl;
        Eigen::MatrixXd A(matA.size(), matA.size());
        for (size_t i = 0; i < matA.size(); i++) {
            for (size_t j = 0; j < matA.size(); j++) {
                A(i, j) = matA[i][j];
            }
        }
        Eigen::VectorXd b(vecB.size());
        for (size_t i = 0; i < vecB.size(); i++) {
            b[i] = vecB[i];
        }
        auto start = chrono::high_resolution_clock::now();

        Eigen::VectorXd res = A.colPivHouseholderQr().solve(b);

        auto end = chrono::high_resolution_clock::now();
        auto duration = duration_cast<chrono::seconds>(end - start).count();
        cout << endl << "solving time = " << duration << " sec" << endl;

        vector<double> x(res.data(), res.data() + res.size());
        return x;
    }
    static vector<double> LUdecomposition(vector<vector<double>>& matA, vector<double>& vecB) {
        cout << endl << "Start solving" << endl;
        Eigen::MatrixXd A(matA.size(), matA.size());
        for (size_t i = 0; i < matA.size(); i++) {
            for (size_t j = 0; j < matA.size(); j++) {
                A(i, j) = matA[i][j];
            }
        }
        Eigen::VectorXd b(vecB.size());
        for (size_t i = 0; i < vecB.size(); i++) {
            b[i] = vecB[i];
        }
        auto start = chrono::high_resolution_clock::now();

        Eigen::VectorXd res = A.partialPivLu().solve(b);

        auto end = chrono::high_resolution_clock::now();
        auto duration = duration_cast<chrono::seconds>(end - start).count();
        cout << endl << "solving time = " << duration << " sec" << endl;

        vector<double> x(res.data(), res.data() + res.size());
        return x;
    }
};