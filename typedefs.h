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

#ifndef RIOENGINE_TYPEDEFS_H_
#define RIOENGINE_TYPEDEFS_H_

#include <map>
#include <vector>


template<typename T>
struct _is_pointer		{ static const bool value = false; };

template<typename T>
struct _is_pointer<T*>	{ static const bool value = true; };

struct TextureCoord
{
    float s;
    float t;
};

struct CLImageDetails
{
    std::string name;
    std::string filename;

    unsigned int texture;
};

struct CGameObjectProperty;
class CGameObject;
class CLGeometry;
class CLEffect;
class CLMaterial;
class CLNode;

//-------------------------------------------------------------
typedef std::map<std::string, CLImageDetails*>      ImagesMapType;
typedef std::pair<std::string, CLImageDetails*>     ImagePairType;

typedef std::map<std::string, CLGeometry*>          GeometriesMapType;
typedef std::pair<std::string, CLGeometry*>         GeometryPairType;

typedef std::map<std::string, CLEffect*>            EffectsMapType;
typedef std::pair<std::string, CLEffect*>           EffectPairType;

typedef std::map<std::string, CLMaterial*>          MaterialsMapType;
typedef std::pair<std::string, CLMaterial*>         MaterialPairType;

typedef std::map<std::string, CLNode*>              NodesMapType;
typedef std::pair<std::string, CLNode*>             NodePairType;
//-------------------------------------------------------------

#endif // RIOENGINE_TYPEDEFS_H_
