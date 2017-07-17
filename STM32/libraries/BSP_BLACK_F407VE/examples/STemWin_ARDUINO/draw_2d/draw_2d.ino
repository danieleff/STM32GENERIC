/*
draw_2d.ino: using STenWin lib demo & sample, whith ARDUINO style.
  hardware:
     board:  stm32f407vet6(BlackF407VE/YL136)  http://wiki.stm32duino.com/index.php?title=STM32F407
       tft:  ili9341 240x320
  software:
     STenWin: v5.32
  huawei huaweiwx@sina.com 2017.7.2
*/

#include "BSP_BLACK_F407VE.h"      //  BLACK_F407VE STemWin config & driver lib
#include "MW_STemWin.h"            //MiddleWare /STemWin 5.32 lib
#include "STemWin.h"               //STemWin arduino style GUI

#define LED  LED_BUILTIN
#define LED1 LED_BUILTIN1

void display_2d(void);
void alpha_display(void);
void drawarcscale(void);
void draw_graph(void);
void draw_polygon(void);

STemWinGUI GUI;
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED1, OUTPUT);
  GUI.Init();
}

void loop() {
  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  display_2d();
  delay(1000);              // wait for a second

  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  alpha_display();
  delay(1000);              // wait for a second

  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  drawarcscale();
  delay(1000);              // wait for a second

  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  draw_graph();
  delay(1000);              // wait for a second

  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  draw_polygon();
  delay(1000);              // wait for a second
}


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

GUI_POINT aEnlargedPoints[GUI_COUNTOF(aPoints)];   //多边形放大后坐标
GUI_POINT aMagnifiedPoints[GUI_COUNTOF(aPoints1)];

//2D图形绘制例程
void display_2d(void)
{
  int i;
  GUI.SetBkColor(GUI_BLUE);   //设置背景颜色
  GUI.Clear();
  
  GUI.SetColor(GUI_YELLOW);
  GUI.SetFont(&GUI_Font24_ASCII);
  GUI.DispStringHCenterAt("Mcudev 2D Display",120,10);
  
  GUI.SetColor(GUI_RED);
  GUI.SetFont(&GUI_Font16_ASCII);
  
  GUI.SetBkColor(GUI_GREEN);
  GUI.ClearRect(10,50,60,100);  //在一个矩形区域填充绿色背景
  GUI.SetBkColor(GUI_BLUE);   //背景改回蓝色
  
  GUI.DrawGradientH(60,50,110,100,0X4117BB,0XC6B6F5); //绘制用水平颜色梯度填充的矩形
  GUI.DrawGradientV(110,50,160,100,0X4117BB,0XC6B6F5);  //绘制用垂直颜色梯度填充的矩形
  GUI.DrawGradientRoundedH(160,50,210,100,25,0X4117BB,0XC6B6F5);  //绘制水平颜色梯度填充的圆角矩形
  GUI.DrawGradientRoundedV(210,50,260,100,25,0X4117BB,0XC6B6F5);  //绘制垂直颜色梯度填充的圆角矩形
  
  GUI.DrawRect(150,120,250,150);  //在当前窗口中指定的位置绘制矩形
  GUI.FillRect(160,120,260,150);  //在当前窗口中指定位置绘制填充的矩形区域
  
  GUI.SetPenSize(5);        //设置画笔颜色,单位像素点
  GUI.DrawLine(10,160,110,230);   //绘制线条
  
  for(i=0;i<50;i+=3) GUI_DrawCircle(100,160,i);   //绘制圆，无法绘制半径大于180的圆
  GUI.FillCircle(220,160,50);           //在指定位置绘制指定尺寸的填充的圆,无法绘制半径大于180的圆

  GUI.SetPenSize(2);
  GUI.SetColor(GUI_CYAN);
  GUI.DrawEllipse(200,220,70,50); //在指定位置绘制指定尺寸的椭圆的轮廓 无法处理大于180的rx/ry参数
  GUI.SetColor(GUI_MAGENTA);
  GUI.FillEllipse(200,220,65,45); //在指定位置绘制指定尺寸的填充的椭圆

  drawarcscale();
  draw_graph();
}

