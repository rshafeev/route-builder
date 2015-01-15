#include "StdAfx.h"
#include "VRPTWData.h"

CVRPTWData::CVRPTWData()
{

}
CVRPTWData::CVRPTWData(TSettingsSolveVRPTW settings)
{
	this->settings = settings;
}
CVRPTWData::CVRPTWData( CVRPTWData const &copy)
{
	this->depots.reserve(copy.depots.size());
	this->ques.reserve(copy.ques.size());
	this->relations.reserve(copy.relations.size());
	this->vehicles.reserve(copy.vehicles.size());
	
	for(vector<TDepotData*>::const_iterator d = copy.depots.begin(); d!=copy.depots.end(); d++ )
		depots.push_back(new TDepotData(*(*d)));
	for(vector<TQueData*>::const_iterator d = copy.ques.begin(); d!=copy.ques.end(); d++ )
		ques.push_back(new TQueData(*(*d)));
	for(vector<TRelationData*>::const_iterator d = copy.relations.begin(); d!=copy.relations.end(); d++ )
		relations.push_back(new TRelationData(*(*d)));
	for(vector<TVehicleData*>::const_iterator d = copy.vehicles.begin(); d!=copy.vehicles.end(); d++ )
		vehicles.push_back(new TVehicleData(*(*d)));

	this->settings = TSettingsSolveVRPTW(copy.settings);

}


CVRPTWData::~CVRPTWData(void)
{
	for(vector<TDepotData*>::iterator d = depots.begin(); d!=depots.end(); d++ )
		delete (*d);
	for(vector<TQueData*>::iterator d = ques.begin(); d!=ques.end(); d++ )
		delete (*d);
	for(vector<TRelationData*>::iterator d = relations.begin(); d!=relations.end(); d++ )
		delete (*d);
	for(vector<TVehicleData*>::iterator d = vehicles.begin(); d!=vehicles.end(); d++ )
		delete (*d);

}
TSettingsSolveVRPTW& CVRPTWData::GetSettings()
{
	return this->settings;
}
