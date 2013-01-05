#include "CocosCheckBox.h"


using namespace cocos2d;

CCocosCheckBox *CCocosCheckBox::CreateCheckBox(int id, CUIListener *pListener, CheckBoxType type)
{
	CCocosCheckBox *pNewCheckBox = CCocosCheckBox::create();
	pNewCheckBox->SetData(id, pListener, true);
	pNewCheckBox->SetType(type);
	return pNewCheckBox;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosCheckBox::onReceiveMsg(int aID, int event , const void *pData)
{
	SetCheck(aID);
	m_pListener->onReceiveMsg(m_id, aID);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosCheckBox::setEnable(bool bEnable)
{
	for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end(); iter++)
	{
		((CCocosUIBase *)(*iter))->setEnable(bEnable);
	}
}

//------------------------------------------------------------------------------------------------------------------------------------/
CCocosButton *CCocosCheckBox::AddBox(cocos2d::CCPoint ptPos, int zOrder, int id, 
                                     bool bUseFrame , const char *szTexture1, const char *szTexture2 )
{
	return AddButton(ptPos, zOrder, id	, this, CCocosButton::TYPE_CALLCHANGE, bUseFrame, szTexture1, szTexture2);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosCheckBox::SetCheck(int id)
{
	if(m_type == TYPE_SINGLE)
	{
		for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end(); iter++)
		{
			CCocosButton *pControl = (CCocosButton *)(*iter);
            
			if(pControl->GetID() == id)
			{
				if(!pControl->IsChange())
				{
					// 由暗变亮
					if(m_listCheckedIDs.size() == 0)
					{
						// 此前没有亮的
						pControl->ShowRelpace();
					}
					else
					{
						// 此前有亮的,由亮变暗
						int idFind = (*m_listCheckedIDs.begin());
						m_listCheckedIDs.clear();
                        
						for(ControlList::iterator iterfind = m_listControls.begin(); iterfind != m_listControls.end(); iterfind++)
						{
							CCocosButton *pControlFind = (CCocosButton *)(*iterfind);
                            
							if(pControlFind->GetID() == idFind)
							{
								pControlFind->ShowNormal();
                                
							}
						}
					}
                    
					m_listCheckedIDs.push_back(id);
					pControl->ShowRelpace();
				}
                
				break;
			}
		}
	}
}