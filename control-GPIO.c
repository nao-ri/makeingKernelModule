#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <delay.h>

/*
inline bool gpio_is_valid(int gpionumber);
int gpio_request(unsigned gpionumber, const char *label);
inline int gpio_direction_output(unsigned gpionumber, int value);
inline void gpio_set_value(unsigned gpionumber, int value);
void gpio_free(unsigned gpionumber);
*/

static int init_gpio(int gpionumber)
{
  //gpio_is_valid　GIPOの23,24ピンを確認する
  // gpio_request をする

  int pinRequest = 0;
  bool PinCheck = false;

  PinCheck = gpio_is_valid(gpionumber);
  if (PinCheck23)
  {
    printk(KERN_INFO "gpio cant used %d\n", gpionumber);
    return 1;
  }

  pinRequest = gpio_request(gpionumber, "sysfs");
  if (pinRequest)
  {
    printk(KERN_INFO "gpio is used by other program %d\n", gpionumber);
  }

  return 0:
}

static int startGpio(void)
{
  //32bit文　0を送信　
  for (int i = 0; i < 32; i++)
  {
    gpio_set_value(23, 0);
    gpio_set_value(24, 1);
    ndelay(500);
    gpio_set_value(24, 0);
    ndelay(500);
  }
  return 0;
}

static int sendBit(int bitValue)
{
  gpio_set_value(23, bitValue);
  gpio_set_value(24, 1);
  ndelay(500);
  gpio_set_value(24, 0);
  ndelay(500);
}

static int sendbyte(int byteValue)
{
  //ビットシフトして、値を取り出す、そしてsendbitを呼ぶ
  int sendValue for (int i = 0; i < 8; i++)
  {
    byteValue = byteValue << i;
    sendValue = byteValue & 0x01;
    sendBit(sendValue);
  }

  byteValue >> i
}

static int setColorGpio(int brightness)
{
  int result;
  result = brightness | 0xe0;
  return result;
}

static int __init hello_init(void)
{
  printk(KERN_INFO "Hello World\n");

  int initStatus = init_gpio(23);
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
  sendbyte(setColorGpio(2));
  for (int i = 0; i < 8; i++)
  {
    sendbyte();
  }
  //end send LED setting
  for (int i = 0; i < 32; i++)
  {
    sendbyte(1);
  }
  return 0;
}

static void __exit hello_exit(void)
{
  printk(KERN_INFO "Bye World\n");
  //gpio_freeを実行
  gpio_free(23);
  gpio_free(24);
  return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("ishimoto naoya");
