#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "usmart.h"
#include "rtc.h"
#include "beep.h"
#include "stmflash.h"

#define FLASH_SAVE_ADDR 0X08070000 //设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

/* 显示时间，index特殊处理 */
void show_index_time(u8 index, _calendar_obj calendar_temp, _calendar_obj alarm);

int main(void)
{
    /* 按键返回值 */
    u8 key = 0;
    /* 修改指向下标 */
    u8 index = 0;
    /* 日历结构体 */
    _calendar_obj calendar_temp;
    _calendar_obj alarm;
    // 修改模式标志
    u8 mode = 0;
    // 停止闹铃按键标志位
    u8 flag = 0;
    // 闹铃循环次数
    u16 count = 0;

    /* 延时函数初始化 */
    delay_init();
    /* 设置中断优先级分组为组2：2位抢占优先级，2位响应优先级 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /* 串口初始化为115200 */
    uart_init(115200);
    /* LED端口初始化 */
    LED_Init();
    /* LCD初始化 */
    LCD_Init();
    /* 按键初始化 */
    KEY_Init();
    /* 初始化USMART */
    usmart_dev.init(SystemCoreClock / 1000000);
    /* RTC初始化 */
    RTC_Init();
    /* 初始化蜂鸣器端口 */
    BEEP_Init();

    /*
    * 显示时间
    * 设置字体为蓝色
    */
    POINT_COLOR = BLUE;
    LCD_ShowString(60, 130, 300, 16, 16, "N O W    :  :  ");
    LCD_ShowString(60, 162, 300, 16, 16, "ALARM    :  :  ");
	 LCD_ShowString(20, 220, 200, 16, 16, "Press KEY1 or KEY_UP");
    LCD_ShowString(20, 236, 200, 16, 16, "to STOP BEEP");
    calendar_temp = calendar;

    // 从指定地址开始读出指定长度的数据
    // ReadAddr:起始地址
    // pBuffer:数据指针
    // NumToWrite:半字(16位)数
    STMFLASH_Read(FLASH_SAVE_ADDR, (u16 *)&alarm.hour, 1);
    STMFLASH_Read(FLASH_SAVE_ADDR + 0X2, (u16 *)&alarm.min, 1);
    alarm.sec = 0;

    while (1)
    {
        /* 根据index显示不同日历变量 */
        if (0 == index)
            show_index_time(index, calendar, alarm);
        else
            show_index_time(index, calendar_temp, alarm);

        /*
        * 键处理函数
        * 返回按键值
        * mode:0,不支持连续按;1,支持连续按;
        * 0，没有任何按键按下
        * 1，KEY0按下
        * 2，KEY1按下
        * 3，KEY3按下 WK_UP
        */
        key = KEY_Scan(0);
        /* KEY0 进入修改模式，依次顺序循环 */
        if (1 == key)
        {
            index++;
            index = index % 6;
            /* 进入修改 */
            if (1 == index)
            {
                calendar_temp = calendar;
					 mode = 1;
            }
            /* 退出修改 */
            else if (0 == index)
            {
                calendar = calendar_temp;
                RTC_Set(calendar_temp.w_year, calendar_temp.w_month, calendar_temp.w_date, calendar_temp.hour, calendar_temp.min, calendar_temp.sec);
                // 从指定地址开始写入指定长度的数据
                // WriteAddr:起始地址(此地址必须为2的倍数!!)
                // pBuffer:数据指针
                // NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
                STMFLASH_Write(FLASH_SAVE_ADDR, (u16 *)&alarm.hour, 1);
                STMFLASH_Write(FLASH_SAVE_ADDR + 0X2, (u16 *)&alarm.min, 1);
					 mode = 0;
            }
        }
        /* KEY1 选中值+1 */
        else if (2 == key)
        {
            if (1 == index)
            {
                calendar_temp.hour++;
                calendar_temp.hour = calendar_temp.hour > 23 ? 0 : calendar_temp.hour;
            }
            else if (2 == index)
            {
                calendar_temp.min++;
                calendar_temp.min = calendar_temp.min > 59 ? 0 : calendar_temp.min;
            }
            else if (3 == index)
            {
                calendar_temp.sec++;
                calendar_temp.sec = calendar_temp.sec > 59 ? 0 : calendar_temp.sec;
            }
            else if (4 == index)
            {
                alarm.hour++;
                alarm.hour = alarm.hour > 23 ? 0 : alarm.hour;
            }
            else if (5 == index)
            {
                alarm.min++;
                alarm.min = alarm.min > 59 ? 0 : alarm.min;
            }
            else if (0 == index)
            {
                // 按键标志位置1 停止蜂鸣器 计数归零
                flag = 1;
                BEEP = 0;
                count = 0;
            }
        }
        /* KEY_UP 选中值-1 */
        else if (3 == key)
        {
            if (1 == index)
            {
                calendar_temp.hour = calendar_temp.hour == 0 ? 23 : calendar_temp.hour - 1;
            }
            else if (2 == index)
            {
                calendar_temp.min = calendar_temp.min == 0 ? 59 : calendar_temp.min - 1;
            }
            else if (3 == index)
            {
                calendar_temp.sec = calendar_temp.sec == 0 ? 59 : calendar_temp.sec - 1;
            }
            else if (4 == index)
            {
                alarm.hour = alarm.hour == 0 ? 23 : alarm.hour - 1;
            }
            else if (5 == index)
            {
                alarm.min = alarm.min == 0 ? 59 : alarm.min - 1;
            }
            else if (0 == index)
            {
                // 按键标志位置1 停止蜂鸣器 计数归零
                flag = 1;
                BEEP = 0;
                count = 0;
            }
        }
        else if (0 == key)
        {
            // 普通模式下
            if (0 == mode)
            {
                // 闹钟到点 && 蜂鸣器没叫 && 没有按下KEY1或KEY_UP键
                if (calendar.hour == alarm.hour && calendar.min == alarm.min && BEEP == 0 && 0 == flag)
                {
                    BEEP = 1;
                }
                // 闹钟没到点 && 按下了KEY1或KEY_UP键
                else if ((calendar.hour != alarm.hour || calendar.min != alarm.min) && 1 == flag)
                {
                    flag = 0;
                }
            }
        }

        // 闹铃循环次数计数
        if (1 == BEEP)
        {
            POINT_COLOR = RED;
            LCD_ShowString(20, 204, 200, 16, 16, "BEEP!!!  (>__<)  ");
            POINT_COLOR = BLUE;
            count++;
        }
        else
        {
            LCD_ShowString(20, 204, 200, 16, 16, "      <(^ v ^)>");
        }

        // 闹铃循环次数达到峰值
        if (5 * 6000 <= count)
        {
            // 按键标志位置1 停止蜂鸣器 计数归零
            flag = 1;
            BEEP = 0;
            count = 0;
        }

        delay_ms(10);
    }
}

/* 显示时间，index特殊处理 */
void show_index_time(u8 index, _calendar_obj calendar_temp, _calendar_obj alarm)
{
    POINT_COLOR = BLUE;
    if (1 == index)
        POINT_COLOR = RED;
    LCD_ShowNum(116, 130, calendar_temp.hour, 2, 16);
    POINT_COLOR = BLUE;
    if (2 == index)
        POINT_COLOR = RED;
    LCD_ShowNum(140, 130, calendar_temp.min, 2, 16);
    POINT_COLOR = BLUE;
    if (3 == index)
        POINT_COLOR = RED;
    LCD_ShowNum(164, 130, calendar_temp.sec, 2, 16);
    POINT_COLOR = BLUE;
    if (4 == index)
        POINT_COLOR = RED;
    LCD_ShowNum(116, 162, alarm.hour, 2, 16);
    POINT_COLOR = BLUE;
    if (5 == index)
        POINT_COLOR = RED;
    LCD_ShowNum(140, 162, alarm.min, 2, 16);
    POINT_COLOR = BLUE;
    LCD_ShowNum(164, 162, alarm.sec, 2, 16);
}