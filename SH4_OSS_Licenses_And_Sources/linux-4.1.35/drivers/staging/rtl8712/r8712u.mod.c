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

MODULE_INFO(staging, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7f29ae64, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0x246a845e, __VMLINUX_SYMBOL_STR(netdev_info) },
	{ 0xc8af564b, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x2b688622, __VMLINUX_SYMBOL_STR(complete_and_exit) },
	{ 0x13c0a296, __VMLINUX_SYMBOL_STR(__udivsi3_i4i) },
	{ 0x51eafc8e, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0xcca27eeb, __VMLINUX_SYMBOL_STR(del_timer) },
	{ 0xd0d8621b, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0xfe9ad9fa, __VMLINUX_SYMBOL_STR(arch_local_save_flags) },
	{ 0x536124fa, __VMLINUX_SYMBOL_STR(find_vpid) },
	{ 0x47939e0d, __VMLINUX_SYMBOL_STR(__tasklet_hi_schedule) },
	{ 0x804c105f, __VMLINUX_SYMBOL_STR(netif_carrier_on) },
	{ 0xec901f33, __VMLINUX_SYMBOL_STR(skb_clone) },
	{ 0x11f7ed4c, __VMLINUX_SYMBOL_STR(hex_to_bin) },
	{ 0xfa082128, __VMLINUX_SYMBOL_STR(down_interruptible) },
	{ 0x8692223e, __VMLINUX_SYMBOL_STR(netif_carrier_off) },
	{ 0xe0dcf0e4, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0xe2fae716, __VMLINUX_SYMBOL_STR(kmemdup) },
	{ 0x4a5fa29b, __VMLINUX_SYMBOL_STR(__dev_kfree_skb_any) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0xfa2bcf10, __VMLINUX_SYMBOL_STR(init_timer_key) },
	{ 0x62b72b0d, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xf5d9504e, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x5a025f7b, __VMLINUX_SYMBOL_STR(arch_local_irq_restore) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xb71871f6, __VMLINUX_SYMBOL_STR(param_ops_string) },
	{ 0x1f686882, __VMLINUX_SYMBOL_STR(__netdev_alloc_skb) },
	{ 0xc73a85d2, __VMLINUX_SYMBOL_STR(netif_rx) },
	{ 0xbd73a2b7, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0x63b87fc5, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0x5baaba0, __VMLINUX_SYMBOL_STR(wait_for_completion) },
	{ 0x62a79a6c, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0xb4979444, __VMLINUX_SYMBOL_STR(skb_queue_purge) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x45e6d4e3, __VMLINUX_SYMBOL_STR(netif_tx_wake_queue) },
	{ 0x8af452a7, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x5fe2cb13, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0xdc798d37, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0xd3e4b7d7, __VMLINUX_SYMBOL_STR(free_netdev) },
	{ 0xaafdc258, __VMLINUX_SYMBOL_STR(strcasecmp) },
	{ 0xe82a9752, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0x71ae0e7e, __VMLINUX_SYMBOL_STR(wireless_send_event) },
	{ 0xa40563b1, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x1ddf17ab, __VMLINUX_SYMBOL_STR(skb_push) },
	{ 0xe16b893b, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xf6770391, __VMLINUX_SYMBOL_STR(mac_pton) },
	{ 0x9545af6d, __VMLINUX_SYMBOL_STR(tasklet_init) },
	{ 0xc8fd727e, __VMLINUX_SYMBOL_STR(mod_timer) },
	{ 0x37ffb36e, __VMLINUX_SYMBOL_STR(kill_pid) },
	{ 0x43b0c9c3, __VMLINUX_SYMBOL_STR(preempt_schedule) },
	{ 0xbd92e7a9, __VMLINUX_SYMBOL_STR(skb_pull) },
	{ 0x7f9943ab, __VMLINUX_SYMBOL_STR(request_firmware_nowait) },
	{ 0x29b67263, __VMLINUX_SYMBOL_STR(skb_queue_tail) },
	{ 0xa6d58057, __VMLINUX_SYMBOL_STR(netif_device_attach) },
	{ 0x872f3f04, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x33a109b0, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0xd7b8e01d, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x1e672be1, __VMLINUX_SYMBOL_STR(netif_device_detach) },
	{ 0xdf512840, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0x86e4c014, __VMLINUX_SYMBOL_STR(usb_reset_device) },
	{ 0x3bd1b1f6, __VMLINUX_SYMBOL_STR(msecs_to_jiffies) },
	{ 0x891b3d3c, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0xf02a5650, __VMLINUX_SYMBOL_STR(eth_type_trans) },
	{ 0xc873f792, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xc406042, __VMLINUX_SYMBOL_STR(netdev_err) },
	{ 0x4e9019f9, __VMLINUX_SYMBOL_STR(netdev_warn) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x801678, __VMLINUX_SYMBOL_STR(flush_scheduled_work) },
	{ 0x6df1aaf1, __VMLINUX_SYMBOL_STR(kernel_sigaction) },
	{ 0xdf8f4a7c, __VMLINUX_SYMBOL_STR(dev_alloc_name) },
	{ 0x364b3fff, __VMLINUX_SYMBOL_STR(up) },
	{ 0x771ace5, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0x4ca69a6e, __VMLINUX_SYMBOL_STR(skb_dequeue) },
	{ 0x27a88aa1, __VMLINUX_SYMBOL_STR(unregister_netdev) },
	{ 0xe2e8065e, __VMLINUX_SYMBOL_STR(memdup_user) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0x60f71cfa, __VMLINUX_SYMBOL_STR(complete) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x23b6c68d, __VMLINUX_SYMBOL_STR(consume_skb) },
	{ 0xd0e0bccc, __VMLINUX_SYMBOL_STR(skb_put) },
	{ 0xf9b06f20, __VMLINUX_SYMBOL_STR(skb_copy_bits) },
	{ 0x91b2d94a, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x6c6d8e5a, __VMLINUX_SYMBOL_STR(release_firmware) },
	{ 0x2d9b0ff0, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0x22ced7f1, __VMLINUX_SYMBOL_STR(alloc_etherdev_mqs) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v0BDAp8171d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8173d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8712d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8713d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDApC512d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v07B8p8188d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0B05p1786d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0B05p1791d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v050Dp945Ad*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v050Dp11F1d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v07AAp0047d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2001p3306d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v07D1p3306d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v7392p7611d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1740p9603d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0E66p0016d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v06F8pE034d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v06F8pE032d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0789p0167d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2019pAB28d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2019pED16d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p0057d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p0045d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p0059d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p004Bd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p005Bd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p005Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p0063d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v177Fp0154d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp5077d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1690p0752d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v20F4p646Bd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v083ApC512d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v25D4p4CA1d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v25D4p4CABd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8172d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8192d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0EB0p9061d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3323d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3311d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3342d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3333d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3334d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3335d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3336d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3309d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v050Dp815Fd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v07D1p3302d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v07D1p3300d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v07D1p3303d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v7392p7612d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v1740p9605d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v06F8pE031d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0E66p0015d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3306d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2019pED18d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2019p4901d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p0058d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p0049d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p004Cd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p006Cd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0DF6p0064d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v14B2p3300d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v14B2p3301d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v14B2p3302d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04F2pAFF2d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04F2pAFF5d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04F2pAFF6d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3339d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3340d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3341d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3310d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13D3p3325d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8174d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v050Dp845Ad*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v07AAp0051d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v7392p7622d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0409p02B6d*dc*dsc*dp*ic*isc*ip*in*");
