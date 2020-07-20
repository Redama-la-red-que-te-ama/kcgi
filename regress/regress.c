/*	$Id$ */
/*
 * Copyright (c) 2014 Kristaps Dzonsons <kristaps@bsd.lv>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "../config.h"

#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

#include <ctype.h>
#if HAVE_ERR
# include <err.h>
#endif
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <curl/curl.h>

#include "../kcgiregress.h"
#include "regress.h"

static int
dochild(void *arg)
{
	cb_child	child = arg;

	return child();
}

static int
doparent(void *arg)
{
	CURL		*curl;
	cb_parent	 parent = arg;
	int		 rc;

	if ((curl = curl_easy_init()) == NULL)
		errx(1, "curl_easy_init");

	rc = parent(curl);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return rc;
}

int
regress_cgi(cb_parent parent, cb_child child)
{

	return kcgi_regress_cgi(doparent, parent, dochild, child);
}

int
regress_fcgi(cb_parent parent, cb_child child)
{

	return kcgi_regress_fcgi(doparent, parent, dochild, child);
}
