/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
  * \file syscalls_sam3.c
  *
  * Implementation of newlib syscall.
  *
  */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/


#include "syscalls.h"

#include <stdio.h>
#include <stdarg.h>
#if defined (  __GNUC__  ) /* GCC CS3 */
  #include <sys/types.h>
  #include <sys/stat.h>
#endif

#include "Arduino.h"

// Helper macro to mark unused parameters and prevent compiler warnings.
// Appends _UNUSED to the variable name to prevent accidentally using them.
#ifdef __GNUC__
#define UNUSED_PARAM(x) x ## _UNUSED __attribute__((__unused__))
#else
#define UNUSED_PARAM(x) x ## _UNUSED
#endif

/*----------------------------------------------------------------------------
 *        Exported variables
 *----------------------------------------------------------------------------*/

#undef errno
extern int errno ;
extern int  _end ;

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
extern void _exit( int status ) ;
extern void _kill( int pid, int sig ) ;
extern int _getpid ( void ) ;

static unsigned char *heap_brk = NULL;
static unsigned char *heap_end = NULL;

void setHeap(unsigned char *start, unsigned char *end) {
    heap_brk = start;
    heap_end = end;
}

extern caddr_t _sbrk ( int incr )
{
   caddr_t prev_heap;

  if ( heap_brk == NULL )
  {
      heap_brk = (unsigned char *)&_end ;
  }
  prev_heap = (caddr_t)heap_brk;

  if (heap_end != NULL && (heap_brk + incr) > heap_end) {
      return (caddr_t)-1;
  }
  
  heap_brk += incr ;

  return prev_heap ;
}

extern int link( UNUSED_PARAM(char *cOld), UNUSED_PARAM(char *cNew) )
{
  return -1 ;
}

extern int _close( UNUSED_PARAM(int file) )
{
  return -1 ;
}

extern int _fstat( UNUSED_PARAM(int file), struct stat *st )
{
  st->st_mode = S_IFCHR ;

  return 0 ;
}

extern int _isatty( UNUSED_PARAM(int file) )
{
  return 1 ;
}

extern int _lseek( UNUSED_PARAM(int file), UNUSED_PARAM(int ptr), UNUSED_PARAM(int dir) )
{
  return 0 ;
}

extern int _read(UNUSED_PARAM(int file), UNUSED_PARAM(char *ptr), UNUSED_PARAM(int len) )
{
  return 0 ;
}


extern void _exit( int status )
{
  printf( "Exiting with status %d.\n", status ) ;

  for ( ; ; ) ;
}

extern void _kill( UNUSED_PARAM(int pid), UNUSED_PARAM(int sig) )
{
  return ;
}

extern int _getpid ( void )
{
  return -1 ;
}
