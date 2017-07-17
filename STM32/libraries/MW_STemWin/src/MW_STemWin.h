#ifndef  __STemWin_H__
#define  __STemWin_H__

#include "STemWin/inc/GUI.h"
#include "STemWin/inc/WM.h"

/* class */
#ifdef __cplusplus

class emWinGUI
{
  public:	
		emWinGUI(){};
//int Init(void){
// 	   TFTLCD_Init();
//	   return GUI_Init();
//	};
		int IsInitialized(void){ return  GUI_IsInitialized();};
		void  Exit(void){return GUI_Exit();};
		void  SetDefaultFont(const GUI_FONT * pFont){GUI_SetDefaultFont(pFont);};
		void  SetDefault(void){GUI_SetDefault();};
		GUI_DRAWMODE SetDrawMode(GUI_DRAWMODE dm){return GUI_SetDrawMode(dm);};
		const char * GetVersionString(void){return GUI_GetVersionString();};
		void  SaveContext(GUI_CONTEXT * pContext){ GUI_SaveContext(pContext);};
		void  RestoreContext(const GUI_CONTEXT * pContext){GUI_RestoreContext(pContext);};
		void  GUI_SetScreenSizeX(int xSize){GUI_SetScreenSizeX(xSize);};
		void  SetScreenSizeY(int ySize){GUI_SetScreenSizeY(ySize);};
		int   GetScreenSizeX(void){return GUI_GetScreenSizeX();};
		int   GetScreenSizeY(void){return GUI_GetScreenSizeY();};
		const GUI_RECT * SetClipRect(const GUI_RECT * pRect){return GUI_SetClipRect(pRect);};

/*********************************************************************
*
*       Get / Set Attributes
*/
		GUI_COLOR getBkColor(void){return GUI_GetBkColor();};
		int       getBkColorIndex(void){return GUI_GetBkColorIndex();};
		GUI_COLOR getColor(void){return GUI_GetColor();};
		int       GetColorIndex(void){return GUI_GetColorIndex();};
		U8        GetLineStyle   (void){ return GUI_GetLineStyle();};
		U8        GetPenSize     (void){return GUI_GetPenSize();};
		U8        GetPenShape    (void){return GUI_GetPenShape();};
		unsigned  GetPixelIndex  (int x,int y){return GUI_GetPixelIndex  (x,y);};

		void      SetBkColor   (GUI_COLOR c){GUI_SetBkColor   (c);};
		void      SetColor     (GUI_COLOR c){GUI_SetColor     (c);};
		void      SetBkColorIndex(int Index){GUI_SetBkColorIndex(Index);};
		void      SetColorIndex(int Index){GUI_SetColorIndex(Index);};

		U8        SetPenSize   (U8 Size){return GUI_SetPenSize(Size);};
		U8        SetPenShape  (U8 Shape){return GUI_SetPenShape(Shape);};
		U8        SetLineStyle (U8 Style){return GUI_SetLineStyle(Style);};

/* Get/Set Character used as decimal point (usually '.' or ',') */
		char      GetDecChar(void){return GUI_GetDecChar();};
		char      SetDecChar(char c){return GUI_SetDecChar(c);};

/*
*
*       Color / Index related functions};
*/

		int       Color2Index(GUI_COLOR color){return GUI_Color2Index(color);};
		GUI_COLOR Color2VisColor(GUI_COLOR color){return GUI_Color2VisColor(color);};
		char      ColorIsAvailable(GUI_COLOR color){return GUI_ColorIsAvailable(color);};
		GUI_COLOR Index2Color(int Index){return GUI_Index2Color(Index);};
		uint32_t       CalcColorDist (GUI_COLOR Color0,GUI_COLOR  Color1){return GUI_CalcColorDist (Color0,Color1);};
		uint32_t       CalcVisColorError(GUI_COLOR color){return GUI_CalcVisColorError(color);};

/*
*       2d - GL
*/

