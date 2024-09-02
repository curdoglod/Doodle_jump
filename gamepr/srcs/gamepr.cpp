#include "../includes/engine.h"
#include "../includes/object.h"
#include "../includes/sprite.h"
#include "iostream"
#include "math.h"


enum typeBonus {
    NONE,
    INVINCIBLE,
    PROPELLER
};

class Doodle : public Object {
public:
    Doodle(const float& pos_x, const float& pos_y, Engine* _game)
    {
        game = _game;
        pathes_doodle = { "data/blue-lik-puca.png", "data/blue-lik-left.png" ,"data/blue-lik-right.png", "data/blue-lik-shoot-left.png", "data/blue-lik-shoot-right.png" };        pathes_bonuses = { "","data/invincible.png", "data/propeller.png" };
        sprite = game->createSprite(pathes_doodle[0]);
        die = false;
        init_size();
        Set_position(pos_x, pos_y - size.second);
        //spawn on platform
        bonus_type = NONE;
    }

    void set_new_sprite(int num_sprite) {
        delete sprite;
        sprite = game->createSprite(pathes_doodle[num_sprite]);
    }

    void set_dir_x(int dir) {
        direction_x = dir;
        if (dir == 1) set_new_sprite(2); else if (dir == -1) set_new_sprite(1);
    }

    int get_dir_x() {
        return direction_x;
    }

    void drawObj() {
        sprite->draw (position.first, position.second);
        if (bonus_type != NONE)bonus_Spr->draw(position.first, position.second);;
    }

    void on_bonus(typeBonus _bonus) {
        bonus_Spr = game->createSprite(pathes_bonuses[_bonus]);
        bonus_type = _bonus;
    }

    void off_bonus() {
        bonus_type = NONE;
        delete bonus_Spr;
    }

    typeBonus getTypeBonus() {
        return bonus_type;
    }

    void setSpeed(const float& _speed) {
        speed = _speed;
    }
    float getSpeed() {
        return speed;
    }

    bool is_jumping(Object* obj)
    {
        std::pair<float, float> temp_pos = obj->get_position();
        std::pair<float, float> temp_size = obj->get_size();
        //obj is below
        return speed > 0 && ((temp_pos.second + temp_size.second) >= (position.second + size.second)) &&
            (temp_pos.second) <= (position.second + size.second) &&
            (temp_pos.first + temp_size.first * 0.65f >= position.first) && temp_pos.first <= position.first + size.first * 0.65f;
    }

    ~Doodle() {
        if (bonus_type != NONE)
        delete bonus_Spr;
    }

private:
    Sprite* bonus_Spr;
    float speed;
    int direction_x = 0;
    std::vector<const char*> pathes_doodle;
    std::vector<const char*> pathes_bonuses;
    bool die;
    typeBonus bonus_type; //bonuses

};

class Text_num {
public:
    Text_num(const float& pos_x, const float& pos_y, Engine* _game) {
        game = _game;
        position = std::make_pair(pos_x, pos_y);
        text = "0"; Add_Text(0);

    }

    void Add_Text(int num) {

        text = std::to_string(std::stoi(text) + num); // new text
        int delta_size = static_cast<int>(text.length()) - nums.size();
        for (int i = 0; i < delta_size; i++) nums.push_back(game->createSprite("data/nums/0.png")); //add sprites for the number

        for (int i = 0; i < text.length(); i++)
        {
            delete (nums[i]);
            nums[i] = game->createSprite(("data/nums/" + std::string(1, text[i]) + ".png").c_str());    //changeSprite
        }
    }
    void Set_Text(int num)
    {
        int delta = num - std::stoi(text);

        if (delta != 0)
            Add_Text(delta);
    }
    void draw() {
        for (int i = 0; i < nums.size(); i++)    nums[i] ->draw( 75 + i * 18, position.second);
    }

    ~Text_num() {
        for (auto& num : nums) delete(num);
    }

private:
    Sprite* text_spr;
    std::vector <Sprite*> nums;
    std::pair<float, float> position;
    std::string text;
    Engine* game;
};

