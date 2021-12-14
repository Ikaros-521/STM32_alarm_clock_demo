# 前言
开发板：正点原子 STM32F103 精英版  
语言：C语言  
开发环境：Keil5  
使用了 KEY LED LCD RTC FLASH（用于存储闹钟信息） BEEP（充当闹铃）  
## **代码下载**
[码云](https://gitee.com/ikaros-521/STM32_alarm_clock_demo)  [GitHub](https://github.com/Ikaros-521/STM32_alarm_clock_demo)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210413170028662.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

代码参考：正点原子 源码RTC实验例程  
## 工程配置
![在这里插入图片描述](https://img-blog.csdnimg.cn/9d61a79c3c9b46e1845d2f6bbef455f4.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/e9e2e6bb3484459a961a7c15b2747089.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBATG92ZeS4tuS8iuWNoea0m-aWrw==,size_20,color_FFFFFF,t_70,g_se,x_16)

## 功能介绍：
1、LCD 显示当前时间和闹钟设定的时间 以及 停止闹钟的提示语。  
2、按**KEY0**进入**修改**模式，分别可以对**当前时间**的时分秒和**闹钟时间**的时分进行修改（处于修改下的值会变红），最后退出修改模式。  
3、在**修改**模式下：**KEY1**表示数值 +1，**KEY_UP**表示数值 -1。在**普通**模式下：当闹铃响起时，KEY1 和 KEY_UP可以停止闹铃。  
4、闹铃在 当前时间和闹钟设定时间**相同**时响起，可以通过**KEY1**和**KEY_UP**结束闹铃，如果**不按**结束按键，闹铃则会一直响，直到到达**count**计数上限，才会模拟按下结束按键来停止闹铃。  
5、闹钟响起时，会有闹钟标识字符串打印~  
# 效果图
## v2.0
![在这里插入图片描述](https://img-blog.csdnimg.cn/d3aafb93cc3c485da0699f3a132cbc19.png)

## v1.0
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210413135517870.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210413135456167.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210413135321994.gif#pic_center)

# 参考图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210414090938658.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

