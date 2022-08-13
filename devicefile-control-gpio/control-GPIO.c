#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <linux/cdev.h>    //cdev
#include <linux/uaccess.h> //copy_to_user copy_from_user
#include <linux/types.h>   // uint8_t
#include <linux/mutex.h>
#define BUFSIZE 256

/*あとは、mutexでの点滅速度と強制終了の変更*/

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
static DEFINE_MUTEX(my_mutex);
/*ロード時（insmod)に呼ばれる
使用する23,24ピンGPIOの疎通チェック->使用予約*/
static int init_gpio(int gpionumber)
{
  // gpio_is_valid　GIPOの23,24ピンを確認する
  //  gpio_request をする

  int pinRequest = 0;
  bool PinCheck = false;

  PinCheck = gpio_is_valid(gpionumber);

  if (PinCheck)
  {
    printk(KERN_INFO "gpio can used %d\n", gpionumber);
    // return 1;
  }

  pinRequest = gpio_request(gpionumber, "sysfs"); // 0,-1

  if (pinRequest)
  {
    printk(KERN_INFO "gpio is used by other program %d\n", gpionumber);
  }
  gpio_direction_output(gpionumber, 0);
  return 0;
}

/*GPIOピンにデータを送信する際に呼ばれる一連処理*/
static int startGpio(void)
{
  // 32bit文　0を送信　
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

/*指定されたビット（0or1)を送る関数*/
static int sendBit(int bitValue)
{
  gpio_set_value(23, bitValue);
  gpio_set_value(24, 1);
  ndelay(500);
  gpio_set_value(24, 0);
  ndelay(500);
  return 0;
}

// LED制御の際に呼ばれるバイトを送る関数　ビットシフトで１ビットずつ取り出し
static int sendbyte(int byteValue)
{
  //ビットシフトして、値を取り出す、そしてsendbitを呼ぶ 上位ビットからsendValueに入っていることは確認
  int i, sendValue;
  for (i = 7; i > -1; i--)
  {
    // byteValue = byteValue << i;
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

// -----------copy from devicefiletest.c

static unsigned char kernelbuf[BUFSIZE]; // kernelbuf[0]
static int read_flag = 0;
static u8 blinkt_status; // kernelbuf[0]と同じ

// writeに合わせてblinktを制御する関数を実装->writeの中に
// blinktの制御をする関数
static int write_control_blinkt(u8 blinkt_status)
{
  int blueColor = 100, greenColor = 100, redColor = 100;
  // blinkt start
  startGpio();

  int i, light_flag;
  for (i = 7; i > -1; i--)
  {
    light_flag = blinkt_status >> i & 1;

    // bitシフトとflagでオンオフを判定
    if (light_flag == 1)
    {
      sendbyte(setColorGpio(30));
      sendbyte(blueColor);  // blue
      sendbyte(greenColor); // green
      sendbyte(redColor);   // red
    }
    else
    {
      sendbyte(setColorGpio(0));
      sendbyte(blueColor);  // blue
      sendbyte(greenColor); // green
      sendbyte(redColor);   // red
    }
  }

  // end send LED setting
  endSendGpio();
  return 0;
}

static int chardev_open(struct inode *inode, struct file *filp)
{

  printk(KERN_INFO "Device Open \n");
  // bufferを定義してfilp->private_dataにポインターを渡す
  // char buf[BUFSIZE];
  // uint8 1B  linux/type.h
  // uint8_t buf = 2;
  // filp->private_data = &buf;
  //排他制御回
  int minor = iminor(inode);
  filp->private_data = (void *)minor;

  return 0;
}

static ssize_t chardev_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{

  printk(KERN_INFO "Device Read \n");
  printk(KERN_INFO "Read count =%ld", count);
  size_t countnum = count;
  unsigned long read_num;
  //排他制御回
  printk(KERN_INFO "debuging read\n");
  int minor = (int)filp->private_data;
  printk(KERN_INFO "debuging read1 %d \n", minor);

  if (minor)
  {
    printk(KERN_INFO "debuging read2 if minor >0 %d\n ", minor);
    static u8 returnValue;
    printk(KERN_INFO "debuging read4  %d \n", minor);
    returnValue = kernelbuf[0] >> minor - 1 & 1;

    printk(KERN_INFO "debuging read5  %d \n", minor);
    read_num = copy_to_user(buff, &returnValue, count);
    printk(KERN_INFO "debuging read6  %d \n", minor);
    if (read_num != 0)
    {
      printk(KERN_INFO "read memory exchange fault\n");
      return -EFAULT; // need cheak
    }
    printk(KERN_INFO "debuging read7  %d \n", minor);
  }
  else
  {

    //いらない
    if (read_flag)
    {
      read_flag = 0;
      // return 0;
    }

    if (count > 256)
    {
      count = 256;
    }

    read_num = copy_to_user(buff, kernelbuf, count); // ok:0 Bad:cant copy num
    printk(KERN_INFO "Read: count = %lu	", read_num);

    if (read_num != 0)
    {
      printk(KERN_INFO "read memory exchange fault\n");
      return -EFAULT; // need cheak
    }
    else
    {
      read_flag = 1;
    }
    // mutex_unlock(&my_mutex);
  }

  printk(KERN_INFO "Device readed %s\n", kernelbuf);
  return count;
}

// loff_t *offpって何？　開いているファイルの基準値でいいの？
static ssize_t chardev_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
  if (mutex_lock_interruptible(&my_mutex) != 0)
  {
    printk(KERN_INFO "pushed ctrl C ");
    return -1;
  }

  unsigned long write_num;
  //排他制御回

  printk(KERN_INFO "Device Write\n");
  // kokko
  printk(KERN_INFO "debuging write1   \n");
  int minor = (int)filp->private_data;
  u8 switchPlace;
  u8 switchedValue;
  printk(KERN_INFO "debuging write2  %d \n", minor);

  if (minor)
  {
    printk(KERN_INFO "debuging write2  %d \n", minor);
    // 0,1で帰ってくるから、シフト演算して（1 だとずらして、or）（０だとずらして、反転してand)

    u8 switchValue; // 0 or 1
    write_num = copy_from_user(&switchValue, buff, count);
    printk(KERN_INFO "debuging write3  %d \n", switchValue);

    /*デバイスファイルに対応した　操作したいLEDの場所のビットを動かす*/
    switchPlace = 0x01;
    switchPlace = switchPlace << minor - 1;

    /*デバイスファイルに対応した　操作したいLEDの場所のビットを操作する*/
    if (switchValue)
    {
      printk(KERN_INFO "debuging write6 %d \n", switchValue);
      printk(KERN_INFO, "switch-On-control: %da\n", minor);
      // swich On -> OR
      switchedValue = kernelbuf[0] | switchPlace;
    }
    else
    {
      printk(KERN_INFO "debuging write7 %d \n", switchValue);
      printk(KERN_INFO, "switch-Off-control: %d \n", minor);
      // switch off →XOR（反転、AND）
      switchPlace = ~switchPlace;
      switchedValue = kernelbuf[0] & switchPlace;
    }
    kernelbuf[0] = switchedValue;
  }
  else
  {
    write_num = copy_from_user(kernelbuf, buff, count);
    if (write_num != 0)
    {
      printk(KERN_INFO "write memory exchange fault\n");
      return -EFAULT; // need cheak
    }
    else
    {
      printk(KERN_INFO "Device written %s\n", kernelbuf);
    }
  }

  // echo等の入力をblinktに渡す
  printk(KERN_INFO "kernelbuf[0] = %d", kernelbuf[0]);

  write_control_blinkt(kernelbuf[0]);
  mutex_unlock(&my_mutex);

  printk(KERN_INFO "write: count = %lu", write_num);
  printk(KERN_INFO "Device written %s\n", kernelbuf);
  return count;
}

// release との差分は？
static int chardev_close(struct inode *inode, struct file *filp)
{
  printk(KERN_INFO "Device Close\n");
  return 0;
}

//ここら辺は宣言を変更できる？
static struct file_operations chardev_fops = {
    .owner = THIS_MODULE,
    .open = chardev_open,
    .read = chardev_read,
    .write = chardev_write,
    .release = chardev_close,
};

// device fileにモジュール登録処理　udevに登録すると自動

// dev_t dev0 = MKDEV(238, 0);
//排他制御
dev_t dev[9];
struct cdev cdev[9];
// struct cdev *cdev[9];

// // &cdev[1]

// -----------

static int __init controlMain_init(void)
{
  int initStatus;
  int blueColor = 100, greenColor = 100, redColor = 100;
  // module_param(blueColor, int, S_IRUGO);
  // module_param(greenColor, int, S_IRUGO);
  // module_param(redColor, int, S_IRUGO);
  blinkt_status = 0xff;
  kernelbuf[0] = 0xff;
  int i;

  // &cdev[1]

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
  for (i = 0; i < 8; i++)
  {
    printk(KERN_INFO "color setting \n");
    sendbyte(setColorGpio(30));
    sendbyte(blueColor);  // blue
    sendbyte(greenColor); // green
    sendbyte(redColor);   // red
  }

  // end send LED setting
  endSendGpio();
  //---- copy from devicefiletest.c
  // why does this set 1?

  // device fileにモジュール登録処理　udevに登録すると自動

  //排他制御
  for (i = 0; i < 9; i++)
  {
    dev[i] = MKDEV(238, i);
  }
  register_chrdev_region(dev[0], 9, "blinktdev");
  //排他制御
  int err;
  for (i = 0; i < 9; i++)
  {
    cdev_init(&cdev[i], &chardev_fops);
    cdev[i].owner = THIS_MODULE;
    err = cdev_add(&cdev[i], dev[i], 1);
    if (err)
    {
      printk(KERN_INFO, "cdev_add err %d", i);
    }
  }

  // struct cdev *で宣言した時
  // cdev[0] = cdev_alloc();       // sizeを見てみる
  // cdev[0]->ops = &chardev_fops; // need cheak &chardev_fops ?
  // err = cdev_add(cdev[0], dev[0], 1);
  // printk(KERN_INFO "%d", err);

  // int err = cdev_add(cdev, dev, 1);

  //---

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
    sendbyte(100); // blue
    sendbyte(100); // green
    sendbyte(100); // red
  }
  endSendGpio();
  // gpio_freeを実行
  gpio_free(23);
  gpio_free(24);
  //---- copy from devicefiletest.c

  unregister_chrdev_region(dev[0], 9);
  for (i = 0; i < 9; i++)
  {
    cdev_del(&cdev[i]);
  }

  // -----
  return;
}

module_init(controlMain_init);
module_exit(controlMain_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("ishimoto naoya");
