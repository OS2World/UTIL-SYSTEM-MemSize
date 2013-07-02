/* 
 * This file is based on the datasheet for the National Semiconductor LM78
 * chip. The datasheet is available from
 * <URL:http://www.national.com/pf/LM/LM78.html>
 *
 *   bjorn@mork.no
 */

#ifndef LM78_H
#define LM78_H
#include <sys/hw.h>

/* port numbers */
#define LM78_BASE	0x290
#define LM78_ADDRESS	(LM78_BASE+0x05)
#define LM78_DATA	(LM78_BASE+0x06)

/* operations - must be changed for other operating systems! */
#ifdef __EMX__
#define LM78_INIT		_portaccess(LM78_BASE,LM78_DATA)	/* enabling access to the range of ports */
#define LM78_EXIST		({_outp8(LM78_ADDRESS,LM78_IN0);_inp8(LM78_DATA);_inp8(LM78_ADDRESS)==LM78_IN0;})
#define LM78_BUSY		(_inp8(LM78_ADDRESS)>>7)
#define LM78_READ(addr) 	({_outp8(LM78_ADDRESS,addr);_inp8(LM78_DATA);})
#define LM78_WRITE(addr,data) 	_outp8(LM78_ADDRESS,addr);_outp8(LM78_DATA,data)
#endif

/* internal addresses */
#define LM78_IN0	0x20
#define LM78_IN1	0x21
#define LM78_IN2	0x22
#define LM78_IN3	0x23
#define LM78_IN4	0x24
#define LM78_IN5_NEG	0x25
#define LM78_IN6_NEG	0x26
#define LM78_TEMP	0x27
#define LM78_FAN1	0x28
#define LM78_FAN2	0x29
#define LM78_FAN3	0x2A
#define LM78_IN0_HIGH	0x2B
#define LM78_IN0_LOW	0x2C
#define LM78_IN1_HIGH	0x2D
#define LM78_IN1_LOW	0x2E
#define LM78_IN2_HIGH	0x2F
#define LM78_IN2_LOW	0x30
#define LM78_IN3_HIGH	0x31
#define LM78_IN3_LOW	0x32
#define LM78_IN4_HIGH	0x33
#define LM78_IN4_LOW	0x34
#define LM78_IN5_HIGH	0x35
#define LM78_IN5_LOW	0x36
#define LM78_IN6_HIGH	0x37
#define LM78_IN6_LOW	0x38
#define LM78_TEMP_HIGH	0x39
#define LM78_TEMP_LOW	0x3A
#define LM78_FAN1_LIMIT	0x3B
#define LM78_FAN2_LIMIT	0x3C
#define LM78_FAN3_LIMIT	0x3D
/* reserved	0x3E */
/* reserved	0x3F */
#define LM78_CONFIG	0x40	/* bit mapped register */
#define LM78_STATUS1	0x41	/* read only bit mapped register */
#define LM78_STATUS2	0x42	/* read only bit mapped register */
#define LM78_SMI_MASK1	0x43	/* bit mapped register */
#define LM78_SMI_MASK2	0x44	/* bit mapped register */
#define LM78_NMI_MASK1	0x45	/* bit mapped register */
#define LM78_NMI_MASK2	0x46	/* bit mapped register */
#define LM78_FANDIV	0x47	/* bit mapped register */
#define LM78_SERIAL	0x48	/* bit mapped register */
#define LM78_RESET	0x49	/* bit mapped register */

/* conversion routines */
#define LM78_FANDIV1	(1<<((LM78_READ(LM78_FANDIV)&0x30)>>4))	/* bits 4 and 5 */
#define LM78_FANDIV2	(1<<(LM78_READ(LM78_FANDIV)>>6))	/* bits 6 and 7 */
#define LM78_FANDIV3	2					/* constant */
#define LM78_RPM(count,fandiv)	(1350000/(count*fandiv))

#endif /* LM78 */
