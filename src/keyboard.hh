#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
/* Keyboard related stuff */
/*************************
 * keyboard(key,x,y)
 * Called when a key on the keyboard is pressed
 * Handles rotation, resetting of the scene etc
 * Eventually, the keyboard will dispatch general "input events"
 * to the other systems. For now it is a pretty ugly hack.
 *************************/
void keyboard_handler(unsigned char key, int x, int y);
#endif
