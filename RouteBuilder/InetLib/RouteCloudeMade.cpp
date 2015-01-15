#include "StdAfx.h"
#include "RouteCloudeMade.h"
#include "OSMCtrlHelper.h"
#include "cnominatim.h"
#include "WebGrab.h"
#include <libxml\debugXML.h>
#include <libxml\parser.h>
#include <stdio.h>
#include <stdlib.h> 

CRouteCloudeMade::CRouteCloudeMade(CString key)
{
	this->key = key;
}


CRouteCloudeMade::~CRouteCloudeMade(void)
{

}

CString CRouteCloudeMade::GetRequest(double lat1, double lon1, double lat2, double lon2)
{
       CString sRequest;
       sRequest.Preallocate(1024);
      sRequest.Format(_T("http://routes.cloudmade.com/%s/api/0.3/%f,%f,%f,%f/car.gpx?units=km&lang=en&translation=common"),key,lon1,lat1,lon2,lat2);
    return sRequest;
}
CWebGrabData CRouteCloudeMade::LoadData(double lat1, double lon1, double lat2, double lon2)
{
	posBegin = COSMCtrlPosition(lon1,lat1);
	posEnd   = COSMCtrlPosition(lon2,lat2);
    CWebGrab grab;
    if (grab.GetFile(GetRequest(lon1,lat1,lon2,lat2), // the url
                     this->data))           // Plain ol' CStatic window
    {
        TRACE0("everything went OK\n");
    }
	return data;

  }

 COSMCtrlPolyline* CRouteCloudeMade::GetPolyLine(double lat1, double lon1, double lat2, double lon2)
 {
	 COSMCtrlPolyline *line = new COSMCtrlPolyline;
	 MSXML2::IXMLDOMDocument2Ptr document;
	 LoadData(lat1,lon1,lat2,lon2);
	 
	 CWebGrabData::CWebGrabDataElem &combineData = data.Combine();
	 xmlDocPtr doc;
	 doc = xmlReadMemory((const char*)combineData.mas,combineData.size,"noname.xml", NULL, 0);
	 //XMLNodePointer_t mainnode = xml->DocGetRootElement(doc);

	 //xmlTextReaderPtr	text = xmlReaderWalker(doc);
	xmlNodePtr node =  xmlDocGetRootElement(doc);
	node = node->children;
	while(node->next!=NULL)
	{
		if(node->next->name!=NULL&&strcmp((const char*)node->next->name,"wpt")==0)
		{
			_xmlAttr* attr = node->next->properties;
			if(attr==NULL)
				continue;
			double lat = atof((const char*)attr->children->content);
			attr = attr->next;
			double lon = atof((const char*)attr->children->content);
			
			line->m_Nodes.Add(COSMCtrlNode(lon,lat));
		}

		node = node->next;
	}
	
	xmlFreeDoc(doc); // free document
    xmlCleanupParser(); // Free globals

	return line;
 }


bool CRouteCloudeMade::GetGeom(double lat1, double lon1, double lat2, double lon2,CString &geom, double &distance,double &time, double smoothing)
{
	 COSMCtrlPolyline *line = new COSMCtrlPolyline;
	 MSXML2::IXMLDOMDocument2Ptr document;
	 LoadData(lat1,lon1,lat2,lon2);
	 
	 CWebGrabData::CWebGrabDataElem &combineData = data.Combine();
	 xmlDocPtr doc;
	 doc = xmlReadMemory((const char*)combineData.mas,combineData.size,"noname.xml", NULL, 0);
	 //XMLNodePointer_t mainnode = xml->DocGetRootElement(doc);
	 //xmlTextReaderPtr	text = xmlReaderWalker(doc);
	xmlNodePtr node =  xmlDocGetRootElement(doc);
	if(node==NULL)
		return false;
	node = node->children;
	geom = "";
	distance = 0.0;


	double lastLat = -10000.0;
	double lastLon = -10000.0;
	while(node->next!=NULL)
	{
		if(node->next->name!=NULL&&strcmp((const char*)node->next->name,"wpt")==0)
		{
			_xmlAttr* attr = node->next->properties;
			if(attr==NULL)
				continue;
			CString latS = (const char*)attr->children->content;
			attr = attr->next;
			CString lonS = (const char*)attr->children->content;

			if(smoothing>0)
			{
				double lat = atof(latS);
				double lon = atof(lonS);
				
				if(COSMCtrlHelper::DistanceBetweenPoints(COSMCtrlPosition(lastLon,lastLat),COSMCtrlPosition(lon,lat),NULL,NULL)/1000.0 < smoothing )
				{
					node = node->next;
					continue;
				}
				lastLat = lat;
				lastLon = lon;
			}
			CString pointS;
			pointS.Format("(%s;%s)",latS,lonS);
			geom+= pointS +",";
		}
		else
		if(node->next->name!=NULL&&strcmp((const char*)node->next->name,"extensions")==0)
		{
			_xmlNode* ch = node->next->children;
			while(ch!=NULL)
			{
				if(strcmp((const char*)ch->name,"distance")==0)
				{
					_xmlNode* distanceNode =ch->children;
					if(distanceNode!=NULL)
						distance =atof((const char*)distanceNode->content)/1000.0;
					break;
				}
				ch = ch->next;
			}
			ch = node->next->children;
			while(ch!=NULL)
			{
				if(strcmp((const char*)ch->name,"time")==0)
				{
					_xmlNode* timeNode = ch->children;
					if(timeNode!=NULL)
						time =atof((const char*)timeNode->content);
					break;
				}
				ch = ch->next;
			}
		}
		node = node->next;
	}
	if(geom.GetLength()>0)
	{
		CString pointS;
		pointS.Format("(%f;%f)",lat2,lon2);
		geom+= pointS;
	}
	xmlFreeDoc(doc); // free document
    xmlCleanupParser(); // Free globals

	return true;
 }

 CWebGrabData&  CRouteCloudeMade::GetLoadedData()
 {
	 return this->data;
 }