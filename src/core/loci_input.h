#ifndef LOCI_INPUT_H
#define LOCI_INPUT_H

#include "loci_time.h"

typedef struct Loci_Input
{
    bool down;
    bool pressed;
    float lastPressed;
}Loci_Key,
Loci_Button;

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

void loci_updateButtons();

typedef struct Loci_Cursor
{
    vec2 position;
    vec2 deltaPosition;
}Loci_Cursor;

Loci_Cursor loci_cursor;

void loci_createCursor();
void loci_updateCursor();


#endif