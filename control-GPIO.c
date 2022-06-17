#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>

inline bool gpio_is_valid(int gpionumber);
int gpio_request(unsigned gpionumber, const char *label);
inline int gpio_direction_output(unsigned gpionumber, int value);
inline void gpio_set_value(unsigned gpionumber, int value);

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

static int __init hello_init(void)
{
  printk(KERN_INFO "Hello World\n");

  int initStatus = init_gpio(23);
  if (initStatus)
  {
    printk(KERN_INFO "check GPIO \n");
  }

  return 0;
}

static void __exit hello_exit(void)
{
  printk(KERN_INFO "Bye World\n");
  //gpio_freeを実行
  return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("ishimoto naoya");
