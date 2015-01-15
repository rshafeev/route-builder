#pragma once

#import <msxml6.dll> //MSXML 6 support

#include "OSMCtrlPolyline.h"
#include "OSMCtrlPosition.h"
#include "WebGrabData.h"
class CRouteCloudeMadeException
{
	CString error;
public:
	CRouteCloudeMadeException(CString error)
	{
		this->error = error;
	}
	CString GetErrorText()
	{
		return error;
	}
};
class AFX_EXT_CLASS CRouteCloudeMade
{
  //Member variables
  HINTERNET m_hSession;
  HINTERNET m_hConnection;
  HINTERNET m_hFile;
  COSMCtrlPolyline line;
  COSMCtrlPosition posBegin, posEnd;
  CString key;
  CWebGrabData data;

  CString GetRequest(double lat1, double lon1, double lat2, double lon2);
  CWebGrabData LoadData(double lat1, double lon1, double lat2, double lon2);
public:
	CRouteCloudeMade(CString key);
	~CRouteCloudeMade(void);
    
	COSMCtrlPolyline* GetPolyLine(double lat1, double lon1, double lat2, double lon2);
	bool GetGeom(double lat1, double lon1, double lat2, double lon2,CString &geom, double &distance,double &time, double smoothing=-1);
    CWebGrabData& GetLoadedData();

private:
  

};