enum Type_platform {
    STANDART,
    DISPOSABLE,
    MOVING
};

class Platform :public Object {
public:
    Platform(const float& pos_x, const float& pos_y, Type_platform _type, Engine* _game) {
        pathes = { "data/platform-standart.png", "data/platform-disposable.png", "data/platform-moving.png" };
        type = _type; game = _game;
        sprite = game->createSprite(pathes[type]);
        Set_position(pos_x, pos_y); init_size();
        move_dir = 0.5f; distance = 0;
    }

    //move platform if its type is moving
    void MovePlatform(const float& speed) {
        if (type == MOVING)
        {
            int screen_w, screen_h; game->getScreenSize(screen_w, screen_h);
            if (distance > 200 || static_cast<int>(position.first) < 0 || static_cast<int>(position.first) + size.first > screen_w)
            {
                move_dir *= -1; distance = 0;
            }
            distance += speed;
            move_x(move_dir * speed);
        }

    }

    Type_platform get_Type() { return type; }

private:
    float distance; float move_dir;
    Type_platform type;
    std::vector<const char*> pathes;
};

class Bullet :public Object
{
public:
    Bullet(const float& dir_x, const float& dir_y, Doodle* doodle, Engine* _game)
    {
        game = _game;
        Set_position(doodle->get_position().first + static_cast<float>(doodle->get_size().first) / 2, doodle->get_position().second + static_cast<float>(doodle->get_size().second) / 5);
        sprite = game->createSprite("data/bubble.png"); init_size();
        direction_x = (dir_x - position.first) / (fmax(abs(dir_x - position.first), abs(dir_y - position.second)));
        direction_y = -abs(dir_y - position.second) / (fmax(abs(dir_x - position.first), abs(dir_y - position.second)));
    };

    void shoot(const float& delta_time) {
        sprite->draw (position.first,position.second);
        position.first += direction_x * delta_time / 1.5f;
        position.second += direction_y * delta_time / 1.5f;
    }

private:
    float direction_x, direction_y;
};

class Enemy :public Object
{
public:
    Enemy(const float& pos_x, const float& pos_y, Engine* _game)
    {
        game = _game;
        sprite = game->createSprite("data/enemy-0.png");
        Set_position_onPlatform(pos_x, pos_y);
    }

private:


};

class BonusObj : public Object {
public:
    BonusObj(const float& pos_x, const float& pos_y, Engine* _game) {
        pathes = { "", "data/invincible-obj.png", "data/propeller-obj.png" };
        countBonuses = pathes.size() - 1;
        bonus = typeBonus(1 + rand() % countBonuses);
        game = _game;
        sprite = game->createSprite(pathes[bonus]);

        Set_position_onPlatform(pos_x, pos_y);
    }

    typeBonus getTypeBonus() {
        return bonus;
    }

private:
    std::vector<const char*> pathes;
    int countBonuses = 0; typeBonus bonus;
};



class Game : public Engine {
public:
    
    void Init() override
    {

        getScreenSize(display_width, display_height);
        background = createSprite("data/bck.png");background->setSize(2.5, 2.5);
        scores = createSprite("data/localscores.png");
        new_game();
    }

