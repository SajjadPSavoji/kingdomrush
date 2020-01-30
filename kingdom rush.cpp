#include <iostream>
#include "rsdl.hpp"
#include <vector>
#include <ctime>
#include <cmath>
#include <sstream>

#define FONT_FREESANS "RSDL/example/assets/OpenSans.ttf"
#define FONT_SIZE 20
#define BACKGROUND_WIDTH 800
#define BACKGROUND_HEIGHT 800
#define BACKGROUNG_IMAGE_LEVEL1 "assets/levels/1/level1-map.png"
#define BACKGROUNG_IMAGE_LEVEL2 "assets/levels/2/level2-map.png"

#define EPSILONE 10
#define RANDOM_OFFSET 15
#define IN_ROW_OFFSET 30
#define TOWER_ACCEPTED_RADIUS 30
#define TOWER_ATTACK_RANGE 85
#define SCORE_W 20
#define SCORE_H 20
#define CORPSES_DECOMPOSITION_TIME 1.5
#define WIN_IMAGE "assets/win.png"
#define LOSE_IMAGE "assets/lose.png"
#define START_PAGE_IMAGE "assets/starter.png"
#define START_PAGE_W_UPPER_LIMIT 486
#define START_PAGE_W_LOWER_LIMIT 306
#define START_PAGE_H_UPPER_LIMIT 560
#define START_PAGE_H_LOWER_LIMIT 460

#define ARCHER_TOWER_IMAGE "assets/towers/archer.png"
#define ARCHER_DAMAGE 50
#define ARCHER_DELAY 0.7
#define ARCHER_COST 70
#define ARCHER_BULLET_IMAGE "assets/towers/archer_bullet.png"
#define ARCHER_BULLET_SPEED 150
#define ARCHER_BULLET_SIZE 15

#define CANON_TOWER_IMAGE "assets/towers/canon.png"
#define CANON_DAMAGE 140
#define CANON_DELAY 2
#define CANON_COST 125
#define CANNON_BULLET_IMAGE "assets/towers/canon_bullet.png"
#define CANNON_BULLET_SPEED 150
#define CANNON_EXPLOSION_RADIUS 50
#define CANNON_BULLET_SIZE 15
#define EXPLOADING_BULLET "assets/towers/explodin_bullet.png"
#define EXPLOSION_DURATION 0.3

#define MAGE_TOWER_IMAGE "assets/towers/mage.png"
#define MAGE_DAMAGE 140
#define MAGE_DELAY 1.5
#define MAGE_COST 90
#define MAGE_BULLET_IMAGE "assets/towers/mage_bullet.png"
#define MAGE_BULLET_SPEED 150
#define MAGE_BULLET_SIZE 15

#define ICY_TOWER_IMAGE "assets/towers/icy.png"
#define ICY_DAMAGE 30
#define ICY_DELAY 2
#define ICY_COST 80
#define ICY_BULLET_IMAGE "assets/towers/icy_bullet.png"
#define ICY_BULLET_SPEED 150
#define ICY_FRIZING_RADUS 50
#define ICY_BULLET_SIZE 15
#define FRIZING_BULLET "assets/towers/frizing_bullet.png"
#define FRIZING_DURATION 0.3
#define FRIZING_RECOVERY_DURATION 3
#define ICY_SPEED_REDUCTION 3

#define TOWER_SIZE 55

#define ARMORED_SIZE 30
#define ARMORED_SPEED 21
#define ARMORED_LIFE 450
#define ARMORED_BONUS 8
#define ARMORED_DAMAGE 3 
#define ARMORED_BACK "assets/enemies/armored/back.png"
#define ARMORED_FRONT "assets/enemies/armored/front.png"
#define ARMORED_LEFT "assets/enemies/armored/left.png"
#define ARMORED_RIGHT "assets/enemies/armored/right.png"
#define ARMORED_DEAD  "assets/enemies/armored/dead.png"

#define DEMON_SIZE 30
#define DEMON_SPEED 28
#define DEMON_LIFE 400
#define DEMON_BONUS 8
#define DEMON_DAMAGE 3
#define DEMON_BACK "assets/enemies/demon/back.png"
#define DEMON_FRONT "assets/enemies/demon/front.png"
#define DEMON_LEFT "assets/enemies/demon/left.png"
#define DEMON_RIGHT "assets/enemies/demon/right.png"
#define DEMON_DEAD	"assets/enemies/demon/dead.png"

#define WOLF_SIZE 30 
#define WOLF_SPEED 85
#define WOLF_LIFE 100
#define WOLF_BONUS 4
#define WOLF_DAMAGE 1
#define WOLF_BACK "assets/enemies/wolf/back.png"
#define WOLF_FRONT "assets/enemies/wolf/front.png"
#define WOLF_LEFT "assets/enemies/wolf/left.png"
#define WOLF_RIGHT "assets/enemies/wolf/right.png"
#define WOLF_DEAD "assets/enemies/wolf/dead.png"

#define ORC_SIZE 30
#define ORC_SPEED 42
#define ORC_LIFE 200
#define ORC_BONUS 5
#define ORC_DAMAGE 1
#define ORC_BACK "assets/enemies/orc/back.png"
#define ORC_FRONT "assets/enemies/orc/front.png"
#define ORC_LEFT "assets/enemies/orc/left.png"
#define ORC_RIGHT "assets/enemies/orc/right.png"
#define ORC_DEAD "assets/enemies/orc/dead.png"


using namespace std;
/*const int towers_position_width[]={477,332,137,287,417,337,517,427};
const int towers_position_height[]={202,212,327,352,472,502,572,607};*/
/*const int path_points_position_width[]={800,573,535,425,393,243,223,478,483,358,353};
const int path_points_position_height[]={286,286,144,144,281,301,416,416,536,571,800};*/

