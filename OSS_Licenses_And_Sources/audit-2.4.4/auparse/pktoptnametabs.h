/* This is a generated file, see Makefile.am for its inputs. */
static const char pktoptname_strings[] = "PACKET_ADD_MEMBERSHIP\0PACKET_AUXDATA\0PACKET_COPY_THRESH\0PACKET_DROP_MEMBERSHIP\0PACKET_FANOUT\0PACKET_HDRLEN\0PACKET_LOSS\0PACKET_ORIGDEV\0PACKET_QDISC_BYPASS\0PACKET_RECV_OUTPUT\0"
	"PACKET_RESERVE\0PACKET_RX_RING\0PACKET_STATISTICS\0PACKET_TIMESTAMP\0PACKET_TX_HAS_OFF\0PACKET_TX_RING\0PACKET_TX_TIMESTAMP\0PACKET_VERSION\0PACKET_VNET_HDR";
static const unsigned pktoptname_i2s_direct[] = {
	0,56,154,-1u,188,203,37,22,119,291,
	93,173,256,107,306,271,221,79,238,134,
};
static const char *pktoptname_i2s(int v) {
	return i2s_direct__(pktoptname_strings, pktoptname_i2s_direct, 1, 20, v);
}
