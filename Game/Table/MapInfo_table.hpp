#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>


enum E_MAPINFOTABLE_TYPE
{
    MAPINFO_TYPE_NONE                           =    0      ,   //  None
    MAPINFO_TYPE_MAINMAP                        =    1      ,   //  Main
    MAPINFO_TYPE_MISSIONMAP                     =    2      ,   //  Mission
};


enum M_MAPINFOTABLE_PROPERTY
{
    MAPINFO_PROPERTY_ALLHERO                    =    1 << 1        ,   //  AllHeros
    MAPINFO_PROPERTY_USEAI                      =    1 << 2        ,   //  AISupported
};

class MapInfoTable
{
public:
    unsigned int               uiMapTypeID                         ;   //  MapID
    std::string                kName                               ;   //  MapName
    int                        uiType                              ;   //  MapType
    unsigned int               uiProperty                          ;   //  MapAttr
};

class TiXmlElement;
class MapInfoTableMgr
{
public:
    static MapInfoTableMgr* Get();
    MapInfoTableMgr();
    ~MapInfoTableMgr();

    const MapInfoTable* GetMapInfoTable(unsigned int iTypeID);
    const std::map<unsigned int, MapInfoTable*>& GetMapInfoTableMap();

private:
    bool    Load();
    void    FillData(MapInfoTable* row, TiXmlElement* element);

    std::map<unsigned int,MapInfoTable*>      m_kMapInfoTableMap;
    static MapInfoTableMgr* m_pkMapInfoTableMgr;
};
