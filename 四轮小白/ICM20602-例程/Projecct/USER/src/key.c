#include "key.h"




void Switch_init(void)    //拨码开关初始化
{
    gpio_init(B7,GPI,1);    //拨码第一个开关
    port_init_NoAlt(B7,PULLUP);
    gpio_init(B9,GPI,1);    //拨码第二个开关
    port_init_NoAlt(B9,PULLUP);
    gpio_init(B11,GPI,1);    //拨码第三个开关
    port_init_NoAlt(B11,PULLUP);
    gpio_init(B17,GPI,1);    //拨码第四个开关
    port_init_NoAlt(B17,PULLUP);
}





void FiveToSwitchInit()
{
      gpio_init(A27,GPI,1);    //上
      port_init_NoAlt(A27,PULLUP);
      gpio_init(A29,GPI,1);    //下
      port_init_NoAlt(A29,PULLUP);
      gpio_init(B1,GPI,1);    //左
      port_init_NoAlt(B1,PULLUP);
      gpio_init(A28,GPI,0);    //右
      port_init_NoAlt(A28,PULLUP);
      gpio_init(B0,GPI,1);    //中
      port_init_NoAlt(B0,PULLUP);
}