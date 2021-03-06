/*	$NetBSD: get_alias.c,v 1.1.1.2 2017/06/08 15:59:26 skrll Exp $	*/

/*
 * libfdt - Flat Device Tree manipulation
 *	Testcase for fdt_get_alias()
 * Copyright (C) 2006 David Gibson, IBM Corporation.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <libfdt.h>

#include "tests.h"
#include "testdata.h"

static void check_alias(void *fdt, const char *path, const char *alias)
{
	const char *aliaspath;

	aliaspath = fdt_get_alias(fdt, alias);

	if (path && !aliaspath)
		FAIL("fdt_get_alias(%s) failed\n", alias);

	if (strcmp(aliaspath, path) != 0)
		FAIL("fdt_get_alias(%s) returned %s instead of %s\n",
		     alias, aliaspath, path);
}

int main(int argc, char *argv[])
{
	void *fdt;

	test_init(argc, argv);
	fdt = load_blob_arg(argc, argv);

	check_alias(fdt, "/subnode@1", "s1");
	check_alias(fdt, "/subnode@1/subsubnode", "ss1");
	check_alias(fdt, "/subnode@1/subsubnode/subsubsubnode", "sss1");

	PASS();
}
