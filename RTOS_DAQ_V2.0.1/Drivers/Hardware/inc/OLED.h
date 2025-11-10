#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include "OLED_Data.h"

/*参数宏定义*********************/

/*FontSize参数取值*/
/*此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度*/
#define OLED_8X16				8
#define OLED_6X8				6

/*IsFilled参数数值*/
#define OLED_UNFILLED			0
#define OLED_FILLED				1

/*********************参数宏定义*/


/*函数声明*********************/

/*初始化函数*/
void OLED_Init(void);

/*更新函数*/
void OLED_Update(void);                                                     //更新全屏
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);  //更新指定区域

/*显存控制函数*/
void OLED_Clear(void);                                                      //清空全屏
void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);   //清空指定区域
void OLED_Reverse(void);                                                    //全屏反色
void OLED_ReverseArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height); //指定区域反色

/*显示函数*/
void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize);//字符
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize);//字符串
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);//数字
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);//带符号数字
void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);//16进制数字
void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);//2进制数字
void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);//浮点数字
void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese);//显示中文
void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);//显示图像

void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...);//打印格式化字符串，可以实现上述所有显示函数
/*
关于OLED_Printf打印：
显示字符：            %c  OLED_Printf(0,0,OLED_6X8,"Num1:%c,Num2:%c",156,-134);
显示字符串：          %S  或直接写
显示无符号十进制数字：%u，%d  其中+代表前面带正负号，05代表几位显示数字。OLED_Printf(0,0,OLED_6X8,"Num1:%+05d,Num2:%d",123,-134);
显示有符号十进制数字：%d
显示十六进制数字：    %s或者%S     S大写，0xAB...就是大写
显示二进制数字：      %x或者%b(可能会不行)OLED_Printf(0,10,OLED_6X8,"N1:0x%05X,N2:0x%x",0xacab,0xabad);
显示浮点数：          %f    OLED_Printf(0,20,OLED_6X8,"N1:%+5.2f,N2:%+05.3f",15.5575,-134.57);
*/


/*绘图函数*/
void OLED_DrawPoint(uint8_t X, uint8_t Y);//画一个点
uint8_t OLED_GetPoint(uint8_t X, uint8_t Y);//检查指定点是否处于点亮状态,点亮返回1，否则返回0
void OLED_DrawLine(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1);//画直线
void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);//画矩形，起始点x，起始点y，矩形宽x，矩形长y，是否填充
void OLED_DrawTriangle(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t IsFilled);//画三角形
void OLED_DrawCircle(uint8_t X, uint8_t Y, uint8_t Radius, uint8_t IsFilled);//画圆
void OLED_DrawEllipse(uint8_t X, uint8_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);//画椭圆
void OLED_DrawArc(uint8_t X, uint8_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);//画圆弧

/*********************函数声明*/

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