    void tick() override {
        current_time = getTickCount();
        deltaTime = (current_time - prevTime);// deltaTime is time of one frame in the game


        if (!pause)
        {
            tick_dist = doodle->getSpeed() * deltaTime; // distance covered in one frame
            float accel_t = (gravity * delta_speed) * deltaTime / 1000; //delta_speed 0.5f-3.0f (50%-300%)  :1000 ms->s
            doodle->setSpeed(doodle->getSpeed() + accel_t);
          
            doodle->move_y(tick_dist);
               
            for (int i = 0; i < enemies.size(); i++)
            {
                    //check crossing with enemy, if true the doodle will be killed
                     if (doodle->is_jumping(enemies[i])) {
                        to_jump(dist_jump);
                        delete enemies[i];
                        enemies.erase(enemies.begin() + i);
                        i--;
                        killEnemy();
                    }
                    else if (enemies[i]->crossing(doodle) && (doodle->getTypeBonus() == NONE))
                    {
                        clear(); new_game();
                    }
                    
            }
            
            
          
            //check if the Doodle has jumped onto the platform
            for (int i = 0; i < platforms.size(); i++)
            {
                    if (doodle->is_jumping(platforms[i]))
                    {
                        if (prev_platform != nullptr && prev_platform == platforms[i] && delta_speed < 1.5) delta_speed += 0.15f;
                        else if (delta_speed > 0.85) delta_speed -= 0.05f;

                        to_jump(dist_jump);

                        //delete platform
                        if (platforms[i]->get_Type() == DISPOSABLE)
                        {
                            delete platforms[i]; platforms.erase(platforms.begin() + i); i--;
                        }
                        prev_platform = platforms[i];
                        break;
                    }
            }

            

            
            
            // turn on/off bonus
            if (doodle->getTypeBonus() == NONE)
            {
                for (int i = 0; i < bonuses.size(); i++)
                {
                    if (bonuses[i]->crossing(doodle))
                    {
                        doodle->on_bonus(bonuses[i]->getTypeBonus());

                        if (doodle->getTypeBonus() == INVINCIBLE)
                        {
                            time_bonus = 20000 + current_time; //20sec
                        }
                        else if (doodle->getTypeBonus() == PROPELLER) {

                            to_jump(dist_jump * 10);
                        }

                        for (int j = 0; j < bonuses.size(); j++)
                        {
                            delete bonuses[j];
                            bonuses.erase(bonuses.begin() + j);
                            j--;
                        }

                        break;
                    }
                }
            }
            else
            {
                if (doodle->getTypeBonus() == INVINCIBLE && time_bonus < current_time) {
                    doodle->off_bonus();
                }
                else if (doodle->getTypeBonus() == PROPELLER && doodle->getSpeed()>0) {

                    doodle->off_bonus();

                }

            }

            // move the doodle to the right and left
            doodle->move_x(doodle->get_dir_x() * 0.5f * sqrt(delta_speed) * deltaTime);

            // go out of screen
            if (doodle->get_position().first < 0)
                doodle->Set_position((display_width + doodle->get_position().first), doodle->get_position().second);
            else if (doodle->get_position().first > display_width)
                doodle->Set_position((doodle->get_position().first - display_width), doodle->get_position().second);

            move_camera();

            //die if the doodle fell down
            if (doodle->get_position().second - doodle->get_size().second > display_height)
            {
                clear();
                new_game();
            }

        }


        prevTime = getTickCount();
    }
    
    void drawScene() override
    {
        background->draw (0, 0);

        for (int i = 0; i < platforms.size(); i++)
        {
            platforms[i]->drawObj();
            if (!pause && platforms[i]->get_Type() == MOVING)
                platforms[i]->MovePlatform(0.1f * sqrt(delta_speed) * deltaTime);
        }

        for (int i = 0; i < enemies.size(); i++)
            enemies[i]->drawObj();
        for (int i = 0; i < bonuses.size(); i++)
            bonuses[i]->drawObj();

        doodle->drawObj();
        scores->draw(0, 0);
        score_jumps_txt->draw(); score_distance_txt->draw();

        for (int i = 0; i < bullets.size(); i++)
        {
            //kill enemy
            for (int j = 0; j < enemies.size(); j++)
                if (enemies[j]->crossing(bullets[i])) {
                    delete enemies[j];
                    enemies.erase(enemies.begin() + j);
                    j--;
                    killEnemy(); // after kill enemy
                }

            bullets[i]->shoot(deltaTime);

            //delete projectile
            if (bullets[i]->get_position().second<0 || bullets[i]->get_position().second > display_height ||
                bullets[i]->get_position().first < 0 || bullets[i]->get_position().first > display_width) {
                delete bullets[i];
                bullets.erase(bullets.begin() + i);
                i--;
            }

        }

    }
   
