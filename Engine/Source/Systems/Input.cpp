#include "Input.h"
#include <iostream>

std::unordered_map<int, bool> Input::keyMap;
Input::MouseData Input::mouseData = { 0, 0 };


void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        keyMap[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keyMap[key] = false;
    }
}

void Input::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	mouseData.x = xpos;
	mouseData.y = ypos;
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        keyMap[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        keyMap[button] = false;
    }
}


bool Input::getKeyDown(Key key) {
    // check if this key is pressed
    int keyInt = static_cast<int>(key);

    return keyMap.contains(keyInt) && keyMap[keyInt];
}


bool Input::getKeyPressed(Key key) {
    int keyInt = static_cast<int>(key);
    bool pressed = keyMap[keyInt];
    keyMap[keyInt] = false;
    return pressed;
}

Input::MouseData Input::getMousePos()
{
    return mouseData;
}
