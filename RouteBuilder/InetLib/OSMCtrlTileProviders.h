/*
Module : OSMCtrlTileProviders.H
Purpose: Defines the interface for an MFC GUI control which implements display of OpenStreetMaps tiles
Created: PJN / 28-11-2009

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

#ifndef __OSMCTRLTILEPROVIDERS_H__
#define __OSMCTRLTILEPROVIDERS_H__


////////////////////////////////// Classes ////////////////////////////////////

//The tile provider base class
class AFX_EXT_CLASS  IOSMCtrlTileProvider
{
public:
//Methods
  virtual CString GetDownloadURL(int nZoom, int nTileX, int nTileY) = 0;
  virtual CString GetDownloadServer() = 0;
  virtual CString GetDownloadObject(int nZoom, int nTileX, int nTileY) = 0;
};


class AFX_EXT_CLASS  COSMCtrlMapnikTileProvider : public IOSMCtrlTileProvider
{
public:
//Methods
  virtual CString GetDownloadURL(int nZoom, int nTileX, int nTileY);

  virtual CString GetDownloadServer();

  virtual CString GetDownloadObject(int nZoom, int nTileX, int nTileY);
};


class AFX_EXT_CLASS  COSMCtrlOsmarenderTileProvider : public IOSMCtrlTileProvider
{
//Methods
  virtual CString GetDownloadURL(int nZoom, int nTileX, int nTileY)
  {
    CString sURL;
    sURL.Format(_T("http://%s/%s"), GetDownloadServer().operator LPCTSTR(), GetDownloadObject(nZoom, nTileX, nTileY).operator LPCTSTR());
    return sURL;
  }

  virtual CString GetDownloadServer()
  { 
    return CString(_T("tah.openstreetmap.org"));
  }

  virtual CString GetDownloadObject(int nZoom, int nTileX, int nTileY)
  { 
    CString sObject;
    sObject.Format(_T("/Tiles/tile/%d/%d/%d.png"), nZoom, nTileX, nTileY);
    return sObject;
  }
};


class COSMCtrlCyclemapTileProvider : public IOSMCtrlTileProvider
{
//Methods
  virtual CString GetDownloadURL(int nZoom, int nTileX, int nTileY)
  {
    CString sURL;
    sURL.Format(_T("http://%s/%s"), GetDownloadServer().operator LPCTSTR(), GetDownloadObject(nZoom, nTileX, nTileY).operator LPCTSTR());
    return sURL;
  }

  virtual CString GetDownloadServer()
  { 
    return CString(_T("andy.sandbox.cloudmade.com"));
  }

  virtual CString GetDownloadObject(int nZoom, int nTileX, int nTileY)
  { 
    CString sObject;
    sObject.Format(_T("/tiles/cycle/%d/%d/%d.png"), nZoom, nTileX, nTileY);
    return sObject;
  }
};


class AFX_EXT_CLASS COSMCtrlMapquestOSMTileProvider : public IOSMCtrlTileProvider
{
//Methods
  virtual CString GetDownloadURL(int nZoom, int nTileX, int nTileY)
  {
    CString sURL;
    sURL.Format(_T("http://%s/%s"), GetDownloadServer().operator LPCTSTR(), GetDownloadObject(nZoom, nTileX, nTileY).operator LPCTSTR());
    return sURL;
  }

  virtual CString GetDownloadServer()
  { 
    return CString(_T("otile1.mqcdn.com"));
  }

  virtual CString GetDownloadObject(int nZoom, int nTileX, int nTileY)
  { 
    CString sObject;
    sObject.Format(_T("/tiles/1.0.0/osm/%d/%d/%d.png"), nZoom, nTileX, nTileY);
    return sObject;
  }
};


class AFX_EXT_CLASS COSMCtrlMapquestOpenAerialTileProvider : public IOSMCtrlTileProvider
{
//Methods
  virtual CString GetDownloadURL(int nZoom, int nTileX, int nTileY)
  {
    CString sURL;
    sURL.Format(_T("http://%s/%s"), GetDownloadServer().operator LPCTSTR(), GetDownloadObject(nZoom, nTileX, nTileY).operator LPCTSTR());
    return sURL;
  }

  virtual CString GetDownloadServer()
  { 
    return CString(_T("oatile1.mqcdn.com"));
  }

  virtual CString GetDownloadObject(int nZoom, int nTileX, int nTileY)
  { 
    CString sObject;
    sObject.Format(_T("/naip/%d/%d/%d.png"), nZoom, nTileX, nTileY);
    return sObject;
  }
};



#endif //__OSMCTRLTILEPROVIDERS_H__