		void Clear            (void){GUI_Clear();};
		void ClearRect        (int x0,int y0,int x1,int y1){GUI_ClearRect(x0,y0,x1,y1);};
		void ClearRectEx      (const GUI_RECT * pRect){GUI_ClearRectEx(pRect);};
		void CopyRect         (int x0,int y0,int x1,int y1,int dx,int dy){GUI_CopyRect(x0,y0,x1,y1,dx,dy);};
		void DrawArc          (int x0,int y0,int rx,int ry,int a0,int a1){GUI_DrawArc(x0,y0,rx,ry,a0,a1);};
		void DrawBitmap       (const GUI_BITMAP * pBM,int x0,int y0){GUI_DrawBitmap(pBM,x0,y0);};
		void DrawBitmapMag    (const GUI_BITMAP * pBM,int x0,int y0,int XMul,int YMul){GUI_DrawBitmapMag(pBM,x0,y0,XMul,YMul);};
		void DrawBitmapEx     (const GUI_BITMAP * pBM,int x0,int y0,int xCenter,int yCenter,int xMag,int yMag){
			GUI_DrawBitmapEx(pBM,x0,y0,xCenter,yCenter,xMag,yMag);};
		void DrawBitmapExp    (int x0,int y0,int XSize,int YSize,int XMul, int YMul,int BitsPerPixel,int BytesPerLine,const U8 * pData,const GUI_LOGPALETTE * pPal){
			   GUI_DrawBitmapExp(x0,y0,XSize,YSize,XMul, YMul,BitsPerPixel,BytesPerLine,pData,pPal);};
		void DrawBitmapHWAlpha(const GUI_BITMAP * pBM,int x0,int y0){GUI_DrawBitmapHWAlpha(pBM,x0,y0);};
		void DrawCircle       (int x0,int y0,int r){GUI_DrawCircle(x0,y0,r);};
		void DrawEllipse      (int x0,int y0,int rx,int ry){GUI_DrawEllipse(x0,y0,rx,ry);};
		void DrawGradientH    (int x0,int y0,int x1,int y1,GUI_COLOR Color0,GUI_COLOR Color1){GUI_DrawGradientH(x0,y0,x1,y1, Color0, Color1);};
		void DrawGradientV    (int x0,int y0,int x1,int y1,GUI_COLOR Color0,GUI_COLOR Color1){GUI_DrawGradientV(x0,y0,x1,y1, Color0, Color1);};
		void DrawGradientRoundedH(int x0,int y0,int x1,int y1,int rd,GUI_COLOR Color0,GUI_COLOR Color1){GUI_DrawGradientRoundedH(x0,y0,x1,y1, rd, Color0, Color1);};
		void DrawGradientRoundedV(int x0,int y0,int x1,int y1,int rd,GUI_COLOR Color0,GUI_COLOR Color1){GUI_DrawGradientRoundedV(x0,y0,x1, y1, rd, Color0, Color1);};
		void DrawGraph        (I16 * pay,int NumPoints,int x0,int y0){GUI_DrawGraph(pay, NumPoints, x0, y0);};
		void DrawGraphEx      (I16 * pay,int NumPoints,int x0,int y0,int Numerator,int Denominator,int MirrorX){
			GUI_DrawGraphEx(pay, NumPoints, x0, y0, Numerator, Denominator, MirrorX);};
		void DrawHLine        (int y0,int x0,int x1){GUI_DrawHLine(y0, x0, x1);};
		void DrawLine         (int x0,int y0,int x1,int y1){GUI_DrawLine(x0, y0, x1, y1);};
		void DrawLineRel      (int dx,int dy){GUI_DrawLineRel(dx, dy);};
		void DrawLineTo       (int x,int y){GUI_DrawLineTo(x, y);};
		void DrawPie          (int x0,int y0,int r,int a0,int a1,int Type){GUI_DrawPie(x0, y0, r, a0, a1, Type);};
		void DrawPixel        (int x,int y){GUI_DrawPixel(x, y);};
		void DrawPoint        (int x,int y){GUI_DrawPoint(x, y);};
		void DrawPolygon      (const GUI_POINT * pPoints,int NumPoints,int x0,int y0){GUI_DrawPolygon(pPoints, NumPoints, x0, y0);};
		void DrawPolyLine     (const GUI_POINT * pPoints,int NumPoints,int x0,int y0){GUI_DrawPolyLine(pPoints, NumPoints, x0, y0);};
		void DrawFocusRect    (const GUI_RECT  * pRect,int Dist){GUI_DrawFocusRect    (pRect,Dist);};
		void DrawRect         (int x0,int y0,int x1,int y1){GUI_DrawRect( x0, y0, x1, y1);};
		void DrawRectEx       (const GUI_RECT * pRect){GUI_DrawRectEx(pRect);};
		void DrawRoundedFrame (int x0,int y0,int x1,int y1,int r,int w){GUI_DrawRoundedFrame(x0, y0, x1, y1, r, w);};
		void DrawRoundedRect  (int x0,int y0,int x1,int y1,int r){GUI_DrawRoundedRect(x0, y0, x1, y1, r);};
		void DrawVLine        (int x0,int y0,int y1){GUI_DrawVLine(x0, y0, y1);};
		void FillCircle       (int x0,int y0,int r){GUI_FillCircle(x0, y0, r);};
		void FillEllipse      (int x0,int y0,int rx,int ry){GUI_FillEllipse(x0, y0, rx, ry);};
		void FillPolygon      (const GUI_POINT * pPoints,int NumPoints,int x0,int y0){GUI_FillPolygon(pPoints,NumPoints, x0, y0);};
		void FillRect         (int x0,int y0,int x1,int y1){GUI_FillRect( x0, y0, x1, y1);};
		void FillRectEx       (const GUI_RECT * pRect){GUI_FillRectEx(pRect);};
		void FillRoundedFrame (int x0,int y0,int x1,int y1,int r,int w){GUI_FillRoundedFrame(x0, y0, x1, y1, r, w);};
		void FillRoundedRect  (int x0,int y0,int x1,int y1,int r){GUI_FillRoundedRect(x0, y0, x1, y1, r);};
		void FillRoundedRectB (int x0,int y0,int x1,int y1,int r){GUI_FillRoundedRectB(x0, y0, x1, y1, r);};
		void FillRoundedRectT (int x0,int y0,int x1,int y1,int r){GUI_FillRoundedRectT(x0, y0, x1, y1, r);};
		void GetClientRect    (GUI_RECT * pRect){GUI_GetClientRect(pRect);};
		void InvertRect       (int x0,int y0,int x1,int y1){GUI_InvertRect(x0, y0, x1, y1);};
		void MoveRel          (int dx,int dy){GUI_MoveRel          (dx, dy);};
		void MoveTo           (int x,int y){GUI_MoveTo           (x, y);};
		void SetAlphaMask8888 (uint32_t OrMask,uint32_t AndMask){GUI_SetAlphaMask8888 (OrMask, AndMask);};

/*
       Text related routines
*/
		void DispCEOL             (void){ GUI_DispCEOL();};
		void DispChar             (uint16_t c){ GUI_DispChar(c);};
		void DispCharAt           (uint16_t c,I16P x,I16P y){ GUI_DispCharAt(c,x,y);};
		void DispChars            (uint16_t c,int Cnt){ GUI_DispChars(c,Cnt);};
		void DispNextLine         (void){ GUI_DispNextLine();};
		void DispString           (const char * s){GUI_DispString(s);};
		void DispStringAt         (const char * s,int x,int y){ GUI_DispStringAt(s,x,y);};
		void DispStringAtCEOL     (const char * s,int x,int y){ GUI_DispStringAtCEOL(s, x, y);};
		void DispStringHCenterAt  (const char * s,int x,int y){ GUI_DispStringHCenterAt  (s, x, y);};
		void DispStringInRect     (const char * s,GUI_RECT * pRect,int TextAlign,int MaxNumChars){ GUI__DispStringInRect(s,pRect,TextAlign,MaxNumChars);};
		void DispStringInRect     (const char * s,GUI_RECT * pRect,int TextAlign){ GUI_DispStringInRect(s,pRect, TextAlign);};
#if GUI_SUPPORT_ROTATION
  void DispStringInRectEx (const char * s,GUI_RECT * pRect,int TextAlign,int MaxLen,const GUI_ROTATION * pLCD_Api){
	  GUI_DispStringInRectEx (s,pRect, TextAlign, MaxLen, pLCD_Api);};
#endif
		void DispStringInRectMax  (const char * s,GUI_RECT * pRect,int TextAlign,int MaxLen){ GUI_DispStringInRectMax  (s,pRect, TextAlign, MaxLen);}; /* Not to be doc. */
		void DispStringInRectWrap (const char * s,GUI_RECT * pRect,int TextAlign,GUI_WRAPMODE WrapMode){ GUI_DispStringInRectWrap (s,pRect, TextAlign,WrapMode);}; /* Not to be doc. */
		void DispStringLen        (const char * s,int Len){ GUI_DispStringLen(s, Len);};
		void GetTextExtend        (GUI_RECT* pRect,const char * s,int Len){ GUI_GetTextExtend(pRect,s, Len);};
		int  GetYAdjust           (void) {return GUI_GetYAdjust();};
		int  GetDispPosX          (void) {return GUI_GetDispPosX();};
		int  GetDispPosY          (void) {return GUI_GetDispPosY();};
		const GUI_FONT * GetFont(void){return GUI_GetFont();};
		int  GetCharDistX         (uint16_t c){return GUI_GetCharDistX(c);};
		int  GetCharDistXEx       (uint16_t c,int * pSizeX) {return GUI_GetCharDistXEx(c,pSizeX);};
//int  GetTrailingBlankCols(uint16_t c){return GUI_GetStringDistX(s);};
//      GUI_DRAWMODE {GUI_GetDrawMode   (void);};
		int	GetFontDistY         (void) {return GUI_GetFontDistY();};
		int	GetFontSizeY         (void){return GUI_GetFontSizeY();};
		void	GetFontInfo          (const GUI_FONT * pFont,GUI_FONTINFO * pfi){
				GUI_GetFontInfo          (pFont,pfi);};
		void  GetOrg               (int * px,int * py){return GUI_GetOrg(px, py);};
		int   GetYSizeOfFont       (const GUI_FONT * pFont){return GUI_GetYSizeOfFont(pFont);};
		int   GetYDistOfFont       (const GUI_FONT * pFont){return GUI_GetYDistOfFont(pFont);};
		int   GetTextAlign         (void){return GUI_GetTextAlign         ();};
		int   GetTextMode          (void){return GUI_GetTextMode          ();};
		char  IsInFont             (const GUI_FONT * pFont,uint16_t c){return GUI_IsInFont(pFont, c);};
		int   SetTextAlign         (int Align){return GUI_SetTextAlign         (Align);};
		int   SetTextMode          (int Mode){return GUI_SetTextMode          (Mode);};
		char  SetTextStyle         (char Style){return GUI_SetTextStyle         (Style);};
		int   SetLBorder           (int x){return GUI_SetLBorder           (x);};
		const GUI_FONT * SetFont(const GUI_FONT * pNewFont){return GUI_SetFont(pNewFont);};
		char  GotoXY               (int x,int y){return GUI_GotoXY               (x, y);};
		char  GotoX                (int x){return GUI_GotoX                (x);};
		char  GotoY                (int y){return GUI_GotoY                (y);};
		int   WrapGetNumLines      (const char * pText,int xSize,GUI_WRAPMODE WrapMode){return GUI_WrapGetNumLines(pText, xSize, WrapMode);};

