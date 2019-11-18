/* This is a generated file, see Makefile.am for its inputs. */
static const char fam_strings[] = "alg\0appletalk\0ash\0atmpvc\0atmsvc\0ax25\0bluetooth\0bridge\0caif\0can\0"
	"decnet\0econet\0ieee802154\0inet\0inet6\0ipx\0irda\0isdn\0iucv\0key\0"
	"llc\0local\0netbeui\0netlink\0netrom\0nfc\0packet\0phonet\0pppox\0rds\0"
	"rose\0rxrpc\0security\0sna\0tipc\0vsock\0wanpipe\0x25";
static const unsigned fam_i2s_direct[] = {
	126,88,32,99,4,148,47,18,226,93,
	183,63,132,194,118,140,159,14,70,25,
	179,203,103,173,218,122,-1u,-1u,59,207,
	37,113,188,108,166,77,54,0,155,212,
};
static const char *fam_i2s(int v) {
	return i2s_direct__(fam_strings, fam_i2s_direct, 1, 40, v);
}
