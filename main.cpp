#include<simplecpp>
#include"game.h"

int main(){
    initCanvas();
    
    Line t1(0, (2*TOP_MARGIN), WINDOW_X, (2*TOP_MARGIN));
    t1.setColor(COLOR(0, 0, 255));
    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    cout<<"begin"<<endl;
    Game level1(2,BUBBLE_DEFAULT_RADIUS,3,30,BUBBLE_DEFAULT_COLOR,'r',BUBBLE_DEFAULT_VX,BUBBLE_DEFAULT_AY);
    cout<<"level 1 is done"<<endl;
    if(level_result==1)
    {
        level_result=-1;
        level1.~Game();
        Game level2(3,15,4,40,COLOR("blue"),'g',150.0,50.0);
        if(level_result==1)
        {
            level_result=-1;
            level2.~Game();
            Game level3(4,20,4,40,COLOR("black"),'b',200.0,100.0);
            level3.~Game();
            return 0;
        }
        else{
            level2.~Game();
            return 0;
        }
    }
    else{
        level1.~Game();
        return 0;
    }
}