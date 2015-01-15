// InetLib.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "InetLib.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif




bool InetLib_Initialize()
{
	ULONG_PTR m_GDIPlusToken;
  #ifdef COSMCTRL_NOD2D
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;

  if (Gdiplus::GdiplusStartup(&m_GDIPlusToken, &gdiplusStartupInput, NULL) != Gdiplus::Ok)
  {
    TRACE0("Failed to initialize GDI+");
    return FALSE;
  }
 #endif

  return TRUE;
}