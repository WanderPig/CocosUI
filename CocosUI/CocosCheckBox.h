////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosCheckBox.h
//
// 选择框，可以单选可以复选
//
//                                            By Wander 2012.06.06
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef COCOSCHECKBOX_H_20120606
#define COCOSCHECKBOX_H_20120606

#include "CocosUIContainer.h"

#include <list>

typedef std::list<int> CheckBoxIDList;
class CCocosCheckBox : public CCocosUIContainer , public CUIListener
{
public:
	enum CheckBoxType
	{
		TYPE_SINGLE,        // 单选
		TYPE_COMPLEX        // 复选
	};
private:
	CREATE_FUNC(CCocosCheckBox);
	virtual void onReceiveMsg(int aID, int event /* = 0 */, const void *pData /* = NULL */);
	void SetType(CheckBoxType type) {m_type = type;}

public:

	virtual void setEnable(bool bEnable);
	static CCocosCheckBox *CreateCheckBox(int id, CUIListener *pListener, CheckBoxType type = TYPE_SINGLE);

	CCocosButton *AddBox(cocos2d::CCPoint ptPos, int zOrder, int id, bool bUseFrame, const char *szTexture1 , const char *szTexture2);

	void SetCheck(int id);

	const CheckBoxIDList *GetIDList() {return &m_listCheckedIDs;}

private:
	CheckBoxType m_type;

	CheckBoxIDList m_listCheckedIDs;

};
#endif