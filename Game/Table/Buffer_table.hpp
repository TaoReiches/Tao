#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>



enum M_BUFFERTABLE_PROPERTY
{
    BUFFER_PROPERTY_DEBUFF                      =    1 << 1        ,   //  Debuff
    BUFFER_PROPERTY_NOCLEAN                     =    1 << 2        ,   //  NoClean
    BUFFER_PROPERTY_EFFECTFOLLOW                =    1 << 3        ,   //  EffectFollow
    BUFFER_PROPERTY_BEATTACKTRIGGER             =    1 << 4        ,   //  BeAttackTrigger
    BUFFER_PROPERTY_ATTACKTRIGGER               =    1 << 5        ,   //  AttackTrigger
    BUFFER_PROPERTY_DAMAGETRIGGER               =    1 << 6        ,   //  DamageTrigger
    BUFFER_PROPERTY_DEADTRIGGER                 =    1 << 7        ,   //  DeadTrigger
    BUFFER_PROPERTY_CDTRIGGER                   =    1 << 8        ,   //  CDTrigger
};

class BufferTable
{
public:
    unsigned int               uiBufferTypeID                      ;   //  TypeID
    std::string                kName                               ;   //  Name
    unsigned int               uiProperty                          ;   //  Attr
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

private:
    bool    Load();
    void    FillData(BufferTable* row, TiXmlElement* element);

    std::map<unsigned int,BufferTable*>      m_kBufferTableMap;
    static BufferTableMgr* m_pkBufferTableMgr;
};
