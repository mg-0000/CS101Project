#include <simplecpp>
#include<stdlib.h>
#include "shooter.h"
#include"bubble.h"
//includeing bullet.h here isnt required as it is already included in shooter.h


/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);

//for color combination of bubbles and background
struct color_combo{
    Color bubble_color;
    char background_char;
    int background_c1,background_c2;
};

//simple pow function to prevent problems with double to int conversion
int simple_pow(int a,int b)
{
    int simplepow=1;
    for(int i=0;i<b;i++)
    {
        simplepow*=a;
    }
    return simplepow;
}

//set the background
//main_color is red, blue or green
void set_background(char main_color,int c1,int c2){
    int upper_bound=40; //upper border
    int lower_bound=PLAY_Y_HEIGHT;    //lower border
    
    Rectangle r1(0,upper_bound+2,1000,2);   //rectangle which covers two entire rows of pixels
    r1.setFill(true);
    
    //iterate through each row in the canvas, coloring it.
    while(r1.getY()>=(upper_bound+2) && r1.getY()<=lower_bound)
    {
        if(main_color=='b')     r1.setColor(COLOR(c1,c2,(r1.getY()/2)));
        else if(main_color=='r')    r1.setColor(COLOR(((r1.getY()/2)),c1,c2));
        else if(main_color=='g')    r1.setColor(COLOR(c1,((r1.getY()/2)),c2));
        r1.imprint();
        r1.move(0,2);
    }
    
}

void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles(int no_of_bubbles,double rad,double vx,Color bubble_color,double ay)
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    for(int i=0;i<no_of_bubbles;i++)
    {
        bubbles.push_back(Bubble((WINDOW_X*(i+1)/(no_of_bubbles+1)),BUBBLE_START_Y,rad,simple_pow(-1,i)*vx,BUBBLE_DEFAULT_VY,bubble_color,ay));
    }
    
    return bubbles;
}

void check_for_collision(vector<Bubble> &bubbles , vector<Bullet> &bullets , Shooter &shooter, Text &score, double bubble_vx,Color bubble_color, double bubble_ay){
    //check for collision between all bubbles and bullets
    for (unsigned int i=0; i<bullets.size(); i++){
        for(unsigned int j=0; j<bubbles.size(); j++){
            //check for collision.
            //collision occurs if the x/y distance between the centres of rectangle and circle is lesser than or equal to the sum of half of corresponding width/height and radius
            if(
                (
                    abs(bubbles[j].get_center_x()-bullets[i].get_center_x())<=(bubbles[j].get_radius()+(bullets[i].get_width()/2))  //x axis check
                )
                &&
                (
                    abs(bubbles[j].get_center_y()-bullets[i].get_center_y())<=(bubbles[j].get_radius()+(bullets[i].get_height()/2))   //y axis check
                )
            )
                {
                    bullets.erase(bullets.begin()+i);
                    if(bubbles[j].get_radius()<=BUBBLE_RADIUS_THRESHOLD)
                    {
                        bubbles.erase(bubbles.begin()+j);
                    }
                    else
                    {
                        double cx=bubbles[j].get_center_x();
                        double cy=bubbles[j].get_center_y();
                        int rad=bubbles[j].get_radius()/2;
                        bubbles.erase(bubbles.begin()+j);
                        bubbles.push_back(Bubble(cx,cy,rad,bubble_vx,0,bubble_color,bubble_ay));
                        bubbles.push_back(Bubble(cx,cy,rad,-bubble_vx,0,bubble_color,bubble_ay));
                    }

                    shooter.score++;
                    score.reset(((WINDOW_X/2)+textWidth("SCORE:")), TOP_MARGIN, shooter.score);
                }
        }
    }
}

//check for collision between shooter and bubble
void check_for_collision_2(Shooter &shooter, vector<Bubble> &bubbles, Text &lives){
    bool check_whether_to_change_color=true;    //to determine when to change shooter color back to default
    for(unsigned int i=0;i<bubbles.size();i++){

            //check for collision
            //collision occurs if the x/y distance between the centres of rectangle and circle is lesser than or equal to the sum of half of corresponding width/height and radius
            if(
                (
                    (abs(bubbles[i].get_center_x()-(shooter.get_body_center_x()))<=(bubbles[i].get_radius()+(shooter.get_body_width()/2)))  //x axis check for shooter bodu
                &&
                    (abs(bubbles[i].get_center_y()-shooter.get_body_center_y())<=(bubbles[i].get_radius()+(shooter.get_body_height()/2)))   //y axis check for shooter body
                )
                ||
                (
                    (abs(bubbles[i].get_center_x()-shooter.get_head_center_x())<=bubbles[i].get_radius()+shooter.get_head_radius()) //x axis check for shooter head
                &&
                    (abs(bubbles[i].get_center_y()-shooter.get_head_center_y())<=bubbles[i].get_radius()+shooter.get_head_radius()) //x axis check for shooter body
                )
            )
            {
                //code if in collision
                if(bubbles[i].in_collision==false)
                {
                    bubbles[i].in_collision=true;
                    shooter.lives_left--;
                    lives.reset((WINDOW_X-textWidth("   ")),TOP_MARGIN,shooter.lives_left);
                    shooter.set_color(COLOR("black"));
                }
            }
            else
            {
                if(bubbles[i].in_collision==true)
                {
                    bubbles[i].in_collision=false;
                }
            }
            if(bubbles[i].in_collision==true)   check_whether_to_change_color=false;
    }
    //change color if all bubbles are not in_collision and shooter current color is black
    if((check_whether_to_change_color==true) and (shooter.get_color()==COLOR("black")))  shooter.set_color(SHOOTER_DEFAULT_COLOR);

}


