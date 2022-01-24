#include<simplecpp>
#include"game.h"

//color combinations:
//b+(150,150,175)
//r+(55,126,176)
//g+(122,103,129)
//(80,130,blue)+(60,60,32)
//(red,67,54)+(29,201,153)
//(29,green,153)+(56,36,36)
const struct color_combo color1={(unsigned int)COLOR(150,150,175),'b',0,0},color2={(unsigned int)COLOR(55,126,176),'r',0,0},color3={(unsigned int)COLOR(122,103,129),'g',0,0},color4={(unsigned int)COLOR(60,60,32),'b',80,130},color5={(unsigned int)COLOR(29,201,153),'r',67,54},color6={(unsigned int)COLOR(56,36,36),'g',29,153};


int main(){
    initCanvas();
    
    Line t1(0, (2*TOP_MARGIN), WINDOW_X, (2*TOP_MARGIN));
    t1.setColor(COLOR(0, 0, 255));
    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    int level1_result=-1,level2_result=-1,level3_result=-1;

    Game level1(color4,1,2,BUBBLE_DEFAULT_RADIUS,3,30,BUBBLE_DEFAULT_VX,BUBBLE_DEFAULT_AY);
    level1_result=level1.main_game_loop();
    level1.~Game();
    if(level1_result==0)
    {
        Text msg(WINDOW_X/2,WINDOW_Y/2,"GAME OVER!!");
        msg.scale(2);
        msg.setColor(COLOR("red"));
        msg.setFill(true);
        wait(2);
        return 0;
    }
    else{
        Text msg(WINDOW_X/2,WINDOW_Y/2,"LEVEL 2");
        msg.scale(2);
        msg.setColor(COLOR("green"));
        msg.setFill(true);
        wait(2);
        msg.hide();
        Game level2(color5,2,3,BUBBLE_DEFAULT_RADIUS,3,30,BUBBLE_DEFAULT_VX,BUBBLE_DEFAULT_AY);
        level2_result=level2.main_game_loop();
        level2.~Game();
        if(level2_result==0){
            Text msg(WINDOW_X/2,WINDOW_Y/2,"GAME OVER!!");
            msg.scale(2);
            msg.setColor(COLOR("red"));
            msg.setFill(true);
            wait(2);
            return 0;
        }    
        else{
            Text msg(WINDOW_X/2,WINDOW_Y/2,"LEVEL 3");
            msg.scale(2);
            msg.setColor(COLOR("green"));
            msg.setFill(true);
            wait(2);
            msg.hide();
            Game level3(color6,3,4,BUBBLE_DEFAULT_RADIUS,3,30,BUBBLE_DEFAULT_VX,BUBBLE_DEFAULT_AY);
            level3_result=level3.main_game_loop();
            level3.~Game();
            if(level3_result==1){
                Text msg(WINDOW_X/2,WINDOW_Y/2,"GAME OVER");
                msg.scale(2);
                msg.setColor(COLOR("green"));
                msg.setFill(true);
                wait(2);
                return 0;
            }
            else{
                Text msg(WINDOW_X/2,WINDOW_Y/2,"GAME OVER");
                msg.scale(2);
                msg.setColor(COLOR("red"));
                msg.setFill(true);
                wait(2);
                return 0;
            }
        }
    }
}