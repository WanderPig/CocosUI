////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosNumBand.h
//
// 分数版，用于显示一个分数，分数资源为图片
//
//                                            By Wander 2012.09.3
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef Archer2_CocosNumBand_h
#define Archer2_CocosNumBand_h

#include "CocosUIBase.h"
#include <string>

class CCocosNumBand : public cocos2d::CCLayer
{
public:
    enum
    {
        NUMCOUNT_MAX= 10,
    };
    enum BandType
    {
        TYPE_IMAGE,
        TYPE_FONT,
    };
    enum CountStyle
    {
        STYLE_SIMPLE,
        STYLE_EACHBIT,
    };
public:
    CREATE_FUNC(CCocosNumBand);
    static CCocosNumBand *Create(bool bUseFrame, const char *szFormat, float space = 0);
    static CCocosNumBand *Create(const char *szFontName, float size, cocos2d::ccColor3B color);
    
    void ShowNum(long showNum);
    void RollNum(long showNum, bool bScale);
    void CountNum(long toNum, cocos2d::CCAction *pAction, CountStyle style = STYLE_EACHBIT, long step = 1, long fromNum = 0);
    void StopCount();
    
    void FadeOut(float d);
    
    void SetEffect(std::string strEffect) {m_strEffect = strEffect;}

private:
    CCocosNumBand();
    void InitStateImage(bool bUseFrame, const char *szFormat, float space);
    void InitStateFont(const char *szFontName, float size, cocos2d::ccColor3B color);
    
    void ShowNumImage(long num);
    void ShowNumFont(long mum);
    
    void OnCount(float dt);
    
    void CalNextBits(long nextShowNum);
    void ResortNums();
    
    void OnRemoveNum(cocos2d::CCNode *pNode);
private:
    BandType             m_bandType;
    
    std::string          m_strFormat;
    bool                 m_bUseFrame;
    float                m_space;
    
    cocos2d::CCSprite   *m_pSprCur[NUMCOUNT_MAX];
    char                 m_lastBits[NUMCOUNT_MAX];
    char                 m_curBits[NUMCOUNT_MAX];
    bool                 m_bFlipBits[NUMCOUNT_MAX];
    int                  m_lastBitCount;
    int                  m_curBitCount;
    
    cocos2d::CCLabelTTF *m_pLabelTTF;
    
    long                 m_countTo;
    CountStyle           m_countStyle;
    long                 m_countStep;
    float                m_curCounting;
    
    float                m_countTimer;
    
    cocos2d::CCAction   *m_pActOver;
    
    std::string          m_strEffect;
    int                  m_idEffect;
    
};

#endif
