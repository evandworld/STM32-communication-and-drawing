#include "sys.h"
#include "usart2.h"	
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "lcd.h"
#include "touch.h"
#include "delay.h"

void evandplotdot(u16 t,u16 h,u16 *color)
{
		//在指定区域内填充指定颜色块			 
	//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
	//color:要填充的颜色
//	void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
	LCD_Color_Fill(t-1,h-1,t+1,h+1,color);
}
void evandplotmiss(u16 t,u16 min,u16 max,u16 color)
{
	LCD_Fill(t-1,min,t+1,max,color);
}


u8 chosedot(void)
{
	Show_Str(20,70,200,16,"选择需要查看的监测点序号：",16,0);
	Show_Str(20,100,200,16,"测试点1",16,0);
	Show_Str(20,130,200,16,"测试点2",16,0);
	Show_Str(20,160,200,16,"测试点3",16,0);
	Show_Str(20,190,200,16,"测试点4",16,0);
	Show_Str(20,220,200,16,"测试点5",16,0);
	Show_Str(20,250,200,16,"测试点6",16,0);
	Show_Str(20,280,200,16,"测试点7",16,0);
	Show_Str(20,310,200,16,"测试点8",16,0);
	tp_dev.scan(0);
	while(tp_dev.sta&TP_PRES_DOWN)//触摸屏被按下
		tp_dev.scan(0);
	delay_ms(100);
	while(!(tp_dev.sta&TP_PRES_DOWN))
		tp_dev.scan(0);
	if(tp_dev.sta&TP_PRES_DOWN)			
	{
		if(tp_dev.x[0]>20&&tp_dev.x[0]<100&&tp_dev.y[0]>100&&tp_dev.y[0]<120)
		{
			return 1;
		}
		else
		{
			if(tp_dev.x[0]>20&&tp_dev.x[0]<100&&tp_dev.y[0]>130&&tp_dev.y[0]<150)
			{
				return 2;
			}
			else
			{
				if(tp_dev.x[0]>20&&tp_dev.x[0]<100&&tp_dev.y[0]>160&&tp_dev.y[0]<180)
				{
					return 3;
				}
				else
				{
					if(tp_dev.x[0]>20&&tp_dev.x[0]<100&&tp_dev.y[0]>190&&tp_dev.y[0]<210)
					{
						return 4;
					}
					else
					{
						if(tp_dev.x[0]>20&&tp_dev.x[0]<100&&tp_dev.y[0]>220&&tp_dev.y[0]<240)
						{
							return 5;
						}
						else
						{
							if(tp_dev.x[0]>20&&tp_dev.x[0]<100&&tp_dev.y[0]>250&&tp_dev.y[0]<270)
							{
								return 6;
							}
							else
							{
								if(tp_dev.x[0]>20&&tp_dev.x[0]<100&&tp_dev.y[0]>280&&tp_dev.y[0]<300)
								{
								return 7;
								}
								else
								{
									if(tp_dev.x[0]>20&&tp_dev.x[0]<100&&tp_dev.y[0]>310&&tp_dev.y[0]<330)
									{
										return 8;
									}
									else
										return 0;
								}
							}
						}
					}
				}
			}
		}
	}
}
	
void chosedotprint(u8 i)
{
	LCD_Clear(WHITE);
	Show_Str(20,120,200,16,"你选择的测试点为：  号",16,0);
	LCD_ShowString(170,120,200,16,16,&i);
	delay_ms(1000);
	LCD_Clear(WHITE);
}
	