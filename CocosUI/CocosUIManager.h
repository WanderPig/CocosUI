////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosUIManager.h
//
// UI管理容器，用于一个页面所有UI的控制
//
//                                            By Wander 2012.07.18
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef Archer2_CocosUIManager_h
#define Archer2_CocosUIManager_h

#include "CocosUIBase.h"

class CCocosUIManager
{
public:
    void AddControl(CCocosUIBase *pControl);
	void RemoveControl(CCocosUIBase *pControl);
    
    void EnableControls(bool bEnable, CCocosUIBase *pExclude = NULL);
    
    void SwitchTo(CCocosUIBase *pControl);
    
private:
    ControlList               m_listControls;
    ControlList               m_listControlsTemp;
};

#endif
