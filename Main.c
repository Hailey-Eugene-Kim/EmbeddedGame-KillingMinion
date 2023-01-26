/***********************************************************/
// 다음은 사용자 함수를 호출하는 루틴임 지우지 말것!
/***********************************************************/

#include "device_driver.h"

#define printf 	Uart_Printf

void User_Main(void);

void Main(void)
{
	MMU_Init();
	Led_Init();
	Key_Poll_Init();
	Uart_Init(115200);
	Timer0_Init();
	Timer3_Buzzer_Init();
	Timer4_Init();
	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0xf800);

	Uart_Printf("Welcome GBOX World!\n");
	User_Main();
	Uart_Printf("Good Bye~\n");
}

/*===== << 여기부터 사용자 프로그램을 작성함 >> =====*/

#include "./Image/img01.h" //눈알 두개 미니언
#include "./Image/img02.h" //눈알 두개 미니언
#include "./Image/img03.h" //눈알 두개 미니언
#include "./Image/img04.h" //외눈박이 미니언
#include "./Image/bg.h"

const unsigned short * Disp[] = {img01, img02, img03, img04};

/* 5:5:5:I Color Definition */

#define BLACK	0x0000
#define WHITE	0xfffe
#define BLUE	0x003e
#define GREEN	0x07c0
#define RED		0xf800

void Intro(void);
void Show_Minions(void);
void Cal_Score(void);
void Show_Dead_Minion(void);

int mouse_num;
int score;

void User_Main(void)
{
	score = 0;
	Intro();

	for(;;)
	{
		Lcd_Clr_Screen(WHITE);

		Show_Minions();

		Cal_Score();

		Lcd_Clr_Screen(RED);
		Lcd_Printf(120,100,WHITE,RED,2,2,"Score: %d", score);
		Timer4_Delay(300);


		if (score == -5)
		{
			Lcd_Clr_Screen(BLACK);
			Lcd_Printf(80,120,RED,BLACK,2,1,"You Lose", 5);//You Lose
			break;
		}
		else if (score == 5)
		{
			Lcd_Clr_Screen(BLACK);
			Lcd_Printf(80,120,BLUE,BLACK,2,1,"You Win", 5);//You Win
			break;
		}
	}
}

void Intro(void)
{
	Lcd_Draw_BMP(0, 0, bg);
	Lcd_Printf(10,50,BLACK,WHITE,1,1,"Time Attack *^____^* Killing Minion~!!");
	Lcd_Printf(10,100,RED,WHITE,1,1,"주어진 시간은 1초");
	Lcd_Printf(10,150,RED,WHITE,1,1,"'외눈박이 미니언'을 죽여라");
	Timer4_Delay(1300);
}

void Show_Minions(void)
{
	//외눈박이 미니언 = Disp[3]

	mouse_num = rand() % 4;

	Lcd_Draw_BMP(130, 30, Disp[(mouse_num + 0) % 4]); //Lcd_Draw_BMP: 그림 그려주는 기능 // 그리기 (0, 0)시작 위치
	Lcd_Draw_BMP(50, 90, Disp[(mouse_num + 1) % 4]);
	Lcd_Draw_BMP(130, 150, Disp[(mouse_num + 2) % 4]);
	Lcd_Draw_BMP(210, 90, Disp[(mouse_num + 3) % 4]);

}

void Cal_Score(void)
{
	Timer0_Repeat(1000);

	for(;;)
	{
		int key;
		key = Key_Get_Pressed(); //사용자 키

		if (key != 0)
		{

			if (mouse_num + key == 4) //사용자가 정답 맞추는 조건 (0, 4) (1, 3) (2, 2)(3, 1)(4, 0)
			{
				score++;
				Show_Dead_Minion();
				break;
			}

			else
			{
				score--;
				break;
			}
		}

		if (Timer0_Check_Expired())
		{
			score--;
			break;
		}

	}

}

void Show_Dead_Minion(void)
{
	//Jerry 빨강 박스 처리

	if (mouse_num == 0) Lcd_Draw_Bar(210, 90, 270, 150, RED);
	if (mouse_num == 1) Lcd_Draw_Bar(130, 150, 190, 210, RED);
	if (mouse_num == 2) Lcd_Draw_Bar(50, 90, 110, 150, RED);
	if (mouse_num == 3) Lcd_Draw_Bar(130, 30, 190, 90, RED);

	Timer4_Delay(200);

}