struct Position{
	double width;
	double height;
};
struct Player{
	string name;
	int score=220;
	int life=15;
};
enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
enum enemy_type
{
	ARMORED,
	DEMON,
	WOLF,
	ORC
};
struct Wave{
	double start_time;
	enemy_type type;
	int number_of_type;
};
enum tower_type
{
	ARCHER,
	CANON,
	MAGE,
	ICY
};
struct enemy {
	Position position;
	enemy_type type;
	int spsilone=10;
	int speed;
	int default_speed;
	int life;
	int next_path_point_index=0;
	Direction direction= LEFT;
	int width;
	int height;
	int bonus;
	double dead_time=0.0;
	double frizing_time=0.0;
	int random_offset;
};
struct Tower{
	int position_index;
	double loading_delay;
	double current_loading_delay=0;
	int damage;
	tower_type type;
};
struct Bullet
{
	Position position;
	int speed;
	int damage;
	tower_type type;
	int tower_position_index;
	enemy *locked_enemy;
	double distruction_time=0;
};
int string_to_int(string input)
{
	stringstream my_stream(input);
	int output;
	my_stream>>output;
	return output;
}
Direction enemy_direction(enemy a_enemy,vector <int> path_points_position_width ,vector<int> path_points_position_height)
{
	int vector_x=a_enemy.position.width - path_points_position_width[a_enemy.next_path_point_index];
	int vector_y=a_enemy.position.height - path_points_position_height[a_enemy.next_path_point_index];
	if(vector_x >=0)
	{
		if (vector_y>=0)
		{
			return vector_x>=vector_y ? LEFT:UP;
		}
		else
		{
			return vector_x >= -1*vector_y ? LEFT:DOWN;
		}
	}
	else
	{
		if (vector_y>=0)
		{
			return -1*vector_x >= vector_y ? RIGHT:UP;
		}
		else
		{
			return vector_x>= vector_y ? DOWN:RIGHT;
		}
	}
}
void change_background(Window &main_window , string BACKGROUNG_IMAGE)
{
	main_window.draw_png(BACKGROUNG_IMAGE,0,0,BACKGROUND_WIDTH,BACKGROUND_HEIGHT);
	main_window.update_screen();
}
void place_png( Window &main_window, string PNG_FILE_NAME,int PNG_WIDTH, int PNG_HAIGHT,int png_positon_x,int png_positon_y)
{
	main_window.draw_png(PNG_FILE_NAME,png_positon_x - PNG_WIDTH/2,png_positon_y- PNG_HAIGHT/2,PNG_WIDTH,PNG_HAIGHT);
}

string enemy_file_name(enemy a_enemy)
{
	if(a_enemy.type ==ARMORED)
	{
		if(a_enemy.direction==UP)
			return ARMORED_BACK;
		else if (a_enemy.direction==DOWN)
			return ARMORED_FRONT;
		else if (a_enemy.direction== LEFT)
			return ARMORED_LEFT;
		else if (a_enemy.direction==RIGHT)
			return ARMORED_RIGHT;
		else
		{
			return "";
		};
	} 
	else if(a_enemy.type==DEMON)
	{
		if(a_enemy.direction==UP)
			return DEMON_BACK;
		else if (a_enemy.direction==DOWN)
			return DEMON_FRONT;
		else if (a_enemy.direction== LEFT)
			return DEMON_LEFT;
		else if (a_enemy.direction==RIGHT)
			return DEMON_RIGHT;
		else
		{
			return "";
		};
	} 
	else if(a_enemy.type==WOLF)
	{
		if(a_enemy.direction==UP)
			return WOLF_BACK;
		else if (a_enemy.direction==DOWN)
			return WOLF_FRONT;
		else if (a_enemy.direction== LEFT)
			return WOLF_LEFT;
		else if (a_enemy.direction==RIGHT)
			return WOLF_RIGHT;
		else
		{
			return "";
		};
	} 
	else if(a_enemy.type==ORC)
	{
		if(a_enemy.direction==UP)
			return ORC_BACK;
		else if (a_enemy.direction==DOWN)
			return ORC_FRONT;
		else if (a_enemy.direction== LEFT)
			return ORC_LEFT;
		else if (a_enemy.direction==RIGHT)
			return ORC_RIGHT;
		else
		{
			
			return "";
		};
	} 
	else 
		{
			return "";
		} 
}

