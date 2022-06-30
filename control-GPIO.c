#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>

/*
inline bool gpio_is_valid(int gpionumber);
int gpio_request(unsigned gpionumber, const char *label);
inline int gpio_direction_output(unsigned gpionumber, int value);
inline void gpio_set_value(unsigned gpionumber, int value);
void gpio_free(unsigned gpionumber);
*/
static int init_gpio(int);
static int startGpio(void);
static int sendBit(int);

static int init_gpio(int gpionumber)
{
  //gpio_is_valid　GIPOの23,24ピンを確認する
  // gpio_request をする

  int pinRequest = 0;
  bool PinCheck = false;

  PinCheck = gpio_is_valid(gpionumber);
  if (PinCheck)
  {
    printk(KERN_INFO "gpio can used %d\n", gpionumber);
    // return 1;
  }

  pinRequest = gpio_request(gpionumber, "sysfs"); //0,-1
  if (pinRequest)
  {
    printk(KERN_INFO "gpio is used by other program %d\n", gpionumber);
  }
  gpio_direction_output(gpionumber, 0);
  return 0;
}

static int startGpio(void)
{
  //32bit文　0を送信　
  int i;
  for (i = 0; i < 32; i++)
  {
    // gpio_set_value(23, 0);
    // gpio_set_value(24, 1);
    // ndelay(500);
    // gpio_set_value(24, 0);
    // ndelay(500);
    sendBit(0);
  }
  return 0;
}

//ビットを送る関数
static int sendBit(int bitValue)
{
  gpio_set_value(23, bitValue);
  gpio_set_value(24, 1);
  ndelay(500);
  gpio_set_value(24, 0);
  ndelay(500);
  return 0;
}

//バイトを送る関数　ビットシフトで１ビットずつ取り出し
static int sendbyte(int byteValue)
{
  //ビットシフトして、値を取り出す、そしてsendbitを呼ぶ 上位ビットからsendValueに入っていることは確認
  int i, sendValue;
  for (i = 7; i > -1; i--)
  {
    //byteValue = byteValue << i;
    sendValue = byteValue >> i & 1;
    sendBit(sendValue);
  }
  return 0;
}

static int setColorGpio(int brightness)
{
  int result;
  result = brightness | 0xe0;
  return result;
}

static int endSendGpio(void)
{
  int i;
  for (i = 0; i < 32; i++)
  {
    sendBit(1);
  }
  return 0;
}

static int __init controlMain_init(void)
{
  int initStatus;
  int buleColor = 100, greenColor = 100, RedColor = 100；
  module_param（buleColor, int,S_IRUGO);
  module_param（greenColor, int,S_IRUGO);
  module_param（RedColor, int,S_IRUGO);

  printk(KERN_INFO "Hello World\n");

  initStatus = init_gpio(23);
  if (initStatus)
  {
    printk(KERN_INFO "errer check GPIO \n");
  }
  initStatus = init_gpio(24);
  if (initStatus)
  {
    printk(KERN_INFO "errer check GPIO \n");
  }

  startGpio();
  // sendbyte(setColorGpio(10));
  int i;
  for (i = 0; i < 8; i++)
  {
    printk(KERN_INFO "color setting \n");
    sendbyte(setColorGpio(30));
    sendbyte(buleColor);  //bule
    sendbyte(greenColor); //green
    sendbyte(RedColor);   //red
  }

  //end send LED setting
  endSendGpio();
  return 0;
}

static void __exit controlMain_exit(void)
{
  printk(KERN_INFO "Bye World\n");
  startGpio();
  int i;
  for (i = 0; i < 8; i++)
  {
    printk(KERN_INFO "color setting \n");
    sendbyte(setColorGpio(0));
    sendbyte(100); //bule
    sendbyte(100); //green
    sendbyte(100); //red
  }
  endSendGpio();
  //gpio_freeを実行
  gpio_free(23);
  gpio_free(24);
  return;
}

module_init(controlMain_init);
module_exit(controlMain_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("ishimoto naoya");
