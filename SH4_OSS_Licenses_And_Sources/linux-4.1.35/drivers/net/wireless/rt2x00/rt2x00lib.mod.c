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
	{ 0x13c0a296, __VMLINUX_SYMBOL_STR(__udivsi3_i4i) },
	{ 0xceabb6be, __VMLINUX_SYMBOL_STR(mem_map) },
	{ 0xc068440e, __VMLINUX_SYMBOL_STR(__kfifo_alloc) },
	{ 0xb55ad0a7, __VMLINUX_SYMBOL_STR(ieee80211_queue_work) },
	{ 0x43a53735, __VMLINUX_SYMBOL_STR(__alloc_workqueue_key) },
	{ 0xfe9ad9fa, __VMLINUX_SYMBOL_STR(arch_local_save_flags) },
	{ 0x9469482, __VMLINUX_SYMBOL_STR(kfree_call_rcu) },
	{ 0xcc8c2244, __VMLINUX_SYMBOL_STR(hrtimer_cancel) },
	{ 0x30eef3fb, __VMLINUX_SYMBOL_STR(ieee80211_rts_get) },
	{ 0x6b06fdce, __VMLINUX_SYMBOL_STR(delayed_work_timer_fn) },
	{ 0xeaad455f, __VMLINUX_SYMBOL_STR(ieee80211_beacon_get_tim) },
	{ 0x4205ad24, __VMLINUX_SYMBOL_STR(cancel_work_sync) },
	{ 0xef05ba04, __VMLINUX_SYMBOL_STR(ieee80211_unregister_hw) },
	{ 0xa1425906, __VMLINUX_SYMBOL_STR(ieee80211_channel_to_frequency) },
	{ 0x4a5fa29b, __VMLINUX_SYMBOL_STR(__dev_kfree_skb_any) },
	{ 0xfa2bcf10, __VMLINUX_SYMBOL_STR(init_timer_key) },
	{ 0x4a8907de, __VMLINUX_SYMBOL_STR(cancel_delayed_work_sync) },
	{ 0x62b72b0d, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x904bdc0c, __VMLINUX_SYMBOL_STR(ieee80211_iterate_active_interfaces_atomic) },
	{ 0xbe31ed5f, __VMLINUX_SYMBOL_STR(wiphy_rfkill_start_polling) },
	{ 0x5a025f7b, __VMLINUX_SYMBOL_STR(arch_local_irq_restore) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x4a98a5c6, __VMLINUX_SYMBOL_STR(skb_trim) },
	{ 0xebae8191, __VMLINUX_SYMBOL_STR(ieee80211_stop_queues) },
	{ 0x1f686882, __VMLINUX_SYMBOL_STR(__netdev_alloc_skb) },
	{ 0xad185c80, __VMLINUX_SYMBOL_STR(ieee80211_stop_queue) },
	{ 0xbb74be47, __VMLINUX_SYMBOL_STR(ieee80211_tx_status) },
	{ 0x2bc95bd4, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x8af452a7, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xdc798d37, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0xc923f85e, __VMLINUX_SYMBOL_STR(ieee80211_free_txskb) },
	{ 0x3c3fce39, __VMLINUX_SYMBOL_STR(__local_bh_enable_ip) },
	{ 0x7a437c96, __VMLINUX_SYMBOL_STR(ieee80211_rx) },
	{ 0x1ddf17ab, __VMLINUX_SYMBOL_STR(skb_push) },
	{ 0xe16b893b, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x8c03d20c, __VMLINUX_SYMBOL_STR(destroy_workqueue) },
	{ 0x9545af6d, __VMLINUX_SYMBOL_STR(tasklet_init) },
	{ 0x2469810f, __VMLINUX_SYMBOL_STR(__rcu_read_unlock) },
	{ 0x43b0c9c3, __VMLINUX_SYMBOL_STR(preempt_schedule) },
	{ 0xbd92e7a9, __VMLINUX_SYMBOL_STR(skb_pull) },
	{ 0x71b4ae8e, __VMLINUX_SYMBOL_STR(ieee80211_ctstoself_get) },
	{ 0xc7f7a7e, __VMLINUX_SYMBOL_STR(wiphy_rfkill_stop_polling) },
	{ 0x91ae4c8d, __VMLINUX_SYMBOL_STR(ieee80211_queue_delayed_work) },
	{ 0x82072614, __VMLINUX_SYMBOL_STR(tasklet_kill) },
	{ 0x872f3f04, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x7a27c184, __VMLINUX_SYMBOL_STR(ewma_init) },
	{ 0xd7b8e01d, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x9963a089, __VMLINUX_SYMBOL_STR(queue_delayed_work_on) },
	{ 0xeaea8e4f, __VMLINUX_SYMBOL_STR(wiphy_rfkill_set_hw_state) },
	{ 0x3bd1b1f6, __VMLINUX_SYMBOL_STR(msecs_to_jiffies) },
	{ 0x51ca84b2, __VMLINUX_SYMBOL_STR(ieee80211_get_buffered_bc) },
	{ 0x7553976f, __VMLINUX_SYMBOL_STR(ieee80211_wake_queue) },
	{ 0xdb760f52, __VMLINUX_SYMBOL_STR(__kfifo_free) },
	{ 0xd303d73e, __VMLINUX_SYMBOL_STR(ieee80211_get_hdrlen_from_skb) },
	{ 0xd1d895ee, __VMLINUX_SYMBOL_STR(ieee80211_register_hw) },
	{ 0x1eb9516e, __VMLINUX_SYMBOL_STR(round_jiffies_relative) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xbcc736b3, __VMLINUX_SYMBOL_STR(request_firmware) },
	{ 0x4ec46d25, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xaa39f95c, __VMLINUX_SYMBOL_STR(memmove) },
	{ 0x23b6c68d, __VMLINUX_SYMBOL_STR(consume_skb) },
	{ 0x8d522714, __VMLINUX_SYMBOL_STR(__rcu_read_lock) },
	{ 0xd0e0bccc, __VMLINUX_SYMBOL_STR(skb_put) },
	{ 0xea1abf4e, __VMLINUX_SYMBOL_STR(ieee80211_iterate_interfaces) },
	{ 0x6c6d8e5a, __VMLINUX_SYMBOL_STR(release_firmware) },
	{ 0xbfdf42f1, __VMLINUX_SYMBOL_STR(dma_ops) },
	{ 0xa14f3d8c, __VMLINUX_SYMBOL_STR(ewma_add) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "CEF63963A707700E3E54F57");