void place_enemy(Window &main_window, enemy a_enemy,vector <int> path_points_position_width ,vector<int> path_points_position_height)
{
	a_enemy.direction=enemy_direction(a_enemy, path_points_position_width , path_points_position_height);
	if (a_enemy.direction == UP || a_enemy.direction == DOWN)
	{
		place_png(main_window,enemy_file_name(a_enemy),a_enemy.width,a_enemy.height,a_enemy.position.width,a_enemy.position.height + a_enemy.random_offset);
	}
	else 
	{
		place_png(main_window,enemy_file_name(a_enemy),a_enemy.width,a_enemy.height,a_enemy.position.width,a_enemy.position.height +a_enemy.random_offset);
	}
}
void lose_life(Player &a_player, enemy a_enemy)
{
	if(a_enemy.type==ARMORED)
		a_player.life-=ARMORED_DAMAGE;
	else if(a_enemy.type==WOLF)
		a_player.life-=WOLF_DAMAGE;
	else if(a_enemy.type==ORC)
		a_player.life-=ORC_DAMAGE;
	else if(a_enemy.type==DEMON)
		a_player.life-=DEMON_DAMAGE;
	else 
		cout<<"unidentified object passed finish line"<<endl;
}
void set_next_path_point_index(Player &a_player,vector<enemy> &current_enemies,int enemy_index,vector <int> path_points_position_width ,vector<int> path_points_position_height) 
{
	if (abs(current_enemies[enemy_index].position.width - path_points_position_width[current_enemies[enemy_index].next_path_point_index]) <= EPSILONE  &&
		abs(current_enemies[enemy_index].position.height - path_points_position_height[current_enemies[enemy_index].next_path_point_index]) <= EPSILONE )
	{
		current_enemies[enemy_index].next_path_point_index += 1;
		if(current_enemies[enemy_index].next_path_point_index >= path_points_position_width.size())
		{
			lose_life(a_player,current_enemies[enemy_index]);	
			current_enemies.erase(current_enemies.begin()+enemy_index);
		}

			
	}
}
void move_bullet(Bullet &a_bullet, double time_difference)
{
	int vector_x=a_bullet.position.width - a_bullet.locked_enemy->position.width;
	int vector_y=a_bullet.position.height - a_bullet.locked_enemy->position.height;
	if(vector_x >= EPSILONE)
	{
		
		if (vector_y >= EPSILONE)
		{
			a_bullet.position.height -= (time_difference * double (a_bullet.speed)/2);
			a_bullet.position.width -= (time_difference * double (a_bullet.speed)/2);
		}
		if (vector_y <= -1* EPSILONE)
		{
			a_bullet.position.height +=  (time_difference * double (a_bullet.speed)/2);
			a_bullet.position.width -=  (time_difference * double (a_bullet.speed)/2);
		}
		if (vector_y < EPSILONE && vector_y > -1* EPSILONE )
		{
			a_bullet.position.width -= time_difference * double (a_bullet.speed);	
		}
	}
	if(vector_x <=  -1 * EPSILONE)
	{
		
		if (vector_y >= EPSILONE)
		{
			a_bullet.position.height -=  (time_difference * double (a_bullet.speed)/2);
			a_bullet.position.width +=  (time_difference * double (a_bullet.speed)/2);
		}
		if (vector_y<= -1* EPSILONE)
		{
			a_bullet.position.height +=  (time_difference * double (a_bullet.speed)/2);
			a_bullet.position.width +=  (time_difference * double (a_bullet.speed)/2);
		}
		if (vector_y < EPSILONE && vector_y > -1* EPSILONE )
		{
			a_bullet.position.width += time_difference * double (a_bullet.speed);
		}
	}
	if (vector_x < EPSILONE && vector_x > -1*EPSILONE)
	{
		if (vector_y >= EPSILONE)
		{
			a_bullet.position.height -= time_difference * double (a_bullet.speed);
		}
		if (vector_y <= -1* EPSILONE)
		{
			a_bullet.position.height += time_difference * double (a_bullet.speed);
		}
		if (vector_y < EPSILONE && vector_y > -1* EPSILONE )
		{
			cout<<"invalid_movemet in move bullet funcion"<<endl;	
		}
	}

}
void move_bullets(vector<Bullet> &bullets ,double time_difference)
{
	for (int i = 0; i < bullets.size(); ++i)
	{
		move_bullet(bullets[i], time_difference);
	}
}
void move_enemy(Player &a_player,vector<enemy> &current_enemies,int enemy_index, double  time_difference,vector <int> path_points_position_width ,vector<int> path_points_position_height)
{
	set_next_path_point_index(a_player, current_enemies,enemy_index,path_points_position_width,path_points_position_height);

	int vector_x=current_enemies[enemy_index].position.width - path_points_position_width[current_enemies[enemy_index].next_path_point_index];
	int vector_y=current_enemies[enemy_index].position.height - path_points_position_height[current_enemies[enemy_index].next_path_point_index];

	if(vector_x >= EPSILONE)
	{
		
		if (vector_y >= EPSILONE)
		{
			current_enemies[enemy_index].position.height -= (time_difference * double ((current_enemies[enemy_index].speed))/2);
			current_enemies[enemy_index].position.width -= (time_difference * double ((current_enemies[enemy_index].speed))/2);
		}
		if (vector_y <= -1* EPSILONE)
		{
			current_enemies[enemy_index].position.height +=  (time_difference * double ((current_enemies[enemy_index].speed))/2);
			current_enemies[enemy_index].position.width -=  (time_difference * double ((current_enemies[enemy_index].speed))/2);
		}
		if (vector_y < EPSILONE && vector_y > -1* EPSILONE )
		{
			current_enemies[enemy_index].position.width -= time_difference * double ((current_enemies[enemy_index].speed));	
		}
	}
	if(vector_x <=  -1 * EPSILONE)
	{
		
		if (vector_y >= EPSILONE)
		{
			current_enemies[enemy_index].position.height -=  (time_difference * double ((current_enemies[enemy_index].speed))/2);
			current_enemies[enemy_index].position.width +=  (time_difference * double ((current_enemies[enemy_index].speed))/2);
		}
		if (vector_y<= -1* EPSILONE)
		{
			current_enemies[enemy_index].position.height +=  (time_difference * double ((current_enemies[enemy_index].speed))/2);
			current_enemies[enemy_index].position.width +=  (time_difference * double ((current_enemies[enemy_index].speed))/2);
		}
		if (vector_y < EPSILONE && vector_y > -1* EPSILONE )
		{
			current_enemies[enemy_index].position.width += time_difference * double ((current_enemies[enemy_index].speed));
		}
	}
	if (vector_x < EPSILONE && vector_x > -1*EPSILONE)
	{
		if (vector_y >= EPSILONE)
		{
			current_enemies[enemy_index].position.height -= time_difference * double ((current_enemies[enemy_index].speed));
		}
		if (vector_y <= -1* EPSILONE)
		{
			current_enemies[enemy_index].position.height += time_difference * double ((current_enemies[enemy_index].speed));
		}
		if (vector_y < EPSILONE && vector_y > -1* EPSILONE )
		{
			cout<<"invalid_movemet in move enemy funcion"<<endl;	
		}
	}
	
}
void initialize_enemy(enemy &a_enemy, string enemy_type,vector <int> path_points_position_width ,vector<int> path_points_position_height)
{
	a_enemy.position.width=path_points_position_width[0];
	a_enemy.position.height=path_points_position_height[0];

	if (enemy_type == "armored")
	{
		a_enemy.type= ARMORED;
		a_enemy.width=ARMORED_SIZE;
		a_enemy.height=ARMORED_SIZE;
		a_enemy.life=ARMORED_LIFE;
		a_enemy.speed=ARMORED_SPEED;
	}
	else if (enemy_type == "wolf")
	{
		a_enemy.type=WOLF;
		a_enemy.width=WOLF_SIZE;
		a_enemy.height=WOLF_SIZE;
		a_enemy.life=WOLF_LIFE;
		a_enemy.speed=WOLF_SPEED;
	}
	else if (enemy_type == "orc")
	{
		a_enemy.type=ORC;
		a_enemy.width=ORC_SIZE;
		a_enemy.height=ORC_SIZE;
		a_enemy.life=ORC_LIFE;
		a_enemy.speed=ORC_SPEED;
	}
	else if (enemy_type == "demon")
	{
		a_enemy.type=DEMON;
		a_enemy.width=DEMON_SIZE;
		a_enemy.height=DEMON_SIZE;
		a_enemy.life=DEMON_LIFE;
		a_enemy.speed=DEMON_SPEED;
	}
	else
		cout<<"wrong enemy"<<endl;
}

