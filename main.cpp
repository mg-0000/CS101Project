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

void check_for_collision(vector<Bubble> &bubbles , vector<Bullet> &bullets){
    //check for collision between all bubbles and bullets
    for (unsigned int i=0; i<bullets.size(); i++){
        for(unsigned int j=0; j<bubbles.size(); j++){
            //check for collision.
            //4 casses for each axes
            if(((bullets[i].get_center_y()<=(bubbles[j].get_center_y()+bubbles[j].get_radius()) && bullets[i].get_center_y()>=(bubbles[j].get_center_y()-bubbles[j].get_radius()))
                ||
                (bubbles[j].get_center_y()<=(bullets[i].get_center_y()+(bullets[i].get_height()/2)) && bubbles[j].get_center_y()>=(bullets[i].get_center_y()-(bullets[i].get_height()/2)))
                ||
                (((bullets[i].get_center_y()-bullets[i].get_height()/2)<=(bubbles[j].get_center_y()+bubbles[j].get_radius())) && ((bullets[i].get_center_y()+bullets[i].get_height()/2)>=(bubbles[j].get_center_y()+bubbles[j].get_radius())))
                ||
               (((bullets[i].get_center_y()-bullets[i].get_height()/2)<=(bubbles[j].get_center_y()-bubbles[j].get_radius())) && ((bullets[i].get_center_y()+bullets[i].get_height()/2)>=(bubbles[j].get_center_y()-bubbles[j].get_radius())))
                )
                &&
                ((bullets[i].get_center_x()<=(bubbles[j].get_center_x()+bubbles[j].get_radius()) && bullets[i].get_center_x()>=(bubbles[j].get_center_x()-bubbles[j].get_radius()))
                ||
                (bubbles[j].get_center_x()<=(bullets[i].get_center_x()+(bullets[i].get_width()/2)) && bubbles[j].get_center_x()>=(bullets[i].get_center_x()-(bullets[i].get_width()/2)))
                ||
                (((bullets[i].get_center_x()-bullets[i].get_width()/2)<=(bubbles[j].get_center_x()+bubbles[j].get_radius())) && ((bullets[i].get_center_x()+bullets[i].get_width()/2)>=(bubbles[j].get_center_x()+bubbles[j].get_radius())))
                ||
               (((bullets[i].get_center_x()-bullets[i].get_width()/2)<=(bubbles[j].get_center_x()-bubbles[j].get_radius())) && ((bullets[i].get_center_x()+bullets[i].get_width()/2)>=(bubbles[j].get_center_x()-bubbles[j].get_radius())))
                ))
                {
                    bullets.erase(bullets.begin()+i);
                    bubbles.erase(bubbles.begin()+j);
                }
        }
    }
}

int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while (true)
    {
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
        check_for_collision(bubbles,bullets);

        wait(STEP_TIME);
    }
}
