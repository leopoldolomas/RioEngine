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

#ifndef RIOENGINE_MISC_DIRECTORYHELPER_H_
#define RIOENGINE_MISC_DIRECTORYHELPER_H_

#include <regex>
#include "constants.h"
#include "misc/dirent.h"
#include "misc/stringhelper.hpp"

class DirectoryHelper {
 public:
    static std::string getProjectDirectory() {
        return std::string(k_RioEngineProjectPath); // TODO load this value from an INI file instead of hardcode
    }

    //-----------------------------------------------------------------------------

    static std::string getAssetsDirectory() {
        return DirectoryHelper::getProjectDirectory().append("assets\\");
    }

    //-----------------------------------------------------------------------------

    // TODO return a CArray instead of std::vector
    static std::vector<std::string> getFileList(std::string dir) {
        std::vector<std::string> files;

        DIR *dp;
        struct dirent *ep;
        dp = opendir(dir.c_str());

        if (dp != NULL) {
            while (ep = readdir (dp)) {
                files.push_back(ep->d_name);
            }
            (void)closedir(dp);
        }

        return files;
    }

    //-----------------------------------------------------------------------------

    // TODO return a CArray instead of std::vector
    static std::vector<std::string> getFileList(std::string dir, std::string extension) {
        std::vector<std::string> files = DirectoryHelper::getFileList(dir);
        std::vector<std::string> matches;

        std::string regex_ext_str = "(.*).";
        regex_ext_str.append(extension);

        for (unsigned int i = 0; i < files.size(); i ++) {
            std::string file = files.at(i);
            std::regex regex_ext(regex_ext_str);

            if (std::regex_search(file, regex_ext))
                matches.push_back(file);
        }
        return matches;
    }
};

#endif  // RIOENGINE_MISC_DIRECTORYHELPER_H_
