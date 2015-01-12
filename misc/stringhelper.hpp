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

#ifndef RIOENGINE_MISC_STRINGHELPER_H_
#define RIOENGINE_MISC_STRINGHELPER_H_

//-----------------------------------------------------------------------------

#include <algorithm>
#include <string>
#include <vector>

//-----------------------------------------------------------------------------

class StringHelper {
public:
    static std::vector<std::string> split(const std::string& str, const std::string& delimiters) {
        std::vector<std::string> tokens;

        // Skip delimiters at beginning.
        std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        // Find first "non-delimiter".
        std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

        while (std::string::npos != pos || std::string::npos != lastPos) {
            // Found a token, add it to the vector.
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            // Skip delimiters.  Note the "not_of"
            lastPos = str.find_first_not_of(delimiters, pos);
            // Find next "non-delimiter"
            pos = str.find_first_of(delimiters, lastPos);
        }
        return tokens;
    }

    //-----------------------------------------------------------------------------

    static std::string addPrefix(const std::string& prefix, const std::string& str) {
        std::string result = prefix;
        result.append(str);
        return result;
    }

    //-----------------------------------------------------------------------------

    static std::string& removeOccurrencesOfString(std::string& original_str, std::string& str_to_remove) {
        std::string::size_type i = original_str.find(str_to_remove);

        if (i != std::string::npos) {
            original_str.erase(i, str_to_remove.length());
        }
        return original_str;
    }

    //-----------------------------------------------------------------------------

    static std::string& removeOccurrencesOfChar(std::string& original_str, char char_to_remove) {
        std::string str_to_remove = std::string(&char_to_remove);
        return removeOccurrencesOfString(original_str, str_to_remove);
    }
};

#endif // RIOENGINE_MISC_STRINGHELPER_H_
