#include "StdAfx.h"
#include "MapConverter.h"
#include "OSMExport/Configuration.h"
#include "OSMExport/ConfigurationParserCallback.h"
#include "OSMExport/OSMDocument.h"
#include "OSMExport/OSMDocumentParserCallback.h"
#include "OSMExport/Export2DB.h"

TOsmDBExportProp::TOsmDBExportProp()
{

}
TOsmDBExportProp::~TOsmDBExportProp()
{
		
}
MapConverter::MapConverter(IConverterReport* exportReport)
{
	//this->exportReport = exportReport;
}


MapConverter::~MapConverter(void)
{
	//exportReport = NULL;
}
bool MapConverter::ExportOsm2PostGis(TOsmDBExportProp* prop)
{
	std::string file = prop->file;
	std::string cFile = prop->cFile;
	std::string host=prop->host;
	std::string user = prop->user;
	std::string port=prop->port;
	std::string dbname = prop->dbname;
	std::string passwd = prop->passwd;
	bool skipnodes = prop->skipnodes;
	bool clean = prop->clean;
	if(file.empty() || cFile.empty() || dbname.empty())
	{
		return false;
	}
	
	Export2DB test(host, user, dbname, port, passwd);
	if(test.connect()==1)
		return false;

    XMLParser parser;
	
	cout << "Trying to load config file " << cFile.c_str() << endl;

	Configuration* config = new Configuration();
        ConfigurationParserCallback cCallback( *config );

	cout << "Trying to parse config" << endl;

	int ret = parser.Parse( cCallback, cFile.c_str() );
	if( ret!=0 ) return 1;
	
	cout << "Trying to load data" << endl;

	OSMDocument* document = new OSMDocument( *config );
        OSMDocumentParserCallback callback( *document );

	cout << "Trying to parse data" << endl;

	ret = parser.Parse( callback, file.c_str() );
	if( ret!=0 ) 
		return 1;
	
	cout << "Split ways" << endl;

	document->SplitWays();
	//############# Export2DB
	{
		if( clean )
		{
			cout << "Dropping tables..." << endl;

			test.dropTables();
		}
		
        cout << "Creating tables..." << endl;
			test.createTables();
		std::map<std::string, Type*>& types= config->m_Types;
		std::map<std::string, Type*>::iterator tIt(types.begin());
		std::map<std::string, Type*>::iterator tLast(types.end());
		
        cout << "Adding tag types and classes to database..." << endl;

		while(tIt!=tLast)
		{
			Type* type = (*tIt++).second;
			test.exportType(type);

			std::map<std::string, Class*>& classes= type->m_Classes;
			std::map<std::string, Class*>::iterator cIt(classes.begin());
			std::map<std::string, Class*>::iterator cLast(classes.end());

			while(cIt!=cLast)
			{
				Class* clss = (*cIt++).second;
				test.exportClass(type, clss);
			}
			
		}
		
		
		cout << "Adding relations to database..." << endl;

		// START RELATIONS CODE
		std::vector<Relation*>& relations= document->m_Relations;
		std::vector<Relation*>::iterator it_relation( relations.begin() );
		std::vector<Relation*>::iterator last_relation( relations.end() );	
		while( it_relation!=last_relation )
		{
			Relation* pRelation = *it_relation++;

			test.exportRelation(pRelation);
		}
		// END RELATIONS CODE
		
		// Optional user argument skipnodes will not add nodes to the database (saving a lot of time if not necessary)
		if ( !skipnodes) {
			cout << "Adding nodes to database..." << endl;

			std::map<long long, Node*>& nodes= document->m_Nodes;
			std::map<long long, Node*>::iterator it(nodes.begin());
			std::map<long long, Node*>::iterator last(nodes.end());
		

			while(it!=last)
			{
				Node* node = (*it++).second;
				test.exportNode(node->id,node->lon, node->lat, node->numsOfUse);
			}
		}

        cout << "Adding ways to database..." << endl;
		std::vector<Way*>& ways= document->m_SplittedWays;
		std::vector<Way*>::iterator it_way( ways.begin() );
		std::vector<Way*>::iterator last_way( ways.end() );	
		while( it_way!=last_way )
		{
			Way* pWay = *it_way++;
			
			test.exportWay(pWay);
		}
		cout << "Creating topology..." << endl;
		test.createTopology(prop->IsClearingData);
	}	
	return true;
}
