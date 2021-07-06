#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24cxx.h"
#include "w25qxx.h"
#include "touch.h"
//#include "usart2.h"
#include "rtc.h" 
#include "text.h"
#include "sdio_sdcard.h"  
#include "malloc.h"
#include "sram.h"
#include "exfuns.h" 
#include "usmart.h" 
#include "ff.h" 
#define Max_BUFF_Len 18
unsigned char Uart2_Buffer[Max_BUFF_Len];
unsigned int Uart2_Rx=0;
/************************************************
 ALIENTEK战舰STM32开发板实验27
 触摸屏 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/



int main(void)
{
	u8 key,dot,check;
  u8 t1=0;
	u16 t,i; 
	u8 a0,a1,a2,a3;
	u8 hum10,hum1,tem10,tem1;
//	int i[1];
	u8 checkstr[9]="000000000";
	u8 tem10str[9]="000000000";
	u8 hum10str[9]="000000000"; 
	u8 tem1str[9]="000000000";
	u8 hum1str[9]="000000000"; 
	u16 len;	
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600，zigbee
//	usart2_init(9600);	//串口2初始化为9600,指南针
 	LED_Init();		    	 //LED端口初始化
	LCD_Init();	
	KEY_Init();	 
	RTC_Init();
	W25QXX_Init();				//初始化W25Q128
 	my_mem_init(SRAMIN);		//初始化内部内存池
	exfuns_init();				//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	f_mount(fs[1],"1:",1); 		//挂载FLASH.
	GPIO_SetBits(GPIOC,GPIO_Pin_0); //L
	GPIO_SetBits(GPIOC,GPIO_Pin_1); //L
	GPIO_SetBits(GPIOC,GPIO_Pin_3); //L
	//LED0=1;
		while(font_init()) 			//检查字库
	{
UPD:    
		LCD_Clear(WHITE);		   	//清屏
 		POINT_COLOR=RED;			//设置字体为红色	   	   	  
		LCD_ShowString(30,50,200,16,16,"WarShip STM32");
		while(SD_Init())			//检测SD卡
		{
			LCD_ShowString(30,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(30,70,200+30,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(30,70,200,16,16,"SD Card OK");
		LCD_ShowString(30,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");//更新字库
		while(key)//更新失败		
		{			 		  
			LCD_ShowString(30,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(30,110,200,16,16,"Font Update Success!   ");
		delay_ms(1500);	
		LCD_Clear(WHITE);//清屏	       
	}  

	i=40;
	dot=0x31;
	
	//////////////////////以下为触摸程序
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
//	uart_init(115200);	 	//串口初始化为115200
	 
 	
 	tp_dev.init();
	 
 	POINT_COLOR=RED;//设置字体为红色 
//	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
//	LCD_ShowString(60,70,200,16,16,"TOUCH TEST");	
//	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(60,110,200,16,16,"2015/1/15");
//  LCD_ShowString(60,130,200,16,16,"Press KEY0 to Adjust");
//	key=KEY_Scan(0);	
  if(tp_dev.touchtype!=0XFF)
		LCD_ShowString(60,130,200,16,16,"Press KEY0 to Adjust");//电阻屏才显示
	delay_ms(1500);
		if(KEY0==0)	//KEY0按下,则执行校准程序
		{
		
			LCD_Clear(WHITE);//清屏
		    TP_Adjust();  	//屏幕校准  
//			Load_Drow_Dialog();
		}
		LCD_Clear(WHITE);
//	Load_Drow_Dialog();	 	
//	rtp_test(); 						//电阻屏测试
	////////////////////////
	
		while((USART_RX_STA&0x3fff)==0)
	{
		POINT_COLOR=RED;
		Show_Str(20,190,300,24,"无信号，检查发射端",24,0);	
	}
	LCD_Clear(WHITE);
	
	while(1)
	{

		if(t1!=calendar.sec)
		{
			t1=calendar.sec;
			POINT_COLOR=BLACK;//设置字体为黑色
			Show_Str(10,420,200,16,"这个时间和日期不用太当真",16,0);
	    LCD_ShowString(210,420,200,16,16,"    -  -  ");	   
	    LCD_ShowString(210,455,200,16,16,"  :  :  ");	
			LCD_ShowNum(210,420,calendar.w_year,4,16);									  
			LCD_ShowNum(250,420,calendar.w_month,2,16);									  
			LCD_ShowNum(274,420,calendar.w_date,2,16);	 
			switch(calendar.week)
			{
				case 0:
					Show_Str(210,436,200,16,"   星期日  ",16,0);
					break;
				case 1:
					Show_Str(210,436,200,16,"   星期一   ",16,0);
					break;
				case 2:
					Show_Str(210,436,200,16,"   星期二  ",16,0);
					break;
				case 3:
					Show_Str(210,436,200,16,"   星期三",16,0);
					break;
				case 4:
					Show_Str(210,436,200,16,"   星期四 ",16,0);
					break;
				case 5:
					Show_Str(210,436,200,16,"   星期五   ",16,0);
					break;
				case 6:
					Show_Str(210,436,200,16,"   星期六 ",16,0);
					break;  
			}
			LCD_ShowNum(210,455,calendar.hour,2,16);									  
			LCD_ShowNum(234,455,calendar.min,2,16);									  
			LCD_ShowNum(258,455,calendar.sec,2,16);
//			LED0=!LED0;
		}
							  
		
//		GPIO_SetBits(GPIOC,GPIO_Pin_2); 
//		USART_SendData(USART2, aa);        		 	//向串口2发送数据
		//串口2暂时不需要使用

		 LED0=!LED0;//每次重复，LED运行状态改变一次，用于辨别
		
		 if(USART_RX_STA&0x3fff)
     {
			 uart_init(9600);
			 delay_ms (3);
			 len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			 printf("\r\n您发送的消息为:\r\n\r\n");//返回一个值给电脑，仅用于与电脑连接测试时使用，后面可以删除
			 for(t=0;t<len;t++)
			 {
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				USART_RX_STA=0;
			 }
			 a0=USART_RX_BUF[0];
			 a1=USART_RX_BUF[1];//采集点代号
			 a2=USART_RX_BUF[2];
			 a3=USART_RX_BUF[3];

			 tem10=USART_RX_BUF[4];
			 tem1=USART_RX_BUF[5];
			 hum10=USART_RX_BUF[6];
			 hum1=USART_RX_BUF[7];
			 
			 checkstr[a1-0x30]=a0;
			 tem10str[a1-0x30]=tem10;
			 hum10str[a1-0x30]=hum10;
			 tem1str[a1-0x30]=tem1;
			 hum1str[a1-0x30]=hum1;
			 
			 if(checkstr[dot-0x30]==0x21)
			 {
				 tem10=tem10str[dot-0x30];
				 tem1=tem1str[dot-0x30];
				 hum10=hum10str[dot-0x30];
				 hum1=hum1str[dot-0x30];
				 check=1;
			 }
			 else
				 check=0;
			 
			 USART_RX_STA=0;
        //	LCD_Init();	
		
		 }
		 
		 if(check==0||dot==0x30)
		 {
			 LCD_Clear(WHITE);
			 POINT_COLOR=RED;
			 Show_Str(20,10,300,24,"监测点  无数据",24,0);	
			 LCD_ShowString(95,10,200,24,24,&dot);
			 Show_Str(20,36,300,24,"请重新选择监测点序号",24,0);
			 POINT_COLOR=BLACK;
			 dot=chosedot();
			 dot+=0x30;
			 chosedotprint(dot);
			 i=40;
			 if(USART_RX_STA&0x3fff)
			 {
				 uart_init(9600);
				 delay_ms (3);
				 len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
				 printf("\r\n您发送的消息为:\r\n\r\n");//返回一个值给电脑，仅用于与电脑连接测试时使用，后面可以删除
				 for(t=0;t<len;t++)
				 {
					USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
					USART_RX_STA=0;
				 }
				 a0=USART_RX_BUF[0];
				 a1=USART_RX_BUF[1];//采集点代号
				 a2=USART_RX_BUF[2];
				 a3=USART_RX_BUF[3];

				 tem10=USART_RX_BUF[4];
				 tem1=USART_RX_BUF[5];
				 hum10=USART_RX_BUF[6];
				 hum1=USART_RX_BUF[7];
				 
				 checkstr[a1-0x30]=a0;
				 tem10str[a1-0x30]=tem10;
				 hum10str[a1-0x30]=hum10;
				 tem1str[a1-0x30]=tem1;
				 hum1str[a1-0x30]=hum1;
				 
				 if(checkstr[dot-0x30]==0x21)
				 {
					 tem10=tem10str[dot-0x30];
					 tem1=tem1str[dot-0x30];
					 hum10=hum10str[dot-0x30];
					 hum1=hum1str[dot-0x30];
					 check=1;
				 }
				 else
					 check=0;
				 
				 USART_RX_STA=0;
					//	LCD_Init();	
			
			 }
		 }
		
		 ///以下应该放到定时器中断里面
		 POINT_COLOR=BLACK ;//设置字体为黑色
		 LCD_ShowString(60,50,100,8,16,"test1");	
		 Show_Str(20,70,240,16,"    Evand设计的温湿度彩屏显示",16,0);	
		 Show_Str(20,90,200,16,"无报警温湿度监测试验",16,0);
		 Show_Str(20,110,200,16,"tem&hum√ ",16,0);	
		 LCD_ShowString(20,130,200,16,16,&dot);	
		 Show_Str(30,130,400,16,"号（选择其他监测点请点击此处）",16,0);	
		 POINT_COLOR=RED ;//设置字体为黑色
		 Show_Str(50,130,400,16,"（选择其他监测点请点击此处）",16,0);	
		 POINT_COLOR=BLACK ;//设置字体为黑色
		 Show_Str(20,150,200,16,"温度：   ℃",16,0);	
		 
		 
//		 tem=0x41;
//		 hum=humstr[1]+0x30;
//     Show_Str(120,150,200,16,&tem,16,0);	
	 	 LCD_ShowString(70,150,200,16,16,&tem10);	
		 LCD_ShowString(80,150,200,16,16,&tem1);
	//			LCD_ShowString(60,270,200,16,16,&b2);	
		 Show_Str(20,170,200,16,"湿度：   %",16,0);	
		 LCD_ShowString(70,170,200,16,16,&hum10);	
		 LCD_ShowString(80,170,200,16,16,&hum1);
	//			LCD_ShowString(60,310,200,16,16,&c2);	
//		 Show_Str(20,190,200,16,"别眨眼，这个程序能画图",16,0);	
		 Show_Str(0,292,200,16," 0℃-",16,0);
		 Show_Str(0,252,200,16,"20℃-",16,0);
		 Show_Str(0,212,200,16,"40℃-",16,0);
		 Show_Str(0,322,200,16," 80%-",16,0);
		 Show_Str(0,352,200,16," 50%-",16,0);
		 Show_Str(0,402,200,16,"  0%-",16,0);
			//POINT_COLOR=RED ;
			//LCD_ShowString(150,200,210,24,24,"drinking");	
//		 POINT_COLOR=RED ;//设置字体为红色
//		 Show_Str(20,70,240,16,"帅气",16,0);	
		 delay_ms (1000);
		 //画图
		 LCD_Fill(i+1,415,i+4,415,RED);  //指示当前刷新位置的横坐标
		 evandplotmiss(i,180,420,WHITE);
		 evandplotdot(i,300-((tem10-0x30)*10+tem1-0x30)*2,BLACK);
		 evandplotdot(i,410-((hum10-0x30)*10+hum1-0x30),LIGHTGREEN);
		 
		 tp_dev.scan(0);
		 if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	  if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			  {
				  if(tp_dev.x[0]>20&&tp_dev.x[0]<240&&tp_dev.y[0]>130&&tp_dev.y[0]<150)
					{
						LCD_Clear(WHITE);
					  dot=chosedot();
						dot+=0x30;
						chosedotprint(dot);
						i=40;      //还原绘图的横坐标
				////选择监测点后不能立刻显示，必须重新刷新数据
					
					}
					
			   }
				else
			Show_Str(20,30,400,16,"没有触摸屏按下信息，触摸编号1s即可选择",16,0);
				
		}
		else
			Show_Str(20,30,240,16,"没有有效的触摸屏按下信息",16,0);

		 if(i>320)
		 {
//		 	 LCD_Clear(WHITE);
			 i=40;
		 }
		 else
			 i+=2;
	}
}
