////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosItemList.h
//
// 指定了一个列表类和一个Item类。
//
//                                            By Wander 2012.07.04
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef Archer2_CocosItemList_h
#define Archer2_CocosItemList_h

#include "CocosUIContainer.h"
#include "CocosUIManager.h"

class CCocosItem : public CCocosUIContainer
{
public:
    enum ItemType
    {
        TYPE_SCALE,
        TYPE_CHANGE
    };
    
    enum ItemEvent
    {
        EVENT_SELECTED,
    };
public:
    
    static CCocosItem *Create(int id, bool bHor, bool bUseFrame = false, const char *szBack = NULL);
    virtual void onReceiveNewTouch();
	virtual void onCurTouchFinish();
    virtual void loseFocus();
    virtual void setEnable(bool bEnable);
    
    
    void SetChangeType(ItemType type);
    void SetChangeItem(bool bHor, bool bUseFrame = false, const char *szBack = NULL);
    void Active(bool bActive);
    
    void  SetSize(float size) {m_size = size;}
    float GetSize() const     {return m_size;}
    cocos2d::CCSprite *NormalBackSprite() {return m_pSprBack;}
    cocos2d::CCSprite *ChangeBackSprite() {return m_pSprChange;}
    
    void AddControlToNormal(CCocosUIBase *pNewControl, cocos2d::CCPoint ptPos, int zOrder);
    void AddControlToChange(CCocosUIBase *pNewControl, cocos2d::CCPoint ptPos, int zOrder);
    
private:
    CREATE_FUNC(CCocosItem);
    CCocosItem()
    : m_pSprBack(NULL)
    , m_bActive(false)
    , m_pSprChange(NULL)
    {
    }
    void InitState(int id, bool bHor, const char *szBack, bool bUseFrame);
private:
    ItemType    m_type;
    float       m_size;
    bool        m_bActive;
    
    cocos2d::CCSprite *m_pSprBack;
    cocos2d::CCSprite *m_pSprChange;
    cocos2d::CCPoint   m_ptLast;
    ControlList        m_listControlNormal;
    ControlList        m_listControlChange;
};

class CCocosItemList : public CCocosUIContainer, CUIListener
{
public:
    enum ItemID
    {
        ID_ITEM_BEGIN = 100,
    };
    enum ItemListType
    {
        ITEMLIST_FROMTOP,           // 锚点再顶部
        ITEMLIST_FROMBOTTOM,
        ITEMLIST_FROMLEFT,
        ITEMLIST_FROMRIGHT,
    };
public:
    virtual void visit();
    virtual bool isContainsTouch(cocos2d::CCTouch *pTouch);
    virtual void onReceiveNewTouch();
	virtual void onCurTouchMoveOut();
	virtual void onCurTouchMoving();
	virtual void onCurTouchFinish();
    virtual void onReceiveMsg(int aID, int event = 0, const void *pData = NULL);
    virtual void setEnable(bool bEnable);
public:
    CREATE_FUNC(CCocosItemList);
    static CCocosItemList *Create(int id, CUIListener *pListener, cocos2d::CCSize s, ItemListType type = ITEMLIST_FROMTOP);
    void   InitState(int id, CUIListener *pListener, ItemListType type, cocos2d::CCSize s);
    void   UpdateList();
    void   AddItem(CCocosItem *pItem);
    void   InsertItem(CCocosItem *pItem, int pos);
    void   RemoveItem(CCocosItem *pItem);
    void   ResortItems();
    void   SetParentScale(float scale);
    std::list<CCocosItem *> *Items() {return &m_listItems;}
    
    CCocosItem *ItemByIndex(int index);
    CCocosItem *ItemByID(int id);
    int         IndexById(int id);
    
private:
    void OnRemoveItem();
    void UnfocusAll();
private:
    CCocosItemList()
        : m_pActiveItem(NULL)
        , m_bTouchMoving(false)
        , m_itemSizeCount(0)
        , m_parentScale(1.0f)
        , m_bStopUpdate(false)
    {
        
    }
    
private:
    cocos2d::CCRect         m_rcShow;
    ItemListType            m_type;
    std::list<CCocosItem *> m_listItems;
    float                   m_itemSizeCount;
    float                   m_edgePos;
    float                   m_scale;
    float                   m_parentScale;
    cocos2d::CCLayer       *m_pLayList;
    bool                    m_bStopUpdate;
    float                   m_startTouchPos;
    
    cocos2d::CCPoint        m_ptLast;
    bool                    m_bAutoMoving;
    bool                    m_bTouchMoving;
    float                   m_curSpeed;
    
    float                   m_startPos;
    float                   m_curSize;
    CCocosItem             *m_pActiveItem;
};

#endif
