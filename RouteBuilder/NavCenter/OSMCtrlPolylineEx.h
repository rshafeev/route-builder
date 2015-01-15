#pragma once
#include "..\inetlib\osmctrlpolyline.h"
#include "..\NavCenter.DB\DBModel.h"

class COSMCtrlPolylineEx : 	public COSMCtrlPolyline
{
public:
	CDBPolyLine m_dbPolyLine;
	COSMCtrlPolylineEx(void);
	~COSMCtrlPolylineEx(void);
};

