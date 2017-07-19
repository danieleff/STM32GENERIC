#include "arduino.h"
#include "MW_STemWin.h"
#include "display2d.h"

const GUI_POINT aPoints[]={
	{40,20},
	{0,20},
	{20,0}
};

const GUI_POINT aPoints1[]={
	{0,20},
	{40,20},
	{20,0}
};

const GUI_POINT aPointArrow[]={
	{ 0, -5},
	{-40, -35},
	{-10, -25},
	{-10, -85},
	{ 10, -85},
	{ 10, -25},
	{ 40, -35},
};

GUI_POINT aEnlargedPoints[GUI_COUNTOF(aPoints)]; 	//����ηŴ������
GUI_POINT aMagnifiedPoints[GUI_COUNTOF(aPoints1)];

//2Dͼ�λ�������
void display_2d(void)
{
	int i;
	GUI_SetBkColor(GUI_BLUE);		//���ñ�����ɫ
	GUI_Clear();
	
	GUI_SetColor(GUI_YELLOW);
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_DispStringHCenterAt("Mcudev 2D Display",120,10);
	
	GUI_SetColor(GUI_RED);
	GUI_SetFont(&GUI_Font16_ASCII);
	
	GUI_SetBkColor(GUI_GREEN);
	GUI_ClearRect(10,50,60,100);	//��һ���������������ɫ����
	GUI_SetBkColor(GUI_BLUE);		//�����Ļ���ɫ
	
	GUI_DrawGradientH(60,50,110,100,0X4117BB,0XC6B6F5);	//������ˮƽ��ɫ�ݶ����ľ���
	GUI_DrawGradientV(110,50,160,100,0X4117BB,0XC6B6F5);	//�����ô�ֱ��ɫ�ݶ����ľ���
	GUI_DrawGradientRoundedH(160,50,210,100,25,0X4117BB,0XC6B6F5);	//����ˮƽ��ɫ�ݶ�����Բ�Ǿ���
	GUI_DrawGradientRoundedV(210,50,260,100,25,0X4117BB,0XC6B6F5);	//���ƴ�ֱ��ɫ�ݶ�����Բ�Ǿ���
	
	GUI_DrawRect(150,120,250,150);	//�ڵ�ǰ������ָ����λ�û��ƾ���
	GUI_FillRect(160,120,260,150);	//�ڵ�ǰ������ָ��λ�û������ľ�������
	
	GUI_SetPenSize(5);				//���û�����ɫ,��λ���ص�
	GUI_DrawLine(10,160,110,230);   //��������
	
	for(i=0;i<50;i+=3) GUI_DrawCircle(100,160,i); 	//����Բ���޷����ư뾶����180��Բ
	GUI_FillCircle(220,160,50); 					//��ָ��λ�û���ָ���ߴ������Բ,�޷����ư뾶����180��Բ

	GUI_SetPenSize(2);
	GUI_SetColor(GUI_CYAN);
	GUI_DrawEllipse(200,220,70,50);	//��ָ��λ�û���ָ���ߴ����Բ������ �޷��������180��rx/ry����
	GUI_SetColor(GUI_MAGENTA);
	GUI_FillEllipse(200,220,65,45);	//��ָ��λ�û���ָ���ߴ��������Բ

	drawarcscale();
	draw_graph();
}

//Alpha�����ʾ
void alpha_display(void)
{
	GUI_EnableAlpha(1);				//ʹ��Alpha����
	GUI_SetBkColor(GUI_WHITE);		//���ñ�����ɫ
	GUI_Clear();
	
	GUI_SetTextMode(GUI_TM_TRANS);	//�ı�͸����ʾ
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font32_1);
	GUI_DispStringHCenterAt("Mcudev ALPHA TEST",150,220);
	
	GUI_SetAlpha(0x40);						//����alphaֵ0x00-0XFF
	GUI_SetColor(GUI_RED);	
	GUI_FillRect(10,10,300,240);			//���ƺ�ɫ����
	
	GUI_SetAlpha(0x80);	
	GUI_SetColor(GUI_GREEN);
	GUI_FillRect(60,60,300,240);			//������ɫ����
	
	GUI_SetAlpha(0xC0);
	GUI_SetColor(GUI_BLUE);
	GUI_FillRect(110,110,300,240);			//������ɫ����
	
	GUI_SetAlpha(0x80);
	GUI_SetColor(GUI_YELLOW);				
	GUI_FillRect(160,160,300,240);			//���ƻ�ɫ����
	
	GUI_SetAlpha(0x40);			
	GUI_SetColor(GUI_BROWN);				
	GUI_FillRect(210,210,300,240);			//������ɫ����
	GUI_SetAlpha(0);						//�ָ�alphaͨ��
}

