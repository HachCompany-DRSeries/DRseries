#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7f29ae64, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x89230671, __VMLINUX_SYMBOL_STR(ieee80211_rts_duration) },
	{ 0x98bb5a22, __VMLINUX_SYMBOL_STR(eeprom_93cx6_read) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0x80b1fe64, __VMLINUX_SYMBOL_STR(ieee80211_rx_irqsafe) },
	{ 0xc8af564b, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0xfe9ad9fa, __VMLINUX_SYMBOL_STR(arch_local_save_flags) },
	{ 0x9c64fbd, __VMLINUX_SYMBOL_STR(ieee80211_frequency_to_channel) },
	{ 0x79aa04a2, __VMLINUX_SYMBOL_STR(get_random_bytes) },
	{ 0xb54533f7, __VMLINUX_SYMBOL_STR(usecs_to_jiffies) },
	{ 0x6b06fdce, __VMLINUX_SYMBOL_STR(delayed_work_timer_fn) },
	{ 0xeaad455f, __VMLINUX_SYMBOL_STR(ieee80211_beacon_get_tim) },
	{ 0xef05ba04, __VMLINUX_SYMBOL_STR(ieee80211_unregister_hw) },
	{ 0x4a5fa29b, __VMLINUX_SYMBOL_STR(__dev_kfree_skb_any) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0xfa2bcf10, __VMLINUX_SYMBOL_STR(init_timer_key) },
	{ 0x4a8907de, __VMLINUX_SYMBOL_STR(cancel_delayed_work_sync) },
	{ 0x62b72b0d, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xbe31ed5f, __VMLINUX_SYMBOL_STR(wiphy_rfkill_start_polling) },
	{ 0x5a025f7b, __VMLINUX_SYMBOL_STR(arch_local_irq_restore) },
	{ 0x9601417c, __VMLINUX_SYMBOL_STR(skb_unlink) },
	{ 0x4a98a5c6, __VMLINUX_SYMBOL_STR(skb_trim) },
	{ 0xe1d84082, __VMLINUX_SYMBOL_STR(usb_unanchor_urb) },
	{ 0x1f686882, __VMLINUX_SYMBOL_STR(__netdev_alloc_skb) },
	{ 0x63b87fc5, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x8af452a7, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x5fe2cb13, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0x3ef7f5a9, __VMLINUX_SYMBOL_STR(ieee80211_alloc_hw_nm) },
	{ 0xdc798d37, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x27cb133b, __VMLINUX_SYMBOL_STR(eeprom_93cx6_multiread) },
	{ 0xa40563b1, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x1ddf17ab, __VMLINUX_SYMBOL_STR(skb_push) },
	{ 0xe16b893b, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xa99368a2, __VMLINUX_SYMBOL_STR(__dev_kfree_skb_irq) },
	{ 0x43b0c9c3, __VMLINUX_SYMBOL_STR(preempt_schedule) },
	{ 0xbd92e7a9, __VMLINUX_SYMBOL_STR(skb_pull) },
	{ 0xc7f7a7e, __VMLINUX_SYMBOL_STR(wiphy_rfkill_stop_polling) },
	{ 0x91ae4c8d, __VMLINUX_SYMBOL_STR(ieee80211_queue_delayed_work) },
	{ 0x29b67263, __VMLINUX_SYMBOL_STR(skb_queue_tail) },
	{ 0x872f3f04, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x33a109b0, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0xd7b8e01d, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xdf512840, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0x3da6f9d3, __VMLINUX_SYMBOL_STR(usb_kill_anchored_urbs) },
	{ 0x9963a089, __VMLINUX_SYMBOL_STR(queue_delayed_work_on) },
	{ 0x86e4c014, __VMLINUX_SYMBOL_STR(usb_reset_device) },
	{ 0xeaea8e4f, __VMLINUX_SYMBOL_STR(wiphy_rfkill_set_hw_state) },
	{ 0x891b3d3c, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0xdc8ef03a, __VMLINUX_SYMBOL_STR(ieee80211_tx_status_irqsafe) },
	{ 0x9f9f4617, __VMLINUX_SYMBOL_STR(kfree_skb) },
	{ 0xce4edd77, __VMLINUX_SYMBOL_STR(ieee80211_generic_frame_duration) },
	{ 0xe24e8799, __VMLINUX_SYMBOL_STR(ieee80211_ctstoself_duration) },
	{ 0xe5fda750, __VMLINUX_SYMBOL_STR(__sdivsi3_i4i) },
	{ 0xd1d895ee, __VMLINUX_SYMBOL_STR(ieee80211_register_hw) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x771ace5, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xbf5937ef, __VMLINUX_SYMBOL_STR(ieee80211_free_hw) },
	{ 0x4ca69a6e, __VMLINUX_SYMBOL_STR(skb_dequeue) },
	{ 0x4ec46d25, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xd0e0bccc, __VMLINUX_SYMBOL_STR(skb_put) },
	{ 0x91b2d94a, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x37f645ca, __VMLINUX_SYMBOL_STR(usb_anchor_urb) },
	{ 0x2d9b0ff0, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0xb36c34fb, __VMLINUX_SYMBOL_STR(ieee80211_queue_stopped) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v0B05p171Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v050Dp705Ed*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8187d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8189d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8197d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8198d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0769p11F2d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0789p010Cd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0846p6100d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0846p6A00d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0846p4260d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v03F0pCA02d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p000Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p0028d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p0029d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v114Bp0150d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1371p9401d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D1pABE6d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v18E8p6232d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1B75p8187d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1737p0073d*dc*dsc*dp*ic*isc*ip*in*");
