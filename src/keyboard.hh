#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
/* Keyboard related stuff */
/*************************
 * keyboard(key,x,y)
 * Called when a key on the keyboard is pressed
 * Handles rotation, resetting of the scene etc
 *************************/
void keyboard_handler(unsigned char key, int x, int y);
#endif