//���ƻ��� ���Ƽ����Ǳ���
void drawarcscale(void) 
{
	int x0 = 630;
	int y0 = 380;
	int i;
	char ac[4];
	GUI_SetPenSize( 5 );
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontComic18B_ASCII);
	GUI_SetColor(GUI_BLACK);
	GUI_DrawArc( x0,y0,150, 150,-30, 210 );
	for (i=0; i<= 23; i++) {
		float a = (-30+i*10)*3.1415926/180;
		int x = -141*cos(a)+x0;
		int y = -141*sin(a)+y0;
		if (i%2 == 0)
		GUI_SetPenSize( 5 );
		else
		GUI_SetPenSize( 4 );
		GUI_DrawPoint(x,y);
		if (i%2 == 0) {
			x = -123*cos(a)+x0;
			y = -130*sin(a)+y0;
			sprintf(ac, "%d", 10*i);
			GUI_SetTextAlign(GUI_TA_VCENTER);
			GUI_DispStringHCenterAt(ac,x,y);
		}
	}
}

//������ͼ
void draw_graph(void)
{
	I16 aY[400];
	int i;
	for(i=0;i<GUI_COUNTOF(aY);i++)	aY[i] = rand()%100;
	GUI_ClearRect(0,300,400,400);   //ÿ����ʾǰ�������ʾ����
	GUI_DrawGraph(aY,GUI_COUNTOF(aY),0,300);
}

//���ƶ����
void draw_polygon(void)
{
	int i, Mag, magy = 24, Count = 4;
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();
	
	GUI_SetTextMode(GUI_TM_TRANS);	//͸����ʾ
	GUI_SetFont(&GUI_Font24_ASCII);	//��������
	GUI_DispStringHCenterAt("Mcudev PLAYGON DISPLAY",150,10);

	GUI_SetColor(GUI_WHITE);
	GUI_SetDrawMode(GUI_DM_XOR);	//���û���ģʽ
	GUI_FillPolygon(aPoints,GUI_COUNTOF(aPoints),140,110);
	for(i=1;i<10;i++)
	{
		GUI_EnlargePolygon(aEnlargedPoints,aPoints,GUI_COUNTOF(aPoints),i * 5); 	//�Ŵ�����
		GUI_FillPolygon(aEnlargedPoints,GUI_COUNTOF(aPoints),140,110);				//���ƷŴ������Ķ����
	}
	GUI_SetDrawMode(GUI_DM_NORMAL);	//�ָ�Ĭ�ϵĻ���ģʽ
	
	GUI_SetColor(GUI_GREEN);
	for (Mag = 1; Mag <= 4; Mag *= 2, Count /= 2) 
	{
		int magx = 160;
		GUI_MagnifyPolygon(aMagnifiedPoints, aPoints, GUI_COUNTOF(aPoints), Mag); 	//��ָ��ϵ���Ŵ�����
		for (i = Count; i > 0; i--, magx += 40 * Mag) 
		{
			GUI_FillPolygon(aMagnifiedPoints, GUI_COUNTOF(aPoints), magx, magy);			//���ƶ����
		}
		magy += 20 * Mag;
	}
	
	GUI_SetFont(&GUI_Font8x16);
	GUI_DispStringAt("Polygons of arbitrary shape ", 10, 200);
	GUI_DispStringAt("in any color", 10, 220);
	GUI_SetColor(GUI_DARKRED);
	GUI_FillPolygon (&aPointArrow[0],7,200,240);		//���ƶ����
}


