#	$NetBSD: Makefile,v 1.10 2019/06/04 08:45:17 hannken Exp $

.include <bsd.init.mk>

.PATH:  ${.CURDIR}/../../dist/lib/libnvpair
.PATH:  ${.CURDIR}/../../dist/common/nvpair

CPPFLAGS+=	-I${OSNETDIR}/sys \
		-I${OPENSOLARIS_USR_DISTDIR}/head \
		-I${OPENSOLARIS_SYS_DISTDIR}/common

CPPFLAGS+=	-Wall \
		-Wno-unknown-pragmas \
		-Wno-shadow \
		-Wno-format-security \
		-Wno-format-nonliteral \
		-Wno-cast-qual

LIB=	nvpair

SRCS=	libnvpair.c nvpair_alloc_system.c nvpair_alloc_fixed.c nvpair.c fnvpair.c nvpair_json.c

.include "../../Makefile.zfs"
.include <bsd.lib.mk>
