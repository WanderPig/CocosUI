////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosSlide.h
//
// 滑块
//
//                                            By Wander 2012.02.16
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef COCOSSLIDE_H_201201216
#define COCOSSLIDE_H_201201216

#include "CocosUIBase.h"

class CCocosSlide : public CCocosUIBase
{
private:
	virtual void onReceiveNewTouch();
	virtual void onCurTouchMoving();
    
	CREATE_FUNC(CCocosSlide);
    
public:
	// 建造一个滑块，其中滑动条为必须加载的。按钮和背景选加。
    
	static CCocosSlide *Create(int id, CUIListener *pListene, bool bUseFrame, 
                               const char *szBarName, const char *szBtnName = NULL , const char *szBackName = NULL);
    
	CCocosSlide(void);
    
	// 权重与数值 
	// 当权重为1时，数值的范围为0-1，其代表进度条显示的百分比
	// 若权重不为1，则数值的范围为0-weight，其代表进度条百分比*weight
    
	void  SetPower(float power);
	void  SetWeight(float weight); 
	float GetPower();
    
private:
	bool InitState(int id, CUIListener *pListener, bool bUseFrame, const char *szBarName, const char *szBtnName = NULL , const char *szBackName = NULL);
	void ChangeState();
private:
	cocos2d::CCSprite *m_pSprBack;		// 背景
	cocos2d::CCSprite *m_pSprBar;		// 显示大小的条
	cocos2d::CCSprite *m_pSprButton;	// 滑块按钮
    
	float              m_lastTouchPosX;	// 上一次触点位置
	float              m_power;			// 当前数值
	float              m_weight;		// 当前权重
    
    float              m_barWidth;
	
};


#endif

