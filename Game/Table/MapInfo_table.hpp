#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>
#include "SeTableResBase.h"


enum E_MAPINFOTABLE_TYPE
{
    MAPINFO_TYPE_NONE                           =    0      ,   //  未定义
    MAPINFO_TYPE_RACE                           =    1      ,   //  竞技
    MAPINFO_TYPE_TEST                           =    2      ,   //  试炼关卡
    MAPINFO_TYPE_BOSS                           =    3      ,   //  boss关卡
    MAPINFO_TYPE_MAIN                           =    4      ,   //  主线关卡
    MAPINFO_TYPE_JIAOHU                         =    5      ,   //  交互关卡
    MAPINFO_TYPE_WAR                            =    6      ,   //  战场
    MAPINFO_TYPE_MMWAR                          =    7      ,   //  人机对战
};


enum M_MAPINFOTABLE_PROPERTY
{
    MAPINFO_PROPERTY_ALLOPEN                    =    1 << 1        ,   //  英雄全开
    MAPINFO_PROPERTY_USEAI                      =    1 << 2        ,   //  支持AI
};

//  自动生成表结构
struct MapInfoTable : SeTableResBase
{
    unsigned int               uiMapTypeID                         ;   //  地图ID   
    std::string                kName                               ;   //  地图名称   
    std::string                kresId                              ;   //  资源ID   
    int                        uiType                              ;   //  地图类型   
    unsigned int               uiProperty                          ;   //  地图特性   
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
    TableResArray GetMapInfoTableVec();

private:
    bool    Load();
    void    FillData(MapInfoTable* row, TiXmlElement* element);

    std::map<unsigned int,MapInfoTable*>      m_kMapInfoTableMap;
    static MapInfoTableMgr* m_pkMapInfoTableMgr;
};
