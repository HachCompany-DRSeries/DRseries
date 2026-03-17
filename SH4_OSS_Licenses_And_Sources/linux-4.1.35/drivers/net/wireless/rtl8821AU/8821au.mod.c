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
	{ 0x771ace5, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xb9a17b9a, __VMLINUX_SYMBOL_STR(__napi_schedule) },
	{ 0x85df9b6c, __VMLINUX_SYMBOL_STR(strsep) },
	{ 0xf02a5650, __VMLINUX_SYMBOL_STR(eth_type_trans) },
	{ 0x36def38f, __VMLINUX_SYMBOL_STR(napi_gro_receive) },
	{ 0x3140551e, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xd2da1048, __VMLINUX_SYMBOL_STR(register_netdevice_notifier) },
	{ 0x9b99eb50, __VMLINUX_SYMBOL_STR(__movmem_i4_even) },
	{ 0xd0d8621b, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x29b67263, __VMLINUX_SYMBOL_STR(skb_queue_tail) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0x20000329, __VMLINUX_SYMBOL_STR(simple_strtoul) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xed43d0e4, __VMLINUX_SYMBOL_STR(proc_mkdir_data) },
	{ 0x76b57056, __VMLINUX_SYMBOL_STR(dev_get_by_name) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xd61a87ef, __VMLINUX_SYMBOL_STR(napi_disable) },
	{ 0x13c0a296, __VMLINUX_SYMBOL_STR(__udivsi3_i4i) },
	{ 0xd3e4b7d7, __VMLINUX_SYMBOL_STR(free_netdev) },
	{ 0x9f634115, __VMLINUX_SYMBOL_STR(unregister_netdevice_queue) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0xd9e3e01, __VMLINUX_SYMBOL_STR(__pskb_copy_fclone) },
	{ 0x63b87fc5, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0x33a109b0, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0x760a0f4f, __VMLINUX_SYMBOL_STR(yield) },
	{ 0x62b72b0d, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x3bd1b1f6, __VMLINUX_SYMBOL_STR(msecs_to_jiffies) },
	{ 0x22ced7f1, __VMLINUX_SYMBOL_STR(alloc_etherdev_mqs) },
	{ 0x91b2d94a, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x8d522714, __VMLINUX_SYMBOL_STR(__rcu_read_lock) },
	{ 0x27a88aa1, __VMLINUX_SYMBOL_STR(unregister_netdev) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x991b0db8, __VMLINUX_SYMBOL_STR(flush_signals) },
	{ 0x9d0d6206, __VMLINUX_SYMBOL_STR(unregister_netdevice_notifier) },
	{ 0x2469810f, __VMLINUX_SYMBOL_STR(__rcu_read_unlock) },
	{ 0xdf8f4a7c, __VMLINUX_SYMBOL_STR(dev_alloc_name) },
	{ 0x3c3fce39, __VMLINUX_SYMBOL_STR(__local_bh_enable_ip) },
	{ 0xe0dcf0e4, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0x804c105f, __VMLINUX_SYMBOL_STR(netif_carrier_on) },
	{ 0x4a5fa29b, __VMLINUX_SYMBOL_STR(__dev_kfree_skb_any) },
	{ 0xcc4ec0d8, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x536124fa, __VMLINUX_SYMBOL_STR(find_vpid) },
	{ 0xbd73a2b7, __VMLINUX_SYMBOL_STR(__copy_user) },
	{ 0xa6505676, __VMLINUX_SYMBOL_STR(usb_alloc_coherent) },
	{ 0x1ddf17ab, __VMLINUX_SYMBOL_STR(skb_push) },
	{ 0x80d817ab, __VMLINUX_SYMBOL_STR(wait_for_completion_timeout) },
	{ 0x43b0c9c3, __VMLINUX_SYMBOL_STR(preempt_schedule) },
	{ 0x6df1aaf1, __VMLINUX_SYMBOL_STR(kernel_sigaction) },
	{ 0x37ffb36e, __VMLINUX_SYMBOL_STR(kill_pid) },
	{ 0x4a98a5c6, __VMLINUX_SYMBOL_STR(skb_trim) },
	{ 0xd5e8444a, __VMLINUX_SYMBOL_STR(__div64_32) },
	{ 0x922912eb, __VMLINUX_SYMBOL_STR(netif_napi_add) },
	{ 0x364b3fff, __VMLINUX_SYMBOL_STR(up) },
	{ 0xe82a9752, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0xe3ada29f, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xfe9ad9fa, __VMLINUX_SYMBOL_STR(arch_local_save_flags) },
	{ 0xdc798d37, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0xd62c833f, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0x8cdff91e, __VMLINUX_SYMBOL_STR(napi_complete_done) },
	{ 0xad9df43a, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x2700b38a, __VMLINUX_SYMBOL_STR(mutex_lock_interruptible) },
	{ 0x8692223e, __VMLINUX_SYMBOL_STR(netif_carrier_off) },
	{ 0xdf512840, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0x149c7752, __VMLINUX_SYMBOL_STR(param_ops_uint) },
	{ 0xf5d9504e, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0xd9305ad9, __VMLINUX_SYMBOL_STR(__vfs_read) },
	{ 0xc873f792, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xcfd04fe, __VMLINUX_SYMBOL_STR(register_netdevice) },
	{ 0xee00b70, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x400b1f0a, __VMLINUX_SYMBOL_STR(PDE_DATA) },
	{ 0x8d4d42f0, __VMLINUX_SYMBOL_STR(skb_copy) },
	{ 0x62a79a6c, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0xc8fd727e, __VMLINUX_SYMBOL_STR(mod_timer) },
	{ 0xf9b06f20, __VMLINUX_SYMBOL_STR(skb_copy_bits) },
	{ 0xc73a85d2, __VMLINUX_SYMBOL_STR(netif_rx) },
	{ 0x82072614, __VMLINUX_SYMBOL_STR(tasklet_kill) },
	{ 0x5fe2cb13, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0x4ca69a6e, __VMLINUX_SYMBOL_STR(skb_dequeue) },
	{ 0x45e6d4e3, __VMLINUX_SYMBOL_STR(netif_tx_wake_queue) },
	{ 0x2b688622, __VMLINUX_SYMBOL_STR(complete_and_exit) },
	{ 0xa6d58057, __VMLINUX_SYMBOL_STR(netif_device_attach) },
	{ 0xa735db59, __VMLINUX_SYMBOL_STR(prandom_u32) },
	{ 0x47939e0d, __VMLINUX_SYMBOL_STR(__tasklet_hi_schedule) },
	{ 0x71ae0e7e, __VMLINUX_SYMBOL_STR(wireless_send_event) },
	{ 0x9a1dfd65, __VMLINUX_SYMBOL_STR(strpbrk) },
	{ 0x1cc22407, __VMLINUX_SYMBOL_STR(init_net) },
	{ 0x2aa0e4fc, __VMLINUX_SYMBOL_STR(strncasecmp) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xce8b3243, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x60f71cfa, __VMLINUX_SYMBOL_STR(complete) },
	{ 0x1f54e93d, __VMLINUX_SYMBOL_STR(param_array_ops) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0x1e6d26a8, __VMLINUX_SYMBOL_STR(strstr) },
	{ 0xe5fda750, __VMLINUX_SYMBOL_STR(__sdivsi3_i4i) },
	{ 0x506cbdc8, __VMLINUX_SYMBOL_STR(seq_open) },
	{ 0xfa082128, __VMLINUX_SYMBOL_STR(down_interruptible) },
	{ 0x1765ec3b, __VMLINUX_SYMBOL_STR(proc_get_parent_data) },
	{ 0xd0e0bccc, __VMLINUX_SYMBOL_STR(skb_put) },
	{ 0xe113bbbc, __VMLINUX_SYMBOL_STR(csum_partial) },
	{ 0x9f984513, __VMLINUX_SYMBOL_STR(strrchr) },
	{ 0x12a38747, __VMLINUX_SYMBOL_STR(usleep_range) },
	{ 0xec901f33, __VMLINUX_SYMBOL_STR(skb_clone) },
	{ 0x891b3d3c, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0xf75c7a14, __VMLINUX_SYMBOL_STR(filp_close) },
	{ 0xaa39f95c, __VMLINUX_SYMBOL_STR(memmove) },
	{ 0x4822b65, __VMLINUX_SYMBOL_STR(netif_wake_subqueue) },
	{ 0x8323201b, __VMLINUX_SYMBOL_STR(netif_receive_skb_sk) },
	{ 0xa40563b1, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x51eafc8e, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0xfa2bcf10, __VMLINUX_SYMBOL_STR(init_timer_key) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xc21943c8, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xe8bce61c, __VMLINUX_SYMBOL_STR(__pskb_pull_tail) },
	{ 0x2d9b0ff0, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0x9e7d6bd0, __VMLINUX_SYMBOL_STR(__udelay) },
	{ 0xc78450af, __VMLINUX_SYMBOL_STR(__movmem_i4_odd) },
	{ 0x86e4c014, __VMLINUX_SYMBOL_STR(usb_reset_device) },
	{ 0x5a025f7b, __VMLINUX_SYMBOL_STR(arch_local_irq_restore) },
	{ 0x67b6d38a, __VMLINUX_SYMBOL_STR(filp_open) },
	{ 0x44f3b130, __VMLINUX_SYMBOL_STR(usb_autopm_get_interface) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xe49eb1db, __VMLINUX_SYMBOL_STR(seq_release) },
	{ 0x9545af6d, __VMLINUX_SYMBOL_STR(tasklet_init) },
	{ 0x913935f0, __VMLINUX_SYMBOL_STR(usb_autopm_put_interface) },
	{ 0xfaef0ed, __VMLINUX_SYMBOL_STR(__tasklet_schedule) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x1f686882, __VMLINUX_SYMBOL_STR(__netdev_alloc_skb) },
	{ 0x11089ac7, __VMLINUX_SYMBOL_STR(_ctype) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0xcca27eeb, __VMLINUX_SYMBOL_STR(del_timer) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x817be5a9, __VMLINUX_SYMBOL_STR(netif_napi_del) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0x4205ad24, __VMLINUX_SYMBOL_STR(cancel_work_sync) },
	{ 0xf1666530, __VMLINUX_SYMBOL_STR(usb_free_coherent) },
	{ 0x85670f1d, __VMLINUX_SYMBOL_STR(rtnl_is_locked) },
	{ 0xbd92e7a9, __VMLINUX_SYMBOL_STR(skb_pull) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v0BDAp0811d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp0821d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp8822d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDApA811d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BDAp0820d*dc*dsc*dp*icFFiscFFipFFin*");
MODULE_ALIAS("usb:v0BDAp0823d*dc*dsc*dp*icFFiscFFipFFin*");
MODULE_ALIAS("usb:v7392pA811d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04BBp0953d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2001p3314d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2001p3318d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0E66p0023d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v056Ep400Ed*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v056Ep400Fd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v20F4p804Bd*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "F6301CC8554AD02DF8A96D1");
