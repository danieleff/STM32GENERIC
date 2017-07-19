#include "arduino.h"
#include "sample.h"

#include "MW_STemWin.hn.h"

#define MSG_CHANGE_TEXT (WM_USER + 0)  //用户自定义的消息类型
#define SPEED           1250

static char _acInfoText[40];

static GUI_COLOR _WindowColor1 = GUI_GREEN;  //窗口1的颜色为绿色
static GUI_COLOR _FrameColor1  = GUI_BLUE;  //窗口1的边界为蓝色
static GUI_COLOR _WindowColor2 = GUI_RED; //窗口2的颜色为红色
static GUI_COLOR _FrameColor2  = GUI_YELLOW;//窗口2的边界为黄色
static GUI_COLOR _ChildColor   = GUI_YELLOW;//子窗口的颜色为黄色
static GUI_COLOR _ChildFrame   = GUI_BLACK; //子窗口的边界为黑色

//回调
static WM_CALLBACK * _cbBkWindowOld;


//窗口句柄
static WM_HWIN _hWindow1;   //窗口1
static WM_HWIN _hWindow2; //窗口2
static WM_HWIN _hChild;   //子窗口


//向背景窗口发送一个消息并使背景窗口无效，
//背景窗口的回调函数就会显示发送的消息
static void _ChangeInfoText(char *pStr)
{
  WM_MESSAGE Message;
  Message.MsgId = MSG_CHANGE_TEXT;
  Message.Data.p = pStr;
  WM_SendMessage(WM_HBKWIN, &Message); //向背景窗口发送消息
  WM_InvalidateWindow(WM_HBKWIN);   //使背景窗口失效
}

//显示一些信息
static void _DrawInfoText(char *pStr)
{
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("WindowManager - Sample", 160, 5);
  GUI_SetFont(&GUI_Font8x16);
  GUI_DispStringAtCEOL(pStr, 5, 40); //在指定位置显示字符串并清除至行末
}


//窗口向上移动
//dy:需要移动坐标距离(像素数)
static void _LiftUp(int dy)
{
  int i;
  int tm;
  for (i = 0; i < (dy / 4); i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hWindow1, 0, -4); //移动窗口1
    WM_MoveWindow(_hWindow2, 0, -4); //移动窗口2
    while ((GUI_GetTime() - tm) < 20)
    {
      WM_Exec(); //通过执行回调函数重绘无效窗口
    }
  }

}

//窗口向下移动
//dy:需要移动坐标距离(像素数)
static void _LiftDown(int dy)
{
  int i;
  int tm;
  for (i = 0; i < (dy / 4); i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hWindow1, 0, 4);
    WM_MoveWindow(_hWindow2, 0, 4);
    while ((GUI_GetTime() - tm) < 20)
    {
      WM_Exec();
    }
  }
}


//背景窗口的回调函数
static void _cbBkWindow(WM_MESSAGE *pMsg)
{
  switch (pMsg->MsgId)
  {
    case MSG_CHANGE_TEXT:
      strcpy(_acInfoText, (char const *)pMsg->Data.p);
    case WM_PAINT:
      GUI_SetBkColor(GUI_BLACK);
      GUI_Clear();
      GUI_SetColor(GUI_WHITE);
      GUI_SetFont(&GUI_Font24_ASCII);
      GUI_DispStringHCenterAt("WindowManager - Sample", 160, 5);
      GUI_SetFont(&GUI_Font8x16);
      GUI_DispStringAt(_acInfoText, 5, 40);
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}

//窗口1回调函数
static void _cbWindow1(WM_MESSAGE *pMsg)
{
  GUI_RECT Rect;
  int x;
  int y;

  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      WM_GetInsideRect(&Rect); //返回客户区的坐标
      GUI_SetBkColor(_WindowColor1);
      GUI_SetColor(_FrameColor1);
      GUI_ClearRectEx(&Rect);     //填充一个矩形区域
      GUI_DrawRectEx(&Rect);    //绘制一个矩形
      GUI_SetColor(GUI_WHITE);
      GUI_SetFont(&GUI_Font24_ASCII);
      x = WM_GetWindowSizeX(pMsg->hWin); //获取窗口的X轴大小
      y = WM_GetWindowSizeY(pMsg->hWin); //获取窗口的Y轴大小
      GUI_DispStringHCenterAt("Window 1", x / 2, (y / 2) - 12);
      break;
    default:
      WM_DefaultProc(pMsg);

  }
}


