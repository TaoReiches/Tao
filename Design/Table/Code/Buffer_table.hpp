#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>
#include "SeTableResBase.h"



enum M_BUFFERTABLE_PROPERTY
{
    BUFFER_PROPERTY_DEBUFFER                    =    1 << 1        ,   //  有害状态
    BUFFER_PROPERTY_NOJINGHUA                   =    1 << 2        ,   //  不可净化
    BUFFER_PROPERTY_GENSUI                      =    1 << 3        ,   //  特效跟随单位
    BUFFER_PROPERTY_AUTOREMOVE                  =    1 << 4        ,   //  自动消除
    BUFFER_PROPERTY_BEIJI                       =    1 << 5        ,   //  被击触发
    BUFFER_PROPERTY_GONGJI                      =    1 << 6        ,   //  攻击触发
    BUFFER_PROPERTY_SHANGHAI                    =    1 << 7        ,   //  伤害触发
    BUFFER_PROPERTY_YINSHEN                     =    1 << 8        ,   //  隐身达成触发
    BUFFER_PROPERTY_YINSHENTUICHU               =    1 << 9        ,   //  隐身退出触发
    BUFFER_PROPERTY_SIWANG                      =    1 << 10       ,   //  死亡触发
    BUFFER_PROPERTY_CDWANCHENG                  =    1 << 11       ,   //  CD完成触发
    BUFFER_PROPERTY_TEXIAODAODA                 =    1 << 12       ,   //  特效到达触发
    BUFFER_PROPERTY_SHIFA                       =    1 << 13       ,   //  施法触发
    BUFFER_PROPERTY_JISHA                       =    1 << 14       ,   //  击杀触发
    BUFFER_PROPERTY_SIWANGQIAN                  =    1 << 15       ,   //  死亡前触发
    BUFFER_PROPERTY_BUXUANZHUAN                 =    1 << 16       ,   //  不随单位旋转
    BUFFER_PROPERTY_CLIENTNEED                  =    1 << 17       ,   //  客户端显示需要
    BUFFER_PROPERTY_DEATHADD                    =    1 << 18       ,   //  死亡添加
    BUFFER_PROPERTY_NONVYAO                     =    1 << 19       ,   //  不进女妖
};

//  自动生成表结构
struct BufferTable : SeTableResBase
{
    unsigned int               uiBufferTypeID                      ;   //  类型ID   
    std::string                kName                               ;   //  名字   
    unsigned int               uiProperty                          ;   //  特性   
    int                        iSingleSpliceNum                    ;   //  单人可叠加次数   
    int                        iMultiSpliceNum                     ;   //  多人可叠加次数   
};

class TiXmlElement;
class BufferTableMgr
{
public:
    static BufferTableMgr* Get();
    BufferTableMgr();
    ~BufferTableMgr();

    const BufferTable* GetBufferTable(unsigned int iTypeID);
    const std::map<unsigned int, BufferTable*>& GetBufferTableMap();
    TableResArray GetBufferTableVec();

private:
    bool    Load();
    void    FillData(BufferTable* row, TiXmlElement* element);

    std::map<unsigned int,BufferTable*>      m_kBufferTableMap;
    static BufferTableMgr* m_pkBufferTableMgr;
};
