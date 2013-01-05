//
//  CocosUIManager.cpp
//  Archer2
//
//  Created by Wu Wang on 12-7-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CocosUIManager.h"



void CCocosUIManager::AddControl(CCocosUIBase *pControl)
{
	m_listControls.push_back(pControl);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosUIManager::RemoveControl(CCocosUIBase *pControl)
{
	m_listControls.remove(pControl);
    m_listControlsTemp.remove(pControl);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosUIManager::EnableControls(bool bEnable, CCocosUIBase *pExclude)
{
    if(!bEnable)
    {
        for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end(); iter++)
        {
            if(*iter == pExclude)
            {
                continue;
            }
            if((CCocosUIBase *)(*iter)->GetIsEnable())
            {
                ((CCocosUIBase *)(*iter))->setEnable(bEnable);
                m_listControlsTemp.push_back(*iter);
            }
            
        }
    }
    else
    {
        for(ControlList::iterator iter = m_listControlsTemp.begin(); iter != m_listControlsTemp.end(); iter++)
        {
            ((CCocosUIBase *)(*iter))->setEnable(bEnable);
        }
        
        m_listControlsTemp.clear();
    }
	
}

//------------------------------------------------------------------------------------------------------------/
void CCocosUIManager::SwitchTo(CCocosUIBase *pControl)
{
    for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end(); iter++)
    {
        if(*iter == pControl)
        {
            pControl->setEnable(true);
        }
        else
        {
            ((CCocosUIBase *)(*iter))->setEnable(false);
        }
    }
}

//------------------------------------------------------------------------------------------------------------/