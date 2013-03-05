/*
 * main.c
 *
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 *
 */
#include <stdio.h>
#include <dvt.h>

int main(int argc, char **argv)
{
	struct dvt *context;
	struct dvt_class *main_class;
	struct dvt_method *main_method;
	void *result;

	int rc = 0;

	printf("DVT - Dalvik Translator\n");
	printf("Copyright (C) University of Edinburgh 2013\n\n");

	if (argc != 3) {
		err("usage: %s <dex-file> <main-class>\n", argv[0]);
		return -1;
	}

	dbg("creating context...\n");
	context = dvt_create_context();
	if (!context) {
		err("unable to create dvt context\n");
		return -1;
	}

	dbg("loading dex file...\n");
	rc = dvt_dex_load_file(context, argv[1]);
	if (rc) {
		err("error whilst loading dex file\n");
		goto out_destroy;
	}

	dbg("locating main class...\n");
	main_class = dvt_find_class(context, argv[2]);
	if (!main_class) {
		err("error whilst locating main class\n");
		goto out_destroy;
	}

	dbg("locating main method...\n");
	main_method = dvt_find_method(main_class, "int main(string[])", F_STATIC);
	if (!main_method) {
		err("error whilst locating main method\n");
		goto out_destroy;
	}

	dbg("invoking main method...\n");
	result = dvt_invoke(main_method, NULL);
	rc = (int)result;

out_destroy:
	dbg("destroying context...\n");
	dvt_destroy(context);

	return rc;
}
