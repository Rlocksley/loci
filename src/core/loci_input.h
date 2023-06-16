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

extern float loci_keyPressTime;

extern Loci_Key loci_keyA;
extern Loci_Key loci_keyD;
extern Loci_Key loci_keyS;
extern Loci_Key loci_keyW;
extern Loci_Key loci_keyX;

void loci_updateKeys();

extern float loci_buttonPressTime;

extern Loci_Button loci_buttonLeft;
extern Loci_Button loci_buttonRight;

void loci_updateButtons();

typedef struct Loci_Cursor
{
    vec2 position;
    vec2 deltaPosition;
}Loci_Cursor;

extern Loci_Cursor loci_cursor;

void loci_createCursor();
void loci_updateCursor();


#endif