		int   GetLeadingBlankCols (uint16_t c){return GUI_GetLeadingBlankCols (c);};
		int   GetTrailingBlankCols(uint16_t c){return GUI_GetTrailingBlankCols(c);};

/*
*       Drawing of binary, decimal and hexadecimal values
*/
		void DispBin(U32  v, U8 Len){GUI_DispBin(v, Len);};
		void DispBinAt(U32  v, I16P x, I16P y, U8 Len){GUI_DispBinAt(v, x, y, Len);};
		void DispDec(I32 v, U8 Len){GUI_DispDec( v, Len);};
		void DispDecAt(I32 v, I16P x, I16P y, U8 Len){GUI_DispDecAt (v, x, y, Len);};
		void DispDecMin(I32 v){GUI_DispDecMin(v);};
		void DispDecShift(I32 v, U8 Len, U8 Shift){GUI_DispDecShift(v, Len, Shift);};
		void DispDecSpace(I32 v, U8 MaxDigits){GUI_DispDecSpace(v, MaxDigits);};
		void DispHex(U32 v, U8 Len){GUI_DispHex(v, Len);};
		void DispHexAt(U32 v, I16P x, I16P y, U8 Len){GUI_DispHexAt(v, x, y, Len);};
		void DispSDec(I32 v, U8 Len){GUI_DispSDec(v, Len);};
		void DispSDecShift(I32 v, U8 Len, U8 Shift){GUI_DispSDecShift(v, Len, Shift);};

/*********************************************************************
*
*       Drawing of floating point values
*/

