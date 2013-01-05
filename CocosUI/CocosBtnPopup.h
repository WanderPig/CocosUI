////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosBtnPopup.h
//
// 指定一个弹出按钮，当按钮按下时，激发PopMenu
//
//                                            By Wander 2012.07.05
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef Archer2_CocosBtnPopup_h
#define Archer2_CocosBtnPopup_h

#include "CocosUIContainer.h"

enum PopUpEvent 
{
    POPBTN_POP = 0,
    POPBTN_UNPOP,
};

class CCocosPopMenu : public CCocosUIContainer
{
public:
    virtual void pop(bool bPop) = 0;
};

class CCocosBtnPopup : public CCocosUIBase , public CUIListener
{
public:
    static CCocosBtnPopup *Create(int id, CUIListener *pListener, bool bUseFrame , const char *szTexture1 , const char *szTexture2 );
    CREATE_FUNC(CCocosBtnPopup);
    CCocosBtnPopup()
        : m_bPoping(false)
        , m_pPopMenu(NULL)
    {
        
    }
    virtual void onReceiveMsg(int aID, int event = 0, const void *pData = NULL);
    virtual void setEnable(bool bEnable);
    
    void Popup(bool bPopup);
    void SetPopupMenu(CCocosPopMenu *pPopMenu) {m_pPopMenu = pPopMenu;}
public:
    void InitState(int id, CUIListener *pListener, bool bUseFrame , const char *szTexture1 , const char *szTexture2 );
private:
    CCocosButton   *m_pBtn;
    bool            m_bPoping;
    CCocosPopMenu  *m_pPopMenu;
};

#endif
