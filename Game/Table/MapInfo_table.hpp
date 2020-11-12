#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>
#include <memory>


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
    static const std::unique_ptr<MapInfoTableMgr>& Get();
    MapInfoTableMgr();
    ~MapInfoTableMgr();

    const std::shared_ptr<const MapInfoTable>& GetMapInfoTable(unsigned int iTypeID) const;
    const std::map<unsigned int, std::shared_ptr<const MapInfoTable>>& GetMapInfoTableMap() const;

private:
    bool    Load();
    void    FillData(MapInfoTable* row, TiXmlElement* element);

    std::map<unsigned int,std::shared_ptr<const MapInfoTable>>      m_kMapInfoTableMap;
    static std::unique_ptr<MapInfoTableMgr>        m_pkMapInfoTableMgr;
};
