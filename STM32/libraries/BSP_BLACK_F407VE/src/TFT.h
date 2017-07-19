#ifndef __TFT_H__
#define __TFT_H__

#include "STemWin_Config/LCD/BSP_tft_lcd.h"

#ifdef __cplusplus

class  UTFT {
  public:	
	   UTFT(){};
void Init(void){TFTLCD_Init();};													   	//��ʼ��
void DisplayOn(void){LCD_DisplayOn();};													//����ʾ
void DisplayOff(void){LCD_DisplayOff();};													//����ʾ
void Clear(uint16_t Color){LCD_Clear(Color);};	 												//����
void SetCursor(uint16_t Xpos, uint16_t Ypos){LCD_SetCursor( Xpos,  Ypos);};										//���ù��
void DrawPoint(uint16_t x,uint16_t y){LCD_DrawPoint( x, y);};											//����
uint16_t  ReadPoint(uint16_t x,uint16_t y){return LCD_ReadPoint( x, y);}; 											//���� 
void Circle(uint16_t x0,uint16_t y0,uint8_t r){Draw_Circle( x0, y0, r);};										//��Բ
void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){LCD_DrawLine( x1,  y1,  x2,  y2);};							//����
void DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){LCD_DrawRectangle( x1,  y1,  x2,  y2);};		   				//������
void Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color){LCD_Fill( sx, sy, ex, ey, color);};		   				//��䵥ɫ
void ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode){LCD_ShowChar( x, y, num, size, mode);};						//��ʾһ���ַ�
void ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size){LCD_ShowNum( x, y, num, len, size);};  						//��ʾһ������
void ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode){LCD_ShowxNum( x, y, num, len, size, mode);};				//��ʾ ����
void ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p){LCD_ShowString( x, y, width, height, size, p);};		//��ʾһ���ַ���,12/16����
//void Dir(uint8_t dir){LCD_Display_Dir(dir);};						//������Ļ��ʾ����
};
#endif

#endif