//窗口2回调函数
static void _cbWindow2(WM_MESSAGE *pMsg)
{
  GUI_RECT Rect;
  int x;
  int y;

  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      WM_GetInsideRect(&Rect);  //返回客户区的坐标
      GUI_SetBkColor(_WindowColor2);
      GUI_SetColor(_FrameColor2);
      GUI_ClearRectEx(&Rect);   //填充一个矩形区域
      GUI_DrawRectEx(&Rect);    //绘制一个矩形
      GUI_SetColor(GUI_WHITE);
      GUI_SetFont(&GUI_Font24_ASCII);
      x = WM_GetWindowSizeX(pMsg->hWin);  //获取窗口的X轴大小
      y = WM_GetWindowSizeY(pMsg->hWin);  //获取窗口的Y轴大小
      GUI_DispStringHCenterAt("Window 2", x / 2, (y / 4) - 12);
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}

//子窗口的回调函数
static void _cbChild(WM_MESSAGE *pMsg)
{
  GUI_RECT Rect;
  int x;
  int y;

  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      WM_GetInsideRect(&Rect);
      GUI_SetBkColor(_ChildColor);
      GUI_SetColor(_ChildFrame);
      GUI_ClearRectEx(&Rect);
      GUI_DrawRectEx(&Rect);
      GUI_SetColor(GUI_RED);
      GUI_SetFont(&GUI_Font24_ASCII);
      x = WM_GetWindowSizeX(pMsg->hWin);
      y = WM_GetWindowSizeY(pMsg->hWin);
      GUI_DispStringHCenterAt("Child window", x / 2, (y / 2) - 12);
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}

//窗口1的另一个回调函数
static void _cbDemoCallback1(WM_MESSAGE *pMsg)
{
  int x, y;
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      GUI_SetBkColor(GUI_GREEN);
      GUI_Clear();
      GUI_SetColor(GUI_RED);
      GUI_SetFont(&GUI_FontComic18B_1);
      x = WM_GetWindowSizeX(pMsg->hWin);
      y = WM_GetWindowSizeY(pMsg->hWin);
      GUI_DispStringHCenterAt("Window 1\nanother Callback", x / 2, (y / 2) - 18);
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}


//窗口2的另一个回调函数
static void _cbDemoCallback2(WM_MESSAGE *pMsg)
{
  int x, y;
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      GUI_SetBkColor(GUI_MAGENTA);
      GUI_Clear();
      GUI_SetColor(GUI_YELLOW);
      GUI_SetFont(&GUI_FontComic18B_1);
      x = WM_GetWindowSizeX(pMsg->hWin);
      y = WM_GetWindowSizeY(pMsg->hWin);
      GUI_DispStringHCenterAt("Window 2\nanother Callback", x / 2, (y / 4) - 18);
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}


//设置窗口的背景颜色,用来演示WM_SetDesktopColor()函数
static void _DemoSetDesktopColor(void)
{
  GUI_SetBkColor(GUI_BLUE);
  GUI_Clear();
  _DrawInfoText("WM_SetDesktopColor()");
  GUI_Delay(SPEED * 3 / 2);
  WM_SetDesktopColor(GUI_BLACK);
  GUI_Delay(SPEED / 2);

  GUI_SetBkColor(GUI_BLACK);
  WM_SetDesktopColor(GUI_INVALID_COLOR); //设置桌面窗口颜色
}

