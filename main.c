#include "asf.h"
#include "oled.h"
#include "bmp.h"
#include <math.h>
//#include <time.h>


//变量定义

int length = 8, width = 128, gameover = 0, wall1_num = 0, wall2_num = 0, flag1 = 0, score = 0, flag = 0
, flag_inside = 0, i_bird = 0, wall_num[3] = { 0,0,0}, i, j, k, flag0[3] = { 101,101,101}, score_open[3] = { 0,0,0}
,start=0,bird_width=17,bird_length=12,width_wall=17,clear_flag=1;
double n_1 = 0, n_2;
char _bird1[3], wall_down, wall_up,bird_death,gameover_;
char a;
//鸟的定义
struct bird{
	

	int x,y;
	
};
void bird_show(struct bird* bird_){
	//putimage(x-34, y-24, &_bird1[i_bird % 3], NOTSRCERASE);
	OLED_DrawBMP(bird_->x-bird_width,0,bird_->x,8,BMP_bird[0]);
}
void bird_death_(struct bird* bird_) {
	//putimage(x - 34, y - 24, &bird_death[0], NOTSRCERASE);
	OLED_DrawBMP(bird_->x-17,0,bird_->x,8,BMP_bird_death);
}
struct bird bird1={30,12};
//墙的定义
struct wall{
	
	int y_up;
	int y_down;
	int wall_x;	
};
void wall_getxy( struct wall *wall_){
	wall_->y_up=rand()%5;
	wall_->y_down=rand()%(int)(length-wall_->y_up)*0.6;
	if (wall_->y_up + wall_->y_down >4)
	wall_->y_up  -= 1;
}
/*void show_wall(struct wall* wall_) {
	//putimage(wall_x, y_up - 480, &wall_up[0], NOTSRCERASE);
	OLED_DrawBMP(wall_->wall_x,0,wall_->wall_x+width_wall,wall_->y_up,BMP_wall_up);
	//putimage(wall_x, 480-y_down, &wall_down[0], NOTSRCERASE);
	OLED_DrawBMP(wall_->wall_x,8-wall_->y_down,wall_->wall_x+width_wall,16-wall_->y_down,BMP_wall_down);
}
*/


void show_wall(struct wall *wall_)
{	if(wall_->wall_x+width_wall<128)
	{OLED_DrawBMP_wall_up(wall_->wall_x,0, wall_->wall_x+width_wall, wall_->y_up, BMP_wall_up);
	delay_ms(3);
	OLED_DrawBMP_wall_up(wall_->wall_x+width_wall, 0, wall_->wall_x+width_wall+2,wall_->y_up, BMP_blank);
	OLED_DrawBMP(wall_->wall_x,8-wall_->y_down, wall_->wall_x+width_wall, 8, BMP_wall_down);
	delay_ms(3);
	OLED_DrawBMP(wall_->wall_x+width_wall, 8-wall_->y_down, wall_->wall_x+width_wall+5,8, BMP_blank);
	wall_->wall_x--;
	}
	else
	{
		OLED_DrawBMP_wall_up(wall_->wall_x,0, 127, wall_->y_up, BMP_wall_up);
		OLED_DrawBMP(wall_->wall_x,8-wall_->y_down, 127, 8, BMP_wall_down);
		wall_->wall_x--;
	}
}
struct wall wall_[4];

//小鸟的上下移动
void down(unsigned char BMP[], int step)
{
	if (step == 0)
		return;
	for (int i = 119; i < 136; i++)
		if (BMP[i] >= 128)
			return;

	for (int i = 0; i < 17; i++)
	{
		for (int j = 7; j >= 0; j--)
		{
			if (BMP[i + 17 * j] != 0x00)
			{
				int temp = (int)(BMP[i + 17 * j]);
				BMP[i + 17 * j] = BMP[i + 17 * j] << 1;
				if (temp > 127 && j <= 6)
					BMP[i + 17 * (j + 1)] += 1;
			}
		}
	}
	down(BMP, step - 1);
}
void up(unsigned char BMP[], int step)
{
	if (step == 0)
		return;
	for (int i = 0; i < 17; i++)
		if (BMP[i] % 2 == 1)
			return;

	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (BMP[i + 17 * j] != 0x00)
			{
				int temp = (int)(BMP[i + 17 * j]);
				BMP[i + 17 * j] = BMP[i + 17 * j] >> 1;
				if ((temp % 2 != 0) && (j >= 1))
					BMP[i + 17 * (j - 1)] += 128;
			}
		}
	}
	up(BMP, step - 1);
}

