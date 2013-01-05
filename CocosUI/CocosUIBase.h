////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosUIBase.h
//
// 所有控件的基类
//
//                                            By Wander 2012.02.21
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef COCOSUIBASE_H_20120221
#define COCOSUIBASE_H_20120221

#include "cocos2d.h"

#include <vector>
#include <queue>
#include <stack>
#include <list>

class CUIListener
{
public:
	virtual void onReceiveMsg(int aID, int event = 0, const void *pData = NULL) = 0;
};

class CCocosUIBase : public cocos2d::CCLayer
{
public:
	virtual void onEnter()
	{
		cocos2d::CCLayer::onEnter();
		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
	}
	virtual void onExit()
	{
		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		cocos2d::CCLayer::onExit();
	}

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
	{
		if(!m_bEnable || !pTouches||!pTouches->count())
		{
			return;
		}

		if(NULL != m_pCurTouch)
		{
			return;
		}

		cocos2d::CCSetIterator iter;
		for(iter = pTouches->begin(); iter != pTouches->end(); iter++)
		{
			if(isContainsTouch((cocos2d::CCTouch *)(*iter)))
			{
				m_pCurTouch = (cocos2d::CCTouch *)(*iter);
				onReceiveNewTouch();
			}
		}

	}
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
	{
		if(!m_bEnable || !pTouches||!pTouches->count())
		{
			return;
		}

		if(NULL == m_pCurTouch)
		{
			return;
		}

		cocos2d::CCSetIterator iter;
		for(iter = pTouches->begin(); iter != pTouches->end(); iter++)
		{
			if(*iter == m_pCurTouch)
			{
				if(!isContainsTouch((cocos2d::CCTouch *)(*iter)))
				{
					onCurTouchMoveOut();
				}

				onCurTouchMoving();
			}
		}
	}
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
	{
		if(NULL == m_pCurTouch)
		{
			return;
		}
		cocos2d::CCSetIterator iter;
		for(iter = pTouches->begin(); iter != pTouches->end(); iter++)
		{
			if(*iter == m_pCurTouch)
			{
                onCurTouchFinish();
				m_pCurTouch = NULL;
			}
		}
	}
	virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
	{
		if(NULL == m_pCurTouch)
		{
			return;
		}
		cocos2d::CCSetIterator iter;
		for(iter = pTouches->begin(); iter != pTouches->end(); iter++)
		{
			if(*iter == m_pCurTouch)
			{
				m_pCurTouch = NULL;
				onCurTouchFinish();
			}
		}
	}

	virtual bool isContainsTouch(cocos2d::CCTouch *pTouch)
	{
		return (cocos2d::CCRect(-this->getContentSize().width*0.5f, -this->getContentSize().height*0.5f, this->getContentSize().width, this->getContentSize().height).containsPoint(this->convertTouchToNodeSpace(pTouch)));
	}


public:
	virtual void setEnable(bool bEnable) {m_bEnable = bEnable;}
	virtual void loseFocus()			 {m_pCurTouch = NULL;}
	virtual void show(bool bShow)
	{
		setVisible(bShow);
		m_bEnable = bShow;
	}

    virtual void fadeOut(float duration)
    {
        for(std::vector<cocos2d::CCNode *>::iterator iter = m_vFadeNodes.begin(); iter != m_vFadeNodes.end(); iter++)
        {
            ((CCNode *)(*iter))->runAction(cocos2d::CCFadeOut::create(duration));
        }
    }
    virtual void fadeIn(float duration)
    {
        for(std::vector<cocos2d::CCNode *>::iterator iter = m_vFadeNodes.begin(); iter != m_vFadeNodes.end(); iter++)
        {
            ((CCNode *)(*iter))->runAction(cocos2d::CCFadeIn::create(duration));
        }
    }

protected:
	virtual void onReceiveNewTouch() {}
	virtual void onCurTouchMoveOut() {}
	virtual void onCurTouchMoving()  {}
	virtual void onCurTouchFinish() {}
	
public:
	CCocosUIBase()
		: m_bEnable(true)
		, m_pCurTouch(NULL)
		, m_id(-1)
		, m_pListener(NULL)
	{
	}

	void SetID(int id)                       {m_id = id;}
	void SetListener(CUIListener *pListener) {m_pListener = pListener;}

	bool         GetIsEnable() const              {return m_bEnable;}
	int          GetID()       const              {return m_id;}
	CUIListener *GetListener() const              {return m_pListener;}

	void SetData(int id, CUIListener *pListener = NULL, bool bEnable = true)
	{
		m_id        = id;
		m_pListener = pListener;
		m_bEnable   = bEnable;
	}
    
    void AddFadeNode(cocos2d::CCNode *pNode)
    {
        m_vFadeNodes.push_back(pNode);
    }

    
protected:
	bool              m_bEnable;
	int               m_id;
	CUIListener      *m_pListener;
	cocos2d::CCTouch *m_pCurTouch;
    std::vector<cocos2d::CCNode *> m_vFadeNodes;
};

typedef std::list<CCocosUIBase *>   ControlList;
typedef std::queue<CCocosUIBase *>  ControlQueue;
typedef std::vector<CCocosUIBase *> ControlVector;

#endif
