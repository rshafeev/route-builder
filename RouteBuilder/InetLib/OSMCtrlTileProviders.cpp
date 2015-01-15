#include "stdafx.h"
#include "OSMCtrlTileProviders.h"


  CString COSMCtrlMapnikTileProvider::GetDownloadURL(int nZoom, int nTileX, int nTileY)
  {
    CString sURL;
    sURL.Format(_T("http://%s/%s"), GetDownloadServer().operator LPCTSTR(), GetDownloadObject(nZoom, nTileX, nTileY).operator LPCTSTR());
    return sURL;
  }

   CString COSMCtrlMapnikTileProvider::GetDownloadServer()
  { 
    return CString(_T("tile.openstreetmap.org"));
  }

   CString COSMCtrlMapnikTileProvider::GetDownloadObject(int nZoom, int nTileX, int nTileY)
  { 
    CString sObject;
    sObject.Format(_T("/%d/%d/%d.png"), nZoom, nTileX, nTileY);
    return sObject;
  }