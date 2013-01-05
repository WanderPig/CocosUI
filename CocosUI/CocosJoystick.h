////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosJoystick.h
//
// 虚拟摇杆
//
//                                            By Wander 2012.02.03
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef COCOSJOYSTICK_H_20120203
#define COCOSJOYSTICK_H_20120203

#include "CocosUIBase.h"

class CCocosJoystick : public CCocosUIBase
{
public:
	virtual void onReceiveNewTouch();
	virtual void onCurTouchMoving();
	virtual void onCurTouchFinish();
	CREATE_FUNC(CCocosJoystick);
public:
	CCocosJoystick();

	static CCocosJoystick *CreateJoystick(const char *szJoyPath, const char *szJoybkPath, bool isCanMove);


	void  SetMaxRadius(float r)        {m_maxRadius = r;}
	void  SetResetDuration(float d) {m_resetDuration = d;}

	float GetMaxRadius()   const {return m_maxRadius;}
	float GetCurRotation() const {return m_curRotation;}
	float GetCurPower()    const {return m_curPower;}
	float GetCurPowerX()   const {return m_curPowerX;}
	float GetCurPowerY()   const {return m_curPowerY;}
	float GetResetDuration() const {return m_resetDuration;}
	
	void SetMoveJoyArea(float x, float y, float width, float height);
	void ResetState();

private:
	void InitState(const char *szJoyPath, const char *szJoybkPath, bool isCanMove);
private:
	cocos2d::CCSprite *m_pSprBack;
	cocos2d::CCSprite *m_pSprCenter;
	float              m_maxRadius;
	float              m_resetDuration;

	bool               m_bCanMove;

	cocos2d::CCPoint   m_ptCenter;
	cocos2d::CCPoint   m_ptCur;

	float              m_curRotation;
	float              m_curPower;
	float              m_curPowerX;
	float              m_curPowerY;
};

#endif