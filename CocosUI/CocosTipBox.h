////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosTipBox.h
//
// 提示框
//
//                                            By Wander 2012.06.08
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef COCOSTIPBOX_H_20120608
#define COCOSTIPBOX_H_20120608


#include "CocosUIBase.h"
#include "CocosUIManager.h"
#include "CocosButton.h"

class CCocosTipBox : public CCocosUIBase, CUIListener
{
public:
	enum BoxType
	{
		TYPE_BMFONT,
		TYPE_TTF,
	};
    
    enum BoxStyle
    {
        STYLE_BACKGROUND    = 0x0001,
        STYLE_MSG           = 0x002,
        STYLE_MSG_BK        = 0x0003,
        STYLE_OK            = 0x0004,
        STYLE_OK_NORMAL     = 0x0007,
        STYLE_CANCEL        = 0x0008,
        STYLE_CANCEL_NORMAL = 0x000B,
        STYLE_ALL           = 0x0010,
    };
    
    enum BoxControl
    {
        CONTROL_OK,
        CONTROL_CANCEL,
    };
    
    enum
    {
        LAYER_GROUND,
        LAYER_MSG,
        LAYER_BUTTON,
    };
public:
	CREATE_FUNC(CCocosTipBox);
	static CCocosTipBox *Create(BoxType type, const char *szFontName, float sizeOrScale = 1.0f, unsigned char alpha = 128, 
							    bool bUseFrame = false, const char *szBoxTexture = NULL);
    
    static bool ShowCurTip(const char *szMsg, float waitTimer =0, long style = 0, int id = -1,
                           CUIListener *pListener = NULL, cocos2d::CCAction *pAction = NULL);
    static void CancelCurTip();
    static CCocosTipBox *CurTipBox() {return s_pCurTipBox;}
    
    
	void ShowTip(const char *szMsg, float waitTimer =0, long style = 0, int id = -1,
                 CUIListener *pListener = NULL, cocos2d::CCAction *pAction = NULL);
	void CancelWait();
    
    void SetUIManager(CCocosUIManager *pManager) {m_pManager = pManager;}
    void SetOkCancel(CCocosButton *pBtnOk, CCocosButton *pBtnCancel);
    
    virtual void onEnter();
    virtual void onExit();
    virtual void onReceiveMsg(int aID, int event = 0, const void *pData = NULL);
    
protected:
	CCocosTipBox();
	void InitState(BoxType type, const char *szName, float sizeOrScale,bool bUseFrame, const char *szBoxTexture);
	void OnTipFinish();
    virtual void onBeginShow() {};
    virtual void onFinishShow() {};
protected:
	cocos2d::CCSprite        *m_pSprBack;
	cocos2d::CCLabelProtocol *m_pLabel;
    CCocosUIManager          *m_pManager;
    long                      m_style;
    CCocosButton             *m_pBtnOk;
    CCocosButton             *m_pBtnCancel;
    
    cocos2d::CCPoint          m_ptOk;
    cocos2d::CCPoint          m_ptCancel;
    
    static CCocosTipBox *s_pCurTipBox;
    
};
#endif