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

#include <climits>
#include <iostream>
#include <bitset>
#include "misc/mathhelper.hpp"
#include "cengine/cobject.h"
#include "inputmanager/inputmanager.h"
#include "inputmanager/joystick.h"

//-----------------------------------------------------------------------------

namespace USERINPUT {

InputManager::InputManager(void) {
    resetStates();

    m_leftJoystick  = new Joystick();
    m_rightJoystick = new Joystick();
    m_mousePosition = new MousePosition();

    m_joystickEnabled = false;
}

//-----------------------------------------------------------------------------

void InputManager::update() {
    resetStates();
    updateJoysticksState();
    updateKeyboardState();
    updateMouseState();
}

//-----------------------------------------------------------------------------

bool InputManager::getAnyKeyDown() {
    return getAnyTrueValInArray(m_keysDown, k_KEYS_LENGTH);
}

//-----------------------------------------------------------------------------

bool InputManager::getAnyKeyUp() {
    return getAnyTrueValInArray(m_keysUp, k_KEYS_LENGTH);
}

//-----------------------------------------------------------------------------

bool InputManager::getKeyDown(int key_code) {
    return m_keysDown[key_code];
}

//-----------------------------------------------------------------------------

bool InputManager::getKeyUp(int key_code) {
    return m_keysUp[key_code];
}

//-----------------------------------------------------------------------------

void InputManager::setWheelVelocity(float wheel_velocity) {
    m_mouseWheelVelocity = wheel_velocity;
}

//-----------------------------------------------------------------------------

const float InputManager::getWheelVelocity() {
    return m_mouseWheelVelocity;
}

//-----------------------------------------------------------------------------

void InputManager::setMousePosition(const MousePosition& mouse_position) {
    m_mousePosition->x = mouse_position.x;
    m_mousePosition->y = mouse_position.y;
}

//-----------------------------------------------------------------------------

const MousePosition* InputManager::getMousePosition() {
    return m_mousePosition;
}

//-----------------------------------------------------------------------------

bool InputManager::getAnyMouseButtonDown() {
    return getAnyTrueValInArray(m_mouseButtonsDown, k_MBUTTONS_LENGTH);
}

//-----------------------------------------------------------------------------

bool InputManager::getAnyMouseButtonUp() {
    return getAnyTrueValInArray(m_mouseButtonsUp, k_MBUTTONS_LENGTH);
}

//-----------------------------------------------------------------------------

bool InputManager::getMouseButtonDown(MouseButtons button_number) {
    return m_mouseButtonsDown[button_number];
}

//-----------------------------------------------------------------------------

bool InputManager::getMouseButtonUp(MouseButtons button_number) {
    return m_mouseButtonsUp[button_number];
}

//-----------------------------------------------------------------------------

void InputManager::clearBoolArray(bool bool_array[], int array_length) {
    for (int i = 0; i < array_length; i++) {
        bool_array[i] = false;
    }
}

//-----------------------------------------------------------------------------

void InputManager::resetStates() {
    for(int i = 0; i < k_KEYS_LENGTH; i++) {
        m_previousKeysDown[i] = m_keysDown[i];
    }
    clearBoolArray(m_keysDown, k_KEYS_LENGTH);
    clearBoolArray(m_keysUp, k_KEYS_LENGTH);
    clearBoolArray(m_mouseButtonsDown, k_MBUTTONS_LENGTH);
    clearBoolArray(m_mouseButtonsUp, k_MBUTTONS_LENGTH);
    clearBoolArray(m_joystickButtonsDown, k_JOYBUTTONS_LENGTH);

    setWheelVelocity(0.0f);
}

//-----------------------------------------------------------------------------

void InputManager::updateJoysticksState() {
    if (!m_joystickEnabled) {
        return;
    }

    JOYINFOEX joyinfo;

    ZeroMemory(&joyinfo, sizeof(joyinfo));

    joyinfo.dwSize = sizeof(joyinfo);
    joyinfo.dwFlags = JOY_RETURNALL|JOY_RETURNPOVCTS;
    joyGetPosEx(0, &joyinfo);  // retrieve joystick state

    // retrieve the joysticks' positions
    m_leftJoystick->setX(rawPositionToVelocity((unsigned short)joyinfo.dwXpos));
    m_leftJoystick->setY(rawPositionToVelocity((unsigned short)joyinfo.dwYpos));

    m_rightJoystick->setX(rawPositionToVelocity((unsigned short)joyinfo.dwUpos));
    m_rightJoystick->setY(rawPositionToVelocity((unsigned short)joyinfo.dwRpos));

    if (joyinfo.dwButtonNumber > 0) { // at least one button was pressed?
        setJoystickPressedButtonsCount(joyinfo.dwButtonNumber);  // query the pressed buttons count

        std::bitset<32> ba((int)joyinfo.dwButtons);  // convert the integer var to a bit array
        std::string bit_array_as_string = ba.to_string();  // store the bit array as string

        unsigned short current_button = 0;

        for (unsigned int i = bit_array_as_string.length() - 1; i > bit_array_as_string.length() - k_JOYBUTTONS_LENGTH; i--) { // iterate the array backwards
            if (bit_array_as_string[i] == '1') { // if the position contains '1' then we found a pressed button at index 'i'
                m_joystickButtonsDown[current_button] = true;
            }
            current_button++;
        }
    }
}

//-----------------------------------------------------------------------------

void InputManager::updateKeyboardState() {
    bool m_previousKeyState;
    for (unsigned int i = 0; i < k_KEYS_LENGTH; i++) {
        m_previousKeyState = m_previousKeysDown[i];
        m_keysDown[i] = (GetAsyncKeyState(i) & 0x8000) != 0;

        if(m_previousKeyState == true && m_previousKeyState != m_keysDown[i]) {
            m_keysUp[i] = true;
        }
    }
    clearBoolArray(m_previousKeysDown, k_KEYS_LENGTH);
}

//-----------------------------------------------------------------------------

void InputManager::updateMouseState() {
    POINT point;
    RECT rect;

    if (GetCursorPos(&point)) {
        if(GetWindowRect(GetActiveWindow(), &rect)) {
            if(PtInRect(&rect, point)) {
                m_mousePosition->x = point.x;
                m_mousePosition->y = point.y;
            }
        }
    }

    m_mouseButtonsDown[LeftButton]    = GetAsyncKeyState(VK_LBUTTON) < 0;
    m_mouseButtonsDown[MiddleButton]  = GetAsyncKeyState(VK_MBUTTON) < 0;
    m_mouseButtonsDown[RightButton]   = GetAsyncKeyState(VK_RBUTTON) < 0;
}

//-----------------------------------------------------------------------------

bool InputManager::getAnyTrueValInArray(bool bool_array[], int array_length) {
    for (int i = 0; i < array_length; i++) {
        if (bool_array[i]) {
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------

void InputManager::initializeJoystick(HWND handler) {
    if (joySetCapture(handler, JOYSTICKID1, NULL, TRUE)) {
        /*MessageBeep(MB_ICONEXCLAMATION);
        MessageBox(handler, L"Joystick not found", NULL,
            MB_OK | MB_ICONEXCLAMATION); */
    } else {
        m_joystickEnabled = true;
    }
}

//-----------------------------------------------------------------------------

bool InputManager::getAnyJoystickButtonDown() {
    return getAnyTrueValInArray(m_joystickButtonsDown, k_JOYBUTTONS_LENGTH);
}

//-----------------------------------------------------------------------------

bool InputManager::getJoystickButtonDown(int button_number) {
    return m_joystickButtonsDown[button_number - 1];
}

//-----------------------------------------------------------------------------

bool InputManager::getJoystickButtonUp(int) {
    // TODO
    return false;
}

//-----------------------------------------------------------------------------

const int InputManager::getJoystickButtonsPressedCount() {
    return m_joystickPressedButtonsCount;
}

//-----------------------------------------------------------------------------

void InputManager::setJoystickPressedButtonsCount(int buttons_count) {
    m_joystickPressedButtonsCount = buttons_count;
}

//-----------------------------------------------------------------------------

float InputManager::rawPositionToVelocity(unsigned short raw_value) {
    unsigned short ushort_half_value = (unsigned short)MathHelper::roundCeil(USHRT_MAX / 2.0f, 0);

    float velocity = fabs((float)(ushort_half_value - raw_value));
    velocity /= ushort_half_value;  //  sets velocity between 0 and 1

    if (raw_value < ushort_half_value) {
        velocity *= -1; // then velocity must be negative
    }

    return velocity;
}

//-----------------------------------------------------------------------------

Joystick* InputManager::getLeftJoystick() {
    return m_leftJoystick;
}

//-----------------------------------------------------------------------------

Joystick* InputManager::getRightJoystick() {
    return m_rightJoystick;
}

//-----------------------------------------------------------------------------

InputManager::~InputManager(void) {
    SAFE_RELEASE(m_mousePosition);
    SAFE_RELEASE(m_leftJoystick);
    SAFE_RELEASE(m_rightJoystick);
}

} // namespace USERINPUT
