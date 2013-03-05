/*
 * meta/class.c
 *
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <dvt.h>

struct dvt_class *dvt_find_class(struct dvt *dvt, const char *name)
{
	struct dvt_class *c;

	foreach(c, dvt->classes) {
		if (strcmp(c->name, name) == 0)
			return c;
	}

	return NULL;
}

struct dvt_class *dvt_install_class(struct dvt *dvt, struct dvt_class *base, char *name)
{
	struct dvt_class *class;

	class = dvt_alloc(sizeof(*class), DVT_ALLOC_ZERO);
	if (!class)
		return NULL;

	ll_insert_head(&dvt->classes, class);

	class->ctx = dvt;
	class->base = base;
	class->name = name;

	return class;
}
