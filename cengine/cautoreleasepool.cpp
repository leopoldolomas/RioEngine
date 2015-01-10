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

#include "misc/genericshelper.hpp"
#include "cautoreleasepool.h"

//-----------------------------------------------------------------------------

std::vector< std::vector<CObject*>* >* CAutoreleasePool::m_pools =
        new std::vector< std::vector<CObject*>* >();

//-----------------------------------------------------------------------------

CAutoreleasePool::CAutoreleasePool(void) : CObject()
{
    m_localPool = new std::vector<CObject*>();
    m_pools->push_back(m_localPool);
}

//-----------------------------------------------------------------------------

void CAutoreleasePool::addObject(CObject* obj) {
    RE_ASSERT(m_pools->size() > 0);
    std::vector< CObject* >* current_pool = m_pools->at(m_pools->size() - 1);
    current_pool->push_back(obj);
}

//-----------------------------------------------------------------------------

void CAutoreleasePool::drain() {
    std::vector< CObject* >* current_pool = m_pools->at(m_pools->size() - 1);
    if(current_pool->size() > 0) {
        for(std::vector< CObject* >::iterator it = current_pool->begin();
            it != current_pool->end();
            ++it) {
                SAFE_RELEASE(*it);
        }
        SAFE_RELEASE(current_pool);
        m_localPool = new std::vector<CObject*>();
        m_pools->at(m_pools->size() - 1) = m_localPool;
    }
}

//-----------------------------------------------------------------------------

void CAutoreleasePool::log() {
    LOG("=========================================== Autorelease pools");
    LOG("");

    int pos = 0;
    for(std::vector< std::vector<CObject*>* >::iterator it = m_pools->begin();
        it != m_pools->end();
        ++it) {
            pos++;
            LOG("Pool position: %i", pos);
            for(std::vector<CObject*>::iterator it2 = (*it)->begin();
                it2 != (*it)->end();
                ++it2) {
                    CObject* obj = *it2;
                    const char* name = obj->metaObject()->className();
                    LOG("Addr: %p -> Classname: %s -> Retain count: %u", obj, name, obj->retainCount());
                    LOG("-----");
            }
    }

    LOG("");
    LOG("===========================================");
}

//-----------------------------------------------------------------------------

void CAutoreleasePool::releasePools() {
    GenericsHelper::deleteVector(m_pools);
}

//-----------------------------------------------------------------------------

CAutoreleasePool::~CAutoreleasePool(void) {
    m_pools->pop_back();
    SAFE_RELEASE(m_localPool);
}
