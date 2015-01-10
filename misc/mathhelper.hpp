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

#ifndef RIOENGINE_MISC_MATHHELPER_H_
#define RIOENGINE_MISC_MATHHELPER_H_

#include <math.h>

class MathHelper {
public:
    static int clampi(int value, int min, int max) {
        return clamp<int>(value, min, max);
    }

    //-----------------------------------------------------------------------------

    static float clampf(float value, float min, float max) {
        return clamp<float>(value, min, max);
    }

    //-----------------------------------------------------------------------------

    static float roundCeil(float value, int decimal_places) {
        int factor = getFactorFromDecimalPlaces(decimal_places);
        float val = ceilf(value * factor) / factor;

        if (val == -0.0f) { // Wat?
            val = 0.0f;
        }

        return val;
    }

    //-----------------------------------------------------------------------------

    static float roundNearest(float value, int decimal_places) {
        int factor = getFactorFromDecimalPlaces(decimal_places);
        return floorf(value * (float)factor + 0.5f) / (float)factor;
    }

    //-----------------------------------------------------------------------------

    static float roundFloor(float value, int decimal_places) {
        int factor = getFactorFromDecimalPlaces(decimal_places);
        return floorf(value * (float)factor) / (float)factor;
    }

private:
    template<class T>
    static T clamp(T _value, T min, T max) {
        T value = _value;

        if (value < min) {
            value = min;
        } else if (value > max) {
            value = max;
        }

        return value;
    }

    //-----------------------------------------------------------------------------

    static int getFactorFromDecimalPlaces(int decimal_places) {
        int factor = 1;

        for (int i=0; i < decimal_places; i++) {
            factor *= 10;
        }

        return factor;
    }
};

#endif // RIOENGINE_MISC_MATHHELPER_H_
