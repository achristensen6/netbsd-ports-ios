/*	 $NetBSD: rasops32.c,v 1.41 2019/07/31 02:04:14 rin Exp $	*/

/*-
 * Copyright (c) 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Andrew Doran.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: rasops32.c,v 1.41 2019/07/31 02:04:14 rin Exp $");

#include "opt_rasops.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/time.h>

#include <dev/wscons/wsdisplayvar.h>
#include <dev/wscons/wsconsio.h>

#define	_RASOPS_PRIVATE
#include <dev/rasops/rasops.h>

static void 	rasops32_putchar(void *, int, int, u_int, long);
static void 	rasops32_putchar_aa(void *, int, int, u_int, long);
#ifndef RASOPS_SMALL
static void	rasops32_putchar8(void *, int, int, u_int, long);
static void	rasops32_putchar12(void *, int, int, u_int, long);
static void	rasops32_putchar16(void *, int, int, u_int, long);
static void	rasops32_makestamp(struct rasops_info *, long);
#endif

/*
 * offset = STAMP_SHIFT(fontbits, nibble #) & STAMP_MASK
 * destination uint32_t[0] = STAMP_READ(offset)
 * destination uint32_t[1] = STAMP_READ(offset +  4)
 * destination uint32_t[2] = STAMP_READ(offset +  8)
 * destination uint32_t[3] = STAMP_READ(offset + 12)
 */
#define	STAMP_SHIFT(fb, n)	((n) ? (fb) : (fb) << 4)
#define	STAMP_MASK		(0xf << 4)
#define	STAMP_READ(o)		(*(uint32_t *)((uint8_t *)stamp + (o)))

/*
 * Initialize a 'rasops_info' descriptor for this depth.
 */
void
rasops32_init(struct rasops_info *ri)
{

	if (ri->ri_rnum == 0) {
		ri->ri_rnum = ri->ri_gnum = ri->ri_bnum = 8;

		ri->ri_rpos = 0;
		ri->ri_gpos = 8;
		ri->ri_bpos = 16;
	}

	if (FONT_IS_ALPHA(ri->ri_font)) {
		ri->ri_ops.putchar = rasops32_putchar_aa;
		return;
	}

	switch (ri->ri_font->fontwidth) {
#ifndef RASOPS_SMALL
	case 8:
		ri->ri_ops.putchar = rasops32_putchar8;
		break;
	case 12:
		ri->ri_ops.putchar = rasops32_putchar12;
		break;
	case 16:
		ri->ri_ops.putchar = rasops32_putchar16;
		break;
#endif
	default:
		ri->ri_ops.putchar = rasops32_putchar;
		return;
	}

#ifndef RASOPS_SMALL
	rasops_allocstamp(ri, sizeof(uint32_t) * 64);
#endif
}

#define	RASOPS_DEPTH	32
#include "rasops_putchar.h"
#include "rasops_putchar_aa.h"

#ifndef RASOPS_SMALL
/*
 * Recompute the blitting stamp.
 */
static void
rasops32_makestamp(struct rasops_info *ri, long attr)
{
	uint32_t *stamp = (uint32_t *)ri->ri_stamp;
	uint32_t fg, bg;
	int i;

	fg = ri->ri_devcmap[((uint32_t)attr >> 24) & 0xf];
	bg = ri->ri_devcmap[((uint32_t)attr >> 16) & 0xf];
	ri->ri_stamp_attr = attr;

	for (i = 0; i < 64; i += 4) {
		stamp[i + 0] = i & 32 ? fg : bg;
		stamp[i + 1] = i & 16 ? fg : bg;
		stamp[i + 2] = i &  8 ? fg : bg;
		stamp[i + 3] = i &  4 ? fg : bg;
	}
}

#define	RASOPS_WIDTH	8
#include "rasops_putchar_width.h"
#undef	RASOPS_WIDTH

#define	RASOPS_WIDTH	12
#include "rasops_putchar_width.h"
#undef	RASOPS_WIDTH

#define	RASOPS_WIDTH	16
#include "rasops_putchar_width.h"
#undef	RASOPS_WIDTH

#endif /* !RASOPS_SMALL */
