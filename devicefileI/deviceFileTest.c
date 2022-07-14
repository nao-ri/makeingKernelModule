#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>    //cdev
#include <linux/uaccess.h> //copy_to_user copy_from_user

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

static int chardev_open(struct inode *inode, struct file *filp)
{
  printk(KERN_INFO "Device Open \n");

  return 0;
}
static ssize_t chardev_read(struct file *filp, char _user *buff, size_t count, loff_t *ppos)
{
  printk(KERN_INFO "Device Read \n");
  if (copy_to_user(ppos, filp->private_data, count) != 0)
  {
    printk(KERN_INFO "read memory exchange fault\n");
    return 0; //need cheak
  }
  return count;
}
//loff_t *f_posって何？　開いているファイルの基準値でいいの？
static ssize_t chardev_write(struct file *filp, const char _user *buff, size_t count, loff_t *ppos)
{
  printk(KERN_INFO "Device Write\n");
  //kokko
  if (copy_from_user(filp->private_data, ppos, count) != 0)
  {
    printk(KERN_INFO "write memory exchange fault\n");
    return 0; //need cheak
  }
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
    .close = chardev_close,
};

struct cdev *cdev;

static int __init hello_init(void)
{
  printk(KERN_INFO "init_module\n");
  //device fileにモジュール登録処理　udevに登録すると自動
  dev_t dev = MKDEV(238, 0);
  // why does this set 1?
  registaer_chrdev_region(dev, 1 ,"hello";
  cdev = cdev_alloc();
  cdev->ops=chardev_fops; //need cheak &chardev_fops ?
  err=cdev_add(cdev,dev,1);

  return 0;
}

static void __exit hello_exit(void)
{
  printk(KERN_INFO "rm this module\n");
  //デバイスファイル登録用を削除する
  cdev_del(cdev);

  return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("ishimoto naoya");
