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

#ifndef RIOENGINE_CONSTANTS_H_
#define RIOENGINE_CONSTANTS_H_

static const char* k_RioEngineProjectPath          = "C:\\rioengine\\";
static const char* k_CSoundPlayer_SoundsFolderName = "sound";
static const char* k_QGLViewport_ShadersFolderName = "shaders";

/**
  * Scene files default extension
  */
static const char* k_CGameScene_DefaultExt = "rioscene";

/**
  defines the id used to store the CLNode Skybox
  */
static const char* k_CLScene_SkyboxNodeId = "Skybox";

/**
  * The higher the value, the bigger the manipulators
  */
static const float k_CLRenderManager_ManipulatorsScaleValue = 1.0f;

/**
  maximum number of characters for MD5 string
  */
static const int k_CLScene_MD5MaxCharactersCount = 4;

static const float k_CLMesh_MaxAxisLength             = 10.0f;
static const float k_QGLGameViewport_WorldAxisLength  = 10000.0f;
static const int   k_bdebugdraw_DefaultDebugMode      = 22735;
static const float k_QGLGameViewport_CrosshairXLength = 0.01f;
static const float k_QGLGameViewport_CrosshairYLength = 0.016f;

// ---------------------------------------- QGLViewport
/**
  * fovy angle used for the projection matrix
  */
static const float k_QGLViewport_fovYAngle                = 45.0f;

/**
  * z near value used by the projection matrix
  */
static const float k_QGLViewport_zNear                    = 0.01f;

/**
  * z far value used by the projection matrix
  */
static const float k_QGLViewport_zFar                     = 10000.0f;

// ---------------------------------------- InputManager

/**
  max number of keys
  */
static const int k_KEYS_LENGTH          = 110;

/**
  max number of mouse buttons
  */
static const int k_MBUTTONS_LENGTH      = 3;

/**
  max number of joystick buttons
  */
static const int k_JOYBUTTONS_LENGTH    = 15;

static const float k_InputManager_MouseSensitivity = 0.12f;

// ---------------------------------------- QGLGameEditorViewport

static const float k_QGLGameEditorViewport_SizeFactor = 0.2f;

/**
  * Director's camera default id
  */
static const char*  k_CDirector_DefaultCameraId = "default_camera";

/**
  * Editor's camera default id
  */
static const char* k_QGLGameEditorViewport_CameraId = "__EditorCamera__";

static const float k_QGLGameEditorViewport_Camera_DefaultPosX = 0.0f;
static const float k_QGLGameEditorViewport_Camera_DefaultPosY = 0.5f;
static const float k_QGLGameEditorViewport_Camera_DefaultPosZ = 4.0f;

/**
  * The higher the values, the faster the Editor Camera will move
  */
static const float k_QGLGameEditorViewport_CameraSpeed_Level1        = 2.0f;
static const float k_QGLGameEditorViewport_CameraSpeed_Level2        = 4.0f;
static const float k_QGLGameEditorViewport_CameraSpeed_Level3        = 10.0f;
static const float k_QGLGameEditorViewport_DefaultCameraDisplacement = 0.02f;

// ---------------------------------------- Time

static const float k_Time_DeltaTimeMaxValue = 1.0f;

// ---------------------------------------- MatrixStack

static const int k_MatrixStack_MaxSize      = 32;

#endif // RIOENGINE_CONSTANTS_H_
