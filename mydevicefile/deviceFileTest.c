#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>    //cdev
#include <linux/uaccess.h> //copy_to_user copy_from_user
#include <linux/types.h>   // uint8_t
#define BUFSIZE 256
//fileoperation 構造体を定義
/*
struct file_oprations{
  struct module *owner;
  int (*open)(struct inode *,struct file *);
  ssize_t (*read)(struct file *, char _user *, size_t, loff_t *);
  ssize_t (*write)(struct file *,const char _user *,size_t,loff_t *);
  int (*release)(struct inode *,stuct file *);
}
*/

// what is filp? A,devicefileをopenしたときにできる構造体へのPointer

//static uint8_t kernelbuf;
static unsigned char kernelbuf[BUFSIZE];
static int read_flag = 0;
static u8 blinkt_status;

static int chardev_open(struct inode *inode, struct file *filp)
{
  printk(KERN_INFO "Device Open \n");
  // bufferを定義してfilp->private_dataにポインターを渡す
  // char buf[BUFSIZE];
  // uint8 1B  linux/type.h
  // uint8_t buf = 2;
  // filp->private_data = &buf;
  return 0;
}

static ssize_t chardev_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
  printk(KERN_INFO "Device Read \n");
  printk(KERN_INFO "Read count =%ld", count);
  size_t countnum = count;

  if (read_flag)
  {
    read_flag = 0;
    return 0;
  }
  if (count > 256)
  {
    count = 256;
  }

  unsigned long read_num = copy_to_user(buff, kernelbuf, count); //ok:0 Bad:cant copy num
  printk(KERN_INFO "Read: count = %lu	", read_num);

  if (read_num != 0)
  {
    printk(KERN_INFO "read memory exchange fault\n");
    return -EFAULT; //need cheak
  }
  else
  {
    read_flag = 1;
  }

  printk(KERN_INFO "Device readed %s\n", kernelbuf);
  return count;
}
//loff_t *offpって何？　開いているファイルの基準値でいいの？
static ssize_t chardev_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
  printk(KERN_INFO "Device Write\n");
  //kokko
  unsigned long write_num = copy_from_user(kernelbuf, buff, count);
  if (write_num != 0)
  {
    printk(KERN_INFO "write memory exchange fault\n");
    return -EFAULT; //need cheak
  }
  else
  {
    printk(KERN_INFO "Device written %s\n", kernelbuf);
  }
  printk(KERN_INFO "write: count = %lu", write_num);
  printk(KERN_INFO "Device written %s\n", kernelbuf);
  return count;
}

//release との差分は？
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

//device fileにモジュール登録処理　udevに登録すると自動
dev_t dev = MKDEV(238, 0);
struct cdev *cdev;

static int __init device_init(void)
{
  printk(KERN_INFO "init_module\n");

  // why does this set 1?
  register_chrdev_region(dev, 1, "hello");
  cdev = cdev_alloc();       //sizeを見てみる
  cdev->ops = &chardev_fops; //need cheak &chardev_fops ?
  int err = cdev_add(cdev, dev, 1);

  return 0;
}

static void __exit device_exit(void)
{
  printk(KERN_INFO "rm this module\n");
  //デバイスファイル登録用を削除する
  unregister_chrdev_region(dev, 1);
  cdev_del(cdev);
  return;
}

module_init(device_init);
module_exit(device_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("ishimoto naoya");
