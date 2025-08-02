#include "WindowManager.h"

#include "SolveWings.h"
int main()
{

   /* vector<Wing> wings(5);
    wings[0].setWingAngle(-15);
    wings[1].setWingAngle(-10);
    wings[2].setWingAngle(0);
    wings[3].setWingAngle(10);
    wings[4].setWingAngle(15);
    wings[0].setWingPosition({ -0.3,0.4 });
    wings[1].setWingPosition({ 0,0.3 });
    wings[2].setWingPosition({ 0.1,0 });
    wings[3].setWingPosition({ 0,-0.3 });
    wings[4].setWingPosition({ -0.3,-0.4 });
    int n = 101;
    wings[0].setGeometryParametrs(15, 4, 12);
    wings[1].setGeometryParametrs(5, 4, 14);
    wings[2].setGeometryParametrs(0, 0, 18);
    wings[3].setGeometryParametrs(-5, 4, 14);
    wings[4].setGeometryParametrs(-15, 4, 12);*/


    vector<Wing> wings(1);

    wings[0].setWingPosition({0,0 });
    //wings[1].setWingPosition({0,-0.3});
    int n = 101;

    for (auto& w : wings) {
        w.setN(n);
        w.updateGeometry();
    }
        

    
    
    WindowManager wm(wings);
    wm.show();
    return 0;
}

