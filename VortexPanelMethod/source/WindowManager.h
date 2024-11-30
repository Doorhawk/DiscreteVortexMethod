#pragma once
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "Wing.h"
#include "Panel.h"
#include "SolveWings.h"

using namespace sf;
using namespace std;
class WindowManager {
public:
    WindowManager(vector<Wing> wings);
    void show();
private:
    void drawInfFrame();
    void drowWingInf();
    void drawFrame(); 
    void drawWing();     
    void drawStreamlines(); 
    void drawPressure(); 
    void drawAnglePlot(); 
    void controlButtons(Event event); 
    void drawText();
    void printStartInf();
    void createStreamlines();
    void changeWingParametrs(int dn, double dm, double dp, double dt, double dangle, point dpos);
    double m;
    double p;
    double t;
    int n;
    double vel;
    double angle;         // wing angle for draw, it changes by pressing buttons  
    double solvedangle;   // wing angle at last solving
    int wingNum;
    SolveWings wi;
    Wing wing;
    vector<vector<point>> streamlines;
    vector<vector<point>> pressureUp;
    vector<vector<point>> pressureDown;
    vector<point> anglePlot;
    RenderWindow window;
    RenderWindow windowInf;
    int wHeight; 
    int wWidth;
    bool areStreamlines;
    bool areMoveStreamlines;
    bool isPresure;
    bool isAnglePlot;
    bool isHelpText;
    int size—oeff;      // zoom
    int startLine;      // for streamlines animation
    int endLine;        // 
    Font font;
};