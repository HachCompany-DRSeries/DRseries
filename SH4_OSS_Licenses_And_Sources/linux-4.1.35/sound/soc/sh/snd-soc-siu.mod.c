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
	{ 0xadf42bd5, __VMLINUX_SYMBOL_STR(__request_region) },
	{ 0xc8af564b, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xd2a941d4, __VMLINUX_SYMBOL_STR(sg_init_table) },
	{ 0x3dd47658, __VMLINUX_SYMBOL_STR(__pm_runtime_idle) },
	{ 0x13c0a296, __VMLINUX_SYMBOL_STR(__udivsi3_i4i) },
	{ 0xceabb6be, __VMLINUX_SYMBOL_STR(mem_map) },
	{ 0x58982d02, __VMLINUX_SYMBOL_STR(__pm_runtime_disable) },
	{ 0x75741b18, __VMLINUX_SYMBOL_STR(snd_soc_unregister_platform) },
	{ 0x788fe103, __VMLINUX_SYMBOL_STR(iomem_resource) },
	{ 0x871180f7, __VMLINUX_SYMBOL_STR(snd_pcm_period_elapsed) },
	{ 0xcd868e4b, __VMLINUX_SYMBOL_STR(snd_soc_set_runtime_hwparams) },
	{ 0x9f340c99, __VMLINUX_SYMBOL_STR(clk_put) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0x721db9fc, __VMLINUX_SYMBOL_STR(__pm_runtime_resume) },
	{ 0x833eaf19, __VMLINUX_SYMBOL_STR(snd_soc_register_component) },
	{ 0x818c989d, __VMLINUX_SYMBOL_STR(snd_pcm_hw_constraint_integer) },
	{ 0x9c177be9, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0xd2a161b1, __VMLINUX_SYMBOL_STR(__ioremap_caller) },
	{ 0x8af452a7, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xfaef0ed, __VMLINUX_SYMBOL_STR(__tasklet_schedule) },
	{ 0x9decdb39, __VMLINUX_SYMBOL_STR(platform_get_resource) },
	{ 0x871fe3db, __VMLINUX_SYMBOL_STR(snd_pcm_lib_free_pages) },
	{ 0x9545af6d, __VMLINUX_SYMBOL_STR(tasklet_init) },
	{ 0x5beb652c, __VMLINUX_SYMBOL_STR(snd_pcm_lib_ioctl) },
	{ 0x1279a277, __VMLINUX_SYMBOL_STR(snd_soc_unregister_component) },
	{ 0x9175cb0f, __VMLINUX_SYMBOL_STR(snd_pcm_lib_malloc_pages) },
	{ 0x82072614, __VMLINUX_SYMBOL_STR(tasklet_kill) },
	{ 0x872f3f04, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0xd7b8e01d, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x95817e70, __VMLINUX_SYMBOL_STR(pm_runtime_enable) },
	{ 0x97b5a025, __VMLINUX_SYMBOL_STR(clk_set_parent) },
	{ 0x38cf5dce, __VMLINUX_SYMBOL_STR(snd_soc_register_platform) },
	{ 0x2c33f000, __VMLINUX_SYMBOL_STR(snd_ctl_new1) },
	{ 0xb493a14b, __VMLINUX_SYMBOL_STR(__dma_request_channel) },
	{ 0x9bce482f, __VMLINUX_SYMBOL_STR(__release_region) },
	{ 0xc0a0f0c9, __VMLINUX_SYMBOL_STR(clk_get) },
	{ 0x8b707c1f, __VMLINUX_SYMBOL_STR(clk_set_rate) },
	{ 0x322d011c, __VMLINUX_SYMBOL_STR(dma_release_channel) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x11f7ce5e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xbcc736b3, __VMLINUX_SYMBOL_STR(request_firmware) },
	{ 0xdbddf1f7, __VMLINUX_SYMBOL_STR(__iounmap) },
	{ 0xe42cdae6, __VMLINUX_SYMBOL_STR(snd_pcm_lib_preallocate_pages_for_all) },
	{ 0x4ec46d25, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0x9113485b, __VMLINUX_SYMBOL_STR(snd_ctl_add) },
	{ 0x8ff25879, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x6c6d8e5a, __VMLINUX_SYMBOL_STR(release_firmware) },
	{ 0xbfdf42f1, __VMLINUX_SYMBOL_STR(dma_ops) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

