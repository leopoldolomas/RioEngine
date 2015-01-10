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

#include <irrklang/irrKlang.h>
#include "constants.h"
#include "misc/directoryhelper.hpp"
#include "misc/genericshelper.hpp"
#include "cengine/csoundplayer.h"

//-----------------------------------------------------------------------------

static int sound_id = 0;

//-----------------------------------------------------------------------------

CSoundPlayer::CSoundPlayer() {
    m_engine    = irrklang::createIrrKlangDevice();
    RE_ASSERT(m_engine);

    m_soundsMap = std::map<int, irrklang::ISound*>();
}

//-----------------------------------------------------------------------------

std::string CSoundPlayer::getPathForFilename(const char* filename) {
    std::string path = DirectoryHelper::getProjectDirectory();
    path.append(k_CSoundPlayer_SoundsFolderName);
    path.append("\\");
    path.append(filename);
    return path;
}

//-----------------------------------------------------------------------------

int CSoundPlayer::play2D(const char* filename, bool play_looped, bool start_paused, bool track) {
    irrklang::ISound* sound = m_engine->play2D(getPathForFilename(filename).c_str(), play_looped, start_paused, track);

    if(track) {
        m_soundsMap.insert(std::pair<int, irrklang::ISound*>(sound_id, sound));
        return sound_id++;
    }

    return -1;
}

//-----------------------------------------------------------------------------

irrklang::ISound* CSoundPlayer::getISound(int id) {
    RE_ASSERT(id >= 0);
    return m_soundsMap.at(id);
}

//-----------------------------------------------------------------------------

CSoundPlayer::~CSoundPlayer() {
    m_engine->drop();
}
