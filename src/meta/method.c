/*
 * meta/method.c
 *
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <dvt.h>

struct dvt_method *dvt_find_method(struct dvt_class *class, const char *sig, find_flags_t flags)
{
	struct dvt_method *m;

	foreach(m, class->methods) {
		dbg("method %s\n", m->name);
	}

	return NULL;
}

struct dvt_method *dvt_install_method(struct dvt_class *class, char *name, int flags)
{
	struct dvt_method *method;

	method = dvt_alloc(sizeof(*method), DVT_ALLOC_ZERO);
	if (!method)
		return NULL;

	method->class = class;
	method->name = name;
	method->flags = flags;

	ll_insert_head(&class->methods, method);
}

void *dvt_invoke(struct dvt_method *method, void **args)
{
	return NULL;
}