		void DispFloat    (float v, char Len){GUI_DispFloat    (v, Len);};
		void DispFloatFix (float v, char Len, char Fract){GUI_DispFloatFix (v, Len, Fract);};
		void DispFloatMin (float v, char Fract){GUI_DispFloatMin (v, Fract);};
		void DispSFloatFix(float v, char Len, char Fract){GUI_DispSFloatFix(v, Len, Fract);};
		void DispSFloatMin(float v, char Fract){GUI_DispSFloatMin(v, Fract);};

//       Dynamic memory management
//       Memory devices: GUI_MEMDEV
		
/*********************************************************************
*       Alpha blending
*/

unsigned EnableAlpha(unsigned OnOff){return GUI_EnableAlpha(OnOff);};
U32      RestoreUserAlpha(GUI_ALPHA_STATE * pAlphaState){return GUI_RestoreUserAlpha(pAlphaState);};
unsigned SetAlpha(U8 Alpha){return GUI_SetAlpha(Alpha);};
U32      SetUserAlpha(GUI_ALPHA_STATE * pAlphaState, U32 UserAlpha){return GUI_SetUserAlpha(pAlphaState, UserAlpha);};
//void     SetFuncAlphaBlending(void (* pfAlphaBlending)(LCD_COLOR *, LCD_COLOR *, LCD_COLOR *, U32)){GUI_SetFuncAlphaBlending(void (* pfAlphaBlending)(LCD_COLOR *, LCD_COLOR *, LCD_COLOR *, U32));};
//void     {GUI_SetFuncMixColors    (LCD_COLOR (* pFunc)(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens));};
//void     {GUI_SetFuncMixColorsBulk(void (* pFunc)(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens));};
unsigned PreserveTrans(unsigned OnOff){return GUI_PreserveTrans(OnOff);};

/*********************************************************************
*       Polygon helpers
*/
void RotatePolygon(GUI_POINT * pDest, const GUI_POINT * pSrc, int NumPoints, float Angle){GUI_RotatePolygon(pDest,pSrc,NumPoints,Angle);};
void MagnifyPolygon(GUI_POINT * pDest, const GUI_POINT * pSrc, int NumPoints, int Mag){GUI_MagnifyPolygon(pDest, pSrc, NumPoints, Mag);};
void EnlargePolygon(GUI_POINT * pDest, const GUI_POINT * pSrc, int NumPoints, int Len){GUI_EnlargePolygon(pDest, pSrc, NumPoints, Len);};
		
	private:

};   
#endif /* __cplusplus */

#endif
