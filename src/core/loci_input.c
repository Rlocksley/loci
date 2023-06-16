#include "loci_input.h"

float loci_keyPressTime = 0.5f;

Loci_Key loci_keyA = {false, false, 0.f};
Loci_Key loci_keyD = {false, false, 0.f};
Loci_Key loci_keyS = {false, false, 0.f};
Loci_Key loci_keyW = {false, false, 0.f};
Loci_Key loci_keyX = {false, false, 0.f};

void loci_updateKeys();

float loci_buttonPressTime = 0.2f;

Loci_Button loci_buttonLeft = {false, false, 0.f};
Loci_Button loci_buttonRight = {false, false, 0.f};


Loci_Cursor loci_cursor;

void loci_updateKeys()
{
    loci_keyA.down = (glfwGetKey(loci_pGLFWwindow, GLFW_KEY_A) == GLFW_PRESS);
    loci_keyA.pressed = (loci_keyA.down && loci_keyA.lastPressed > loci_keyPressTime);
    loci_keyA.lastPressed = (loci_keyA.lastPressed + loci_deltaTime) * !loci_keyA.pressed;

    loci_keyD.down = (glfwGetKey(loci_pGLFWwindow, GLFW_KEY_D) == GLFW_PRESS);
    loci_keyD.pressed = (loci_keyD.down && loci_keyD.lastPressed > loci_keyPressTime);
    loci_keyD.lastPressed = (loci_keyD.lastPressed + loci_deltaTime) * !loci_keyD.pressed;

    loci_keyS.down = (glfwGetKey(loci_pGLFWwindow, GLFW_KEY_S) == GLFW_PRESS);
    loci_keyS.pressed = (loci_keyS.down && loci_keyS.lastPressed > loci_keyPressTime);
    loci_keyS.lastPressed = (loci_keyS.lastPressed + loci_deltaTime) * !loci_keyS.pressed;

    loci_keyW.down = (glfwGetKey(loci_pGLFWwindow, GLFW_KEY_W) == GLFW_PRESS);
    loci_keyW.pressed = (loci_keyW.down && loci_keyW.lastPressed > loci_keyPressTime);
    loci_keyW.lastPressed = (loci_keyW.lastPressed + loci_deltaTime) * !loci_keyW.pressed;

    loci_keyX.down = (glfwGetKey(loci_pGLFWwindow, GLFW_KEY_X) == GLFW_PRESS);
    loci_keyX.pressed = (loci_keyX.down && loci_keyX.lastPressed > loci_keyPressTime);
    loci_keyX.lastPressed = (loci_keyX.lastPressed + loci_deltaTime) * !loci_keyX.pressed;

}

void loci_updateButtons()
{
    loci_buttonLeft.down = (glfwGetMouseButton(loci_pGLFWwindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    loci_buttonLeft.pressed = (loci_buttonLeft.down && loci_buttonLeft.lastPressed > loci_buttonPressTime);
    loci_buttonLeft.lastPressed = (loci_buttonLeft.lastPressed + loci_deltaTime) * loci_buttonLeft.pressed;

    loci_buttonRight.down = (glfwGetMouseButton(loci_pGLFWwindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    loci_buttonRight.pressed = (loci_buttonRight.down && loci_buttonRight.lastPressed > loci_buttonPressTime);
    loci_buttonRight.lastPressed = (loci_buttonRight.lastPressed + loci_deltaTime) * loci_buttonRight.pressed;
}

void loci_createCursor()
{
    glfwSetCursorPos
    (loci_pGLFWwindow,
    loci_windowWidth/2.f, loci_windowHeight/2.f);

    glm_vec2_copy
    ((vec2){loci_windowWidth/2.f, loci_windowHeight/2.f},
    loci_cursor.position);

    glm_vec2_copy
    ((vec2){0.f,0.f}, loci_cursor.deltaPosition);
}

void loci_updateCursor()
{
    double posX, posY;
    glfwGetCursorPos(loci_pGLFWwindow, &posX, &posY);

    vec2 lastPosition;
    glm_vec2_copy
    (loci_cursor.position, lastPosition);

    glm_vec2_copy
    ((vec2){(float)posX, (float)posY},
    loci_cursor.position);

    glm_vec2_sub
    (lastPosition, loci_cursor.position,
    loci_cursor.deltaPosition);
}