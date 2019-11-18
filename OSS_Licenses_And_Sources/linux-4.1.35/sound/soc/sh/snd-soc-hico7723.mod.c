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
	{ 0x73d2bb24, __VMLINUX_SYMBOL_STR(devm_snd_soc_register_card) },
	{ 0x51eafc8e, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x9e7b939c, __VMLINUX_SYMBOL_STR(clk_register) },
	{ 0x7fb1722d, __VMLINUX_SYMBOL_STR(clkdev_add) },
	{ 0xe1bafde8, __VMLINUX_SYMBOL_STR(gpio_fn_request_array) },
	{ 0x9c177be9, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x8af452a7, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xfb58c3d3, __VMLINUX_SYMBOL_STR(gpio_fn_free_array) },
	{ 0x1cb9c286, __VMLINUX_SYMBOL_STR(clkdev_alloc) },
	{ 0xb28e1a1d, __VMLINUX_SYMBOL_STR(snd_soc_dai_set_sysclk) },
	{ 0x8b707c1f, __VMLINUX_SYMBOL_STR(clk_set_rate) },
	{ 0x34ca0a65, __VMLINUX_SYMBOL_STR(snd_soc_dai_set_fmt) },
	{ 0xf5a6692b, __VMLINUX_SYMBOL_STR(clk_unregister) },
	{ 0x8ff25879, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

