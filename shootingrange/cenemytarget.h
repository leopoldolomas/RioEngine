//--------------------------------------------------------------- @License begins
// RioEngine: The late night Coke -without whores- debugging sessions
// 2012-2015 Leopoldo Lomas Flores. Torreon, Coahuila. MEXICO
// leopoldolomas [at] gmail
// www.rioengine.com
// www.leopoldolomas.info
// "You have a problem, you face it like a man."
// 
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
// software, either in source code form or as a compiled binary, for any purpose,
// commercial or non-commercial, and by any means.
// 
// In jurisdictions that recognize copyright laws, the author or authors of this
// software dedicate any and all copyright interest in the software to the public
// domain. We make this dedication for the benefit of the public at large and to
// the detriment of our heirs and successors. We intend this dedication to be
// an overt act of relinquishment in perpetuity of all present and future
// rights to this software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//--------------------------------------------------------------- @License ends

#ifndef RIOENGINE_SHOOTINGRANGE_CENEMYTARGET_H
#define RIOENGINE_SHOOTINGRANGE_CENEMYTARGET_H

#include "cengine/cenemy.h"

class CEnemyTarget : public CEnemy
{
    Q_OBJECT
public:
    CEnemyTarget();

    float delayTime() const;
    void setDelayTime(float delay_time);

    float exposureTime() const;
    void setExposureTime(float exposure_time);

    float slideLength() const;
    void setSlideLength(float slide_length);

    bool doSlide() const;
    void setDoSlide(bool do_slide);

    float launcherHeight() const;
    void setLauncherHeight(float launcher_height);

    virtual void launch();
    virtual void damageReceived(int damage);
    virtual void aboutToBeRemoved();
    virtual void stopAllSounds();

protected:
    float m_delayTime;
    float m_exposureTime;
    float m_slideLength;
    float m_launcherHeight;
    bool  m_doSlide;
    int   m_slideSoundId;

private slots:
    void onGoUpActionStart  (CAction* action);
    void onGoDownActionStart(CAction* action);
    void onSequenceDone     (CAction* action);
    void onSlideActionStart (CAction* action);
};
Q_DECLARE_METATYPE(CEnemyTarget)

#endif // RIOENGINE_SHOOTINGRANGE_CENEMYTARGET_H
