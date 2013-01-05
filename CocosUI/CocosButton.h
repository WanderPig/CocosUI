////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosButton.h
//
// 按钮类
//
//                                            By Wander 2011.11.15
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef COCOSBUTTON_H_20111115
#define COCOSBUTTON_H_20111115


#include "CocosUIBase.h"

class CCocosButton : public CCocosUIBase
{
public:
	enum ButtonType
	{
		TYPE_NULL,			// 神马都没有，可以响应一个区域
		TYPE_NONE,			// 点击没有任何动作
		TYPE_SCALE,			// 点击时放大，失去焦点时缩小为原来比例
		TYPE_TRANSPARENT,	// 点击时显示图片，否则隐藏
		TYPE_CALLSHOW,		// 永远不会显示图片，除非调用显示函数，用于做复选按钮
		TYPE_REPLACE,		// 击中状态时改变图片，失去点击则换回原图片
		TYPE_CHANGE,		// 点击后按钮图片改变，再次点击变回
		TYPE_CALLCHANGE,    // 被动变换图片
        TYPE_CALLSCALE,     // 被动缩放
        TYPE_OPACITY,
	};
    enum ButtonEvent
    {
        EVENT_DOWN,
        EVENT_UP,
        EVENT_MOVE
    };
    
    
private:
	CREATE_FUNC(CCocosButton);
	virtual void onReceiveNewTouch();
	virtual void onCurTouchMoveOut();
	virtual void onCurTouchFinish();
    virtual void onExit();
	
	virtual void loseFocus();
    
private:
	void InitState(int id, CUIListener *pListerner, ButtonType type , const char *szTexture1, const char *szTexture2, bool bUseFrame);
	// 将按钮恢复为未点击状态。
    
	void ResetState();
public:
    
	// 静态构造BUTTON
	// 如果TYPE为 NULL 或 NONE，则可以不设置任何文理
	// 如果TYPE为 SCALE，TRANSPARENT,CALLSHOW，则必须加载texture1
	// 其他类型必须加载texture1和texture2
    
	static CCocosButton *Create(int id, CUIListener *pListener, ButtonType type = TYPE_NULL, bool bUseFrame = false, const char *szTexture1 = NULL, const char *szTexture2 = NULL);
    
	CCocosButton();
    
	ButtonType GetType()        const {return m_type;}
	float      GetScaleFactor() const {return m_scaleFactor;}
    bool       IsButtonDown()   const {return m_bReceiveDown;}
    
    
	cocos2d::CCSprite *GetNormalSprite()  {return m_pSprNormal;}
	cocos2d::CCSprite *GetReplaceSprite() {return m_pSprReplace;}
	bool               IsChange();
	void               ShowRelpace();
	void               ShowNormal();
    void               ChangeState();
    
	// 设置缩放因数，影响TYPE_SCALE
	void SetScaleFactor(float scale) ;
    void SetScaleNormal(float scale) ;
    void SetOpacityFactor(float scale);
	void ChangeButton(bool bUseReplace = false);
	void CreateLable(const char *szFontFile, cocos2d::CCTextAlignment alignment, float scale, const char *szNormal, const char *szReplace = NULL);
    
private:
	ButtonType         m_type;
	cocos2d::CCSprite *m_pSprReplace;
	cocos2d::CCSprite *m_pSprNormal;
	cocos2d::CCLabelBMFont *m_pFntReplace;
	cocos2d::CCLabelBMFont *m_pFntNormal;

	float              m_scaleFactor;
    float              m_scaleNormal;
    float              m_opaticyFactor;
    bool               m_bReceiveDown;
    
private:
    static CCocosButton *s_pCurFocus;   // 互斥判定
};

#endif