//--------------------------------------------------------------- @License begins
// RioEngine: The late night Coke -without whores- debugging sessions
// 2012-2014 Leopoldo Lomas Flores. Torreon, Coahuila. MEXICO/MEKSYK
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

#ifndef _Macro_H_
#define _Macro_H_

#define NOMINMAX // TODO Hack needed to include Windows.h
#include <windows.h>

#include <QDebug>
#include "typedefs.h"
#include "buildconfig.h"

#ifdef RE_DEBUG
#define LOG qDebug
#elif
#define LOG dummyLog // TODO define
#endif

//#undef printf
//#define printf LOG

#define REMOVEFCHAR(s) s.erase(0, 1)

#define PI 3.14159265f
#define DEGTORAD(x) x * (PI / 180.0f)
#define RADTODEG(x) x * (180.0f / PI)

#define IS_POINTER(T) _is_pointer<T>::value

#define RE_ASSERT   Q_ASSERT
#define RE_ASSERT_X Q_ASSERT_X

#define VLD_ENABLED 0 // set to 1 to enable memory leak detection

#endif _Macro_H_