void get_level_background(string &background_image_level) 
{
	string temp;
	getline(cin,temp);
	if (temp == "level 1")
		background_image_level=BACKGROUNG_IMAGE_LEVEL1;
	else if(temp == "level 2")
		background_image_level=BACKGROUNG_IMAGE_LEVEL2;
	else 
	{
		cout<<"backgroun image of "<<temp<<"was not found try level 1"<<endl;
		background_image_level=BACKGROUNG_IMAGE_LEVEL1;
	}
}
void  get_path_points( vector <int> &path_points_position) 
{
	string temp;
	string delimiter=" ";
	size_t pos = 0;
	string token;
	getline(cin,temp);
	while ((pos = temp.find(delimiter)) != std::string::npos) {
	    token = temp.substr(0, pos);
	    path_points_position.push_back(string_to_int(token));
	    temp.erase(0, pos + delimiter.length());
	}
	 path_points_position.push_back(string_to_int(temp));
}
enemy_type get_enemy_type(string enemy_name)
{
	
	if (enemy_name == "armored")
		return ARMORED;
	else if(enemy_name == "demon")
		return DEMON;
	else if(enemy_name == "wolf")
		return WOLF;
	else if(enemy_name == "orc")
		return ORC;
	else 
		{
			cout<<"type of enemy not found enemy would be armored"<<endl;
			return ARMORED;
		}
}
double convert_string_to_time_diffrence(string time)
{
	string token;
	double sum_time=0.0;
	string delimiter=":";
	size_t pos=0;
	int co=3600;
	while ((pos = time.find(delimiter)) != std::string::npos) {
		    token = time.substr(0, pos);
		    sum_time+= double (co*string_to_int(token));
		    co/=60;
		    time.erase(0, pos + delimiter.length());
		}
		sum_time+=double(string_to_int(time));
		return sum_time;
}

