//
//  CocosItemList.cpp
//  Archer2
//
//  Created by Wu Wang on 12-7-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CocosItemList.h"

using namespace cocos2d;

CCocosItem *CCocosItem::Create(int id, bool bHor, bool bUseFrame , const char *szBack)
{
    CCocosItem *pItem = CCocosItem::create();
    pItem->InitState(id, bHor, szBack, bUseFrame);
    return pItem;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItem::onReceiveNewTouch()
{
    m_ptLast = m_pCurTouch->getLocationInView();
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItem::onCurTouchFinish()
{
    CCPoint ptCur = m_pCurTouch->getLocationInView();
    
    CCPoint ptDiff = ccpSub(ptCur, m_ptLast);
    
    if(fabsf(ptDiff.x) < 5.0f && fabsf(ptDiff.y) < 5.0f)
    {
        if(NULL != m_pListener)
        {
            m_pListener->onReceiveMsg(m_id, EVENT_SELECTED);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItem::loseFocus()
{

    for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end();iter++)
    {
        (*iter)->loseFocus();
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItem::setEnable(bool bEnable)
{
    CCocosUIContainer::setEnable(bEnable);
    for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end();iter++)
    {
        (*iter)->setEnable(bEnable);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItem::InitState(int id, bool bHor, const char *szBack, bool bUseFrame)
{
    SetID(id);
    if(NULL != szBack)
    {
        if(bUseFrame)
        {
            m_pSprBack = CCSprite::createWithSpriteFrameName(szBack);
        }
        else
        {
            m_pSprBack = CCSprite::create(szBack);
        }
        
        addChild(m_pSprBack);
        
        if(bHor)
        {
            m_size = m_pSprBack->getContentSize().width;
        }
        else
        {
            m_size = m_pSprBack->getContentSize().height;
        }
        
        setContentSize(m_pSprBack->getContentSize());
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItem::SetChangeItem(bool bHor, bool bUseFrame , const char *szBack)
{
    if(NULL != szBack)
    {
        if(bUseFrame)
        {
            m_pSprChange = CCSprite::createWithSpriteFrameName(szBack);
        }
        else
        {
            m_pSprChange = CCSprite::create(szBack);
        }
        
        m_pSprChange->setVisible(false);
        addChild(m_pSprChange);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItem::AddControlToNormal(CCocosUIBase *pNewControl, cocos2d::CCPoint ptPos, int zOrder)
{
	pNewControl->setPosition(ptPos);
	addChild(pNewControl, zOrder);
	m_listControlNormal.push_back(pNewControl);
    m_listControls.push_back(pNewControl);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItem::AddControlToChange(CCocosUIBase *pNewControl, cocos2d::CCPoint ptPos, int zOrder)
{
	pNewControl->setPosition(ptPos);
	addChild(pNewControl, zOrder);
	m_listControlChange.push_back(pNewControl);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItem::Active(bool bActive)
{
    m_bActive =bActive;
    m_listControls.clear();
    if(bActive)
    {
        for(ControlList::iterator iter = m_listControlNormal.begin(); iter != m_listControlNormal.end(); iter++)
        {
            (*iter)->show(false);
        }
        
        for(ControlList::iterator iter = m_listControlChange.begin(); iter != m_listControlChange.end(); iter++)
        {
            (*iter)->show(true);
            m_listControls.push_back(*iter);
        }
        
    }
    else
    {
        for(ControlList::iterator iter = m_listControlChange.begin(); iter != m_listControlChange.end(); iter++)
        {
            (*iter)->show(false);
        }
        
        for(ControlList::iterator iter = m_listControlNormal.begin(); iter != m_listControlNormal.end(); iter++)
        {
            (*iter)->show(true);
            m_listControls.push_back(*iter);
        }
    }
    if(NULL != m_pSprChange)
    {
        m_pSprChange->setVisible(bActive);
        m_pSprBack->setVisible(!bActive);
    }
}

/////////////////////////////////////////////////////////////////////////////

void CCocosItemList::visit()
{
    
    UpdateList();
   
    if(m_rcShow.equals(CCRectZero))
    {
        CCLayer::visit();
        return;
    }
    
    
    glEnable(GL_SCISSOR_TEST);

    // 像素
    
//    glScissor((m_rcShow.origin.x *0.5 *(m_parentScale + 1)+ getPositionX()* m_parentScale/m_scale)  , (m_rcShow.origin.y * 0.5 *(m_parentScale + 1)+ getPositionY()* m_parentScale/m_scale), 
//              m_rcShow.size.width* m_parentScale, m_rcShow.size.height* m_parentScale);
    // 此区域永远相对于屏幕，并且以像素为单位
    CCPoint pt = getParent()->convertToWorldSpace(getPosition());
    glScissor(m_rcShow.origin.x * m_parentScale+ pt.x  /m_scale, 
              m_rcShow.origin.y * m_parentScale + pt.y / m_scale, 
              m_rcShow.size.width* m_parentScale, 
              m_rcShow.size.height* m_parentScale);
        
    CCLayer::visit();
    glDisable(GL_SCISSOR_TEST);
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
bool CCocosItemList::isContainsTouch(cocos2d::CCTouch *pTouch)
{
    CCPoint pt = this->convertTouchToNodeSpaceAR(pTouch);
    CCPoint pt1 = getAnchorPoint();
    return (cocos2d::CCRect(-this->getContentSize().width*this->getAnchorPoint().x,
                            -this->getContentSize().height*this->getAnchorPoint().y,
                            this->getContentSize().width,
                            this->getContentSize().height).containsPoint(this->convertTouchToNodeSpaceAR(pTouch)));
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::onReceiveNewTouch()
{
    m_ptLast = convertTouchToNodeSpace(m_pCurTouch);
    m_bAutoMoving = false;
    m_bTouchMoving = true;
    
    if(m_type == ITEMLIST_FROMTOP || m_type == ITEMLIST_FROMBOTTOM)
    {
        m_startTouchPos = m_ptLast.y;
    }
    else
    {
        m_startTouchPos = m_ptLast.x;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::onCurTouchMoveOut()
{
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::onCurTouchMoving()
{
    CCPoint ptCur = convertTouchToNodeSpace(m_pCurTouch);
    
    
    if(m_type == ITEMLIST_FROMTOP || m_type == ITEMLIST_FROMBOTTOM)
    {
        if(fabsf(m_startTouchPos - ptCur.y) > 5)
        {
            UnfocusAll();
            m_startTouchPos = ptCur.y;
        }
    }
    else
    {
        if(fabsf(m_startTouchPos - ptCur.x) > 5)
        {
            UnfocusAll();
            m_startTouchPos = ptCur.x;
        }
    }
    
    float diff;
    if (m_type == ITEMLIST_FROMLEFT || m_type == ITEMLIST_FROMRIGHT) 
    {
        diff =ptCur.x- m_ptLast.x;
        
        if(getContentSize().width > m_itemSizeCount)
        {
            diff *= 0.5f;
        }
        else if((m_startPos > 0 && diff > 0) ||
                (m_startPos + m_itemSizeCount <getContentSize().width && diff < 0))
        {
            diff *= 0.5f;
        }
         
    }
    else
    {
        diff = ptCur.y - m_ptLast.y ;
        
        if(getContentSize().height > m_itemSizeCount)
        {
            diff *= 0.5f;
        }
        else if((m_startPos - m_itemSizeCount > 0 && diff > 0) ||
                (m_startPos < getContentSize().height && diff < 0))
        {
            diff *= 0.5f;
        }
         
        
    }
    
    m_curSpeed = diff * m_scale;
    m_bTouchMoving = true;
    //m_startPos = diff;
    m_ptLast = ptCur;
    m_startPos += m_curSpeed;
	ResortItems();
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::onCurTouchFinish()
{
    m_bTouchMoving = false;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::onReceiveMsg(int aID, int event , const void *pData )
{
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::setEnable(bool bEnable)
{
    CCocosUIContainer::setEnable(bEnable);
    for(std::list<CCocosItem *>::iterator iter = m_listItems.begin(); iter != m_listItems.end(); iter++)
    {
        (*iter)->setEnable(bEnable);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
CCocosItemList *CCocosItemList::Create(int id, CUIListener *pListerner, cocos2d::CCSize s, ItemListType type)
{
    CCocosItemList *pList = CCocosItemList::create();
    pList->InitState(id, pListerner, type, s);
    return pList;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::InitState(int id, CUIListener *pListener, ItemListType type, cocos2d::CCSize s)
{
    m_scale = CCDirector::sharedDirector()->getWinSize().width / CCDirector::sharedDirector()->getWinSizeInPixels().width ;
    
    CCocosUIBase::SetData(id, pListener, true);
    m_type = type;
    m_rcShow.origin = CCPointZero;
    m_rcShow.size   = s;
    
    setContentSize(CCSize(s.width * m_scale, s.height * m_scale));
    

    if(!s.equals(CCSizeZero))
    {
        if(type == ITEMLIST_FROMTOP)
        {
            setAnchorPoint(ccp(0.5, 1));
            m_startPos = getContentSize().height * getAnchorPoint().y;
        }
        else if(type == ITEMLIST_FROMBOTTOM)
        {
            setAnchorPoint(ccp(0.5, 0));
            m_startPos = getContentSize().height * getAnchorPoint().y;
        }
        else if(type == ITEMLIST_FROMLEFT)
        {
            setAnchorPoint(ccp(0, 0.5));
            m_startPos = 0;
        }
        else if(type == ITEMLIST_FROMRIGHT)
        {
            setAnchorPoint(ccp(1, 0.5));
            m_startPos = getContentSize().width * getAnchorPoint().x;
   
        }
    }
    
    m_rcShow.origin.x = - m_rcShow.size.width * getAnchorPoint().x;
    m_rcShow.origin.y = - m_rcShow.size.height * getAnchorPoint().y;
    
    ignoreAnchorPointForPosition(false);
    m_bAutoMoving = false;
    m_curSpeed = 0;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::SetParentScale(float scale)
{
    m_parentScale = scale;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::AddItem(CCocosItem *pNewItem)
{
    m_listItems.push_back(pNewItem);
    m_listControls.push_back(pNewItem);
    m_itemSizeCount += pNewItem->GetSize();
    addChild(pNewItem);
    ResortItems();
}

void CCocosItemList::InsertItem(CCocosItem *pItem, int pos)
{
    m_listItems.insert(m_listItems.begin(), pItem);
    m_listControls.push_back(pItem);
    m_itemSizeCount += pItem->GetSize();
    addChild(pItem);
    ResortItems();
    
}
//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::RemoveItem(CCocosItem *pItem)
{
    if(NULL == pItem)
    {
        return;
    }
    std::list<CCocosItem *>::iterator iter = m_listItems.begin();
    CCocosItem *pItemToRemove;
    for(; iter != m_listItems.end(); iter++)
    {
        if(*iter == pItem)
        {
            pItemToRemove = pItem;
            break;
        }
    }
    
    if(iter == m_listItems.end())
    {
        // 末尾
        return;
    }
    


    iter++;
    if(iter != m_listItems.end())
    {
        float xDiff = 0;
        float yDiff = 0;
        if(m_type == ITEMLIST_FROMLEFT)
        {
            xDiff = (*iter)->getPosition().x - pItem->getPosition().x;
        }
        else if(m_type == ITEMLIST_FROMTOP)
        {
            yDiff = (*iter)->getPosition().y - pItem->getPosition().y;
        }
        for(; iter != m_listItems.end(); iter++)
        {
            (*iter)->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(0.2f, ccp(-xDiff, -yDiff)), CCCallFunc::create(this, callfunc_selector(CCocosItemList::OnRemoveItem))));
        }
        
        m_bStopUpdate = true;
    }

    m_itemSizeCount -= pItem->GetSize();
    m_listControls.remove(pItem);
    m_listItems.remove(pItem);
    pItem->removeFromParentAndCleanup(true);
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::OnRemoveItem()
{
    m_bStopUpdate = false;
}

//------------------------------------------------------------------------------------------------------------------------------------/
CCocosItem *CCocosItemList::ItemByIndex(int index)
{
    if(index >= m_listItems.size())
    {
        return NULL;
    }
    std::list<CCocosItem *>::iterator iter = m_listItems.begin();
    for(int i=0; i<index; i++)
    {
        iter++;
    }
    
    return *iter;
}

//------------------------------------------------------------------------------------------------------------------------------------/
CCocosItem *CCocosItemList::ItemByID(int id)
{
    for(std::list<CCocosItem *>::iterator iter = m_listItems.begin(); iter != m_listItems.end(); iter++)
    {
        if((*iter)->GetID() == id)
        {
            return *iter;
        }
    }
    return NULL;
}

int CCocosItemList::IndexById(int id)
{
    int index = 0;
    for(std::list<CCocosItem *>::iterator iter = m_listItems.begin(); iter != m_listItems.end(); iter++, index++)
    {
        if((*iter)->GetID() == id)
        {
            return index;
        }
    }
    
    return -1;
}
//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::UpdateList()
{
    if(m_bStopUpdate || !m_bEnable)
    {
        return;
    }
    float boxSize;
    float distToTop;
    float minPos;
    float maxPos;
    
    if(m_type == ITEMLIST_FROMTOP || m_type == ITEMLIST_FROMRIGHT)
    {
        boxSize = getContentSize().height;
        distToTop = boxSize - m_startPos;
        maxPos = m_itemSizeCount;
        minPos = boxSize;
    }
    else
    {
        boxSize = getContentSize().width;
        distToTop = -m_startPos;
        minPos = boxSize - m_itemSizeCount;
        maxPos = 0;
    }
    
    if(!m_bTouchMoving)
    {
        if(m_bAutoMoving)
        {
            if(boxSize > m_itemSizeCount)
            {	
                // ITEM尺寸很小
                if(fabsf(distToTop) < fabsf(m_curSpeed))
                {
                    m_bAutoMoving = false;
                    m_startPos += distToTop;
                    m_curSpeed = 0;
                }
            }
            else
            {
                if(fabsf(m_startPos - minPos) < fabsf(m_curSpeed))
                {
                    m_startPos = minPos;
                    m_bAutoMoving = false;
                    m_curSpeed    = 0;

                }
                else if(fabsf(m_startPos - maxPos) < fabsf(m_curSpeed))
                {
                    m_startPos = maxPos;
                    m_bAutoMoving = false;
                    m_curSpeed    = 0;
                }
            }
        }
        else
        {
            // 自动回滚
            if(m_curSpeed > 0)
            {
                m_curSpeed -= 1;
            }
            else if(m_curSpeed < 0)
            {
                m_curSpeed += 1;
            }
            
            if(abs(m_curSpeed) < 1)
            {
                m_curSpeed = 0;
            }
            
            if(boxSize > m_itemSizeCount)
            {
                // ITEM尺寸很小
                m_curSpeed = distToTop / 30.0f;
                m_bAutoMoving = true;
            }
            else
            {
                // 有多余未显示的ITEM
                // 考虑两边超出部分。
                
                if(m_startPos > maxPos)
                {
                    m_curSpeed = (maxPos  - m_startPos) / 30.0f;
                    m_bAutoMoving = true;
                }
                else if(m_startPos < minPos)
                {
                    m_curSpeed = (minPos - m_startPos) / 30.0f;
                    m_bAutoMoving = true;
                }
            }
        }
        
        m_startPos += m_curSpeed;
        ResortItems();
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::ResortItems()
{
    float startPos = m_startPos;
    float lastSize = 0;
    float maxPos;
    float minPos = 0;
    float s= 1;
    
    if(m_type == ITEMLIST_FROMLEFT)
    {
        maxPos = getContentSize().width;
    }
    else if(m_type == ITEMLIST_FROMTOP)
    {
        maxPos = getContentSize().height;
        s = -1;
    }
    
    for(std::list<CCocosItem *>::iterator iter = m_listItems.begin(); iter != m_listItems.end(); iter++)
    {
        CCocosItem *pItem = *iter;
        startPos += ((pItem->GetSize() + lastSize)*0.5f )*s;
        if(m_type == ITEMLIST_FROMTOP)
        {
            pItem->setPosition(ccp(m_rcShow.size.width * 0.5f * m_scale, startPos));
        }
        else
        {
            pItem->setPosition(ccp(startPos, m_rcShow.size.height * 0.5f * m_scale));
        }
        
        lastSize = pItem->GetSize();
        
        if(startPos > maxPos || startPos < minPos)
        {
            pItem->setEnable(false);
        }
        else
        {
            pItem->setEnable(true);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosItemList::UnfocusAll()
{
    for(std::list<CCocosItem *>::iterator iter = m_listItems.begin(); iter != m_listItems.end(); iter++)
    {
        (*iter)->loseFocus();
    }
}
