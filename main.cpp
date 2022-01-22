#include <simplecpp>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);

//set the background
void set_background(Color main_color, Color arrow_color){
    Rectangle r1(0,0,20,20);
    r1.setColor(main_color);
    r1.setFill(true);
    int upper_bound=40;
    int lower_bound=460;
    int arrow_up=40;
    int arrow_down=180;
    int arrow_left=240-(arrow_down-arrow_up);
    int arrow_right=240+(arrow_down-arrow_up);

    int x1=0,y=upper_bound;
    while(x1<=500){
        while(y<=lower_bound){
            if(x1>=arrow_left && x1<=arrow_right)
            {
                if(y>=(arrow_up+abs(x1-240)) && y<=arrow_down)   r1.setColor(arrow_color);
                else if((x1-240==0) && y>=arrow_up)    r1.setColor(arrow_color);
                else    r1.setColor(main_color);
            }
            

            r1.moveTo((x1),(y));
            r1.imprint();
            y+=20;
        }
        y=upper_bound;
        x1+=20;

        r1.moveTo((x1),(y));
        r1.imprint();
    }
    wait(1);
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

vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180),BUBBLE_DEFAULT_AY));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180),BUBBLE_DEFAULT_AY));
    return bubbles;
}

void check_for_collision(vector<Bubble> &bubbles , vector<Bullet> &bullets , Shooter &shooter, Text &score){
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
                        bubbles.push_back(Bubble(cx,cy,rad,BUBBLE_DEFAULT_VX,0,COLOR(255,105,180),BUBBLE_DEFAULT_AY));
                        bubbles.push_back(Bubble(cx,cy,rad,-BUBBLE_DEFAULT_VX,0,COLOR(255,105,180),BUBBLE_DEFAULT_AY));
                    }

                    shooter.score++;
                    score.reset(((WINDOW_X/2)+textWidth("SCORE:")), TOP_MARGIN, shooter.score);
                }
        }
    }
}

//check for collision between shooter and bubble
void check_for_collision_2(Shooter &shooter, vector<Bubble> &bubbles, Text &lives){
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
                    shooter.set_color(COLOR(0,255,0));
                }
                
            }
    }
}


int main()
{
    int temp_timer_var=0;   //timer variable for updating the timer

    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);
    //possible colours:
    //main: (80,130,240)|(94,63,226)
    //arrow: (255,167,38)|(254,209,71)
    set_background(COLOR(94,63,226), COLOR(244,67,54));   //set the background

    Line t1(0, (2*TOP_MARGIN), WINDOW_X, (2*TOP_MARGIN));
    t1.setColor(COLOR(0, 0, 255));
    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y,INITIAL_HEALTH,INITIAL_TIME, SHOOTER_VX);

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);
    string msg_time("TIME:");
    Text timer(LEFT_MARGIN, TOP_MARGIN, msg_time);
    Text timer_countdown((LEFT_MARGIN+textWidth("TIME:")), TOP_MARGIN, INITIAL_TIME);
    string msg_score("SCORE:");
    Text score_msg((WINDOW_X/2), TOP_MARGIN, msg_score);
    Text score(((WINDOW_X/2)+textWidth("SCORE:")), TOP_MARGIN, INITIAL_SCORE);
    Text health((WINDOW_X-textWidth("LIVES LEFT:    ")), TOP_MARGIN, "LIVES LEFT:");
    Text lives_left((WINDOW_X-textWidth("   ")),TOP_MARGIN,shooter.lives_left);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

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
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        //check for hits
        check_for_collision(bubbles,bullets,shooter, score);

        //CHECK FOR COLLISION BETWEEN SHOOTER AND BUBBLE
        check_for_collision_2(shooter,bubbles,lives_left);

        

        //check whether game is over
        if((shooter.time==0)    ||  (shooter.lives_left==0))
        {
            Text lose_msg(WINDOW_X/2,WINDOW_Y/2,"YOU LOSE.");
            lose_msg.scale(2);
            lose_msg.setColor(COLOR("red"));
            lose_msg.setFill(true);
            wait(2);
            return 0;
        }
        else if(bubbles.size()==0){
            Text win_msg(WINDOW_X/2,WINDOW_Y/2,"YOU WIN!!");
            win_msg.scale(2);
            win_msg.setColor(COLOR("green"));
            win_msg.setFill(true);
            wait(2);
            return 0;
            break;
        }
        

        wait(STEP_TIME);
    }


}