//创建窗口的演示函数,主要演示WM_CreateWindow
static void _DemoCreateWindow(void)
{
  _cbBkWindowOld = WM_SetCallback(WM_HBKWIN, _cbBkWindow); //设置背景桌面的回调函数
  //创建窗口
  _ChangeInfoText("WM_CreateWindow()");
  GUI_Delay(SPEED);
  _hWindow1 = WM_CreateWindow(50, 70, 165, 100,  WM_CF_SHOW | WM_CF_MEMDEV, _cbWindow1, 0); //创建窗口1，使用自动存储设备
  GUI_Delay(SPEED / 3);
  _hWindow2 = WM_CreateWindow(105, 125, 165, 100, WM_CF_SHOW | WM_CF_MEMDEV, _cbWindow2, 0); //创建窗口2，使用自动存储设备
  GUI_Delay(SPEED);
}


//创建子窗口，主要演示WM_CreateWindowAsChild()函数
static void _DemoCreateWindowAsChild(void)
{
  _ChangeInfoText("WM_CreateWindowChild()");
  GUI_Delay(SPEED);
  _hChild = WM_CreateWindowAsChild(10, 50, 145, 40, _hWindow2, WM_CF_SHOW | WM_CF_MEMDEV, _cbChild, 0); //向窗口2创建一个子窗口
  GUI_Delay(SPEED);
}


//让指定的窗口无效，主要演示WM_InvalidateWindow()函数
static void _DemoInvalidateWindow(void)
{
  _ChangeInfoText("WM_InvalidateWindow()");
  _WindowColor1 = GUI_BLUE;
  _FrameColor1 = GUI_GREEN;
  GUI_Delay(SPEED);
  //使窗口1无效，会重绘窗口，注意窗口颜色的变化
  WM_InvalidateWindow(_hWindow1);
  GUI_Delay(SPEED);
}

//将指定窗口放在其他同属窗口的前面，主要展示WMBringToTop()函数的使用
static void _DemoBringToTop(void)
{
  _ChangeInfoText("WM_BringToTop()");
  GUI_Delay(SPEED);
  WM_BringToTop(_hWindow1); //使窗口1放在其他同属窗口的前面
  GUI_Delay(SPEED);
}