    virtual void onMouseMove(int x, int y) override  {
        mouse_pos_x = x*(display_width/(float)getScreenSize().first);
        mouse_pos_y = y*(display_height/(float)getScreenSize().second);
    };

    virtual void onMouseButtonClick(MouseButton button, bool isReleased) override {
        if (isReleased && button == MouseButton::LEFT) {

            //set sprite if the doodle is shooting
            if (mouse_pos_x < display_width / 2)
            {
                doodle->set_new_sprite(3);
            }
            else doodle->set_new_sprite(4);
            //create new bullet
            bullets.push_back(new Bullet(mouse_pos_x, mouse_pos_y, doodle,this));
        }
    };

    virtual void onKeyPressed(KeyButton key) override {
        if (key == KeyButton::RIGHT) {
            doodle->set_dir_x(1);
        }
        else if (key == KeyButton::LEFT)
        {
            doodle->set_dir_x(-1);
        }

        pause = false;
    };

    virtual void onKeyReleased (KeyButton key) override {
        if ((key == KeyButton::RIGHT && doodle->get_dir_x() == 1) || (key == KeyButton::LEFT && doodle->get_dir_x() == -1))
            doodle->set_dir_x(0);
    };

private:
    int frequency;  // frequency of platforms
    float delta_speed;
    float gravity;
    float deltaTime = 0; unsigned int prevTime = 0; unsigned int current_time; unsigned int time_bonus; //time realization
    int dist_jump; float tick_dist = 0; //position
    float moving_cam = 0, cam_pos = 0;  //cam moving realization
    unsigned int count_killedEnemies;
    float total_distance;
    
    int display_width;
    int display_height;
    
    int mouse_pos_x, mouse_pos_y;

    bool pause;