Wave generate_a_wave(string enemy_number_x_type, string time)
{
	Wave a_wave;
	string delimiter= "x";
	size_t pos = 0;
	string token;
	pos=enemy_number_x_type.find(delimiter);
	a_wave.number_of_type = string_to_int(enemy_number_x_type.substr(0,pos));

	enemy_number_x_type.erase(0, pos + delimiter.length());
	a_wave.type=get_enemy_type(enemy_number_x_type);
	a_wave.start_time=convert_string_to_time_diffrence(time);
	return a_wave;
}
void get_waves(vector<Wave> &waves)
{
	string temp;
	string start_time;
	string delimiter= " ";
	size_t pos = 0;
	string token;
	while(getline(cin, temp))
	{
		pos = temp.find(delimiter);
		start_time=temp.substr(0, pos);
		temp.erase(0, pos + delimiter.length());
		while ((pos = temp.find(delimiter)) != std::string::npos) {
		    token = temp.substr(0, pos);
		    waves.push_back(generate_a_wave(token,start_time));

		    temp.erase(0, pos + delimiter.length());
		}
		waves.push_back(generate_a_wave(temp,start_time));
		
	}
}
enemy make_enemy(enemy_type a_type,vector <int> path_points_position_width ,vector<int> path_points_position_height, int number_of_equivalent_enemies)
{
	enemy a_enemy;
	a_enemy.position.width=path_points_position_width[0]+number_of_equivalent_enemies*IN_ROW_OFFSET;
	a_enemy.position.height=path_points_position_height[0];
	srand(a_enemy.position.width);
	a_enemy.random_offset=pow(-1,rand()%2)*(rand()%RANDOM_OFFSET);

	if (a_type == ARMORED)
	{
		a_enemy.type= ARMORED;
		a_enemy.width=ARMORED_SIZE;
		a_enemy.height=ARMORED_SIZE;
		a_enemy.life=ARMORED_LIFE;
		a_enemy.speed=ARMORED_SPEED;
		a_enemy.default_speed=ARMORED_SPEED;
		a_enemy.bonus=ARMORED_BONUS;
	}
	else if (a_type == WOLF)
	{
		a_enemy.type=WOLF;
		a_enemy.width=WOLF_SIZE;
		a_enemy.height=WOLF_SIZE;
		a_enemy.life=WOLF_LIFE;
		a_enemy.speed=WOLF_SPEED;
		a_enemy.default_speed=WOLF_SPEED;
		a_enemy.bonus=WOLF_BONUS;
	}
	else if (a_type == ORC)
	{
		a_enemy.type=ORC;
		a_enemy.width=ORC_SIZE;
		a_enemy.height=ORC_SIZE;
		a_enemy.life=ORC_LIFE;
		a_enemy.speed=ORC_SPEED;
		a_enemy.default_speed=ORC_SPEED;
		a_enemy.bonus=ORC_BONUS;
	}
	else if (a_type == DEMON)
	{
		a_enemy.type=DEMON;
		a_enemy.width=DEMON_SIZE;
		a_enemy.height=DEMON_SIZE;
		a_enemy.life=DEMON_LIFE;
		a_enemy.speed=DEMON_SPEED;
		a_enemy.default_speed=DEMON_SPEED;
		a_enemy.bonus=DEMON_BONUS;
	}
	else
		{
			cout<<"wrong enemy! enemy type was selected default armored"<<endl;
			a_enemy.type= ARMORED;
			a_enemy.width=ARMORED_SIZE;
			a_enemy.height=ARMORED_SIZE;
			a_enemy.life=ARMORED_LIFE;
			a_enemy.speed=ARMORED_SPEED;
			a_enemy.bonus=ARMORED_BONUS;

		}

	return a_enemy;
}
void push_active_waves_to_current_enemies(vector<enemy> &current_enemies,vector<Wave> &waves,double time_difference,vector <int> path_points_position_width ,vector<int> path_points_position_height)
{
	for (int i = 0; i< waves.size() && waves[i].start_time <= time_difference ; ++i)
	{	
		for (int j = 0; j < waves[i].number_of_type; ++j)
			current_enemies.push_back(make_enemy(waves[i].type,path_points_position_width,path_points_position_height,j));
		waves.erase(waves.begin());
	}
}
void move_enemies(Player &a_player,vector<enemy> &enemies , double time_difference,vector <int> path_points_position_width ,vector<int> path_points_position_height)
{
	for (int i = 0; i < enemies.size(); ++i)
	{
		move_enemy(a_player,enemies,i,time_difference,path_points_position_width,path_points_position_height);
	}
}
void place_enemies(Window &main_window,vector<enemy> &enemies,vector <int> path_points_position_width ,vector<int> path_points_position_height)
{
	for (int i = 0; i < enemies.size(); ++i)
	{
		 place_enemy(main_window, enemies[i], path_points_position_width , path_points_position_height);

	}
}

