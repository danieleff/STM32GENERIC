#if __has_include("MW_STemWin.h")
#include "arduino.h"
#include "STemWin/inc/GUI.h"

/*********************************************************************

        Defines, configurable

**********************************************************************
*/

//
// Define the available number of bytes available for the GUI
//
#if defined(STM32F4)
	#define GUI_NUMBYTES  (1024) *  64   // x KByte
#else
	#define GUI_NUMBYTES  (1024) *  30   // x KByte
#endif

/*********************************************************************

        Static data

**********************************************************************
*/

/* 32 bit aligned memory area */
static U32 extMem[GUI_NUMBYTES / 4];

/*********************************************************************

        Public code

**********************************************************************
*/
/*********************************************************************

        GUI_X_Config

  Purpose:
    Called during the initialization process in order to set up the
    available memory for the GUI.
*/

void GUI_X_Config(void)
{
  GUI_ALLOC_AssignMemory(extMem, GUI_NUMBYTES);
}
#endif