    Platform* prev_platform;
    Sprite* background, * scores;
    Doodle* doodle;
    Text_num* score_jumps_txt, * score_distance_txt;
    std::vector<Platform*> platforms;
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;
    std::vector<BonusObj*> bonuses;
    void Generation_Platforms(int frequency) {

        //delete old platforms, enemies, bonuses
        for (int i = 0; i < platforms.size(); i++)
        {
            if (platforms[i]->get_position().second > display_height)
            {
                delete platforms[i];
                platforms.erase(platforms.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < enemies.size(); i++) {
            if (enemies[i]->get_position().second > display_height)
            {
                delete enemies[i]; //enemies[i].destroyObj();
                enemies.erase(enemies.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < bonuses.size(); i++) {
            if (bonuses[i]->get_position().second > display_height)
            {
                delete bonuses[i]; //bonuses[i].destroyObj();
                bonuses.erase(bonuses.begin() + i);
                i--;
            }
        }


        //generate new platforms
        while (frequency != platforms.size())
        {
            float lastPlatformY = platforms.back()->get_position().second;// std::cout << platforms.back()->get_position().second << std::endl;

            int add_size = frequency - platforms.size();
            for (int i = 0; i < add_size; i++)
            {
                Type_platform rand_type = STANDART;

                if (total_distance > 100 && rand() % 10 == 0) rand_type = MOVING;
                else if (rand() % 4 == 0) rand_type = DISPOSABLE;

                float random_x = rand() % (display_width - 50), random_y = lastPlatformY - (static_cast<float>(rand() % (2500 / frequency)));

                Platform* new_platform = new Platform(random_x, random_y, rand_type,this);

                // Check if the new platform overlaps with any existing platforms
                bool overlaps = false;
                for (int j = 0; j < platforms.size(); j++)
                {
                    if (new_platform->crossing(platforms[j], 2.0f, 5.0f))
                    {
                        overlaps = true;
                        break;
                    }
                }

                if (!overlaps)
                {
                    platforms.push_back(new_platform);
                    lastPlatformY = random_y;

                    //generate bonuses and enemies on the platform
                    if (platforms.back()->get_Type() != MOVING)
                    {
                        if (rand() % 15 == 0)
                            enemies.push_back(new Enemy(random_x, random_y,this));
                        else if (rand() % 20 == 0 && (doodle == nullptr || doodle->getTypeBonus() == NONE)) {
                            bonuses.push_back(new BonusObj(random_x, random_y,this));

                        }
                    }
                }
                else
                {
                    delete new_platform;
                }

            }



        }
    }
        
    void to_jump(unsigned int _high_jump) {
        doodle->setSpeed(-sqrt(2 * (gravity * delta_speed) * _high_jump / 1000)); // :1000 ms->s

        //add jumps_score
        score_jumps_txt->Add_Text(1);

    }

    void move_camera() {

        if (tick_dist > 0 || doodle->get_position().second > display_height/3.5) return;

        //move all objects
        for (int i = 0; i < platforms.size(); i++) {
            platforms[i]->move_y(-tick_dist);
        }
        for (int i = 0; i < enemies.size(); i++) {
            enemies[i]->move_y(-tick_dist);
        }
        for (int i = 0; i < bonuses.size(); i++) {
            bonuses[i]->move_y(-tick_dist);
        }
        for (int i = 0; i < bullets.size(); i++) {
            bullets[i]->move_y(-tick_dist);
        }
        doodle->move_y(-tick_dist);
        moving_cam += tick_dist;
        total_distance -= tick_dist;

        Generation_Platforms(frequency);
        score_distance_txt->Set_Text((int)total_distance); // update distance text

    }
    
    void clear() {
        delete score_jumps_txt; delete score_distance_txt;
        delete doodle;
        for (int i = 0; i < platforms.size(); i++)
        {
            delete platforms[i];
            platforms.erase(platforms.begin() + i);
            i--;
        }
        for (int i = 0; i < enemies.size(); i++)
        {
            delete enemies[i];
            enemies.erase(enemies.begin() + i);
            i--;
        }
        for (int i = 0; i < bonuses.size(); i++)
        {
            delete bonuses[i];
            bonuses.erase(bonuses.begin() + i);
            i--;
        }
        for (int i = 0; i < bullets.size(); i++)
        {
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
    
    void new_game() {
        pause = false;
        total_distance = 0;
        delta_speed = 1; //speed change min = 0.5, max =3
        gravity = 0.6f; //initial acceleration
        frequency = 40; // frequency of platforms in one generation
        mouse_pos_x = 0;  mouse_pos_y = 0;
        dist_jump = 200; //  high of jump
        count_killedEnemies = 0;
        //jumps and distance scores
        score_jumps_txt = new Text_num(0, 11,this); score_distance_txt = new Text_num(0, 55,this);

        platforms.push_back(new Platform(display_width / 2, display_height - 40, STANDART, this)); // create first platform in the center of the screen
        doodle = new Doodle(platforms[0]->get_position().first, platforms[0]->get_position().second, this); // create doodle
        //doodle = new Doodle(0, 0, this);
        Generation_Platforms(frequency);
    }

    void killEnemy() {
        count_killedEnemies++;

        //generate new bonus on the random platform after killing 5 enemies
        if (count_killedEnemies % 5 == 0) {
            bool crossingBonus = true;
            while (crossingBonus)
            {
                crossingBonus = false;
                int randPl = rand() % platforms.size();
                if (platforms[randPl]->get_Type() == MOVING || platforms[randPl]->get_position().second < 0) crossingBonus = true;
                else {
                    BonusObj* new_bonus = new BonusObj(platforms[randPl]->get_position().first, platforms[randPl]->get_position().second,this);
                    for (auto& b : bonuses)
                        if (new_bonus->crossing(b))
                        {
                            crossingBonus = true;
                            break;
                        }
                    if (!crossingBonus) {
                        bonuses.push_back(new_bonus);
                    }
                    else delete new_bonus;
                }
            }
        }

    }
};






int main()
{
    Game game;
    game.run();

    return 0;
}
