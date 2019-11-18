/* NMI: set/get NMI flag for SH7723
 *
 * author: mschreiber
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/delay.h>

#define HL_GEN_MAJOR  43

// commands for ioctl
#define HL_GEN_INIT   0x01

// registers
#define PSCR   0xA405011E
#define PWCR   0xA4050146
#define PSELD  0xA4050154
#define HIZCRB 0xA405015A
#define HIZCRD 0xA405015E
#define DRVCRA 0xA405018A

// #define DBG_SH4_REGS
#ifdef DBG_SH4_REGS
struct reg_t{
  char *regDef;
  char *regName;
  u32   regAddr;
  u8    regSiz;
};

struct reg_t regArray[] = {
  {"Common control register","CMNCR", 0xFEC10000, 32},
  {"CS0 space bus control register", "CS0BCR",  0xFEC10004, 32},
  {"CS4 space bus control register", "CS4BCR",  0xFEC10010, 32},
  {"CS5A space bus control register", "CS5ABCR",  0xFEC10014, 32},
  {"CS5B space bus control register", "CS5BBCR",  0xFEC10018, 32},
  {"CS6A space bus control register", "CS6ABCR",  0xFEC1001C, 32},
  {"CS6B space bus control register", "CS6BBCR",  0xFEC10020, 32},
  {"CS0 space wait control register", "CS0WCR",  0xFEC10024, 32},
  {"CS4 space wait control register", "CS4WCR",  0xFEC10030, 32},
  {"CS5A space wait control register", "CS5AWCR",  0xFEC10034, 32},
  {"CS5B space wait control register", "CS5BWCR",  0xFEC10038, 32},
  {"CS6A space wait control register", "CS6AWCR",  0xFEC1003C, 32},
  {"CS6B space wait control register", "CS6BWCR",  0xFEC10040, 32},
  {"Reset bus wait counter", "RBWTCNT", 0xFEC10054, 32},
  {"SDRAM control register", "SDCR",  0xFE400008, 32},
  {"SDRAM wait control register", "SDWCR",  0xFE40000C, 32},
  {"SDRAM pin control register", "SDPCR",  0xFE400010, 32},
  {"Refresh timer control/status register", "RTCSR",  0xFE400014, 32},
  {"Refresh timer counter", "RTCNT",  0xFE400018, 32},
  {"Refresh time constant register", "RTCOR",  0xFE40001C, 32},
  {"Refresh count register", "RFCR",  0xFE400020, 32},
  {"Burst refresh count register", "BSTRCNT",  0xFE400040, 32},
  {"Power-down control register", "SDPDCR",  0xFE40002C, 32},
  {"SBSC timing control register", "SDTMGCR",  0xFE400044, 32},
  {"DMA0 source address register_0", "DMA0_SAR_0",  0xFE008020, 32},
  {"DMA0 destination address register_0", "DMA0_DAR_0",  0xFE008024, 32},
  {"DMA0 transfer count register_0", "DMA0_TCR_0",  0xFE008028, 32},
  {"DMA0 channel control register_0", "DMA0_CHCR_0",  0xFE00802C, 32},
  {"DMA0 source address register_1", "DMA0_SAR_1",  0xFE008030, 32},
  {"DMA0 destination address register_1", "DMA0_DAR_1",  0xFE008034, 32},
  {"DMA0 transfer count register_1", "DMA0_TCR_1",  0xFE008038, 32},
  {"DMA0 channel control register_1", "DMA0_CHCR_1",  0xFE00803C, 32},
  {"DMA0 source address register_2", "DMA0_SAR_2",  0xFE008040, 32},
  {"DMA0 destination address register_2", "DMA0_DAR_2",  0xFE008044, 32},
  {"DMA0 transfer count register_2", "DMA0_TCR_2",  0xFE008048, 32},
  {"DMA0 channel control register_2", "DMA0_CHCR_2",  0xFE00804C, 32},
  {"DMA0 source address register_3", "DMA0_SAR_3",  0xFE008050, 32},
  {"DMA0 destination address register_3", "DMA0_DAR_3",  0xFE008054, 32},
  {"DMA0 transfer count register_3", "DMA0_TCR_3",  0xFE008058, 32},
  {"DMA0 channel control register_3", "DMA0_CHCR_3",  0xFE00805C, 32},
  {"DMA0 operation register", "DMA0_DMAOR",  0xFE008060, 16},
  {"DMA0 source address register_4", "DMA0_SAR_4",  0xFE008070, 32},
  {"DMA0 destination address register_4", "DMA0_DAR_4",  0xFE008074, 32},
  {"DMA0 transfer count register_4", "DMA0_TCR_4",  0xFE008078, 32},
  {"DMA0 channel control register_4", "DMA0_CHCR_4",  0xFE00807C, 32},
  {"DMA0 source address register_5", "DMA0_SAR_5",  0xFE008080, 32},
  {"DMA0 destination address register_5", "DMA0_DAR_5",  0xFE008084, 32},
  {"DMA0 transfer count register_5", "DMA0_TCR_5",  0xFE008088, 32},
  {"DMA0 channel control register_5", "DMA0_CHCR_5",  0xFE00808C, 32},
  {"DMA0 source address register", "B_0 DMA0_SARB_0",  0xFE008120, 32},
  {"DMA0 destination address register", "B_0 DMA0_DARB_0",  0xFE008124, 32},
  {"DMA0 transfer count register", "B_0 DMA0_TCRB_0",  0xFE008128, 32},
  {"DMA0 source address register", "B_1 DMA0_SARB_1",  0xFE008130, 32},
  {"DMA0 destination address register", "B_1 DMA0_DARB_1",  0xFE008134, 32},
  {"DMA0 transfer count register", "B_1 DMA0_TCRB_1",  0xFE008138, 32},
  {"DMA0 source address register", "B_2 DMA0_SARB_2",  0xFE008140, 32},
  {"DMA0 destination address register", "B_2 DMA0_DARB_2",  0xFE008144, 32},
  {"DMA0 transfer count register", "B_2 DMA0_TCRB_2",  0xFE008148, 32},
  {"DMA0 source address register", "B_3 DMA0_SARB_3",  0xFE008150, 32},
  {"DMA0 destination address register", "B_3 DMA0_DARB_3",  0xFE008154, 32},
  {"DMA0 transfer count register", "B_3 DMA0_TCRB_3",  0xFE008158, 32},
  {"DMA0 extended resource selector 0", "DMA0_DMARS0",  0xFE009000, 16},
  {"DMA0 extended resource selector 1", "DMA0_DMARS1",  0xFE009004, 16},
  {"DMA0 extended resource selector 2", "DMA0_DMARS2",  0xFE009008, 16},
  {"DMA1 source address register_0", "DMA1_SAR_0",  0xFDC08020, 32},
  {"DMA1 destination address register_0", "DMA1_DAR_0",  0xFDC08024, 32},
  {"DMA1 transfer count register_0", "DMA1_TCR_0",  0xFDC08028, 32},
  {"DMA1 channel control register_0", "DMA1_CHCR_0",  0xFDC0802C, 32},
  {"DMA1 source address register_1", "DMA1_SAR_1",  0xFDC08030, 32},
  {"DMA1 destination address register_1", "DMA1_DAR_1",  0xFDC08034, 32},
  {"DMA1 transfer count register_1", "DMA1_TCR_1",  0xFDC08038, 32},
  {"DMA1 channel control register_1", "DMA1_CHCR_1",  0xFDC0803C, 32},
  {"DMA1 source address register_2", "DMA1_SAR_2",  0xFDC08040, 32},
  {"DMA1 destination address register_2", "DMA1_DAR_2",  0xFDC08044, 32},
  {"DMA1 transfer count register_2", "DMA1_TCR_2",  0xFDC08048, 32},
  {"DMA1 channel control register_2", "DMA1_CHCR_2",  0xFDC0804C, 32},
  {"DMA1 source address register_3", "DMA1_SAR_3",  0xFDC08050, 32},
  {"DMA1 destination address register_3", "DMA1_DAR_3",  0xFDC08054, 32},
  {"DMA1 transfer count register_3", "DMA1_TCR_3",  0xFDC08058, 32},
  {"DMA1 channel control register_3", "DMA1_CHCR_3",  0xFDC0805C, 32},
  {"DMA1 operation register", "DMA1_DMAOR",  0xFDC08060, 16},
  {"DMA1 source address register_4", "DMA1_SAR_4",  0xFDC08070, 32},
  {"DMA1 destination address register_4", "DMA1_DAR_4",  0xFDC08074, 32},
  {"DMA1 transfer count register_4", "DMA1_TCR_4",  0xFDC08078, 32},
  {"DMA1 channel control register_4", "DMA1_CHCR_4",  0xFDC0807C, 32},
  {"DMA1 source address register_5", "DMA1_SAR_5",  0xFDC08080, 32},
  {"DMA1 destination address register_5", "DMA1_DAR_5",  0xFDC08084, 32},
  {"DMA1 transfer count register_5", "DMA1_TCR_5",  0xFDC08088, 32},
  {"DMA1 channel control register_5", "DMA1_CHCR_5",  0xFDC0808C, 32},
  {"DMA1 source address register", "B_0 DMA1_SARB_0",  0xFDC08120, 32},
  {"DMA1 destination address register", "B_0 DMA1_DARB_0",  0xFDC08124, 32},
  {"DMA1 transfer count register", "B_0 DMA1_TCRB_0",  0xFDC08128, 32},
  {"DMA1 source address register", "B_1 DMA1_SARB_1",  0xFDC08130, 32},
  {"DMA1 destination address register", "B_1 DMA1_DARB_1",  0xFDC08134, 32},
  {"DMA1 transfer count register", "B_1 DMA1_TCRB_1",  0xFDC08138, 32},
  {"DMA1 source address register", "B_2 DMA1_SARB_2",  0xFDC08140, 32},
  {"DMA1 destination address register", "B_2 DMA1_DARB_2",  0xFDC08144, 32},
  {"DMA1 transfer count register", "B_2 DMA1_TCRB_2",  0xFDC08148, 32},
  {"DMA1 source address register", "B_3 DMA1_SARB_3",  0xFDC08150, 32},
  {"DMA1 destination address register", "B_3 DMA1_DARB_3",  0xFDC08154, 32},
  {"DMA1 transfer count register", "B_3 DMA1_TCRB_3",  0xFDC08158, 32},
  {"DMA1 extended resource selector 0", "DMA1_DMARS0",  0xFDC09000, 16},
  {"DMA1 extended resource selector 1", "DMA1_DMARS1",  0xFDC09004, 16},
  {"DMA1 extended resource selector 2", "DMA1_DMARS2",  0xFDC09008, 16},
  {"Frequency control register", "FRQCR",  0xA4150000, 32},
  {"Video clock frequency control register", "VCLKCR",  0xA4150004, 32},
  {"SIU clock A frequency control register", "SCLKACR",  0xA4150008, 32},
  {"SIU clock B frequency control register", "SCLKBCR",  0xA415000C, 32},
  {"IrDA clock frequency control register", "IrDACLKCR",  0xA4150018, 32},
  {"PLL control register", "PLLCR",  0xA4150024, 32},
  {"DLL multiplication register", "DLLFRQ",  0xA4150050, 32},
  {"Standby control register", "STBCR",  0xA4150020, 32},
  {"Module stop register0", "MSTPCR0",  0xA4150030, 32},
  {"Module stop register1", "MSTPCR1",  0xA4150034, 32},
  {"Module stop register2", "MSTPCR2",  0xA4150038, 32},
  {"RCLK watchdog timer counter", "RWTCNT",  0xA4520000, 8},
  {"RCLK watchdog timer control/status register", "RWTCSR",  0xA4520004, 8},
  {"Timer start register0", "TSTR",  0xFFD80004, 8},
  {"Timer constant register0_0", "TCOR0_0",  0xFFD80008, 32},
  {"Timer counter0_0", "TCNT0_0",  0xFFD8000C, 32},
  {"Timer control register0_0", "TCR0_0",  0xFFD80010, 16},
  {"Timer constant register0_1", "TCOR0_1",  0xFFD80014, 32},
  {"Timer counter0_1", "TCNT0_1",  0xFFD80018, 32},
  {"Timer control register0_1", "TCR0_1",  0xFFD8001C, 16},
  {"Timer constant register0_2", "TCOR0_2",  0xFFD80020, 32},
  {"Timer counter0_2", "TCNT0_2",  0xFFD80024, 32},
  {"Timer control register0_2", "TCR0_2",  0xFFD80028, 16},
  {"Timer start register1", "TSTR1",  0xFFD90004, 8},
  {"Timer constant register1_0", "TCOR1_0",  0xFFD90008, 32},
  {"Timer counter1_0", "TCNT1_0",  0xFFD9000C, 32},
  {"Timer control register1_0", "TCR1_0",  0xFFD90010, 16},
  {"Timer constant register1_1", "TCOR1_1",  0xFFD90014, 32},
  {"Timer counter1_1", "TCNT1_1",  0xFFD90018, 32},
  {"Timer control register1_1", "TCR1_1",  0xFFD9001C, 16},
  {"Timer constant register1_2", "TCOR1_2",  0xFFD90020, 32},
  {"Timer counter1_2", "TCNT1_2",  0xFFD90024, 32},
  {"Timer control register1_2", "TCR1_2",  0xFFD90028, 16},
  {"Timer start register", "TPU_TSTR",  0xA4C90000, 16},
  {"Timer control register0", "TPU_TCR0",  0xA4C90010, 16},
  {"Timer mode register 0", "TPU_TMDR0",  0xA4C90014, 16},
  {"Timer I/O control register 0", "TPU_TIOR0",  0xA4C90018, 16},
  {"Timer interrupt enable register 0", "TPU_TIER0",  0xA4C9001C, 16},
  {"Timer status register 0", "TPU_TSR0",  0xA4C90020, 16},
  {"Timer counter 0", "TPU_TCNT0",  0xA4C90024, 16},
  {"Timer general register0A", "TPU_TGR0A",  0xA4C90028, 16},
  {"Timer general register0B", "TPU_TGR0B",  0xA4C9002C, 16},
  {"Timer general register0C", "TPU_TGR0C",  0xA4C90030, 16},
  {"Timer general register0D", "TPU_TGR0D",  0xA4C90034, 16},
  {"Timer control register1", "TPU_TCR1",  0xA4C90050, 16},
  {"Timer mode register1", "TPU_TMDR1",  0xA4C90054, 16},
  {"Timer I/O control register1", "TPU_TIOR1",  0xA4C90058, 16},
  {"Timer interrupt enable register1", "TPU_TIER1",  0xA4C9005C, 16},
  {"Timer status register1", "TPU_TSR1",  0xA4C90060, 16},
  {"Timer counter1", "TPU_TCNT1",  0xA4C90064, 16},
  {"Timer general register1A", "TPU_TGR1A",  0xA4C90068, 16},
  {"Timer general register1B", "TPU_TGR1B",  0xA4C9006C, 16},
  {"Timer general register1C", "TPU_TGR1C",  0xA4C90070, 16},
  {"Timer general register1D", "TPU_TGR1D",  0xA4C90074, 16},
  {"Timer general register2 ", "TPU_TCR2",  0xA4C90090, 16},
  {"Timer mode register2", "TPU_TMDR2",  0xA4C90094, 16},
  {"Timer I/O control register2", "TPU_TIOR2",  0xA4C90098, 16},
  {"Timer interrupt enable register2", "TPU_TIER2",  0xA4C9009C, 16},
  {"Timer status register2", "TPU_TSR2",  0xA4C900A0, 16},
  {"Timer counter2", "TPU_TCNT2",  0xA4C900A4, 16},
  {"Timer general register2A", "TPU_TGR2A",  0xA4C900A8, 16},
  {"Timer general register2B", "TPU_TGR2B",  0xA4C900AC, 16},
  {"Timer general register2C", "TPU_TGR2C",  0xA4C900B0, 16},
  {"Timer general register2D", "TPU_TGR2D",  0xA4C900B4, 16},
  {"Timer control register3 ", "TPU_TCR3",  0xA4C900D0, 16},
  {"Timer mode register3", "TPU_TMDR3",  0xA4C900D4, 16},
  {"Timer interrupt enable register3", "TPU_TIER3",  0xA4C900DC, 16},
  {"Timer status register3", "TPU_TSR3",  0xA4C900E0, 16},
  {"Timer counter 3", "TPU_TCNT3",  0xA4C900E4, 16},
  {"Timer I/O control register3", "TPU_TIOR3",  0xA4C900D8, 16},
  {"Timer general register3A", "TPU_TGR3A",  0xA4C900E8, 16},
  {"Timer general register3B", "TPU_TGR3B",  0xA4C900EC, 16},
  {"Timer general register3C", "TPU_TGR3C",  0xA4C900F0, 16},
  {"Timer general register3D", "TPU_TGR3D",  0xA4C900F4, 16},
  {"Compare match timer start register", "CMSTR",  0xA44A0000, 16},
  {"Compare match timer control/status register", "CMCSR",  0xA44A0060, 16},
  {"Compare match timer counter", "CMCNT",  0xA44A0064, 32},
  {"Compare match timer constant register", "CMCOR",  0xA44A0068, 32},
  {"MSIOF0 transmit mode register1 ", "MSIOF0_SITMDR1",  0xA4C40000, 32},
  {"MSIOF0 transmit mode register2 ", "MSIOF0_SITMDR2",  0xA4C40004, 32},
  {"MSIOF0 transmit mode register3 ", "MSIOF0_SITMDR3",  0xA4C40008, 32},
  {"MSIOF0 receive mode register1 ", "MSIOF0_SIRMDR1",  0xA4C40010, 32},
  {"MSIOF0 receive mode register2 ", "MSIOF0_SIRMDR2",  0xA4C40014, 32},
  {"MSIOF0 receive mode register3 ", "MSIOF0_SIRMDR3",  0xA4C40018, 32},
  {"MSIOF0 transmit clock select register", "MSIOF0_SITSCR",  0xA4C40020, 16},
  {"MSIOF0 receive clock select register", "MSIOF0_SIRSCR",  0xA4C40022, 16},
  {"MSIOF0 control register", "MSIOF0_SICTR",  0xA4C40028, 32},
  {"MSIOF0 FIFO control register", "MSIOF0_SIFCTR",  0xA4C40030, 32},
  {"MSIOF0 status register", "MSIOF0_SISTR",  0xA4C40040, 32},
  {"MSIOF0 interrupt enable register", "MSIOF0_SIIER",  0xA4C40044, 32},
  {"MSIOF0 receive control data register1 ", "MSIOF0_SIRDR1", 0xA4C40058, 32},
  {"MSIOF0 receive control data register2 ", "MSIOF0_SIRDR2", 0xA4C4005C, 32},
  {"MSIOF0 receive FIFO data register", "MSIOF0_SIRFDR", 0xA4C40060, 32},
  {"MSIOF1 transmit mode register1 ", "MSIOF1_SITMDR1",  0xA4C50000, 32},
  {"MSIOF1 transmit mode register2 ", "MSIOF1_SITMDR2",  0xA4C50004, 32},
  {"MSIOF1 transmit mode register3 ", "MSIOF1_SITMDR3",  0xA4C50008, 32},
  {"MSIOF1 receive mode register1 ", "MSIOF1_SIRMDR1",  0xA4C50010, 32},
  {"MSIOF1 receive mode register2 ", "MSIOF1_SIRMDR2",  0xA4C50014, 32},
  {"MSIOF1 receive mode register3 ", "MSIOF1_SIRMDR3",  0xA4C50018, 32},
  {"MSIOF1 transmit clock select register", "MSIOF1_SITSCR",  0xA4C50020, 16},
  {"MSIOF1 receive clock select register", "MSIOF1_SIRSCR",  0xA4C50022, 16},
  {"MSIOF1 control register", "MSIOF1_SICTR",  0xA4C50028, 32},
  {"MSIOF1 FIFO control register", "MSIOF1_SIFCTR",  0xA4C50030, 32},
  {"MSIOF1 status register", "MSIOF1_SISTR",  0xA4C50040, 32},
  {"MSIOF1 interrupt enable register", "MSIOF1_SIIER",  0xA4C50044, 32},
  {"MSIOF1 receive control data register1", "MSIOF1_SIRDR1", 0xA4C50058, 32},
  {"MSIOF1 receive control data register2", "MSIOF1_SIRDR2", 0xA4C5005C, 32},
  {"MSIOF1 receive FIFO data register", "MSIOF1_SIRFDR", 0xA4C50060, 32},
  {"Serial mode register0", "SCSMR0",  0xFFE00000, 16},
  {"Bit rate register0", "SCBRR0",  0xFFE00004, 8},
  {"Serial control register0", "SCSCR0",  0xFFE00008, 16},
  {"Serial status register0", "SCFSR0",  0xFFE00010, 16},
  {"Receive FIFO data register0", "SCFRDR0", 0xFFE00014, 8},
  {"FIFO control register0", "SCFCR0",  0xFFE00018, 16},
  {"FIFO data count register0", "SCFDR0", 0xFFE0001C, 16},
  {"Line status register0", "SCLSR0",  0xFFE00024, 16},
  {"Serial mode register1", "SCSMR1",  0xFFE10000, 16},
  {"Bit rate register1", "SCBRR1",  0xFFE10004, 8},
  {"Serial control register1", "SCSCR1",  0xFFE10008, 16},
  {"Serial status register1", "SCFSR1",  0xFFE10010, 16},
  {"Receive FIFO data register1", "SCFRDR1", 0xFFE10014, 8},
  {"FIFO control register1", "SCFCR1",  0xFFE10018, 16},
  {"FIFO data count register1", "SCFDR1", 0xFFE1001C, 16},
  {"Line status register1", "SCLSR1",  0xFFE10024, 16},
  {"Serial mode register2", "SCSMR2",  0xFFE20000, 16},
  {"Bit rate register2", "SCBRR2",  0xFFE20004, 8},
  {"Serial control register2", "SCSCR2",  0xFFE20008, 16},
  {"Serial status register2", "SCFSR2",  0xFFE20010, 16},
  {"Receive FIFO data register2", "SCFRDR2", 0xFFE20014, 8},
  {"FIFO control register2", "SCFCR2",  0xFFE20018, 16},
  {"FIFO data count register2", "SCFDR2", 0xFFE2001C, 16},
  {"Line status register2", "SCLSR2",  0xFFE20024, 16},
  {"Serial mode registerA3", "SCASMR3",  0xA4E30000, 16},
  {"Bit rate registerA3", "SCABRR3",  0xA4E30004, 8},
  {"Serial control registerA3", "SCASCR3",  0xA4E30008, 16},
  {"Transmit data stop registerA3", "SCATDSR3",  0xA4E3000C, 8},
  {"FIFO error count registerA3", "SCAFER3", 0xA4E30010, 16},
  {"Serial status registerA3", "SCASSR3",  0xA4E30014, 16},
  {"FIFO control registerA3", "SCAFCR3",  0xA4E30018, 16},
  {"FIFO data count registerA3", "SCAFDR3", 0xA4E3001C, 16},
  {"Receive FIFO data registerA3", "SCAFRDR3", 0xA4E30024, 8},
  {"Serial mode registerA4", "SCASMR4",  0xA4E40000, 16},
  {"Bit rate registerA4", "SCABRR4",  0xA4E40004, 8},
  {"Serial control registerA4", "SCASCR4",  0xA4E40008, 16},
  {"Transmit data stop registerA4", "SCATDSR4",  0xA4E4000C, 8},
  {"FIFO error count registerA4", "SCAFER4", 0xA4E40010, 16},
  {"Serial status registerA4", "SCASSR4",  0xA4E40014, 16},
  {"FIFO control registerA4", "SCAFCR4",  0xA4E40018, 16},
  {"FIFO data count registerA4", "SCAFDR4", 0xA4E4001C, 16},
  {"Receive FIFO data registerA4", "SCAFRDR4", 0xA4E40024, 8},
  {"Serial mode registerA5", "SCASMR5",  0xA4E50000, 16},
  {"Bit rate registerA5", "SCABRR5",  0xA4E50004, 8},
  {"Serial control registerA5", "SCASCR5",  0xA4E50008, 16},
  {"Transmit data stop registerA5", "SCATDSR5",  0xA4E5000C, 8},
  {"FIFO error count registerA5", "SCAFER5", 0xA4E50010, 16},
  {"Serial status registerA5", "SCASSR5",  0xA4E50014, 16},
  {"FIFO control registerA5", "SCAFCR5",  0xA4E50018, 16},
  {"FIFO data count registerA5", "SCAFDR5", 0xA4E5001C, 16},
  {"Receive FIFO data registerA5", "SCAFRDR5", 0xA4E50024, 8},
  {"64-Hz counter", "R64CNT", 0xA465FEC0, 8},
  {"Second counter", "RSECCNT",  0xA465FEC2, 8},
  {"Minute counter", "RMINCNT",  0xA465FEC4, 8},
  {"Hour counter", "RHRCNT",  0xA465FEC6, 8},
  {"Day of week counter", "RWKCNT",  0xA465FEC8, 8},
  {"Date counter", "RDAYCNT",  0xA465FECA, 8},
  {"Month counter", "RMONCNT",  0xA465FECC, 8},
  {"Year counter", "RYRCNT",  0xA465FECE, 16},
  {"Second alarm register", "RSECAR",  0xA465FED0, 8},
  {"Minute alarm register", "RMINAR",  0xA465FED2, 8},
  {"Hour alarm register", "RHRAR",  0xA465FED4, 8},
  {"Day of week alarm register", "RWKAR",  0xA465FED6, 8},
  {"Date alarm register", "RDAYAR",  0xA465FED8, 8},
  {"Month alarm register", "RMONAR",  0xA465FEDA, 8},
  {"RTC control register1", "RCR1",  0xA465FEDC, 8},
  {"RTC control register2", "RCR2",  0xA465FEDE, 8},
  {"Year alarm register", "RYRAR",  0xA465FEE0, 16},
  {"RTC control register3", "RCR3",  0xA465FEE4, 8},
  {"IrDA-SIR10 control register", "IRIF_SIR0",  0xA45D0020, 16},
  {"IrDA-SIR10 baud rate error correction register", "IRIF_SIR1",  0xA45D0022, 16},
  {"IrDA-SIR10 baud rate count set register", "IRIF_SIR2",  0xA45D0024, 16},
  {"IrDA-SIR10 status register", "IRIF_SIR3", 0xA45D0026, 16},
  {"Hardware frame processing set register", "IRIF_SIR_FRM",  0xA45D0028, 16},
  {"EOF value register", "IRIF_SIR_EOF",  0xA45D002A, 16},
  {"UART status register2", "IRIF_UART_STS2",  0xA45D002E, 16},
  {"UART control register", "IRIF_UART0",  0xA45D0030, 16},
  {"UART status register", "IRIF_UART1", 0xA45D0032, 16},
  {"UART mode register", "IRIF_UART2",  0xA45D0034, 16},
  {"UART receive data register", "IRIF_UART4", 0xA45D0038, 16},
  {"UART interrupt mask register", "IRIF_UART5",  0xA45D003A, 16},
  {"UART baud rate error correction register", "IRIF_UART6",  0xA45D003C, 16},
  {"UART baud rate count set register", "IRIF_UART7",  0xA45D003E, 16},
  {"CRC engine control register", "IRIF_CRC0",  0xA45D0040, 16},
  {"CRC engine output data register1", "IRIF_CRC3", 0xA45D0046, 16},
  {"CRC engine output data register2", "IRIF_CRC4", 0xA45D0048, 16},
  {"Key scan control register1", "KYCR1",  0xA44B0000, 16},
  {"Key scan control register2", "KYCR2",  0xA44B0004, 16},
  {"Key scan-in data register", "KYINDR", 0xA44B0008, 16},
  {"Key scan-out data register", "KYOUTDR",  0xA44B000C, 16},
  {"System configuration control register", "SYSCFG",  0xA4D80000, 16},
  {"CPU bus wait setting register", "BUSWAIT",  0xA4D80002, 16},
  {"System configuration status register", "SYSSTS", 0xA4D80004, 16},
  {"Device state control register", "DVSTCTR",  0xA4D80008, 16},
  {"Test mode register", "TESTMODE",  0xA4D8000C, 16},
  {"CFIFO port register", "CFIFO",  0xA4D80014, 32},
  {"D0FIFO port register", "D0FIFO",  0xA4D80018, 32},
  {"D1FIFO port register", "D1FIFO",  0xA4D8001C, 32},
  {"CFIFO port select register", "CFIFOSEL",  0xA4D80020, 16},
  {"CFIFO port control register", "CFIFOCTR",  0xA4D80022, 16},
  {"D0FIFO port select register", "D0FIFOSEL",  0xA4D80028, 16},
  {"D0FIFO port control register", "D0FIFOCTR",  0xA4D8002A, 16},
  {"D1FIFO port select register", "D1FIFOSEL",  0xA4D8002C, 16},
  {"D1FIFO port control register", "D1FIFOCTR",  0xA4D8002E, 16},
  {"Interrupt enable register0", "INTENB0",  0xA4D80030, 16},
  {"Interrupt enable register1", "INTENB1",  0xA4D80032, 16},
  {"BRDY interrupt enable register", "BRDYENB",  0xA4D80036, 16},
  {"NRDY interrupt enable register", "NRDYENB",  0xA4D80038, 16},
  {"BEMP interrupt enable register", "BEMPENB",  0xA4D8003A, 16},
  {"SOF output configuration register", "SOFCFG",  0xA4D8003C, 16},
  {"Interrupt status register0", "INTSTS0",  0xA4D80040, 16},
  {"Interrupt status register1", "INTSTS1",  0xA4D80042, 16},
  {"BRDY interrupt status register", "BRDYSTS",  0xA4D80046, 16},
  {"NRDY interrupt status register", "NRDYSTS",  0xA4D80048, 16},
  {"BEMP interrupt status register", "BEMPSTS",  0xA4D8004A, 16},
  {"Frame number register", "FRMNUM",  0xA4D8004C, 16},
  {"μFrame number register", "UFRMNUM", 0xA4D8004E, 16},
  {"USB address register", "USBADDR", 0xA4D80050, 16},
  {"USB request type register", "USBREQ",  0xA4D80054, 16},
  {"USB request value register", "USBVAL",  0xA4D80056, 16},
  {"USB request index register", "USBINDX",  0xA4D80058, 16},
  {"USB request length register", "USBLENG",  0xA4D8005A, 16},
  {"DCP configuration register", "DCPCFG",  0xA4D8005C, 16},
  {"DCP maximum packet size register", "DCPMAXP",  0xA4D8005E, 16},
  {"DCP control register", "DCPCTR",  0xA4D80060, 16},
  {"Pipe window select register", "PIPESEL",  0xA4D80064, 16},
  {"Pipe configuration register", "PIPECFG",  0xA4D80068, 16},
  {"Pipe buffer setting register", "PIPEBUF",  0xA4D8006A, 16},
  {"Pipe maximum packet size register", "PIPEMAXP",  0xA4D8006C, 16},
  {"Pipe cycle control register", "PIPEPERI",  0xA4D8006E, 16},
  {"Pipe 1 control register", "PIPE1CTR",  0xA4D80070, 16},
  {"Pipe 2 control register", "PIPE2CTR",  0xA4D80072, 16},
  {"Pipe 3 control register", "PIPE3CTR",  0xA4D80074, 16},
  {"Pipe 4 control register", "PIPE4CTR",  0xA4D80076, 16},
  {"Pipe 5 control register", "PIPE5CTR",  0xA4D80078, 16},
  {"Pipe 6 control register", "PIPE6CTR",  0xA4D8007A, 16},
  {"Pipe 7 control register", "PIPE7CTR",  0xA4D8007C, 16},
  {"Pipe 8 control register", "PIPE8CTR",  0xA4D8007E, 16},
  {"Pipe 9 control register", "PIPE9CTR",  0xA4D80080, 16},
  {"Pipe 1 transaction counter enable register", "PIPE1TRE",  0xA4D80090, 16},
  {"Pipe 1 transaction counter register", "PIPE1TRN",  0xA4D80092, 16},
  {"Pipe 2 transaction counter enable register", "PIPE2TRE",  0xA4D80094, 16},
  {"Pipe 2 transaction counter register", "PIPE2TRN",  0xA4D80096, 16},
  {"Pipe 3 transaction counter enable register", "PIPE3TRE",  0xA4D80098, 16},
  {"Pipe 3 transaction counter register", "PIPE3TRN",  0xA4D8009A, 16},
  {"Pipe 4 transaction counter enable register", "PIPE4TRE",  0xA4D8009C, 16},
  {"Pipe 4 transaction counter register", "PIPE4TRN",  0xA4D8009E, 16},
  {"Pipe 5 transaction counter enable register", "PIPE5TRE",  0xA4D800A0, 16},
  {"Pipe 5 transaction counter register", "PIPE5TRN",  0xA4D800A2, 16},
  {"Device address 0 configuration register", "DEVADD0",  0xA4D800D0, 16},
  {"Device address 1 configuration register", "DEVADD1",  0xA4D800D2, 16},
  {"Device address 2 configuration register", "DEVADD2",  0xA4D800D4, 16},
  {"Device address 3 configuration register", "DEVADD3",  0xA4D800D6, 16},
  {"Device address 4 configuration register", "DEVADD4",  0xA4D800D8, 16},
  {"Device address 5 configuration register", "DEVADD5",  0xA4D800DA, 16},
  {"Device address 6 configuration register", "DEVADD6",  0xA4D800DC, 16},
  {"Device address 7 configuration register", "DEVADD7",  0xA4D800DE, 16},
  {"Device address 8 configuration register", "DEVADD8",  0xA4D800E0, 16},
  {"Device address 9 configuration register", "DEVADD9",  0xA4D800E2, 16},
  {"Device address A configuration register", "DEVADDA",  0xA4D800E4, 16},
  {"I2C bus data register", "ICDR",  0xA4470000, 8},
  {"I2C bus control register", "ICCR",  0xA4470004, 8},
  {"I2C bus status register", "ICSR",  0xA4470008, 8},
  {"I2C interrupt control register", "ICIC",  0xA447000C, 8},
  {"I2C clock control register", "low ICCL",  0xA4470010, 8},
  {"I2C clock control register", "high ICCH",  0xA4470014, 8},
  {"Common control register", "FLCMNCR",  0xA4530000, 32},
  {"Command control register", "FLCMDCR",  0xA4530004, 32},
  {"Command code register", "FLCMCDR",  0xA4530008, 32},
  {"Address register", "FLADR",  0xA453000C, 32},
  {"Address register2", "FLADR2",  0xA453003C, 32},
  {"Data register", "FLDATAR",  0xA4530010, 32},
  {"Data counter register", "FLDTCNTR",  0xA4530014, 32},
  {"Interrupt DMA control register", "FLINTDMACR",  0xA4530018, 32},
  {"Ready busy timeout setting register", "FLBSYTMR",  0xA453001C, 32},
  {"Ready busy timeout counter", "FLBSYCNT", 0xA4530020, 32},
  {"Data FIFO register", "FLDTFIFO",  0xA4530024, 32},
  {"Control code FIFO register", "FLECFIFO",  0xA4530028, 32},
  {"Transfer control register", "FLTRCR",  0xA453002C, 8},
  {"4-symbol ECCRESULT0", "FL4ECCRESULT0", 0xA4530080, 32},
  {"4-symbol ECCRESULT1", "FL4ECCRESULT1", 0xA4530084, 32},
  {"4-symbol ECCRESULT2", "FL4ECCRESULT2", 0xA4530088, 32},
  {"4-symbol ECCRESULT3", "FL4ECCRESULT3", 0xA453008C, 32},
  {"4-symbol ECC control register", "FL4ECCCR",  0xA4530090, 32},
  {"ECC error count register", "FL4ECCCNT",  0xA4530094, 32},
  {"Error address indication register", "FLERRADR", 0xA4530098, 32},
  {"VPU control register", "VP4_CTRL",  0xFE900000, 32},
  {"VOL control register", "VP4_VOL_CTRL",  0xFE900004, 32},
  {"Image size register", "VP4_IMAGE_SIZE",  0xFE900008, 32},
  {"Number of macroblocks register", "VP4_MB_NUM",  0xFE90000C, 32},
  {"Decoded image luminance address register", "VP4_DWY_ADDR",  0xFE900010, 32},
  {"Decoded image chrominance address register", "VP4_DWC_ADDR",  0xFE900014, 32},
  {"Output filtered image luminance address register", "VP4_D2WY_ADDR",  0xFE900018, 32},
  {"Output filtered image chrominance address register", "VP4_D2WC_ADDR",  0xFE90001C, 32},
  {"Data partition 1 address register", "VP4_DP1_ADDR",  0xFE900020, 32},
  {"Data partition 2 address register", "VP4_DP2_ADDR",  0xFE900024, 32},
  {"Stream address register", "VP4_STRS_ADDR",  0xFE900028, 32},
  {"Stream buffer end address register", "VP4_STRE_ADDR",  0xFE90002C, 32},
  {"VOP control register", "VP4_VOP_CTRL",  0xFE900030, 32},
  {"VOP time stamp register", "VP4_VOP_TIME",  0xFE900034, 32},
  {"Short Header control register", "VP4_263_CTRL",  0xFE900038, 32},
  {"AVC control register", "VP4_264_CTRL",  0xFE90003C, 32},
  {"VLC control register", "VP4_VLC_CTRL",  0xFE900040, 32},
  {"Endian setting register", "VP4_ENDIAN",  0xFE900044, 32},
  {"VPU command register", "VP4_CMD",  0xFE900048, 32},
  {"Motion detection threshold setting 1 register", "VP4_ME_TH1",  0xFE90004C, 32},
  {"Motion detection threshold setting 2 register", "VP4_ME_TH2",  0xFE900050, 32},
  {"Macroblock partition cost control register", "VP4_ME_COSTMB",  0xFE900054, 32},
  {"Motion detection block control register", "VP4_ME_CTRL",  0xFE90005C, 32},
  {"Intra refresh control register", "VP4_MBRF_CTRL",  0xFE900060, 32},
  {"Motion compensation/deblocking control register", "VP4_MC_CTRL",  0xFE900064, 32},
  {"Coefficient prediction block control register", "VP4_PRED_CTRL",  0xFE900068, 32},
  {"Slice and video packet size register", "VP4_SLC_SIZE",  0xFE90006C, 32},
  {"VOP minimum bit count register", "VP4_VOP_MINBIT",  0xFE900070, 32},
  {"Macroblock maximum bit count register", "VP4_MB_MAXBIT",  0xFE900074, 32},
  {"Macroblock target bit count register", "VP4_MB_TBIT",  0xFE900078, 32},
  {"Rate control register", "VP4_RCQNT",  0xFE90007C, 32},
  {"Reaction parameter register", "VP4_RCRP",  0xFE900080, 32},
  {"Virtual buffer register", "VP4_RCDJ",  0xFE900084, 32},
  {"Weighted quantization parameter register", "VP4_RCWQ",  0xFE900088, 32},
  {"Forward-predicted image time stamp register", "VP4_FWD_TIME",  0xFE900094, 32},
  {"Backward-predicted image time stamp register", "VP4_BWD_TIME",  0xFE900098, 32},
  {"Decoded image time stamp register", "VP4_PST_TIME",  0xFE90009C, 32},
  {"Interlace frame interval register", "VP4_ILTFRAME",  0xFE9000A0, 32},
  {"Error concealment reference register", "VP4_EC_REF",  0xFE9000A4, 32},
  {"Activity control register", "VP4_MQ_CTRL",  0xFE9000A8, 32},
  {"Difference control register", "VP4_CTD_CTRL",  0xFE9000AC, 32},
  {"Difference value control SAD threshold register", "VP4_CTD_SADTHR",  0xFE9000B0, 32},
  {"VC1 control register", "VP4_VC1_CTRL",  0xFE9000B4, 32},
  {"VC1 VLC control register", "VP4_VC1_VLC",  0xFE9000B8, 32},
  {"VC1 QUANT control register", "VP4_VC1_QUANT",  0xFE9000BC, 32},
  {"VPU status register", "VP4_STATUS", 0xFE900100, 32},
  {"Interrupt enable register", "VP4_IRQ_ENB",  0xFE900104, 32},
  {"Interrupt status register", "VP4_IRQ_STA",  0xFE900108, 32},
  {"VOP bit count register", "VP4_VOP_BIT", 0xFE90010C, 32},
  {"Emulation prevention byte bit count register", "VP4_PRV_BIT", 0xFE900110, 32},
  {"Slice size macroblock count register", "VP4_SLC_MB", 0xFE900114, 32},
  {"Quantized coefficient sum register", "VP4_QSUM", 0xFE900118, 32},
  {"Error code register", "VP4_DEC_ERR", 0xFE90011C, 32},
  {"Error macroblock position register", "VP4_ERR_AREA", 0xFE900120, 32},
  {"Next start code register", "VP4_NEXT_CODE", 0xFE900124, 32},
  {"Macroblock attribute register", "VP4_MB_ATTR", 0xFE900128, 32},
  {"System error detection register", "VP4_ERR_DET",  0xFE900134, 32},
  {"Clock supply control register", "VP4_CLK_STOP",  0xFE900138, 32},
  {"Macroblock intra SAD register", "VP4_MB_SADA", 0xFE90013C, 32},
  {"Macroblock inter SAD register", "VP4_MB_SADR", 0xFE900140, 32},
  {"Activity sum register", "VP4_ASUM", 0xFE900144, 32},
  {"VEU start register", "V0STR", 0xFE920000, 32},
  {"VEU source memory width register", "V0SWR", 0xFE920010, 32},
  {"VEU source size register", "V0SSR", 0xFE920014, 32},
  {"VEU source address Y register", "V0SAYR", 0xFE920018, 32},
  {"VEU source address C register", "V0SACR", 0xFE92001C, 32},
  {"VEU bundle source size register", "V0BSSR", 0xFE920020, 32},
  {"VEU destination memory width register", "V0DWR", 0xFE920030, 32},
  {"VEU destination address Y register", "V0DAYR", 0xFE920034, 32},
  {"VEU destination address C register", "V0DACR", 0xFE920038, 32},
  {"VEU transform control register", "V0TRCR", 0xFE920050, 32},
  {"VEU resize filter control register", "V0RFCR", 0xFE920054, 32},
  {"VEU resize filter size clip register", "V0RFSR", 0xFE920058, 32},
  {"VEU enhance register", "V0ENHR", 0xFE92005C, 32},
  {"VEU filter mode control register", "V0FMCR", 0xFE920070, 32},
  {"VEU vertical tap coefficient register", "V0VTCR", 0xFE920074, 32},
  {"VEU horizontal tap coefficient register", "V0HTCR", 0xFE920078, 32},
  {"VEU designated color register", "V0APCR", 0xFE920080, 32},
  {"VEU conversion color register", "V0CCR", 0xFE920084, 32},
  {"VEU address fixed register", "V0AFXR", 0xFE920090, 32},
  {"VEU swapping register", "V0SWPR", 0xFE920094, 32},
  {"VEU event interrupt enable register", "V0EIER", 0xFE9200A0, 32},
  {"VEU event register", "V0EVTR", 0xFE9200A4, 32},
  {"VEU status register", "V0STAR", 0xFE9200B0, 32},
  {"VEU module reset register", "V0BSRR", 0xFE9200B4, 32},
  {"Matrix coefficient register00", "V0MCR00", 0xFE920200, 32},
  {"Matrix coefficient register01", "V0MCR01", 0xFE920204, 32},
  {"Matrix coefficient register02", "V0MCR02", 0xFE920208, 32},
  {"Matrix coefficient register10", "V0MCR10", 0xFE92020C, 32},
  {"Matrix coefficient register11", "V0MCR11", 0xFE920210, 32},
  {"Matrix coefficient register12", "V0MCR12", 0xFE920214, 32},
  {"Matrix coefficient register20", "V0MCR20", 0xFE920218, 32},
  {"Matrix coefficient register21", "V0MCR21", 0xFE92021C, 32},
  {"Matrix coefficient register22", "V0MCR22", 0xFE920220, 32},
  {"Color conversion offset register", "V0COFFR", 0xFE920224, 32},
  {"Color conversion clip register", "V0CBR", 0xFE920228, 32},
  {"FIR filter control register", "V0FCR", 0xFE920230, 32},
  {"FIR filter horizontal Y register0", "V0HYCR0", 0xFE920234, 32},
  {"FIR filter horizontal Y register1", "V0HYCR1", 0xFE920238, 32},
  {"FIR filter horizontal Y register2", "V0HYCR2", 0xFE92023C, 32},
  {"FIR filter vertical Y register0", "V0VYCR0", 0xFE920240, 32},
  {"FIR filter vertical Y register1", "V0VYCR1", 0xFE920244, 32},
  {"FIR filter vertical Y register2", "V0VYCR2", 0xFE920248, 32},
  {"FIR filter horizontal U register0", "V0HUCR0", 0xFE92024C, 32},
  {"FIR filter horizontal U register1", "V0HUCR1", 0xFE920250, 32},
  {"FIR filter horizontal U register2", "V0HUCR2", 0xFE920254, 32},
  {"FIR filter vertical U register0", "V0VUCR0", 0xFE920258, 32},
  {"FIR filter vertical U register1", "V0VUCR1", 0xFE92025C, 32},
  {"FIR filter vertical U register2", "V0VUCR2", 0xFE920260, 32},
  {"FIR filter horizontal V register0", "V0HVCR0", 0xFE920264, 32},
  {"FIR filter horizontal V register1", "V0HVCR1", 0xFE920268, 32},
  {"FIR filter horizontal V register2", "V0HVCR2", 0xFE92026C, 32},
  {"FIR filter vertical V register0", "V0VVCR0", 0xFE920270, 32},
  {"FIR filter vertical V register1", "V0VVCR1", 0xFE920274, 32},
  {"FIR filter vertical V register2", "V0VVCR2", 0xFE920278, 32},
  {"VEU start register", "V1STR", 0xFE924000, 32},
  {"VEU source memory width register", "V1SWR", 0xFE924010, 32},
  {"VEU source size register", "V1SSR", 0xFE924014, 32},
  {"VEU source address Y register", "V1SAYR", 0xFE924018, 32},
  {"VEU source address C register", "V1SACR", 0xFE92401C, 32},
  {"VEU bundle source size register", "V1BSSR", 0xFE924020, 32},
  {"VEU destination memory width register", "V1DWR", 0xFE924030, 32},
  {"VEU destination address Y register", "V1DAYR", 0xFE924034, 32},
  {"VEU destination address C register", "V1DACR", 0xFE924038, 32},
  {"VEU transform control register", "V1TRCR", 0xFE924050, 32},
  {"VEU resize filter control register", "V1RFCR", 0xFE924054, 32},
  {"VEU resize filter size clip register", "V1RFSR", 0xFE924058, 32},
  {"VEU enhance register", "V1ENHR", 0xFE92405C, 32},
  {"VEU filter mode control register", "V1FMCR", 0xFE924070, 32},
  {"VEU vertical tap coefficient register", "V1VTCR", 0xFE924074, 32},
  {"VEU horizontal tap coefficient register", "V1HTCR", 0xFE924078, 32},
  {"VEU designated color register", "V1APCR", 0xFE924080, 32},
  {"VEU conversion color register", "V1CCR", 0xFE924084, 32},
  {"VEU address fixed register", "V1AFXR", 0xFE924090, 32},
  {"VEU swapping register", "V1SWPR", 0xFE924094, 32},
  {"VEU event interrupt enable register", "V1EIER", 0xFE9240A0, 32},
  {"VEU event register", "V1EVTR", 0xFE9240A4, 32},
  {"VEU status register", "V1STAR", 0xFE9240B0, 32},
  {"VEU module reset register", "V1BSRR", 0xFE9240B4, 32},
  {"Matrix coefficient register00", "V1MCR00", 0xFE924200, 32},
  {"Matrix coefficient register01", "V1MCR01", 0xFE924204, 32},
  {"Matrix coefficient register02", "V1MCR02", 0xFE924208, 32},
  {"Matrix coefficient register10", "V1MCR10", 0xFE92420C, 32},
  {"Matrix coefficient register11", "V1MCR11", 0xFE924210, 32},
  {"Matrix coefficient register12", "V1MCR12", 0xFE924214, 32},
  {"Matrix coefficient register20", "V1MCR20", 0xFE924218, 32},
  {"Matrix coefficient register21", "V1MCR21", 0xFE92421C, 32},
  {"Matrix coefficient register22", "V1MCR22", 0xFE924220, 32},
  {"Color conversion offset register", "V1COFFR", 0xFE924224, 32},
  {"Color conversion clip register", "V1CBR", 0xFE924228, 32},
  {"FIR filter control register", "V1FCR", 0xFE924230, 32},
  {"FIR filter horizontal Y register0", "V1HYCR0", 0xFE924234, 32},
  {"FIR filter horizontal Y register1", "V1HYCR1", 0xFE924238, 32},
  {"FIR filter horizontal Y register2", "V1HYCR2", 0xFE92423C, 32},
  {"FIR filter vertical Y register0", "V1VYCR0", 0xFE924240, 32},
  {"FIR filter vertical Y register1", "V1VYCR1", 0xFE924244, 32},
  {"FIR filter vertical Y register2", "V1VYCR2", 0xFE924248, 32},
  {"FIR filter horizontal U register0", "V1HUCR0", 0xFE92424C, 32},
  {"FIR filter horizontal U register1", "V1HUCR1", 0xFE924250, 32},
  {"FIR filter horizontal U register2", "V1HUCR2", 0xFE924254, 32},
  {"FIR filter vertical U register0", "V1VUCR0", 0xFE924258, 32},
  {"FIR filter vertical U register1", "V1VUCR1", 0xFE92425C, 32},
  {"FIR filter vertical U register2", "V1VUCR2", 0xFE924260, 32},
  {"FIR filter horizontal V register0", "V1HVCR0", 0xFE924264, 32},
  {"FIR filter horizontal V register1", "V1HVCR1", 0xFE924268, 32},
  {"FIR filter horizontal V register2", "V1HVCR2", 0xFE92426C, 32},
  {"FIR filter vertical V register0", "V1VVCR0", 0xFE924270, 32},
  {"FIR filter vertical V register1", "V1VVCR1", 0xFE924274, 32},
  {"FIR filter vertical V register2", "V1VVCR2", 0xFE924278, 32},
  {"TSIF control register", "TSCTLR", 0xA4C80000, 32},
  {"TSIF PID data register", "TSPIDR", 0xA4C80004, 32},
  {"TSIF command register", "TSCMDR", 0xA4C80008, 32},
  {"TSIF interrupt status register", "TSSTR", 0xA4C8000C, 32},
  {"TSIF TS data register", "TSTSDR", 0xA4C80010, 32},
  {"TSIF buffer clear register", "TSBUFCLRR", 0xA4C80014, 32},
  {"TSIF interrupt enable register", "TSINTER", 0xA4C80018, 32},
  {"TSIF PSCALE register", "TSPSCALER", 0xA4C80020, 32},
  {"TSIF PSCALE_R register", "TSPSCALERR", 0xA4C80024, 32},
  {"TSIF PCRADC mode register", "TSPCRADCMDR", 0xA4C80028, 32},
  {"TSIF PCRADC register", "TSPCRADCR", 0xA4C8002C, 32},
  {"TSIF TR_PCRADC register", "TSTRPCRADCR", 0xA4C80030, 32},
  {"TSIF D_PCRADC register", "TSDPCRADCR", 0xA4C80034, 32},
  {"TSIF free-running counter", "TSFRC", 0xA4C80040, 32},
  {"Interface control register", "IFCTL", 0xA454C000, 32},
  {"Software reset control register", "SRCTL", 0xA454C004, 32},
  {"Serial format register", "SFORM", 0xA454C008, 32},
  {"Clock cycle register", "CKCTL", 0xA454C00C, 32},
  {"Data transfer method register", "TRDAT", 0xA454C010, 32},
  {"FIFO operation enable register", "STFIFO", 0xA454C014, 32},
  {"PCM data pack format register", "DPAK", 0xA454C01C, 32},
  {"BT and LR clock polarity reverse register", "CKREV", 0xA454C020, 32},
  {"Event clear register", "EVNTC", 0xA454C028, 32},
  {"SPB control register", "SBCTL", 0xA454C040, 32},
  {"SPB start address set register", "SBPSET", 0xA454C044, 32},
  {"SPB bus control register", "SBBUS", 0xA454C048, 32},
  {"SPB flag write register", "SBWFLG", 0xA454C058, 32},
  {"SPB flag read register", "SBRFLG", 0xA454C05C, 32},
  {"SPB realtime data write register", "SBWDAT", 0xA454C060, 32},
  {"SPB realtime data read register", "SBRDAT", 0xA454C064, 32},
  {"SPB FIFO status register", "SBFSTS", 0xA454C068, 32},
  {"SPB digital volume control register", "A SBDVCA", 0xA454C06C, 32},
  {"SPB digital volume control register", "B SBDVCB", 0xA454C070, 32},
  {"SPB activation condition register", "SBACTIV", 0xA454C074, 32},
  {"Port A input DMA transfer output register", "DMAIA", 0xA454C090, 32},
  {"Port B input DMA transfer output register", "DMAIB", 0xA454C094, 32},
  {"Port A output DMA transfer input register", "DMAOA", 0xA454C098, 32},
  {"Port B output DMA transfer input register", "DMAOB", 0xA454C09C, 32},
  {"SPDIF L-channel status receive information register", "SPLRI", 0xA454C0B8, 32},
  {"SPDIF R-channel status receive information register", "SPRRI", 0xA454C0BC, 32},
  {"SPDIF user receive information register", "SPURI", 0xA454C0C4, 32},
  {"SPDIF timing register", "SPTIS", 0xA454C0C8, 32},
  {"SPDIF status register", "SPSTS", 0xA454C0CC, 32},
  {"SPDIF control register", "SPCTL", 0xA454C0D0, 32},
  {"SPDIF interrupt information register", "SPIRI", 0xA454C0D4, 32},
  {"SPDIF Q code store register1", "SPQCF", 0xA454C0D8, 32},
  {"SPDIF Q code store register2", "SPQCS", 0xA454C0DC, 32},
  {"SPDIF Q code store register3", "SPQCT", 0xA454C0E0, 32},
  {"PCM data peak detection register", "DPEAK", 0xA454C0F0, 32},
  {"PCM data mute sampling period specification register", "DSLPD", 0xA454C0F4, 32},
  {"PCM data mute level specification register", "DSLLV", 0xA454C0F8, 32},
  {"BRGA baud rate generator select register", "BRGASEL", 0xA454C100, 32},
  {"BRGA baud rate set register", "BRRA", 0xA454C104, 32},
  {"BRGB baud rate generator select register", "BRGBSEL", 0xA454C108, 32},
  {"BRGB baud rate set register", "BRRB", 0xA454C10C, 32},
  {"ATAPI control register", "ATAPI_CONTROL1", 0xA4DA2180, 32},
  {"ATAPI status register", "ATAPI_STATUS", 0xA4DA2184, 32},
  {"Interrupt enable register", "ATAPI_INT_ENABLE", 0xA4DA2188, 32},
  {"Descriptor table base address register", "ATAPI_DTB_ADR", 0xA4DA2198, 32},
  {"DMA start address register", "ATAPI_DMA_START_ADR", 0xA4DA219C, 32},
  {"DMA transfer count register", "ATAPI_DMA_TRANS_CNT", 0xA4DA21A0, 32},
  {"ATAPI control 2 register", "ATAPI_CONTROL2", 0xA4DA21A4, 32},
  {"ATAPI signal status register", "ATAPI_SIG_ST", 0xA4DA21B0, 32},
  {"Byte swap register", "ATAPI_BYTE_SWAP", 0xA4DA21BC, 32},
  {"PIO timing 1 register", "ATAPI_PIO_TIMING1", 0xA4DA21C0, 32},
  {"PIO timing 2 register", "ATAPI_PIO_TIMING2", 0xA4DA21C4, 32},
  {"Multiword DMA timing register", "ATAPI_MULTI_TIMING", 0xA4DA21C8, 32},
  {"Ultra DMA timing register", "ATAPI_ULTRA_TIMING", 0xA4DA21CC, 32},
  {"Port A control register", "PACR", 0xA4050100, 16},
  {"Port B control register", "PBCR", 0xA4050102, 16},
  {"Port C control register", "PCCR", 0xA4050104, 16},
  {"Port D control register", "PDCR", 0xA4050106, 16},
  {"Port E control register", "PECR", 0xA4050108, 16},
  {"Port F control register", "PFCR", 0xA405010A, 16},
  {"Port G control register", "PGCR", 0xA405010C, 16},
  {"Port H control register", "PHCR", 0xA405010E, 16},
  {"Port J control register", "PJCR", 0xA4050110, 16},
  {"Port K control register", "PKCR", 0xA4050112, 16},
  {"Port L control register", "PLCR", 0xA4050114, 16},
  {"Port M control register", "PMCR", 0xA4050116, 16},
  {"Port N control register", "PNCR", 0xA4050118, 16},
  {"Port Q control register", "PQCR", 0xA405011A, 16},
  {"Port R control register", "PRCR", 0xA405011C, 16},
  {"Port S control register", "PSCR", 0xA405011E, 16},
  {"Port T control register", "PTCR", 0xA4050140, 16},
  {"Port U control register", "PUCR", 0xA4050142, 16},
  {"Port V control register", "PVCR", 0xA4050144, 16},
  {"Port W control register", "PWCR", 0xA4050146, 16},
  {"Port X control register", "PXCR", 0xA4050148, 16},
  {"Port Y control register", "PYCR", 0xA405014A, 16},
  {"Port Z control register", "PZCR", 0xA405014C, 16},
  {"Pin select registerA", "PSELA", 0xA405014E, 16},
  {"Pin select registerB", "PSELB", 0xA4050150, 16},
  {"Pin select registerC", "PSELC", 0xA4050152, 16},
  {"Pin select registerD", "PSELD", 0xA4050154, 16},
  {"I/O buffer Hi-Z control registerA", "HIZCRA", 0xA4050158, 16},
  {"I/O buffer Hi-Z control registerB", "HIZCRB", 0xA405015A, 16},
  {"I/O buffer Hi-Z control registerC", "HIZCRC", 0xA405015C, 16},
  {"I/O buffer Hi-Z control registerD", "HIZCRD", 0xA405015E, 16},
  {"Module function select registerA", "MSELCRA", 0xA4050180, 16},
  {"Module function select registerB", "MSELCRB", 0xA4050182, 16},
  {"Pull-up/pull-down control register", "PULCR", 0xA4050184, 16},
  {"I/O buffer drive control registerA", "DRVCRA", 0xA405018A, 16},
  {"I/O buffer drive control registerB", "DRVCRB", 0xA405018C, 16},
  {"Port A data register", "PADR", 0xA4050120, 8},
  {"Port B data register", "PBDR", 0xA4050122, 8},
  {"Port C data register", "PCDR", 0xA4050124, 8},
  {"Port D data register", "PDDR", 0xA4050126, 8},
  {"Port E data register", "PEDR", 0xA4050128, 8},
  {"Port F data register", "PFDR", 0xA405012A, 8},
  {"Port G data register", "PGDR", 0xA405012C, 8},
  {"Port H data register", "PHDR", 0xA405012E, 8},
  {"Port J data register", "PJDR", 0xA4050130, 8},
  {"Port K data register", "PKDR", 0xA4050132, 8},
  {"Port L data register", "PLDR", 0xA4050134, 8},
  {"Port M data register", "PMDR", 0xA4050136, 8},
  {"Port N data register", "PNDR", 0xA4050138, 8},
  {"Port Q data register", "PQDR", 0xA405013A, 8},
  {"Port R data register", "PRDR", 0xA405013C, 8},
  {"Port S data register", "PSDR", 0xA405013E, 8},
  {"Port T data register", "PTDR", 0xA4050160, 8},
  {"Port U data register", "PUDR", 0xA4050162, 8},
  {"Port V data register", "PVDR", 0xA4050164, 8},
  {"Port W data register", "PWDR", 0xA4050166, 8},
  {"Port X data register", "PXDR", 0xA4050168, 8},
  {"Port Y data register", "PYDR", 0xA405016A, 8},
  {"Port Z data register", "PZDR", 0xA405016C, 8},
  {"A/D data registerA", "ADDRA", 0xA4610000, 16},
  {"A/D data registerB", "ADDRB", 0xA4610002, 16},
  {"A/D data registerC", "ADDRC", 0xA4610004, 16},
  {"A/D data registerD", "ADDRD", 0xA4610006, 16},
  {"A/D control/status register", "ADCSR", 0xA4610008, 16},
  {"Match condition setting register0", "CBR0", 0xFF200000, 32},
  {"Match operation setting register0", "CRR0", 0xFF200004, 32},
  {"Match address setting register0", "CAR0", 0xFF200008, 32},
  {"Match address mask setting register0", "CAMR0", 0xFF20000C, 32},
  {"Match condition setting register1", "CBR1", 0xFF200020, 32},
  {"Match operation setting register1", "CRR1", 0xFF200024, 32},
  {"Match address setting register1", "CAR1", 0xFF200028, 32},
  {"Match address mask setting register1", "CAMR1", 0xFF20002C, 32},
  {"Match data setting register1", "CDR1", 0xFF200030, 32},
  {"Match data mask setting register1", "CDMR1", 0xFF200034, 32},
  {"Execution count break register1", "CETR1", 0xFF200038, 32},
  {"Channel match flag register", "CCMFR", 0xFF200600, 32},
  {"Break control register", "CBCR", 0xFF200620, 32},
  {"Instruction register", "SDIR", 0xFC110000, 16},
  {"Data registerH", "SDDR/SDDRH", 0xFC110008, 32},
  {"Data registerL", "SDDRL", 0xFC11000A, 16},
  {"Interrupt source register", "SDINT", 0xFC110018, 16}
};
#endif


// prototypes
static int hl_gen_open (struct inode *inode, struct file *file);
static ssize_t hl_gen_read (struct file *file, 
                            char *buf, 
                            size_t count,
                            loff_t *offset);
static long hl_gen_ioctl (struct file *file,
			  unsigned int cmd, unsigned long arg);

#ifdef MODULE
extern int init_module(void);
extern int cleanup_module(void);
static int __init hl_gen_init(void);
#else
extern int __init hl_gen_init(void);
#endif

static struct file_operations hl_gen_fops = {
unlocked_ioctl: hl_gen_ioctl,
read:           hl_gen_read,
open:           hl_gen_open,
};

static ssize_t hl_gen_read (struct file *file, 
                            char *buf, 
                            size_t count,
                            loff_t *offset)
{
  memset(buf,0,2); // currently not used
  // memcpy(buf,(char*)NMIFCR,2); // NMIFCR length = 16bit
  return 2;
}

#ifdef DBG_SH4_REGS
void Dbg_SH4_Regs(void){
  int regCnt = sizeof(regArray)/sizeof(struct reg_t);
  int iReg;
  u32 regVal;
  for (iReg=0; iReg<regCnt; iReg++){
    if (regArray[iReg].regSiz == 8)
      regVal = __raw_readb(regArray[iReg].regAddr);
    else if (regArray[iReg].regSiz == 16)
      regVal = __raw_readw(regArray[iReg].regAddr);
    else
      regVal = __raw_readl(regArray[iReg].regAddr);
    printk("%10s 0x%08x [%02d] (%s)\n",regArray[iReg].regName, regVal, 
	   regArray[iReg].regSiz, regArray[iReg].regDef); 
    mdelay(10);
  }
}
#else
void Dbg_SH4_Regs(void){}
#endif

static long hl_gen_ioctl (struct file *file,
			  unsigned int cmd, unsigned long arg)
{
  unsigned short sTmpPWCR, sTmpHIZCRD;
  unsigned short sTmpPSCR, sTmpHIZCRB;
  
  switch(cmd)
  {
    case HL_GEN_INIT:
      /* read registers */
      /******************/
      sTmpPWCR   = __raw_readw(PWCR);
      sTmpHIZCRD = __raw_readw(HIZCRD);
      sTmpPSCR   = __raw_readw(PSCR);
      sTmpHIZCRB = __raw_readw(HIZCRB);

      /* set values */
      /**************/
      printk("HL_GEN: set PTS5 (camera reset) to output\n");
      sTmpPSCR &= ~0x0C00;   // init to 0
      sTmpPSCR |=  0x0400;   // PTS5: PSCR bit10,11: output=01
      sTmpHIZCRB &= ~0x0008; // PTS5: HIZCRB bit3: i/o buffer operates normally=0

      printk("HL_GEN: set PTS6 (exposure) to output\n");
      sTmpPSCR &= ~0x3000;   // init to 0
      sTmpPSCR |=  0x1000;   // PTS6: PSCR bit12,13: output=01
      sTmpHIZCRB &= ~0x0004; // PTS6: HIZCRB bit2: i/o buffer operates normally=0

      printk("HL_GEN: set PTW0 (LED B - green) to output\n");
      sTmpPWCR &= ~0x0003;   // init to 0
      sTmpPWCR |=  0x0001;   // PTW0: PWCR bit0,1: output=01
      sTmpHIZCRD &= ~0x0100; // PTW0: HIZCRD bit8: i/o buffer operates normally=0

      printk("HL_GEN: set PTW1 (LED A - yellow) to output\n");
      sTmpPWCR &= ~0x000C;   // init to 0
      sTmpPWCR |=  0x0004;   // PTW1: PWCR bit2,3: output=01
      sTmpHIZCRD &= ~0x0200; // PTW1: HIZCRD bit9: i/o buffer operates normally=0

      printk("HL_GEN: set PTW4 (IRQ A) to input\n");
      sTmpPWCR |= 0x0300;    // PTW4: PWCR bit8,9: input=11      

      /* write back */
      /**************/
      __raw_writew(sTmpPWCR,  PWCR);
      __raw_writew(sTmpHIZCRD,HIZCRD);
      __raw_writew(sTmpPSCR,  PSCR);
      __raw_writew(sTmpHIZCRB,HIZCRB);
      
      /* test */
      /********/
      /* printk("PTS6: PSCR[13:12]=%x, HIZB[2]=%x\n",  */
      /*        (__raw_readw(PSCR)>>12)&0x3, (__raw_readw(HIZCRB)>>2)&0x1); */
      /* printk("PTS5: PSCR[11:10]=%x, HIZB[3]=%x\n",  */
      /*        (__raw_readw(PSCR)>>10)&0x3, (__raw_readw(HIZCRB)>>3)&0x1); */
      /* printk("PTW0: PWCR[1:0]=%x, PSELD[7:6]=%x, HIZD[8]=%x, DRVA[11:10]=%x\n", */
      /*        __raw_readw(PWCR)&0x3,(__raw_readw(PSELD)>>6)&0x3, */
      /*        (__raw_readw(HIZCRD)>>8)&0x1, (__raw_readw(DRVCRA)>>10)&0x3); */
      
#ifdef DBG_SH4_REGS
      Dbg_SH4_Regs();
#endif
      break;
      
    default:
      return -EINVAL;
      break;
  }
  return 0;
}

static int hl_gen_open (struct inode *inode, struct file *file)
{
  return 0;
}

int __init hl_gen_init(void)
{
  printk("HL_GEN init\n");
  
  if (register_chrdev(HL_GEN_MAJOR,"hl_gen",&hl_gen_fops)) 
  {
    printk("HL_GEN: unable to get major %d for HL_GEN\n",
           HL_GEN_MAJOR);
    return -EIO;
  }
  return 0;
}

int hl_gen_cleanup(void)
{
  unregister_chrdev(HL_GEN_MAJOR, "hl_gen");
  return 0;
}

int __init hl_gen_module_init(void)
{
  return hl_gen_init();
}

static void __exit hl_gen_module_exit(void)
{
  hl_gen_cleanup();
  return;
}

module_init(hl_gen_module_init);
module_exit(hl_gen_module_exit);
