// u8g2头文件
#include <Arduino.h>
#include <U8g2lib.h>
// AHTX0 头文件
#include <Adafruit_AHTX0.h>
// u8g2 预编译
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// 创建 AHTX0 对象
Adafruit_AHTX0 aht;

// 初始化U8G2_SSD1306_128X64_NONAME_F_HW_I2C对象
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 21);

// 初始化U8G2_SSD1306_128X64_NONAME_F_HW_I2C对象的参数说明：
// U8G2_R0：U8G2的库类型
// reset：复位引脚，此处设置为U8X8_PIN_NONE，表示不使用复位引脚
// clock：时钟引脚，此处设置为22
// data：数据引脚，此处设置为21

// 0.96oled显示温湿度
void oledDisplay(float temp,float humi);
// aht10传感器获取数据 [指针传参]
// void aht10(float *t,float *h);
// aht10传感器获取数据 [引用传参]
void aht10(float &t,float &h);

void setup() {
  
  u8g2.begin();    // 初始化u8g2库
  u8g2.enableUTF8Print();		// 启用UTF8打印支持，使Arduino的print()函数支持UTF8编码
  aht.begin();    // 初始化AHT对象
  // 串口输出 -- 是否检测到AHT10/AHT20
  // Serial.begin(115200);
  // Serial.println("Adafruit AHT10/AHT20 demo!");

  // if (! aht.begin()) {
  //   Serial.println("Could not find AHT? Check wiring");
  //   while (1) delay(10);
  // }
  // Serial.println("AHT10 or AHT20 found");
}

void loop() {
  float temp,humi;    // 定义温度，湿度变量
  // 测温湿度 
  // aht10(&temp,&humi); // 指针传参
  aht10(temp,humi);    // 引用传参
  oledDisplay(temp,humi);    // oled显示温湿度
  delay(1000);    // 延时1s
}


void oledDisplay(float temp,float humi) {
  // u8g2.setFont(u8g2_font_unifont_t_chinese2);  // use chinese2 for all the glyphs of "你好世界"
  u8g2.setFont(u8g2_font_wqy15_t_gb2312); // 设置字体为 wqy15_t_gb2312
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  // 显示温度
  u8g2.setCursor(20, 15);
  u8g2.print("温度: ");  
  u8g2.print(temp);
  u8g2.print("℃");
  // 显示湿度
  u8g2.setCursor(20, 40);
  u8g2.print("湿度: "); 
  u8g2.print(humi);
  u8g2.print("%");

  /*判读温度在18到25度之间，湿度在百分之40到70之间为适宜温度，以笑脸作为提示*/
  u8g2.setCursor(44,60);
  if((temp >= 18 && temp <=25) && (humi >= 40 && humi <=70))
  {
    u8g2.println("(^_^)");
  }
  else /*判读温度或者湿度不适宜，以哭脸作为提示*/
  {
    u8g2.println("(T_T)");
  }

  u8g2.sendBuffer();
}

// void aht10(float *t,float *h){
//     sensors_event_t humidity, temp;
//     aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
//     *t = temp.temperature;
//     *h= humidity.relative_humidity;
// }

void aht10(float &t,float &h){
    sensors_event_t humidity, temp;   // 定义传感器事件对象
    aht.getEvent(&humidity, &temp);  // 获取最新的温度和湿度数据
    t = temp.temperature;    // 获取温度值
    h= humidity.relative_humidity;    // 获取湿度值
}   