bool is_near_by(int x1, int y1, int x2, int y2, int epsilone)
{
	if (abs(x1 - x2) <= epsilone && abs(y1 - y2) <= epsilone )
		return true;
	return false;
}
int  index_positon_for_tower(int mouse_x, int mouse_y, vector<int> towers_position_width , vector<int> towers_position_height,vector< bool> tower_taken)
{
	for (int i = 0; i < towers_position_width.size(); ++i)
	{
		if(!tower_taken[i] && is_near_by(mouse_x,mouse_y,towers_position_width[i],towers_position_height[i],TOWER_ACCEPTED_RADIUS))
			return i;
	}
	return towers_position_width.size();
}
bool can_effort(Player a_player, int cost)
{
	return (a_player.score >= cost);
}
void if_tower_selected(Event &event,int &selected_tower_index,bool &valid_tower_selected,Window &main_window,vector<bool> &tower_taken,vector<Tower> &towers, vector<int> &towers_position_width , vector<int> towers_position_height,Player &a_player)
{
	if (event.type() == LCLICK)
	{
		int mouse_x, mouse_y;
		mouse_x=event.mouseX();
		mouse_y=event.mouseY();
		int tower_index=index_positon_for_tower(mouse_x,mouse_y,towers_position_width,towers_position_height,tower_taken);
		if (tower_index!= tower_taken.size()){
			valid_tower_selected=true;
			selected_tower_index=tower_index;
		}
	}
				
	else if (valid_tower_selected && event.type() == KEY_PRESS)
	{
		Tower a_tower;
		a_tower.position_index=selected_tower_index;
		char key_pressed=event.pressedKey();
		if(key_pressed=='a')
		{
			if(!can_effort(a_player,ARCHER_COST ))
				return;
			a_player.score-=ARCHER_COST;
			place_png( main_window, ARCHER_TOWER_IMAGE, TOWER_SIZE, TOWER_SIZE, towers_position_width[selected_tower_index], towers_position_height[selected_tower_index]);
			a_tower.loading_delay=ARCHER_DELAY;
			a_tower.damage=ARCHER_DAMAGE;
			a_tower.type= ARCHER;
			towers.push_back(a_tower);
			tower_taken[selected_tower_index]=true;
		}

		else if (key_pressed=='c')
		{
			if(!can_effort(a_player,CANON_COST ))
				return;
			a_player.score-=CANON_COST;
			place_png( main_window, CANON_TOWER_IMAGE,TOWER_SIZE,TOWER_SIZE, towers_position_width[selected_tower_index], towers_position_height[selected_tower_index]);
			a_tower.loading_delay=CANON_DELAY;
			a_tower.damage=CANON_DAMAGE;
			a_tower.type=CANON;
			towers.push_back(a_tower);
			tower_taken[selected_tower_index]=true;
		}

		else if (key_pressed=='m')
		{
			if(!can_effort(a_player,MAGE_COST ))
				return;
			a_player.score-=MAGE_COST;
			place_png( main_window, MAGE_TOWER_IMAGE,TOWER_SIZE,TOWER_SIZE, towers_position_width[selected_tower_index], towers_position_height[selected_tower_index]);
			a_tower.loading_delay=MAGE_DELAY;
			a_tower.damage=MAGE_DAMAGE;
			a_tower.type=MAGE;
			towers.push_back(a_tower);
			tower_taken[selected_tower_index]=true;
		}

		else if (key_pressed=='i')
		{
			if(!can_effort(a_player,ICY_COST ))
				return;
			a_player.score-=ICY_COST;
			place_png( main_window, ICY_TOWER_IMAGE,TOWER_SIZE,TOWER_SIZE, towers_position_width[selected_tower_index], towers_position_height[selected_tower_index]);
			a_tower.loading_delay=ICY_DELAY;
			a_tower.damage=ICY_DAMAGE;
			a_tower.type=ICY;
			towers.push_back(a_tower);
			tower_taken[selected_tower_index]=true;
		}

		else
			return ;
		
		selected_tower_index=tower_taken.size();
		valid_tower_selected=false;
	}
}
void place_tower(Window &main_window,Tower &a_tower,vector<int> &towers_position_width, vector<int> &towers_position_height)
{
	if (a_tower.type==ARCHER)
		place_png(main_window,ARCHER_TOWER_IMAGE,TOWER_SIZE,TOWER_SIZE,towers_position_width[a_tower.position_index],towers_position_height[a_tower.position_index]);
	else if (a_tower.type==CANON)
		place_png(main_window,CANON_TOWER_IMAGE,TOWER_SIZE,TOWER_SIZE,towers_position_width[a_tower.position_index],towers_position_height[a_tower.position_index]);
	else if (a_tower.type==MAGE)
		place_png(main_window,MAGE_TOWER_IMAGE,TOWER_SIZE,TOWER_SIZE,towers_position_width[a_tower.position_index],towers_position_height[a_tower.position_index]);
	else if (a_tower.type==ICY)
		place_png(main_window,ICY_TOWER_IMAGE,TOWER_SIZE,TOWER_SIZE,towers_position_width[a_tower.position_index],towers_position_height[a_tower.position_index]);
	else
		cout<<"exception"<<endl;

}
void place_towers (vector<Tower> &towers,Window &main_window,vector<int> &towers_position_width, vector<int> &towers_position_height)
{
	for (int i = 0; i < towers.size(); ++i)
	{
		place_tower(main_window,towers[i],towers_position_width,towers_position_height);
	}
}
int get_bullet_speed(tower_type a_type)
{
	if(a_type == ARCHER)
		return ARCHER_BULLET_SPEED;
	else if(a_type == CANON)
		return CANNON_BULLET_SPEED;
	else if(a_type == MAGE)
		return MAGE_BULLET_SPEED;
	else if(a_type == ICY)
		return ICY_BULLET_SPEED;
	else
		return ARCHER_BULLET_SPEED;
}
void shoot_bullet(Tower &a_tower,vector<Bullet> &bullets ,enemy &a_enemy,vector<int> &towers_position_width, vector<int> &towers_position_height)
{
	Bullet a_bullet;
	a_bullet.damage=a_tower.damage;
	a_bullet.type=a_tower.type;
	a_bullet.tower_position_index=a_tower.position_index;
	a_bullet.locked_enemy=&a_enemy;
	a_bullet.speed=get_bullet_speed(a_tower.type);
	a_bullet.position.width=towers_position_width[a_tower.position_index];
	a_bullet.position.height=towers_position_height[a_tower.position_index];
	bullets.push_back(a_bullet);

}
void attack_enemies_from_a_tower(Tower &a_tower,vector<enemy> &current_enemies,vector<Bullet> &bullets , vector<int> &towers_position_width, vector<int> &towers_position_height)
{
	for (int i = 0; i < current_enemies.size(); ++i)
	{
		if(is_near_by(towers_position_width[a_tower.position_index],towers_position_height[a_tower.position_index],current_enemies[i].position.width,current_enemies[i].position.height,TOWER_ATTACK_RANGE)){
			shoot_bullet(a_tower,bullets,current_enemies[i],towers_position_width,towers_position_height);
			a_tower.current_loading_delay=a_tower.loading_delay;
			return;
		}
	}
}
bool can_shoot(Tower a_tower)
{
	if(a_tower.current_loading_delay == 0)
		return true;
	return false;
}
void attack_enemies_from_towers(vector<Tower> &towers, vector<enemy> &current_enemies,vector<Bullet> &bullets,vector<int> &towers_position_width, vector<int> &towers_position_height)
{
	for (int i = 0; i < towers.size(); ++i)
	{
		if(can_shoot(towers[i]))
			attack_enemies_from_a_tower(towers[i],current_enemies,bullets,towers_position_width,towers_position_height);	
	}
}
void update_a_tower_loadin_delays(Tower &a_tower, double time_difference)
{
	if(a_tower.current_loading_delay != 0)
	{
		a_tower.current_loading_delay-=time_difference;
		if(a_tower.current_loading_delay < 0)
			a_tower.current_loading_delay=0;
	}
}
void update_towers_loading_delays(vector<Tower> &towers ,double time_difference)
{
	for (int i = 0; i < towers.size(); ++i)
	{
		update_a_tower_loadin_delays(towers[i],time_difference);
	}
}
void place_a_bullet(Window &main_window,Bullet &a_bullet)
{
	if(a_bullet.type== ARCHER)
			place_png(main_window, ARCHER_BULLET_IMAGE, ARCHER_BULLET_SIZE, ARCHER_BULLET_SIZE, a_bullet.position.width, a_bullet.position.height);
	else if (a_bullet.type == CANON)
			place_png(main_window, CANNON_BULLET_IMAGE, CANNON_BULLET_SIZE,CANNON_BULLET_SIZE, a_bullet.position.width, a_bullet.position.height);
	else if (a_bullet.type == MAGE)
			place_png(main_window, MAGE_BULLET_IMAGE, MAGE_BULLET_SIZE,MAGE_BULLET_SIZE, a_bullet.position.width, a_bullet.position.height);
	else if (a_bullet.type == ICY)
			place_png(main_window, ICY_BULLET_IMAGE, ICY_BULLET_SIZE,ICY_BULLET_SIZE, a_bullet.position.width, a_bullet.position.height);
	else 
			cout<<" undifined boolet"<<endl;

}