/*void Birdup(int step)
{
	int i = 0;
	while (i < step)
	{
		if (i % 2 == 0) 
		{
			OLED_DrawBMP(8, 0, 25, 7, BMP_bird[0]);
			if (i == 0)
			{
				up(BMP_bird[0], step / 4);
				i += step / 4;
			}
			else
			{
				up(BMP_bird[0], step / 2);
				i += step / 2;
			}
		}
		else
		{
			OLED_DrawBMP(8, 0, 25, 7, BMP_bird[2]);
			up(BMP_bird[2], step/2);
			i += step / 2;
		}
	}
}
*/
//得分函数
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
inline void user_control(){
	if (port_pin_get_input_level(CONTROL)==0) {
		
		if(flag1==0)
		{
			a = 1;
			flag1=5;
		}
		else a = 1;
	}
	
}

inline void common_control(){
	
	bird_show(&bird1);
	i_bird++;
	if (flag1 == 0)//没按
	{
		bird1.y +=(int)2*n_1 ;
		down(BMP_bird[0],(int)(2*n_1));
		n_2 = 0.1;
		n_1 += 0.2;
	}
	else//按了
	{
		
		if ((int)(3-2*n_2)>0){
		up(BMP_bird[0],(int)(3-2*n_2));
		bird1.y-=(int)(3-2*n_2);
		if (bird1.y<12)
		bird1.y=12;
		}
		else{
		down(BMP_bird[0],(int)(2*n_2-3));
		bird1.y+=(int)(3-2*n_2);
		}
		flag1--;
		n_2 += 0.2;
		n_1 = 0;
	}
	
	if(bird1.y>76)bird1.y=76;
}
void wall_control() {
	if (wall_num[0] == 0) {
			wall_[0].wall_x = width;
			wall_getxy(&wall_[0]);
			wall_num[0] = 1;
			flag = 0;
		}
		flag++;
		if (flag == (int)width / 3) {
			wall_[1].wall_x = width;
			wall_getxy(&wall_[1]);
			wall_num[1] = 1;
		}
		if (flag == (int)(2 * (width / 3))) {
			wall_[2].wall_x = width;
			wall_getxy(&wall_[2]);
			wall_num[2] = 1;
		}
	
		for (j = 0; j < 3; j++) {

			if (wall_num[j] == 1) {
				
				show_wall(&wall_[j]);
			
			}
		}
		for (i = 0; i < 3; i++) {
		if (wall_[i].wall_x + width_wall <= 0)
			wall_num[i] = 0;
		}		
	
	for (k = 0; k < 3; k++) {
		if (bird1.x == wall_[k].wall_x + 2)
		{
			score_open[k] = 1;
			flag0[k] = 0;
		}
		if (flag0[k] < width_wall)
		{
			if ((bird1.y - bird_length > wall_[k].y_up*8 ) && (bird1.y < (length - wall_[k].y_down )*8-2));

			else
			gameover = 1;
			flag0[k]++;
		}
		if ((flag0[k] == width_wall) && (score_open[k] == 1)) {
			score++;
			score_open[k] = 0;
		}
	}
	score_show(0,0);
}
void beforegame() {

	while(1){
		if(port_pin_get_input_level(CONTROL)==0)
		{
			delay_ms(500);
			start = 1;
			break;
		}
		else
		OLED_DrawBMP(0, 0, 128,8, BMP_title);
	}
}
 int main(void)
 {	    system_init();

	 //! [setup_init]
	 configure_port_pins();
	 //! [setup_init]

		 u8 t;
		//delay_init();	    	 //延时函数初始化	  
	//	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化
		OLED_Init();			//初始化OLED  
		OLED_Clear() ;
		//time (0);
		/*
		 OLED_ShowString(30,0,"OLED TEST");
		OLED_ShowString(8,2,"ZHONGJINGYUAN");  
		OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		t=' '; 
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		*/	
		t=' ';
		int i=0;
		
	up(BMP_bird[0],8);
	up(BMP_bird[0],8);
	up(BMP_bird[0],8);
	
		//up(BMP_bird[0],24);
	//while(1) {		
		//if (i<8)
		//i+=0.1;
		//else
		//i=0;
		//系统自带测试用代码
		/*
		OLED_Clear();
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		OLED_ShowString(0,2,"1.3' OLED TEST");
		OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		OLED_ShowChar(48,6,t);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	
			
		
		delay_ms(500);
		OLED_Clear();
		delay_ms(500);
		OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay_ms(500);
		*/
		
		//测试用，显示数字
		/*
		OLED_DrawBMP(0,0,5,1,BMP_0);
		OLED_DrawBMP(6,0,11,1,BMP_1);
		OLED_DrawBMP(12,0,17,1,BMP_2);
		OLED_DrawBMP(18,0,23,1,BMP_3);
		OLED_DrawBMP(24,0,29,1,BMP_4);
		OLED_DrawBMP(30,0,35,1,BMP_5);
		OLED_DrawBMP(36,0,41,1,BMP_6);
		OLED_DrawBMP(42,0,47,1,BMP_7);
		OLED_DrawBMP(48,0,53,1,BMP_8);
		OLED_DrawBMP(54,0,59,1,BMP_9);
		
		//测试用，显示鸟的动画
		*/
		/*
		i++;
		if (i%3==0) OLED_DrawBMP(6,2,23,4,BMP_bird_0);
		if (i%3==1) OLED_DrawBMP(6,2,23,4,BMP_bird_1);
		if (i%3==2) OLED_DrawBMP(6,2,23,4,BMP_bird_2);
		delay_ms(250);
		OLED_Clear();
		*/
		//上下动，不加动画效果
		/*
		if(i%2==0){
			
		
		OLED_DrawBMP(8,0,25,7,BMP_bird[0]);
		if(i%8>4)i==0;
		 if(i==0)
			up(BMP_bird[0],2);
			else
			up(BMP_bird[0],4);
			i++;
		}
		else
		{
		OLED_DrawBMP(8,0,25,7,BMP_bird[2]);			
		if(i%8>4)i==0;		
			up(BMP_bird[2],4);
			i++;
		}	
			*/
			
		//上下动，增加动画效果
	/*

		Birdup(4);
		delay_ms(1000);
	
		*/
		//分数测试
		/*
		score=rand()%6;
		score_show(0,0);
		delay_ms(1000);
		score=rand()%(int)(length-wall_->y_up)*0.8;
		score_show(20,0);
		delay_ms(1000);
		*/
		//第一个参数是起始x坐标，第二个参数是起始y页数（顶部为0，一页8个像素），第三个参数是x终止坐标，其差必须等于图片像素宽度，
		//第四个参数是y终止页数，其差必须大于图片像素高度
	    //测试test
		//OLED_DrawBMP(54,1,55,2,BMP_test);
		//墙移动
		/*
			if (wall_num[0] == 0) {
			wall_[0].wall_x = width;
			wall_getxy(&wall_[0]);
			wall_num[0] = 1;
			flag = 0;
		}
		flag++;
		if (flag == (int)width / 3) {
			wall_[1].wall_x = width;
			wall_getxy(&wall_[1]);
			wall_num[1] = 1;
		}
		if (flag == (int)(2 * (width / 3))) {
			wall_[2].wall_x = width;
			wall_getxy(&wall_[2]);
			wall_num[2] = 1;
		}
	
		for (j = 0; j < 3; j++) {

			if (wall_num[j] == 1) {
				
				show_wall(&wall_[j]);
			
			}
		}
		for (i = 0; i < 3; i++) {
		if (wall_[i].wall_x + width_wall <= 0)
			wall_num[i] = 0;
		}		
		
		*/
		
/*
		OLED_DrawBMP(startx,starty, startx+17, 8, BMP_wall_down);
		delay_ms(5);
		OLED_DrawBMP(startx+17, starty, startx+34,8, BMP_blank);
		startx--;
		*/
		//显示墙
		/*
		OLED_DrawBMP_wall_up(50,0, 67, 4, BMP_wall_up);
		
		OLED_DrawBMP(69,2, 86, 8, BMP_wall_down);
		*/
		//测试CONTROL
		/*
		if (port_pin_get_input_level(CONTROL)==0) 
		{score++;
		score_show(0,0);}
		*/
		
		//}
		
		beforegame(); 
		OLED_Clear() ; 
		while(start==1){
	if (bird1.y ==76 || gameover == 1)
	{
		//for (i = 0; i < 4; i++)
		//show_wall_static(&wall_[i]);
				if(bird1.y<76)	{
				bird_death_(&bird1);
				down(BMP_bird_death,3);
				delay_ms(40);
				bird1.y+=3;
				}
				else
				{
					if(clear_flag==1)
						{OLED_Clear() ;
						 clear_flag=0;
						}
					else{
			//putimage(250, 200, &gameover_[0], NOTSRCERASE);
						OLED_DrawBMP(5,2,117,5,BMP_gameover);
						score_show(50,6);
							/*if(port_pin_get_input_level(CONTROL)==0)
							{  
								delay_ms(100);
							OLED_Clear();
							gameover=0;
							up(BMP_bird[0],48);						
							bird1.y-=48;		
							}*/
						}
		
				}
	}
	else{
		
		user_control();
		common_control();
		wall_control();
		/*
		score=bird1.y ;
		score_show(40,0);
		*/
		
		score=bird1.y - bird_length;
		score_show(40,0);
		score=wall_[0].y_up*8 ;
		score_show(60,0);
		score=wall_[1].y_up*8 ;
		score_show(80,0);
		score=wall_[2].y_up*8 ;
		score_show(100,0);
		score=bird1.y;
		score_show(40,2);
		score=(length - wall_[0].y_down )*8-2;
		score_show(60,2);
		score=(length - wall_[1].y_down )*8-2;
		score_show(80,2);
		score=(length - wall_[2].y_down )*8-2;
		score_show(100,2);
		delay_ms(40);
		
	}
	
	
	
	
		}
	
		}

