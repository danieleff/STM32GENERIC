#ifndef _TOUCH_H
#define _TOUCH_H
#include "like51.h"

#if __has_include("ott2001a.h")
  #include("ott2001a.h")
#endif
#ifndef OTT_MAX_TOUCH
  #define OTT_MAX_TOUCH 0
#endif
//////////////////////////////////////////////////////////////////////////////////	 
 
//������������֧��ADS7843/7846/UH7843/7846/XPT2046/TSC2046/OTT2001A�ȣ� ����	   
//STM32F4����----�⺯���汾
//�Ա����̣�http://mcudev.taobao.com								   
//////////////////////////////////////////////////////////////////////////////////
#define TP_PRES_DOWN	0X80  //����������
#define TP_CATH_PRES	0X40  //�а���������
typedef struct
{
	uint8_t (*init)(void);  //��ʼ��������
	uint8_t (*scan)(uint8_t);		 //ɨ�败����0,��Ļɨ��;1,��������
	void (*adjust)(void); //������У׼
	uint16_t x[OTT_MAX_TOUCH];  //��ǰ����
	uint16_t y[OTT_MAX_TOUCH];  //�����������5������,��������Ϊx[0],y[0]����:�˴�ɨ��ʱ,��������������
												 //x[4],y[4]�洢��һ�ΰ���ʱ������
	uint8_t sta;  	//�ʵ�״̬
						//b7:����1/�ɿ�0; 
						//b6:0,û�а�������;1,�а�������. 
						//b5:����
						//b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
/////////////////////������У׼����(����������ҪУ׼)//////////////////////			
	float xfac;
	float yfac;
	float xoff;
	float yoff;
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//b0:0,����(�ʺ�����ΪX����,����ΪY�����TP)
//   1,����(�ʺ�����ΪY����,����ΪX�����TP) 
//b1~6:����.
//b7:0,������
//   1,������ 
	uint8_t touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev; //������������touch.c���涨��

//������оƬ��������	   
#define PEN  		PCin(5)  	//T_PEN
#define DOUT 		PBin(14)   	//T_MISO
#define TDIN 		PBout(15)  	//T_MOSI
#define TCLK 		PBout(13)  	//T_SCK
#define TCS  		PBout(12)  	//T_CS  

//����������
void TP_Write_Byte(uint8_t num);						//�����оƬд��һ������
uint16_t TP_Read_AD(uint8_t CMD);							//��ȡADת��ֵ
uint16_t TP_Read_XOY(uint8_t xy);							//���˲��������ȡ(X/Y)
uint8_t TP_Read_XY(uint16_t *x,uint16_t *y);					//˫�����ȡ(X+Y)
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y);					//����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color);//��һ������У׼��
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color);	//��һ�����
void TP_Save_Adjdata(void);						//����У׼����
uint8_t TP_Get_Adjdata(void);						//��ȡУ׼����
void TP_Adjust(void);							//������У׼
void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac);//��ʾУ׼��Ϣ
//������/������ ���ú���
uint8_t TP_Scan(uint8_t tp);								//ɨ��
uint8_t TP_Init(void);								//��ʼ��
#endif

