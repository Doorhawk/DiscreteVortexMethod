#pragma once
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Wing.h"
#include "Panel.h"
using namespace sf;
using namespace std;
class WindowManager {
public:
    WindowManager();
    void show();
private:
    void drawFrame(); 
    void drawWing();     
    void drawStreamlines(); 
    void drawPressure(); 
    void drawAnglePlot(); 
    void controlButtons(Event event); 
    void printStartInf();
    void createStreamlines();
    double m;
    double p;
    double t;
    int n;
    double vel;
    double angle;         // wing angle for draw, it changes by pressing buttons  
    double solvedangle;   // wing angle at last solving
    Wing wing;
    vector<vector<point>> streamlines;
    vector<point> pressureUp;
    vector<point> pressureDown;
    vector<point> anglePlot;
    RenderWindow window;
    int wHeight; 
    int wWidth;
    bool areStreamlines;
    bool isPresure;
    bool isAnglePlot;
    bool isHelpText;
    int size—oeff;      // zoom
    int startLine;      // for streamlines animation
    int endLine;        // 
};