//Alpha混合显示
void alpha_display(void)
{
  GUI.EnableAlpha(1);           //使能Alpha功能
  GUI.SetBkColor(GUI_WHITE);    //设置背景颜色
  GUI.Clear();
  
  GUI.SetTextMode(GUI_TM_TRANS);  //文本透明显示
  GUI.SetColor(GUI_BLACK);
  GUI.SetFont(&GUI_Font32_1);
  GUI.DispStringHCenterAt("Mcudev ALPHA TEST",150,220);
  
  GUI.SetAlpha(0x40);           //设置alpha值0x00-0XFF
  GUI.SetColor(GUI_RED);  
  GUI.FillRect(10,10,300,240);      //绘制红色矩形
  
  GUI.SetAlpha(0x80); 
  GUI.SetColor(GUI_GREEN);
  GUI.FillRect(60,60,300,240);      //绘制绿色矩形
  
  GUI.SetAlpha(0xC0);
  GUI.SetColor(GUI_BLUE);
  GUI.FillRect(110,110,300,240);      //绘制蓝色矩形
  
  GUI.SetAlpha(0x80);
  GUI.SetColor(GUI_YELLOW);       
  GUI.FillRect(160,160,300,240);      //绘制黄色矩形
  
  GUI.SetAlpha(0x40);     
  GUI.SetColor(GUI_BROWN);        
  GUI.FillRect(210,210,300,240);      //绘制棕色矩形
  GUI.SetAlpha(0);            //恢复alpha通道
}

//绘制弧线 绘制简易仪表盘
void drawarcscale(void) 
{
  int x0 = 630;
  int y0 = 380;
  int i;
  char ac[4];
  GUI.SetPenSize( 5 );
  GUI.SetTextMode(GUI_TM_TRANS);
  GUI.SetFont(&GUI_FontComic18B_ASCII);
  GUI.SetColor(GUI_BLACK);
  GUI.DrawArc( x0,y0,150, 150,-30, 210 );
  for (i=0; i<= 23; i++) {
    float a = (-30+i*10)*3.1415926/180;
    int x = -141*cos(a)+x0;
    int y = -141*sin(a)+y0;
    if (i%2 == 0)
    GUI.SetPenSize( 5 );
    else
    GUI.SetPenSize( 4 );
    GUI.DrawPoint(x,y);
    if (i%2 == 0) {
      x = -123*cos(a)+x0;
      y = -130*sin(a)+y0;
      sprintf(ac, "%d", 10*i);
      GUI.SetTextAlign(GUI_TA_VCENTER);
      GUI.DispStringHCenterAt(ac,x,y);
    }
  }
}

//绘制线图
void draw_graph(void)
{
  I16 aY[400];
  int i;
  for(i=0;i<GUI_COUNTOF(aY);i++)  aY[i] = rand()%100;
  GUI.ClearRect(0,300,400,400);   //每次显示前先清除显示区域
  GUI.DrawGraph(aY,GUI_COUNTOF(aY),0,300);
}

//绘制多边形
void draw_polygon(void)
{
  int i, Mag, magy = 24, Count = 4;
  GUI.SetBkColor(GUI_BLUE);
  GUI.SetColor(GUI_YELLOW);
  GUI.Clear();
  
  GUI.SetTextMode(GUI_TM_TRANS);  //透明显示
  GUI.SetFont(&GUI_Font24_ASCII); //设置字体
  GUI.DispStringHCenterAt("Mcudev PLAYGON DISPLAY",150,10);

  GUI.SetColor(GUI_WHITE);
  GUI.SetDrawMode(GUI_DM_XOR);  //设置绘制模式
  GUI.FillPolygon(aPoints,GUI_COUNTOF(aPoints),140,110);
  for(i=1;i<10;i++)
  {
    GUI.EnlargePolygon(aEnlargedPoints,aPoints,GUI_COUNTOF(aPoints),i * 5);   //放大坐标
    GUI.FillPolygon(aEnlargedPoints,GUI_COUNTOF(aPoints),140,110);        //绘制放大坐标后的多边形
  }
  GUI.SetDrawMode(GUI_DM_NORMAL); //恢复默认的绘制模式
  
  GUI.SetColor(GUI_GREEN);
  for (Mag = 1; Mag <= 4; Mag *= 2, Count /= 2) 
  {
    int magx = 160;
    GUI.MagnifyPolygon(aMagnifiedPoints, aPoints, GUI_COUNTOF(aPoints), Mag);   //按指定系数放大多边形
    for (i = Count; i > 0; i--, magx += 40 * Mag) 
    {
      GUI.FillPolygon(aMagnifiedPoints, GUI_COUNTOF(aPoints), magx, magy);      //绘制多边形
    }
    magy += 20 * Mag;
  }
  
  GUI.SetFont(&GUI_Font8x16);
  GUI.DispStringAt("Polygons of arbitrary shape ", 10, 200);
  GUI.DispStringAt("in any color", 10, 220);
  GUI.SetColor(GUI_DARKRED);
  GUI.FillPolygon (&aPointArrow[0],7,200,240);    //绘制多边形
}

