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
 ALIENTEKս��STM32������ʵ��27
 ������ ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
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
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600��zigbee
//	usart2_init(9600);	//����2��ʼ��Ϊ9600,ָ����
 	LED_Init();		    	 //LED�˿ڳ�ʼ��
	LCD_Init();	
	KEY_Init();	 
	RTC_Init();
	W25QXX_Init();				//��ʼ��W25Q128
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH.
	GPIO_SetBits(GPIOC,GPIO_Pin_0); //L
	GPIO_SetBits(GPIOC,GPIO_Pin_1); //L
	GPIO_SetBits(GPIOC,GPIO_Pin_3); //L
	//LED0=1;
		while(font_init()) 			//����ֿ�
	{
UPD:    
		LCD_Clear(WHITE);		   	//����
 		POINT_COLOR=RED;			//��������Ϊ��ɫ	   	   	  
		LCD_ShowString(30,50,200,16,16,"WarShip STM32");
		while(SD_Init())			//���SD��
		{
			LCD_ShowString(30,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(30,70,200+30,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(30,70,200,16,16,"SD Card OK");
		LCD_ShowString(30,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");//�����ֿ�
		while(key)//����ʧ��		
		{			 		  
			LCD_ShowString(30,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(30,110,200,16,16,"Font Update Success!   ");
		delay_ms(1500);	
		LCD_Clear(WHITE);//����	       
	}  

	i=40;
	dot=0x31;
	
	//////////////////////����Ϊ��������
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	 
 	
 	tp_dev.init();
	 
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
//	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
//	LCD_ShowString(60,70,200,16,16,"TOUCH TEST");	
//	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(60,110,200,16,16,"2015/1/15");
//  LCD_ShowString(60,130,200,16,16,"Press KEY0 to Adjust");
//	key=KEY_Scan(0);	
  if(tp_dev.touchtype!=0XFF)
		LCD_ShowString(60,130,200,16,16,"Press KEY0 to Adjust");//����������ʾ
	delay_ms(1500);
		if(KEY0==0)	//KEY0����,��ִ��У׼����
		{
		
			LCD_Clear(WHITE);//����
		    TP_Adjust();  	//��ĻУ׼  
//			Load_Drow_Dialog();
		}
		LCD_Clear(WHITE);
//	Load_Drow_Dialog();	 	
//	rtp_test(); 						//����������
	////////////////////////
	
		while((USART_RX_STA&0x3fff)==0)
	{
		POINT_COLOR=RED;
		Show_Str(20,190,300,24,"���źţ���鷢���",24,0);	
	}
	LCD_Clear(WHITE);
	
	while(1)
	{

		if(t1!=calendar.sec)
		{
			t1=calendar.sec;
			POINT_COLOR=BLACK;//��������Ϊ��ɫ
			Show_Str(10,420,200,16,"���ʱ������ڲ���̫����",16,0);
	    LCD_ShowString(210,420,200,16,16,"    -  -  ");	   
	    LCD_ShowString(210,455,200,16,16,"  :  :  ");	
			LCD_ShowNum(210,420,calendar.w_year,4,16);									  
			LCD_ShowNum(250,420,calendar.w_month,2,16);									  
			LCD_ShowNum(274,420,calendar.w_date,2,16);	 
			switch(calendar.week)
			{
				case 0:
					Show_Str(210,436,200,16,"   ������  ",16,0);
					break;
				case 1:
					Show_Str(210,436,200,16,"   ����һ   ",16,0);
					break;
				case 2:
					Show_Str(210,436,200,16,"   ���ڶ�  ",16,0);
					break;
				case 3:
					Show_Str(210,436,200,16,"   ������",16,0);
					break;
				case 4:
					Show_Str(210,436,200,16,"   ������ ",16,0);
					break;
				case 5:
					Show_Str(210,436,200,16,"   ������   ",16,0);
					break;
				case 6:
					Show_Str(210,436,200,16,"   ������ ",16,0);
					break;  
			}
			LCD_ShowNum(210,455,calendar.hour,2,16);									  
			LCD_ShowNum(234,455,calendar.min,2,16);									  
			LCD_ShowNum(258,455,calendar.sec,2,16);
//			LED0=!LED0;
		}
							  
		
//		GPIO_SetBits(GPIOC,GPIO_Pin_2); 
//		USART_SendData(USART2, aa);        		 	//�򴮿�2��������
		//����2��ʱ����Ҫʹ��

		 LED0=!LED0;//ÿ���ظ���LED����״̬�ı�һ�Σ����ڱ��
		
		 if(USART_RX_STA&0x3fff)
     {
			 uart_init(9600);
			 delay_ms (3);
			 len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			 printf("\r\n�����͵���ϢΪ:\r\n\r\n");//����һ��ֵ�����ԣ���������������Ӳ���ʱʹ�ã��������ɾ��
			 for(t=0;t<len;t++)
			 {
				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				USART_RX_STA=0;
			 }
			 a0=USART_RX_BUF[0];
			 a1=USART_RX_BUF[1];//�ɼ������
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
			 Show_Str(20,10,300,24,"����  ������",24,0);	
			 LCD_ShowString(95,10,200,24,24,&dot);
			 Show_Str(20,36,300,24,"������ѡ��������",24,0);
			 POINT_COLOR=BLACK;
			 dot=chosedot();
			 dot+=0x30;
			 chosedotprint(dot);
			 i=40;
			 if(USART_RX_STA&0x3fff)
			 {
				 uart_init(9600);
				 delay_ms (3);
				 len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				 printf("\r\n�����͵���ϢΪ:\r\n\r\n");//����һ��ֵ�����ԣ���������������Ӳ���ʱʹ�ã��������ɾ��
				 for(t=0;t<len;t++)
				 {
					USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
					USART_RX_STA=0;
				 }
				 a0=USART_RX_BUF[0];
				 a1=USART_RX_BUF[1];//�ɼ������
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
		
		 ///����Ӧ�÷ŵ���ʱ���ж�����
		 POINT_COLOR=BLACK ;//��������Ϊ��ɫ
		 LCD_ShowString(60,50,100,8,16,"test1");	
		 Show_Str(20,70,240,16,"    Evand��Ƶ���ʪ�Ȳ�����ʾ",16,0);	
		 Show_Str(20,90,200,16,"�ޱ�����ʪ�ȼ������",16,0);
		 Show_Str(20,110,200,16,"tem&hum�� ",16,0);	
		 LCD_ShowString(20,130,200,16,16,&dot);	
		 Show_Str(30,130,400,16,"�ţ�ѡ���������������˴���",16,0);	
		 POINT_COLOR=RED ;//��������Ϊ��ɫ
		 Show_Str(50,130,400,16,"��ѡ���������������˴���",16,0);	
		 POINT_COLOR=BLACK ;//��������Ϊ��ɫ
		 Show_Str(20,150,200,16,"�¶ȣ�   ��",16,0);	
		 
		 
//		 tem=0x41;
//		 hum=humstr[1]+0x30;
//     Show_Str(120,150,200,16,&tem,16,0);	
	 	 LCD_ShowString(70,150,200,16,16,&tem10);	
		 LCD_ShowString(80,150,200,16,16,&tem1);
	//			LCD_ShowString(60,270,200,16,16,&b2);	
		 Show_Str(20,170,200,16,"ʪ�ȣ�   %",16,0);	
		 LCD_ShowString(70,170,200,16,16,&hum10);	
		 LCD_ShowString(80,170,200,16,16,&hum1);
	//			LCD_ShowString(60,310,200,16,16,&c2);	
//		 Show_Str(20,190,200,16,"��գ�ۣ���������ܻ�ͼ",16,0);	
		 Show_Str(0,292,200,16," 0��-",16,0);
		 Show_Str(0,252,200,16,"20��-",16,0);
		 Show_Str(0,212,200,16,"40��-",16,0);
		 Show_Str(0,322,200,16," 80%-",16,0);
		 Show_Str(0,352,200,16," 50%-",16,0);
		 Show_Str(0,402,200,16,"  0%-",16,0);
			//POINT_COLOR=RED ;
			//LCD_ShowString(150,200,210,24,24,"drinking");	
//		 POINT_COLOR=RED ;//��������Ϊ��ɫ
//		 Show_Str(20,70,240,16,"˧��",16,0);	
		 delay_ms (1000);
		 //��ͼ
		 LCD_Fill(i+1,415,i+4,415,RED);  //ָʾ��ǰˢ��λ�õĺ�����
		 evandplotmiss(i,180,420,WHITE);
		 evandplotdot(i,300-((tem10-0x30)*10+tem1-0x30)*2,BLACK);
		 evandplotdot(i,410-((hum10-0x30)*10+hum1-0x30),LIGHTGREEN);
		 
		 tp_dev.scan(0);
		 if(tp_dev.sta&TP_PRES_DOWN)			//������������
		{	
		 	  if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			  {
				  if(tp_dev.x[0]>20&&tp_dev.x[0]<240&&tp_dev.y[0]>130&&tp_dev.y[0]<150)
					{
						LCD_Clear(WHITE);
					  dot=chosedot();
						dot+=0x30;
						chosedotprint(dot);
						i=40;      //��ԭ��ͼ�ĺ�����
				////ѡ��������������ʾ����������ˢ������
					
					}
					
			   }
				else
			Show_Str(20,30,400,16,"û�д�����������Ϣ���������1s����ѡ��",16,0);
				
		}
		else
			Show_Str(20,30,240,16,"û����Ч�Ĵ�����������Ϣ",16,0);

		 if(i>320)
		 {
//		 	 LCD_Clear(WHITE);
			 i=40;
		 }
		 else
			 i+=2;
	}
}
