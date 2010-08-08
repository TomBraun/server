/*
Copyright (c) 1998-2010, Enno Rehling <enno@eressea.de>
                         Katja Zedel <katze@felidae.kn-bremen.de
                         Christian Schlittchen <corwin@amber.kn-bremen.de>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**/

#include <platform.h>
#include <kernel/config.h>
#include "oceannames.h"

/* kernel includes */
#include <kernel/region.h>
#include <kernel/faction.h>

/* util includes */
#include <util/attrib.h>

/* libc includes */
#include <stdlib.h>
#include <string.h>

typedef struct namehash {
	struct namehash * next;
	const char * name;
} namehash;

#define NMAXHASH 1023
namehash names[NMAXHASH];

void nhash(const char * name);

typedef struct oceanname {
	struct oceanname * next;
	struct faction_list * factions;
	const char * name;
} oceanname;

static void 
free_names(attrib * a)
{
	oceanname * data = (oceanname*)a->data.v;
	while (a->data.v) {
		a->data.v = data->next;
		free(data);
	}
}

struct attrib_type at_oceanname = { "names", NULL, free_names, NULL/*, write_names, read_names, ATF_UNIQUE */};

const char * 
get_oceanname(const struct region * r, const struct faction * f)
{
	attrib * a = a_find(r->attribs, &at_oceanname);
	if (a) {
		oceanname * names = (oceanname*)a->data.v;
		while (names) {
			faction_list * fl = names->factions;
			while (fl) {
				if (fl->data==f) return names->name;
				fl=fl->next;
			}
			names = names->next;
		}
	}
	return NULL;
}

void
nameocean(struct region *r, struct faction * f, const char * newname)
{
	attrib * a = a_find(r->attribs, &at_oceanname);
	if (!a && newname) a = a_add(&r->attribs, a_new(&at_oceanname));
	if (a) {
		faction_list **oldf = NULL, **newf = NULL;
		faction_list * fl = NULL;
		oceanname * names = (oceanname*)a->data.v;
		while ((names && (!newf && newname)) || !oldf) {
			faction_list ** fli = &names->factions;
			if (oldf==NULL) while (*fli) {
				if ((*fli)->data==f) {
					oldf = fli;
					break;
				}
				fli=&(*fli)->next;
			}
			if (newname && !newf && !strcmp(names->name, newname)) {
				newf = fli;
			}
			names = names->next;
		}

		if (oldf) {
			fl = *oldf;
			*oldf = fl->next;
		} else if (newname) {
			fl = calloc(1, sizeof(faction_list));
		}

		if (newf) {
			fl->data = f;
			fl->next = *newf;
			*newf = fl;
		} else if (newname) {
			oceanname * nm = calloc(1, sizeof(oceanname));
			nm->factions = fl;
			fl->data = f;
			fl->next = NULL;
			nm->next = (oceanname*)a->data.v;
			a->data.v = nm;
		} else if (fl) {
			free(fl);
		}
	}
}
