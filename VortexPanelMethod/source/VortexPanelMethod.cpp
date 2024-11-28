#include "WindowManager.h"

#include "SolveWings.h"
int main()
{

    vector<Wing> wings(5);


    wings[0].setWingAngle(10);
    wings[1].setWingAngle(0);
    wings[2].setWingAngle(-10);

    wings[0].setWingHeight(1);
    wings[1].setWingHeight(+0.5);
    wings[2].setWingHeight(0);
    wings[3].setWingHeight(-0.5);
    wings[4].setWingHeight(-1);
    

    //wings[0].setWingPatametrs(200, 0, 0, 12);
    //wings[1].setWingPatametrs(200, 0, 0, 12);

   

    
    
    WindowManager wm(wings);
    wm.show();
    return 0;
}