void place_bullets(Window &main_window,vector<Bullet> &bullets)
{
	for (int i = 0; i < bullets.size(); ++i)
	{
		place_a_bullet(main_window , bullets[i]);
	}
}
bool is_bullet_in_range(Bullet a_bullet , vector<int> &towers_position_width , vector<int> &towers_position_height)
{
	return is_near_by(a_bullet.position.width , a_bullet.position.height , towers_position_width[a_bullet.tower_position_index],towers_position_height[a_bullet.tower_position_index],TOWER_ATTACK_RANGE);
}
void delete_out_of_range_bullets(vector<Bullet> &bullets,vector<int> &towers_position_width , vector<int> &towers_position_height)
{
	for (int i = 0; i < bullets.size(); ++i)
	{
		if(!is_bullet_in_range(bullets[i],towers_position_width,towers_position_height))
		{
			bullets.erase(bullets.begin()+i);
		}

	}
}
void apply_enemies_bullets_contcts(vector<Bullet> &bullets , vector<enemy> &current_enemies,vector<Bullet> &exploding_bullets)
{
	for (int i = 0; i < bullets.size(); ++i)
	{
		if(is_near_by(bullets[i].position.width, bullets[i].position.height,bullets[i].locked_enemy->position.width,bullets[i].locked_enemy->position.height,EPSILONE))
		{
			if(bullets[i].type == CANON)
			{
				for (int j = 0; j < current_enemies.size(); ++j)
				{
					if (is_near_by(bullets[i].position.width, bullets[i].position.height,current_enemies[j].position.width, current_enemies[j].position.height,CANNON_EXPLOSION_RADIUS))
					{
						current_enemies[j].life-= bullets[i].damage;
						bullets[i].distruction_time=EXPLOSION_DURATION;
						exploding_bullets.push_back( bullets[i]);
					}
				}
			}
			else if (bullets[i].type == ICY)
			{

				for (int j = 0; j < current_enemies.size(); ++j)
				{
					if (is_near_by(bullets[i].position.width, bullets[i].position.height,current_enemies[j].position.width, current_enemies[j].position.height,ICY_FRIZING_RADUS))
					{
						current_enemies[j].life-= bullets[i].damage;
						current_enemies[j].speed=current_enemies[j].default_speed/ICY_SPEED_REDUCTION;
						current_enemies[j].frizing_time=FRIZING_RECOVERY_DURATION;
						bullets[i].distruction_time=FRIZING_DURATION;
						exploding_bullets.push_back( bullets[i]);
					}
				}
			}
			else{
				
				bullets[i].locked_enemy->life -= bullets[i].damage;
			}
			bullets.erase(bullets.begin()+i);
		}
	}
}
void delete_killed_enemies(vector<enemy> &current_enemies, Player &a_player,vector<enemy> &dead_enemies )
{
	for (int i = 0; i < current_enemies.size(); ++i)
	{
		if(current_enemies[i].life <= 0)
		{
			a_player.score+=current_enemies[i].bonus;
			current_enemies[i].dead_time=CORPSES_DECOMPOSITION_TIME;
			dead_enemies.push_back(current_enemies[i]);
			current_enemies.erase(current_enemies.begin()+i);
		}
	}
}
void check_win_or_lose(Player a_player, int number_of_remining_waves, int number_of_alive_enemies , Window &main_window){
	if(a_player.life <= 0)
	{
		Delay(50);
		main_window.draw_png(LOSE_IMAGE,200,200,400,400);
		main_window.update_screen();
		Delay(2000);	
		abort();
	}
	else if (number_of_remining_waves ==0 && number_of_alive_enemies ==0)
	{
		Delay(50);
		main_window.draw_png(WIN_IMAGE,200,200,400,400);
		main_window.update_screen();
		Delay(2000);	
		abort();
	}
}
void show_player_details(Window &main_window, Player a_player)
{
	main_window.show_text(a_player.name+" score : "+to_string(a_player.score)+" life : "+ to_string(a_player.life),SCORE_W,SCORE_H,BLACK,FONT_FREESANS,FONT_SIZE);
}
void decomposit_corpses(vector<enemy> &dead_enemies , double time_difference)
{
	for (int i = 0; i < dead_enemies.size(); ++i)
	{
		dead_enemies[i].dead_time-=time_difference;
		if (dead_enemies[i].dead_time <= 0)
			{
				dead_enemies.erase(dead_enemies.begin()+i);
			}	
	}
}
void place_corps (Window &main_window , enemy a_enemy)
{

	if (a_enemy.type == ARMORED)
	{
		place_png(main_window,ARMORED_DEAD,a_enemy.width,a_enemy.height,a_enemy.position.width,a_enemy.position.height);
	}
	else if (a_enemy.type == WOLF)
	{
		place_png(main_window,WOLF_DEAD,a_enemy.width,a_enemy.height,a_enemy.position.width,a_enemy.position.height);
	}
	else if (a_enemy.type == DEMON)
	{
		place_png(main_window,DEMON_DEAD,a_enemy.width,a_enemy.height,a_enemy.position.width,a_enemy.position.height);
	}
	else if (a_enemy.type == ORC)
	{
		place_png(main_window,ORC_DEAD,a_enemy.width,a_enemy.height,a_enemy.position.width,a_enemy.position.height);
	}
	else
		return;
}
void place_corpses(Window  &main_window , vector<enemy> dead_enemies)
{
	for (int i = 0; i < dead_enemies.size(); ++i)
	{
		place_corps(main_window , dead_enemies[i]);
	}
}
void place_exploding_bullet(Window &main_window,Bullet a_bullet)
{
	if (a_bullet.type == CANON )
	{
		place_png(main_window, EXPLOADING_BULLET, CANNON_EXPLOSION_RADIUS,CANNON_EXPLOSION_RADIUS, a_bullet.position.width, a_bullet.position.height);
	}
	else if (a_bullet.type == ICY)
	{
		place_png(main_window, FRIZING_BULLET, ICY_FRIZING_RADUS,ICY_FRIZING_RADUS, a_bullet.position.width, a_bullet.position.height);
	}
	else 
		return;
}
void update_exploding_bullets_duration_times(vector<Bullet> &exploding_bullets, double time_difference)
{
	for (int i = 0; i < exploding_bullets.size(); ++i)
	{
		exploding_bullets[i].distruction_time -=time_difference;
		if(exploding_bullets[i].distruction_time  <= 0)
			exploding_bullets.erase(exploding_bullets.begin()+i);
	}
}
void place_exploding_bullets(Window &main_window,vector<Bullet> &exploding_bullets)
{
	for (int i = 0; i < exploding_bullets.size(); ++i)
	{
		place_exploding_bullet(main_window,exploding_bullets[i]);
	}
}
void update_frizing_enemies_recovery_time(vector<enemy> &current_enemies , double time_difference)
{
	for (int i = 0; i < current_enemies.size(); ++i)
	{
		if (current_enemies[i].frizing_time != 0.0)
		{
			current_enemies[i].frizing_time -= time_difference;
			if (current_enemies[i].frizing_time <= 0.0)
			{
				current_enemies[i].frizing_time=0.0;
				current_enemies[i].speed=current_enemies[i].default_speed;
			}
		}
	}
}
void starter_page(Window &main_window)
{
	change_background(main_window,START_PAGE_IMAGE);
	Event event;
	bool stay=1;
	while(stay)
	{	
		event=main_window.pollForEvent();
		if (event.type() == LCLICK)
		{
			int mouse_x, mouse_y;
			mouse_x=event.mouseX();
			mouse_y=event.mouseY();
			if (mouse_x <= START_PAGE_W_UPPER_LIMIT && mouse_x >= START_PAGE_W_LOWER_LIMIT && mouse_y <= START_PAGE_H_UPPER_LIMIT && mouse_y >= START_PAGE_H_LOWER_LIMIT )
			{
				stay=0;
			}
		}
	}
}
int main(int argc, char const *argv[])
{

	
	time_t current_time=time(NULL);
	clock_t start=clock();
	clock_t next_time;
	clock_t previous_time=start;
	double time_difference;

	string back_ground_image;
	get_level_background(back_ground_image);

	vector<int> path_points_position_width;
	vector<int> path_points_position_height;
	vector<int> towers_position_width;
	vector<int> towers_position_height;

	get_path_points(path_points_position_width);
	get_path_points(path_points_position_height);
	get_path_points(towers_position_width);
	get_path_points(towers_position_height);

	int selected_tower_index=towers_position_width.size();
	bool valid_tower_selected=false;
	vector<bool> tower_taken(towers_position_width.size(),0);

	vector <Wave> waves;
	get_waves(waves);

	vector<Tower> towers; 
	vector<enemy> current_enemies;
	vector<enemy> dead_enemies;
	vector<Bullet> bullets;
	vector<Bullet> exploding_bullets;
	vector<Bullet> fading_bullets;

	Window main_window (BACKGROUND_WIDTH,BACKGROUND_HEIGHT,"kigndom rush");
	starter_page(main_window);
	change_background(main_window,back_ground_image);

	previous_time=clock();
	Player a_player;
	a_player.name="sajjad";
	time_t start_time=time(NULL);
	Event event=main_window.pollForEvent();

	while(true)
	{
		event=main_window.pollForEvent();
		if_tower_selected(event,selected_tower_index,valid_tower_selected,main_window,tower_taken,towers,towers_position_width , towers_position_height,a_player);
		push_active_waves_to_current_enemies( current_enemies,waves,difftime(time(NULL),start_time),path_points_position_width,path_points_position_height);
		next_time=clock();
		time_difference=10*double (next_time- previous_time) /double (CLOCKS_PER_SEC);
		update_towers_loading_delays(towers,time_difference);
		attack_enemies_from_towers(towers,current_enemies,bullets,towers_position_width,towers_position_height);
		update_frizing_enemies_recovery_time(current_enemies, time_difference);
		move_enemies(a_player,current_enemies,(time_difference),path_points_position_width,path_points_position_height);
		move_bullets(bullets, (time_difference));
		delete_out_of_range_bullets(bullets, towers_position_width, towers_position_height);
		apply_enemies_bullets_contcts(bullets,current_enemies,exploding_bullets);	
		decomposit_corpses(dead_enemies, (time_difference));	
		delete_killed_enemies(current_enemies,a_player,dead_enemies);	
		main_window.clear();
		change_background(main_window,back_ground_image);
		show_player_details(main_window  , a_player);
		place_corpses(main_window, dead_enemies);
		place_towers(towers,main_window,towers_position_width,towers_position_height);
		place_enemies(main_window,current_enemies,path_points_position_width,path_points_position_height);
		place_bullets(main_window,bullets);
		update_exploding_bullets_duration_times(exploding_bullets, (time_difference));
		place_exploding_bullets(main_window , exploding_bullets);
		main_window.update_screen();
		check_win_or_lose(a_player, waves.size(), current_enemies.size(), main_window);
		previous_time=next_time;
		Delay(15);
	}
	
	
}