//implementation of levels
class Game
{
private:
    int no_of_bubbles;  //for function create bubbles
    int initial_lives;  //for shooter constructor
    int game_time;  //for shooter constructor
    Color color_of_bubble;    //for bubble contructor
    double vx_of_bubble;    //for bubble constructor
    double ay_of_bubble;    //for bubble constructor
    double bubble_radius;   //for bubble constructor
    int temp_timer_var=0;       //for updating the timer

    //background color vars
    char main_background_color;
    int background_c1,background_c2;

    //information displayed on screen
    string msg_cmd;
    Text charPressed;
    string msg_time;
    Text timer;
    string msg_score;
    Text score_msg;
    Text timer_countdown;
    Text score;
    Text health;
    Text lives_left;
    Text level_msg;
    Text level;

    //shooter
    Shooter shooter;

    //bubbles
    vector<Bubble> bubbles;

    //bullets
    vector<Bullet> bullets;

    

public:
    //game cinstructor
    Game(struct color_combo color_combo_,char level_,int no_of_bubbles_=BUBBLE_DEFAULT_NO,double bubble_rad_=BUBBLE_DEFAULT_RADIUS,int initial_lives_=3, int game_time_=30,  double vx_of_bubble_=BUBBLE_DEFAULT_VX, double ay_of_bubble_=BUBBLE_DEFAULT_AY)
    {
        no_of_bubbles=no_of_bubbles_;
        initial_lives=initial_lives_;
        game_time=game_time_;
       
        vx_of_bubble=vx_of_bubble_;
        ay_of_bubble=ay_of_bubble_;
        bubble_radius=bubble_rad_;
        //colors
        color_of_bubble=color_combo_.bubble_color;
        main_background_color=color_combo_.background_char;
        background_c1=color_combo_.background_c1;
        background_c2=color_combo_.background_c2;

        //set the background as a color palette.
        //possible arguments are 'r'-red 'b'-blue 'g'-green
        set_background(main_background_color,background_c1,background_c2);

        // Intialize the shooter
        shooter=Shooter(SHOOTER_START_X, SHOOTER_START_Y,initial_lives,game_time);

        // Initialize the bubbles
        bubbles = create_bubbles(no_of_bubbles,bubble_radius,vx_of_bubble,color_of_bubble,ay_of_bubble);

        //initialise the information to be displayed in the screen
        msg_cmd="Cmd: _";
        charPressed=Text(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);
        level_msg=Text(WINDOW_X/2,BOTTOM_MARGIN,"LEVEL:");
        level=Text((WINDOW_X/2)+textWidth("LEVEL:"),BOTTOM_MARGIN,level_);
        msg_time="TIME:";
        timer=Text(LEFT_MARGIN, TOP_MARGIN, msg_time);
        msg_score="SCORE:";
        score_msg=Text((WINDOW_X/2), TOP_MARGIN, msg_score);
        timer_countdown=Text((LEFT_MARGIN+textWidth("TIME:")), TOP_MARGIN, INITIAL_TIME);
        score=Text(((WINDOW_X/2)+textWidth("SCORE:")), TOP_MARGIN, INITIAL_SCORE);
        health=Text((WINDOW_X-textWidth("LIVES LEFT:    ")), TOP_MARGIN, "LIVES LEFT:");
        lives_left=Text((WINDOW_X-textWidth("   ")),TOP_MARGIN,shooter.lives_left);

        

    }

    //destructor
    ~Game(){
    }

    //main game loop
    int main_game_loop(){
        XEvent event;

        // Main game loop
        while (true)
        {
            //update the timer
            //one sec passes after 30 repetations based on step time=0.02. However, some time is taken for computations too.
            //25 repetations has been arrived by trial and error and is not accurate.
            temp_timer_var++;
            if(temp_timer_var==25){
                temp_timer_var=0;
                shooter.time--;
                timer_countdown.reset((LEFT_MARGIN+textWidth("TIME:")), TOP_MARGIN, shooter.time);
            }

            bool pendingEvent = checkEvent(event);
            if (pendingEvent)
            {

                // Get the key pressed
                char c = charFromEvent(event);
                msg_cmd[msg_cmd.length() - 1] = c;
                charPressed.setMessage(msg_cmd);

                // Update the shooter
                if(c == 'a')
                    shooter.move(STEP_TIME, true);
                else if(c == 'd')
                    shooter.move(STEP_TIME, false);
                else if(c == 'w')
                    bullets.push_back(shooter.shoot());
                else if(c == 'q')
                    exit(0);
            }

            // Update the bubbles
            move_bubbles(bubbles);

            // Update the bullets
            move_bullets(bullets);

            //check for hits
            check_for_collision(bubbles,bullets,shooter, score,vx_of_bubble,color_of_bubble,ay_of_bubble);

            //CHECK FOR COLLISION BETWEEN SHOOTER AND BUBBLE
            check_for_collision_2(shooter,bubbles,lives_left);

            

            //check whether game is over
            if((shooter.time==0)    ||  (shooter.lives_left==0))
            {
                Text msg(WINDOW_X/2,WINDOW_Y/2,"YOU LOSE.");
                msg.scale(2);
                msg.setColor(COLOR("red"));
                msg.setFill(true);
                wait(2);
                return 0;
            }
            else if(bubbles.size()==0){
                Text msg(WINDOW_X/2,WINDOW_Y/2,"YOU WIN!!");
                msg.scale(2);
                msg.setColor(COLOR("blue"));
                msg.setFill(true);
                wait(2);
                return 1;
            }
            

            wait(STEP_TIME);
        }
    }

    


};