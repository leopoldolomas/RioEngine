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

#ifndef RIOENGINE_INPUTMANAGER_INPUTMANAGER_H_
#define RIOENGINE_INPUTMANAGER_INPUTMANAGER_H_

#define NOMINMAX // TODO Hack needed to include Windows.h
#include <Windows.h>
#include "constants.h"
#include "cengine/csingleton.h"
#include "inputmanager/joystick.h"

#define INPUTMANAGER CSingleton<USERINPUT::InputManager>::getSharedInstance()

namespace USERINPUT {
enum MouseButtons           { LeftButton, MiddleButton, RightButton };
enum MouseWheelDirection    { Stationary, Up, Down                  };
struct MousePosition        { int x; int y;                         };


class InputManager {
public:
    InputManager(void);

    /**
     * Call this method to update the current Input State
     */
    void update();

    // ------------------------------------------- Keyboard
    bool getAnyKeyDown();
    bool getAnyKeyUp();
    bool getKeyDown(int key_code);
    bool getKeyUp  (int key_code);

    // ------------------------------------------- Mouse
    void setWheelVelocity(float wheel_velocity);
    const float getWheelVelocity();

    const MousePosition* getMousePosition();
    void setMousePosition(const MousePosition& position);
    bool getAnyMouseButtonDown();
    bool getAnyMouseButtonUp();
    bool getMouseButtonDown(MouseButtons button_number);
    bool getMouseButtonUp  (MouseButtons button_number);

    // ------------------------------------------- Joystick
    Joystick* getLeftJoystick();
    Joystick* getRightJoystick();

    void initializeJoystick(HWND handler);
    float rawPositionToVelocity(unsigned short raw_value);
    const int getJoystickButtonsPressedCount();
    bool getAnyJoystickButtonDown();
    bool getJoystickButtonDown(int button_number);
    bool getJoystickButtonUp  (int button_number);

    ~InputManager(void);

private:
    void resetStates();
    void updateJoysticksState();
    void updateKeyboardState();
    void updateMouseState();

    void clearBoolArray      (bool bool_array[], int array_length);
    bool getAnyTrueValInArray(bool bool_array[], int array_length);

    void setJoystickPressedButtonsCount(int buttons_count);

    bool    m_keysDown              [k_KEYS_LENGTH],
            m_keysUp                [k_KEYS_LENGTH],
            m_previousKeysDown      [k_KEYS_LENGTH],
            m_mouseButtonsDown      [k_MBUTTONS_LENGTH],
            m_mouseButtonsUp        [k_MBUTTONS_LENGTH],
            m_joystickButtonsDown   [k_JOYBUTTONS_LENGTH];

    bool  m_joystickEnabled;
    float m_mouseWheelVelocity;
    int   m_joystickPressedButtonsCount;

    Joystick* m_leftJoystick;
    Joystick* m_rightJoystick;
    MousePosition* m_mousePosition;
};
}

#endif // RIOENGINE_INPUTMANAGER_INPUTMANAGER_H_
