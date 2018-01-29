/*Team: Matthew Powaschuk, Brady Kirchner
COMP 2659 - Winter 2016
Prof: Paul Pospisil
Project: "Alien Abduction"
*/

#ifndef VBL_H
#define VBL_H

#define VBL 28

typedef void (*Vector)();

Vector install_vector(int num, Vector vector);
void vblisr();
void do_VBL_ISR();
int set_ipl(int mask);

#endif