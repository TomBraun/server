/* vi: set ts=2:
 +-------------------+  Christian Schlittchen <corwin@amber.kn-bremen.de>
 |                   |  Enno Rehling <enno@eressea-pbem.de>
 | Eressea PBEM host |  Katja Zedel <katze@felidae.kn-bremen.de>
 | (c) 1998 - 2001   |  Henning Peters <faroul@beyond.kn-bremen.de>
 |                   |  Ingo Wilken <Ingo.Wilken@informatik.uni-oldenburg.de>
 +-------------------+  Stefan Reich <reich@halbling.de>

 This program may not be used, modified or distributed 
 without prior permission by the authors of Eressea.
 $Id: graph.h,v 1.1 2001/04/11 17:28:07 corwin Exp $
 */

#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
	int  marker;
	void *object;
} node;

typedef struct {
	int  marker;
	node *node1;
	node *node2;
} vertex;

typedef struct {
	vset *nodes;
	vset *vertices;
} graph;

#endif
