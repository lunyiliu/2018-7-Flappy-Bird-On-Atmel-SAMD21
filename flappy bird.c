#include "asf.h"
#include "oled.h"
int length = 8, width = 128, gameover = 0, wall1_num = 0, wall2_num = 0, flag1 = 0, score = 0, flag = 0
, flag_inside = 0, i_bird = 0, wall_num[4] = { 0,0,0,0 }, i, j, k, flag0[4] = { 101,101,101,101 }, score_open[4] = { 0,0,0,0}
,start=0,bird_width=17,bird_length=8,width_wall=20;
double n_1 = 0, n_2;


char _bird1[3], wall_down, wall_up,bird_death,gameover_;

char a;
void score_show(int a,int b) {
	int score_, _a[5], i = 0,n=0;
	score_ = score;
	do
	{
		_a[i] = score_ % 10;
		score_ = score_ / 10;
		i++;
		n++;
	} while (score_ >= 10);
	if (score >= 10) {
		_a[i] = score_ % 10;
		n++;
	}
	for (int i = n-1; i >= 0; i--)
	{
		//putimage(a+20*(n-i), b, &score1[_a[i]], NOTSRCERASE);
		OLED_DrawBMP(a+6*(n-i),b,a+6*(n-i)+5,b+1,score_bmp[_a[i]]);
	}
}


struct bird{
		

	double x,y;
	
};
void bird_show(struct bird* bird_){
	//putimage(x-34, y-24, &_bird1[i_bird % 3], NOTSRCERASE);
	OLED_DrawBMP(bird_->x-bird_width,bird_->y-bird_length,bird_->x,bird_->y,BMP_bird[i_bird % 3]);
}
void bird_death(struct bird* bird_) {
	//putimage(x - 34, y - 24, &bird_death[0], NOTSRCERASE);
	OLED_DrawBMP(bird_->x-12,bird_->y-3,bird_->x,bird_->y,BMP_bird_death);
}
struct bird bird1={23,4};
class wall{
	private:	
	int y_up;
	int y_down;
	public:
		int wall_x;
		void getxy();
		void show_wall();
		int get_y_up(){return y_up;}
		int get_y_down(){return y_down;}
		
};
void wall::getxy(){
	y_up=rand()%6;
	y_down=rand()%int((length-y_up)*0.8);
	if (y_up + y_down > 6)
		y_up = length - y_down - 2;
}
void wall::show_wall() {
	//putimage(wall_x, y_up - 480, &wall_up[0], NOTSRCERASE);
    OLED_DrawBMP(x,y_up-8,x+width_wall,y_up,BMP_wall_up);
	//putimage(wall_x, 480-y_down, &wall_down[0], NOTSRCERASE);
	 OLED_DrawBMP(x,8-y_down,x+width_wall,16-y_down,BMP_wall_down);
}
wall wall_[4];
void beforegame() {

	while(1){
	if(port_pin_get_input_level(CONTROL)==0)
	{
		start = 1;
		break;
	}			
			}
	 }       

void wall_control() {
	if (wall_num[0] == 0) {
		wall_[0].wall_x = width;
		wall_getxy(&wall_[0]);
		wall_num[0] = 1;
		flag = 0;
	}
	flag++;
	if (flag == (int)width / 4) {
		wall_[1].wall_x = width;
		wall_getxy(&wall_[1]);
		wall_num[1] = 1;
	}
	if (flag == (int)(2 * (width / 4))) {
		wall_[2].wall_x = width;
		wall_getxy(&wall_[2]);
		wall_num[2] = 1;
	}
	if (flag == (int)(3 * (width / 4))) {
		wall_[3].wall_x =width;
		wall_getxy(&wall_[3]);
		wall_num[3] = 1;
	}
	for (j = 0; j < 4; j++) {

		if (wall_num[j] == 1) {
			
			show_wall(&wall_[j]);
			
		}
	}
	for (k = 0; k < 4; k++) {
		if (bird1.x == wall_[k].wall_x + 2)
		{
			score_open[k] = 1;
			flag0[k] = 0;
		}
		if (flag0[k] < width_wall)
		{
			if ((bird1.y - bird_length > wall_[k].get_y_up() -1) && (bird1.y < length - wall_[k].get_y_down() +1));

			else
				gameover = 1;
			flag0[k]++;
		}
		if ((flag0[k] == width_wall) && (score_open[k] == 1)) {
			score++;
			score_open[k] = 0;
		}
	}
}
inline void user_control(){
	if (port_pin_get_input_level(CONTROL)==0) {
      
	if(flag1==0)
		 {
			a = 1;
			flag1=30;
			}
	else a = 1;
			}
	
}

 inline void common_control(){
			bird1.show();	 
			 
			i_bird++;
			if (flag1 == 0)
			{
				bird1.y +=2*n_1 ;
				n_2 = 0.1;
				n_1 += 0.1;
			}
			else
			{
			bird1.y-=5.9-2*n_2;
			flag1--;
			n_2 += 0.1;
			n_1 = 0;
			}
			score_show(0,0);
	
		
	   
}
int main(){
	
	
	srand(unsigned(time(0)));
	
	
	beforegame(); 
	
	while(start==1){	
	
	if (bird1.y > length || gameover == 1)
	{
		for (i = 0; i < 4; i++)
			wall_[i].show_wall();
		bird1.death();
		bird1.y+=3;
		if(bird1.y>64){
		//putimage(250, 200, &gameover_[0], NOTSRCERASE);
		OLED_DrawBMP(20,3,132,6,BMP_gameover);
		score_show(60,7);
		}
}
else{
    wall_control();
	user_control();	
	common_control();}
	OLED_Clear();
	delay_ms(100);
	}
	
	
}
