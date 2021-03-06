#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xc15e5c06, "module_layout" },
	{ 0xccd6119e, "cdev_alloc" },
	{ 0xaed8f042, "cdev_del" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xccafb328, "gpio_to_desc" },
	{ 0x56470118, "__warn_printk" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xdcb764ad, "memset" },
	{ 0xa6c9ad80, "gpiod_direction_output_raw" },
	{ 0x7643628b, "cdev_add" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x92997ed8, "_printk" },
	{ 0xfe990052, "gpio_free" },
	{ 0x6198b2d8, "gpiod_set_raw_value" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "CAFA1E2B0D4FC68423DB500");
