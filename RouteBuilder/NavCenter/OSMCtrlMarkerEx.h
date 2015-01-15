#pragma once
#include "..\inetlib\osmctrlmarker.h"
#include "..\NavCenter.DB\DBModel.h"
class COSMCtrlMarkerEx : 	public COSMCtrlMarker
{
public:
	CDBLocation location;
	COSMCtrlMarkerEx(void);
	~COSMCtrlMarkerEx(void);
};
