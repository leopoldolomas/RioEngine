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

#ifndef RIOENGINE_CENGINE_CAUTORELEASEPOOL_H
#define RIOENGINE_CENGINE_CAUTORELEASEPOOL_H

#include <vector>
#include "cengine/csingleton.h"

#define CAUTORELEASEPOOL CSingleton<CAutoreleasePool>::getSharedInstance()

/**
 * This is my own version of the NSAutoreleasePool class. An autorelease pool
 * stores objects that are sent a release message when the pool itself is
 * drained.
 * I haven't take a look at the original NSAutoreleasePool code, but it seems
 * that I managed to make this class very reliable on my own. If you analize
 * the engine for memory leaks you will hardly find any. Let me know if you
 * ever find one!
 */
class CAutoreleasePool : public CObject
{
public:
    CAutoreleasePool(void);

    /**
     * Adds the object to the current pool.
     */
    virtual void addObject(CObject* obj);

    /**
     * Drains the current pool.
     */
    virtual void drain();

    /**
     * Prints all the objects that are pending to be released. Useful when
     * debugging.
     */
    static  void log();

    /**
     * You should always call this static method before closing the application.
     */
    static  void releasePools();

    ~CAutoreleasePool(void);

protected:
    static std::vector< std::vector<CObject*>* >* m_pools;
    std::vector<CObject*>* m_localPool;
};

#endif // RIOENGINE_CENGINE_CAUTORELEASEPOOL_H

