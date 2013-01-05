////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CCocosInputBox.h
//
// 输入框
//
//                                            By Wander 2012.04.05
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef COCOSINPUTBOX_H_20120405
#define COCOSINPUTBOX_H_20120405

#include "CocosUIBase.h"

class CCocosInputBox : public CCocosUIBase, public cocos2d::CCIMEDelegate , public cocos2d::CCTextFieldDelegate
{
public:
	enum InputEvent
	{
		EVENT_KEYBOARDSHOW = 10000,
		EVENT_KEYBOARDHIDE
	};
	enum ShowType
	{
		TYPE_NORMAL,
		TYPE_PASSWORD,		// 密码星号;
	};
	enum AllowType
	{
		ALLOW_ALL,
		ALLOW_NUMONLY,		// 只接受0-9
		ALLOW_CHARONLY,		// 只接受英文字符
	};
    
private:
	CREATE_FUNC(CCocosInputBox);
	virtual void onReceiveNewTouch();
	virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& info);
	virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& info);
    
	bool onTextFieldInsertText(cocos2d::CCTextFieldTTF * sender, const char * text, int nLen);
	bool onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF * sender, const char * delText, int nLen);
    
    virtual void onExit();
    
private:
	void InitState(int id, CUIListener *pListener, const char *szHolder, float fontSize, const char *szFontName,
                   bool bUseFrame, const char *szTexture, const cocos2d::CCSize boxSize);
	void OnUpdate(float dt);
    
    
public:
	static CCocosInputBox *Create(int id, CUIListener *pListener, const char *szHolder, float fontSize, const char *szFontName, 
		                          bool bUseFrame = false, const char *szTexture = NULL, const cocos2d::CCSize boxSize = cocos2d::CCSizeZero);
    
	CCocosInputBox();
    
	void SetStyle(int maxCharNum = -1, ShowType showType = TYPE_NORMAL, AllowType allowtype = ALLOW_ALL) 
	{
		m_showType   = showType;
		m_nCharLimit = maxCharNum;
		m_allowType  = allowtype;
	}
	void SetMaskChar(char c)                                       {m_cPswMask = c;}
    void SetTrackNodeInfo(cocos2d::CCNode *pNode, float space = 0) {m_pTrackNode = pNode; m_trackSpace = space;}
    
    void SetDetach();
    
    void EnableMultiLine(float fontSize, const char *szFontName);
	const char    *GetText();
    unsigned long  GetTextLen();
    void SetText(std::string strText);
    
    
    
    
    
private:
	cocos2d::CCSprite       *m_pSprBack;
	cocos2d::CCTextFieldTTF *m_pTFInput;
	cocos2d::CCLabelTTF     *m_pLableCaret;				// 光标
    cocos2d::CCLabelTTF     *m_pLabelMultiLine;
    
    bool                     m_bAttaching;				// 用于收起键盘时释放
    cocos2d::CCIMEKeyboardNotificationInfo m_boardInfo;	// 储存获取的键盘信息
    
	std::string              m_strBackup;				// 实际获取的字符串
	std::string              m_strReject;				// 排除的字符串
	std::vector<int>         m_vecSubLens;				// 每个UTF-8字符的长度
    
	int                      m_nCharLimit;				// 字数限制
	char                     m_cPswMask;				// 密码掩码
	ShowType                 m_showType;				// 显示类型
	AllowType                m_allowType;				// 字符允许类型
	
	int                      m_startIndex;				// 当前显示字符的索引
    
	cocos2d::CCSize          m_boxSize;					// 输入框有效框大小，
    
    cocos2d::CCNode         *m_pTrackNode;
    float                    m_trackSpace;
    float                    m_lastAdjustVertY;
	
};
#endif