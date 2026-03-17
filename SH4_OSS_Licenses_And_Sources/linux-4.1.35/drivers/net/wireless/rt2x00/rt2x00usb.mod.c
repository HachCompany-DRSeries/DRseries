#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7f29ae64, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xc8af564b, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0xe8bff83e, __VMLINUX_SYMBOL_STR(skb_pad) },
	{ 0xe0dcf0e4, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0xddac173c, __VMLINUX_SYMBOL_STR(rt2x00lib_resume) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0x62b72b0d, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x8af452a7, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x24deaaf5, __VMLINUX_SYMBOL_STR(rt2x00lib_dmastart) },
	{ 0xc2675f12, __VMLINUX_SYMBOL_STR(rt2x00queue_flush_queue) },
	{ 0x3ef7f5a9, __VMLINUX_SYMBOL_STR(ieee80211_alloc_hw_nm) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xa40563b1, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x341cf9ad, __VMLINUX_SYMBOL_STR(rt2x00queue_for_each_entry) },
	{ 0xe16b893b, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x7eb59bb3, __VMLINUX_SYMBOL_STR(rt2x00queue_start_queue) },
	{ 0x1e5cb5a6, __VMLINUX_SYMBOL_STR(rt2x00lib_remove_dev) },
	{ 0x33a109b0, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0xd7b8e01d, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xdf512840, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0xe9661ce1, __VMLINUX_SYMBOL_STR(rt2x00queue_get_entry) },
	{ 0x86e4c014, __VMLINUX_SYMBOL_STR(usb_reset_device) },
	{ 0x3bd1b1f6, __VMLINUX_SYMBOL_STR(msecs_to_jiffies) },
	{ 0x4f38bdf, __VMLINUX_SYMBOL_STR(rt2x00lib_rxdone) },
	{ 0x891b3d3c, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x5914a2cf, __VMLINUX_SYMBOL_STR(hrtimer_init) },
	{ 0xbf5937ef, __VMLINUX_SYMBOL_STR(ieee80211_free_hw) },
	{ 0x4ec46d25, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0x410fca83, __VMLINUX_SYMBOL_STR(rt2x00lib_txdone_noinfo) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0x6d37cd9e, __VMLINUX_SYMBOL_STR(rt2x00lib_dmadone) },
	{ 0x91b2d94a, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0xc11c5d0e, __VMLINUX_SYMBOL_STR(rt2x00lib_probe_dev) },
	{ 0xc6b888ac, __VMLINUX_SYMBOL_STR(rt2x00queue_stop_queue) },
	{ 0x96bbd8bc, __VMLINUX_SYMBOL_STR(rt2x00lib_suspend) },
	{ 0x2d9b0ff0, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=rt2x00lib";


MODULE_INFO(srcversion, "3F336708588FABE8A5041C1");