//移动窗口到指定地方，主要展示WM_MoveTo()函数
static void _DemoMoveTo(void)
{
  int i;
  int tm;
  int tDiff;

  _ChangeInfoText("WM_MoveTo()");
  GUI_Delay(SPEED);
  for (i = 1; i < 56; i++)
  {
    tm = GUI_GetTime();
    WM_MoveTo(_hWindow1, 50 + i, 70 + i); //移动指定窗口到指定位置
    WM_MoveTo(_hWindow2, 105 - i, 125 - i);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  for (i = 1; i < 56; i++)
  {
    tm = GUI_GetTime();
    WM_MoveTo(_hWindow1, 105 - i, 125 - i);
    WM_MoveTo(_hWindow2, 50 + i, 70 + i);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  GUI_Delay(SPEED);
}

//将指定窗口放在其他同属窗口的前面，主要展示WMBringToBottom()函数的使用
static void _DemoBringToBottom(void)
{
  _ChangeInfoText("WM_BringToBottom()");
  GUI_Delay(SPEED);
  WM_BringToBottom(_hWindow1);  //使窗口1放在其他同属窗口的后面
  GUI_Delay(SPEED);
}


//移动窗口指定的距离，主要展示WM_MoveWindow()函数的使用
//WM_MoveWindow()函数是把窗口移动指定距离
//而WM_MoveTo()是将窗口移动到指定位置,注意两个函数的区别
static void _DemoMoveWindow(void)
{
  int i;
  int tm;
  int tDiff;

  _ChangeInfoText("WM_MoveWindow()");
  GUI_Delay(SPEED);
  for (i = 0; i < 55; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hWindow1, 1, 1); //移动窗口1
    WM_MoveWindow(_hWindow2, -1, -1); //移动窗口2
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  for (i = 0; i < 55; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hWindow1, -1, -1);
    WM_MoveWindow(_hWindow2, 1, 1);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  GUI_Delay(SPEED);
}

//隐藏和显示父窗口,主要演示WM_HideWindow()和WM_ShowWindow()函数的使用
//调用WM_HideWindow()或者WM_ShowWindow()这两个函数后并不会立即隐藏或者
//显示窗口，需要执行WM_Exec()命令重绘，如果要立即隐藏或者显示的话应调用
//WM_Paint()函数。GUI_Delay()函数里面调用了WM_Exec()，所以这里我们看到
//的好像是立即隐藏或者显示了窗口
static void _DemoHideShowParent(void)
{
  _ChangeInfoText("WM_HideWindow(Parent)");
  GUI_Delay(SPEED);
  WM_HideWindow(_hWindow2); //隐藏窗口2
  GUI_Delay(SPEED / 3);
  WM_HideWindow(_hWindow1); //隐藏窗口1
  GUI_Delay(SPEED);

  _ChangeInfoText("WM_ShowWindow(Parent)");
  GUI_Delay(SPEED);
  WM_ShowWindow(_hWindow1); //显示窗口1
  WM_ShowWindow(_hWindow2); //显示窗口2
  GUI_Delay(SPEED);

}

//隐藏和显示子窗口,主要演示WM_HideWindow()和WM_ShowWindow()函数的使用
static void _DemoHideShowChild(void)
{
  _ChangeInfoText("WM_HideWindow(Child)");
  GUI_Delay(SPEED);
  WM_HideWindow(_hChild); //隐藏子窗口
  GUI_Delay(SPEED);

  _ChangeInfoText("WM_ShowWindow(Child)");
  GUI_Delay(SPEED);
  WM_ShowWindow(_hChild); //显示子窗口
  GUI_Delay(SPEED);
}

//演示窗口裁剪功能
static void _DemoClipping(void)
{
  int i;
  int tm;
  int tDiff;

  _ChangeInfoText("Demostrating clipping of child");
  for (i = 0; i < 25; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hChild, 1, 0);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  for (i = 0; i < 25; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hChild, 0, 1);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  for (i = 0; i < 50; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hChild, -1, 0);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  for (i = 0; i < 25; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hChild, 0, -1);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  for (i = 0; i < 25; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hChild, 1, 0);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  GUI_Delay(SPEED);
}

//演示使用回调函数来实现窗口的重绘
static void _DemoRedrawing(void)
{
  int i;
  int tm;
  int tDiff;

  _ChangeInfoText("Demonstrating redrawing");
  GUI_Delay(SPEED);
  _LiftUp(40);  //上移40个像素
  GUI_Delay(SPEED / 3);
  _ChangeInfoText("Using a callback for redrawing");
  GUI_Delay(SPEED / 3);
  for (i = 0; i < 55; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hWindow1,  1,  1);
    WM_MoveWindow(_hWindow2, -1, -1);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  for (i = 0; i < 55; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hWindow1, -1, -1);
    WM_MoveWindow(_hWindow2,  1,  1);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  GUI_Delay(SPEED / 4);
  _LiftDown(30);
  GUI_Delay(SPEED / 2);
  _ChangeInfoText("Without redrawing");
  GUI_Delay(SPEED);
  _LiftUp(30);
  GUI_Delay(SPEED / 4);
  WM_SetCallback(WM_HBKWIN, _cbBkWindowOld);  //取消背景窗口的回调函数
  for (i = 0; i < 55; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hWindow1,  1,  1);
    WM_MoveWindow(_hWindow2, -1, -1);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  for (i = 0; i < 55; i++)
  {
    tm = GUI_GetTime();
    WM_MoveWindow(_hWindow1, -1, -1);
    WM_MoveWindow(_hWindow2,  1,  1);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  GUI_Delay(SPEED / 3);
  WM_SetCallback(WM_HBKWIN, _cbBkWindow); //恢复背景窗口的回调函数
  _LiftDown(40);
  GUI_Delay(SPEED);
}

//演示缩放窗口，主要是WM_ResizeWindow()函数的使用
static void _DemoResizeWindow(void)
{
  int i;
  int tm;
  int tDiff;

  _ChangeInfoText("WM_ResizeWindow()");
  GUI_Delay(SPEED);
  _LiftUp(30);
  for (i = 0; i < 20; i++)
  {
    tm = GUI_GetTime();
    WM_ResizeWindow(_hWindow1, 1, 1); //窗口1的X,Y轴每次增加1个像素大小
    WM_ResizeWindow(_hWindow2, -1, -1); //窗口2的X,Y轴每次减小一个像素大小
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  for (i = 0; i < 40; i++)
  {
    tm = GUI_GetTime();
    WM_ResizeWindow(_hWindow1, -1, -1);
    WM_ResizeWindow(_hWindow2,  1,  1);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  for (i = 0; i < 20; i++)
  {
    tm = GUI_GetTime();
    WM_ResizeWindow(_hWindow1,  1,  1);
    WM_ResizeWindow(_hWindow2, -1, -1);
    tDiff = 15 - (GUI_GetTime() - tm);
    GUI_Delay(tDiff);
  }
  _LiftDown(30);
  GUI_Delay(SPEED);
}


//设置回调函数，主要演示WM_SetCallback()函数的使用
static void _DemoSetCallback(void)
{
  _ChangeInfoText("WM_SetCallback()");
  WM_SetCallback(_hWindow1, _cbDemoCallback1);
  WM_InvalidateWindow(_hWindow1);
  GUI_Delay(SPEED / 2);
  WM_SetCallback(_hWindow2, _cbDemoCallback2);
  WM_InvalidateWindow(_hWindow2);
  GUI_Delay(SPEED * 3);
  WM_SetCallback(_hWindow1, _cbWindow1);
  WM_InvalidateWindow(_hWindow1);
  GUI_Delay(SPEED / 2);
  WM_SetCallback(_hWindow2, _cbWindow2);
  WM_InvalidateWindow(_hWindow2);
  GUI_Delay(SPEED);
}

//删除窗口
static void _DemoDeleteWindow(void)
{
  _ChangeInfoText("WM_DeleteWindow()");
  GUI_Delay(SPEED);
  WM_DeleteWindow(_hWindow2); //删除窗口2
  GUI_Delay(SPEED / 3);
  WM_DeleteWindow(_hWindow1); //删除窗口1
  GUI_Delay(SPEED);
  _ChangeInfoText("");
  GUI_Delay(SPEED);

  //恢复窗口颜色和回调函数
  WM_SetCallback(WM_HBKWIN, _cbBkWindowOld);
  _WindowColor1 = GUI_GREEN;
  _WindowColor2 = GUI_RED;
}


//演示函数
void _ShowDemo(void)
{
  GUI_SetBkColor(GUI_BLACK);
  WM_SetCreateFlags(WM_CF_MEMDEV);
  WM_EnableMemdev(WM_HBKWIN);
  while (1)
  {
    _DemoSetDesktopColor();
    _DemoCreateWindow();
    _DemoCreateWindowAsChild();
    _DemoInvalidateWindow();
    _DemoBringToTop();
    _DemoMoveTo();
    _DemoBringToBottom();
    _DemoMoveWindow();
    _DemoHideShowParent();
    _DemoHideShowChild();
    _DemoClipping();
    _DemoRedrawing();
    _DemoResizeWindow();
    _DemoSetCallback();
    _DemoDeleteWindow();
  }
}

