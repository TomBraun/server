/* vi: set ts=2:
 *
 *	$Id: vset.c,v 1.3 2001/04/11 17:28:07 corwin Exp $
 *	Eressea PB(E)M host Copyright (C) 1998-2000
 *      Christian Schlittchen (corwin@amber.kn-bremen.de)
 *      Katja Zedel (katze@felidae.kn-bremen.de)
 *      Henning Peters (faroul@beyond.kn-bremen.de)
 *      Enno Rehling (enno@eressea-pbem.de)
 *      Ingo Wilken (Ingo.Wilken@informatik.uni-oldenburg.de)
 *
 *  based on:
 *
 * Atlantis v1.0  13 September 1993 Copyright 1993 by Russell Wallace
 * Atlantis v1.7                    Copyright 1996 by Alex Schr�der
 *
 * This program may not be used, modified or distributed without
 * prior permission by the authors of Eressea.
 * This program may not be sold or used commercially without prior written
 * permission from the authors.
 */

#include <config.h>
#include <stdlib.h>
#include "vset.h"

void
vset_init(vset * s)
{
	s->data = 0;
	s->size = 0;
	s->maxsize = 0;
}

void
vset_destroy(vset * s)
{
	if (s->data)
		free(s->data);
}

int
vset_erase(vset * s, void *item)
{
	size_t i;

	for (i = 0; i != s->size; ++i)
		if (s->data[i] == item) {
			s->size--;
			s->data[i] = s->data[s->size];
			return 1;
		}
	return 0;
}

unsigned int
vset_add(vset * s, void *item)
{
	size_t i;

	if (!s->data) {
		s->size = 0;
		s->maxsize = 4;
		s->data = calloc(4, sizeof(void *));
	}
	for (i = 0; i != s->size; ++i)
		if (s->data[i] == item)
			return i;
	if (s->size == s->maxsize) {
		s->maxsize *= 2;
		s->data = realloc(s->data, s->maxsize * sizeof(void *));
	}
	s->data[s->size] = item;
	++s->size;
	return s->size - 1;
}

void *
vset_pop(vset *s)
{
	if(s->size == 0) return NULL;
	s->size--;
	return data[s->size+1];
}

int
vset_count(vset *s, void *item)
{
	size_t i;
	int c = 0;
	
	for(i = 0; i != s->size; ++i) {
		if(s->data[i] == item) c++;
	}

	return c;
}

void
vset_concat(vset *to, vset *from)
{
	size_t i;

	for(i=0; i != from->size; ++i) {
		vset_add(to, from->data[i]);
	}
}

