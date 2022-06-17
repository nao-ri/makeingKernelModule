#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>

inline bool gpio_is_valid(int gpionumber);
int gpio_request(unsigned gpionumber, const char *label);

static int init_gpio(void)
{
  //gpio_is_valid　GIPOの23,24ピンを確認する
  // gpio_request をする

  int gpionumber23 = 23;
  int gpionumber24 = 24;
  bool PinCheck23 = false;
  bool PinCheck24 = false;

  PinCheck23 = gpio_is_valid(gpionumber);
  if (PinCheck23)
  {
    printk(KERN_INFO "gpio is used by other program23\n");
    return 1;
  }
  gpio_request(gpionumber23, "sysfs");

  PinCheck24 = gpio_is_valid(gpionumber24);
  if (PinCheck24)
  {
    printk(KERN_INFO "gpio is used by other program24\n");
    return 1;
  }
  gpio_request(gpionumber24, "sysfs");
  return 0:
}

static int __init hello_init(void)
{
  printk(KERN_INFO "Hello World\n");

  int initStatus = init_gpio();
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
