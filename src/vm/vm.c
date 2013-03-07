/*
 * dex/parse.c
 *
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <dvt.h>

static int init_root_object(struct dvt *dvt)
{
	struct dvt_class *root_class;
	
	root_class = dvt_alloc(sizeof(*root_class), DVT_ALLOC_ZERO);
	if (!root_class)
		return -1;
	
	root_class->base = NULL;
	root_class->ctx = dvt;
	root_class->index = 0;
	root_class->name = "Class";
	
	return 0;
}

int vm_init(struct dvt *dvt)
{
	init_root_object();
	
	return 0;
}

void vm_exit()
{
	//
}