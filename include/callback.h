/*
 * Callback functions
 *
 * Copyright 1995 Alexandre Julliard
 */

#ifndef __WINE_CALLBACK_H
#define __WINE_CALLBACK_H

#include "wintypes.h"

extern
int CallTo32_LargeStack( int (*func)(), int nbargs, ... );


/* List of the 16-bit callback functions. This list is used  */
/* by the build program to generate the file if1632/callto16.S */

                               /* func     parameters */
extern WORD CallTo16_word_     ( FARPROC16 );

#ifndef WINELIB

extern WORD CallTo16_word_w    ( FARPROC16, WORD );
extern WORD CallTo16_word_ww   ( FARPROC16, WORD, WORD );
extern WORD CallTo16_word_wl   ( FARPROC16, WORD, LONG );
extern WORD CallTo16_word_ll   ( FARPROC16, LONG, LONG );
extern WORD CallTo16_word_www  ( FARPROC16, WORD, WORD, WORD );
extern WORD CallTo16_word_wwl  ( FARPROC16, WORD, WORD, LONG );
extern WORD CallTo16_word_wlw  ( FARPROC16, WORD, LONG, WORD );
extern LONG CallTo16_long_wwl  ( FARPROC16, WORD, WORD, LONG );
extern WORD CallTo16_word_llwl ( FARPROC16, LONG, LONG, WORD, LONG );
extern LONG CallTo16_long_wwwl ( FARPROC16, WORD, WORD, WORD, LONG );
extern WORD CallTo16_word_lwww ( FARPROC16, LONG, WORD, WORD, WORD );
extern WORD CallTo16_word_wwll ( FARPROC16, WORD, WORD, LONG, LONG );
extern WORD CallTo16_word_wllwl( FARPROC16, WORD, LONG, LONG, WORD, LONG );
extern LONG CallTo16_long_lwwll( FARPROC16, LONG, WORD, WORD, LONG, LONG );
extern WORD CallTo16_word_wwlll( FARPROC16, WORD, WORD, LONG, LONG, LONG );
extern LONG CallTo16_long_lllllllwlwwwl( FARPROC16, LONG, LONG, LONG,
                                         LONG, LONG, LONG, LONG, WORD, LONG,
                                         WORD, WORD, WORD, LONG );

extern WORD CallTo16_regs_( FARPROC16 func, WORD ds, WORD es, WORD bp, WORD ax,
                            WORD bx, WORD cx, WORD dx, WORD si, WORD di );

#define CallDCHookProc( func, hdc, code, data, lparam) \
    CallTo16_word_wwll( func, hdc, code, data, lparam )
#define CallDriverProc( func, dwId, msg, hdrvr, lparam1, lparam2 ) \
    CallTo16_long_lwwll( func, dwId, msg, hdrvr, lparam1, lparam2 )
#define CallHookProc( func, code, wParam, lParam ) \
    CallTo16_long_wwl( func, code, wParam, lParam )
#define CallTimeFuncProc( func, id, msg, dwUser, dw1, dw2 ) \
    CallTo16_word_wwlll( func, id, msg, dwUser, dw1, dw2 )
#define CallWindowsExitProc( func, nExitType ) \
    CallTo16_word_w( func, nExitType )
#define CallWndProc16( func, hwnd, msg, wParam, lParam ) \
    CallTo16_long_wwwl( (FARPROC16)(func), hwnd, msg, wParam, lParam )
#define CallWordBreakProc( func, lpch, ichCurrent, cch, code ) \
    CallTo16_word_lwww( func, lpch, ichCurrent, cch, code )
#define CallWndProcNCCREATE16( func, exStyle, clsName, winName, style, \
                               x, y, cx, cy, hparent, hmenu, instance, \
                               params, hwnd, msg, wParam, lParam ) \
    CallTo16_long_lllllllwlwwwl( (FARPROC16)(func), exStyle, clsName, winName,\
                                 style, MAKELONG(y,x), MAKELONG(cy,cx), \
                                 MAKELONG(hmenu,hparent), instance, params, \
                                 hwnd, msg, wParam, lParam )
#define _InitTermProc( func ) CallTo32_0( (FARPROC32)func )


/* List of the 32-bit callback functions. This list is used  */
/* by the build program to generate the file if1632/callto32.S */

extern LONG CallTo32_0( FARPROC32 );
extern LONG CallTo32_2( FARPROC32, DWORD, DWORD );
extern LONG CallTo32_3( FARPROC32, DWORD, DWORD, DWORD );
extern LONG CallTo32_4( FARPROC32, DWORD, DWORD, DWORD, DWORD );

#define CallTaskStart32( func ) \
    CallTo32_0( func )
#define CallDLLEntryProc32( func, hmodule, a, b ) \
    CallTo32_3( func, hmodule, a, b )
#define CallWndProc32( func, hwnd, msg, wParam, lParam ) \
    CallTo32_4( func, hwnd, msg, wParam, lParam )


#else  /* WINELIB */

#define CallDCHookProc( func, hdc, code, data, lparam ) \
    (*func)( hdc, code, data, lparam )
#define CallDriverProc( func, dwId, msg, hdrvr, lparam1, lparam2 ) \
    (*func)( dwId, msg, hdrvr, lparam1, lparam2 )
#define CallHookProc( func, code, wParam, lParam ) \
    (*func)( code, wParam, lParam )
#define CallTimeFuncProc( func, id, msg, dwUser, dw1, dw2 ) \
    (*func)( id, msg, dwUser, dw1, dw2 )
#define CallWindowsExitProc( func, nExitType ) \
    (*func)( nExitType )
#define CallWndProc16( func, hwnd, msg, wParam, lParam ) \
    (*func)( hwnd, msg, wParam, lParam )
#define CallWndProc32( func, hwnd, msg, wParam, lParam ) \
    (*func)( hwnd, msg, wParam, lParam )
#define CallWordBreakProc( func, lpch, ichCurrent, cch, code ) \
    (*func)( lpch, ichCurrent, cch, code )
#define _InitTermProc( func ) (*func)()


#endif  /* WINELIB */


#endif /* __WINE_CALLBACK_H */
