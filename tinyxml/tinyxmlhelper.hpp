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

#ifndef RIOENGINE_TINYXML_TINYXMLHELPER_H_
#define RIOENGINE_TINYXML_TINYXMLHELPER_H_

#include <vector>
#include <string>
#include "macro.h"

class TinyXMLHelper
{
public:
    enum VectorType { Integer, Float };

    static void* getVectorFromString(const char* p_as_str, VectorType type)
    {
        std::string p_as_str_tmp = p_as_str;

        void* _vector = NULL;
        char* _context = NULL;

        switch(type) {
        case Integer:	_vector = new std::vector<int>();	 break;
        case Float:		_vector = new std::vector<float>();	 break;
        }

        char* pch;
        pch = strtok_s((char*)p_as_str_tmp.c_str()," ", &_context);

        while (pch != NULL) {
            switch(type)
            {
            case Integer:	static_cast<std::vector<int>*>(_vector)->push_back(atoi(pch));		break;
            case Float:		static_cast<std::vector<float>*>(_vector)->push_back((float)atof(pch));		break;
            }
            pch = strtok_s (NULL, " ", &_context);
        }
        SAFE_RELEASE(pch);
        return _vector;
    }
};

#endif // RIOENGINE_TINYXML_TINYXMLHELPER_H_
