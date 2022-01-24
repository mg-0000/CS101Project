#include<simplecpp>
#include"game.h"

int main(){
    initCanvas();
    
    Line t1(0, (2*TOP_MARGIN), WINDOW_X, (2*TOP_MARGIN));
    t1.setColor(COLOR(0, 0, 255));
    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    int level1_result=-1,level2_result=-1,level3_result=-1;

    cout<<"begin"<<endl;
    Game level1(2,BUBBLE_DEFAULT_RADIUS,3,30,COLOR(56,36,36),'g',BUBBLE_DEFAULT_VX,BUBBLE_DEFAULT_AY);
    cout<<"level 1"<<endl;
    level1_result=level1.main_game_loop();
    level1.~Game();
    if(level1_result==0)
    {
        cout<<"Game over";
        return 0;
    }
    else{
        cout<<"level 2"<<endl;
        Game level2(3,BUBBLE_DEFAULT_RADIUS,3,30,COLOR(56,36,36),'b',BUBBLE_DEFAULT_VX,BUBBLE_DEFAULT_AY);
        level2_result=level2.main_game_loop();
        level2.~Game();
        if(level2_result==0){
            cout<<"game over"<<endl;
            return 0;
        }    
        else{
            cout<<"level 3"<<endl;
            Game level3(4,BUBBLE_DEFAULT_RADIUS,3,30,COLOR(56,36,36),'r',BUBBLE_DEFAULT_VX,BUBBLE_DEFAULT_AY);
            level3_result=level3.main_game_loop();
            level3.~Game();
            if(level3_result==0){
                cout<<"You won!!";
                return 0;
            }
            else{
                cout<<"You lost!!";
                return 0;
            }
        }
    }
}