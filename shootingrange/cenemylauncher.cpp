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

#include "cengine/csoundplayer.h"
#include "cengine/cgamescene.h"
#include "shootingrange/cenemytarget.h"
#include "shootingrange/cenemylauncher.h"

//-----------------------------------------------------------------------------

CEnemyLauncher::CEnemyLauncher() : CEnemy() {
}

//-----------------------------------------------------------------------------

CEnemyLauncher::CEnemyLauncher(const CEnemyLauncher& src_game_object) : CEnemy() {
    CEnemy::copyValuesFromObject(src_game_object);
}

//-----------------------------------------------------------------------------

const CEnemyLauncher& CEnemyLauncher::operator=(const CEnemyLauncher& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    CEnemy::copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CEnemyLauncher::launchEnemy(float delay_time,
                                  float exposure_time,
                                  bool slide) {
    m_slide = slide;

    CEnemyTarget* enemy = static_cast<CEnemyTarget*>(parentScene()->createGameObjectWithNode("Target", true, "CEnemyTarget"));

    enemy->setTransform(m_transform);

    // TODO move the following lines to CEnemyTarget?
    calcSize();
    enemy->calcSize();
    float half_width = size().getY() / 2.0f;
    enemy->setLauncherHeight(size().x());
    enemy->transform().moveY(-(enemy->size().getY() / 2.0f));

    enemy->setDelayTime(delay_time);
    enemy->setExposureTime(exposure_time);
    enemy->setDoSlide(slide);

    float slide_units = ( slide ? half_width * 2.0f : 0.0f );
    enemy->setSlideLength(slide_units);
    enemy->launch();
}

//-----------------------------------------------------------------------------

void CEnemyLauncher::update() {
}

//-----------------------------------------------------------------------------

CEnemyLauncher::~CEnemyLauncher() {
}
