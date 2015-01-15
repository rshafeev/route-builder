/*
Module : OSMCtrlHelper.H
Purpose: Defines the interface for a helper class for COSMCtrl
Created: PJN / 10-04-2011

Copyright (c) 2011 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


///////////////////////// Macros / Includes ///////////////////////////////////

#pragma once

#ifndef __OSMCTRLHELPERS_H__
#define __OSMCTRLHELPERS_H__

#include "OSMCtrlPosition.h"


////////////////////////////////// Classes ////////////////////////////////////


class AFX_EXT_CLASS  COSMCtrlHelper
{
public:
//Enums
  enum Misc
  {
    OSMMaxZoom = 18,
    OSMMinZoom = 0,
  };

//static methods
  static double Longitude2TileX(double Longitude, int nZoom);

  static double Latitude2TileY(double Latitude, int nZoom);

  static double TileX2Longitude(double X, int nZoom);

  static double TileY2Latitude(double Y, int nZoom);

  //This is an implementation of Vincenty's Inverse algorithm (http://en.wikipedia.org/wiki/Vincenty's_formulae). It is based upon the C# 
  //implementation provided by Mike Gavaghan at http://www.gavaghan.org/blog/2007/08/06/c-gps-receivers-and-geocaching-vincentys-formula/
  static double DistanceBetweenPoints(const COSMCtrlPosition& position1, const COSMCtrlPosition& position2, double* pdStartBearing, double* pdReverseBearing);

  //This is an implementation of Vincenty's Direct algorithm (http://en.wikipedia.org/wiki/Vincenty's_formulae). It is based upon the C# 
  //implementation provided by Mike Gavaghan at http://www.gavaghan.org/blog/2007/08/06/c-gps-receivers-and-geocaching-vincentys-formula/
  static COSMCtrlPosition GetPosition(const COSMCtrlPosition& position, double dBearing, double dDistance, double* pdEndBearing);
};


#endif //__OSMCTRLHELPERS_H__
