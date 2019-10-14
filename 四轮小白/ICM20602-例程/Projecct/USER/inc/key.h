#ifndef _KEY_H
#define _KEY_H
#include "headfile.h"

#define KEY_one   gpio_get(B7)
#define KEY_two   gpio_get(B9)
#define KEY_three gpio_get(B11)
#define KEY_four  gpio_get(B17)             






void Switch_init(void);
void FiveToSwitchInit();



#endif