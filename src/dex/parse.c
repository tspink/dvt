/*
 * dex/parse.c
 *
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <dvt.h>
#include "dex.h"

static inline int valid_magic(struct dex_header *header)
{
	const char magic[] = {0x64, 0x65, 0x78, 0x0a, 0x30, 0x33, 0x35, 0x00};
	int i;

	for (i = 0; i < sizeof(header->magic); i++) {
		if (header->magic[i] != magic[i])
			return 0;
	}

	return 1;
}

static int load_types(struct dvt *dvt, struct dex_header *header)
{
	return 0;
}

static int load_class(struct dvt *dvt, struct dex_header *header, int index)
{
	struct class_def_item *class = get_class_def_item(header, index);
	struct type_id_item *type = get_type_id_item(header, class->class_idx);
	struct dvt_class *dvt_class;
	
	dbg("loading class: %s\n", get_string_data(header, type->descriptor_idx));
	
	dvt_class = dvt_install_class(dvt, NULL, get_string_data(header, type->descriptor_idx));
	if (!dvt_class)
		return -1;
	
	dvt_class->index = index;
	
	return 0;
}

static int load_classes(struct dvt *dvt, struct dex_header *header)
{
	int i, rc;

	for (i = 0; i < header->class_defs.size; i++) {
		rc = load_class(dvt, header, i);
		if (rc) {
			err("dex: failed to load class\n");
			return rc;
		}
	}

	return 0;
}

static struct dvt_class *class_from_index(struct dvt *dvt, int index)
{
	struct dvt_class *class;
	
	foreach(class, dvt->classes) {
		if (class->index == index)
			return class;
	}
	
	return NULL;
}

static int load_method(struct dvt *dvt, struct dex_header *header, int index)
{
	struct method_id_item *method = get_method_id_item(header, index);
	struct dvt_method *dvt_method;
	struct dvt_class *container;
	
	dbg("method: %s\n", get_string_data(header, method->name_idx));
	
	container = class_from_index(dvt, method->class_idx);
	if (!container) {
		err("unable to locate containing class %d for method\n", method->class_idx);
		return 0;
	}
	
	dvt_method = dvt_install_method(container, get_string_data(header, method->name_idx), 0);
	return 0;
}

static int load_methods(struct dvt *dvt, struct dex_header *header)
{
	int i, rc;
	
	for (i = 0; i < header->method_ids.size; i++) {
		rc = load_method(dvt, header, i);
		if (rc) {
			err("dex: failed to load method\n");
			return rc;
		}
	}
	
	return 0;
}

int dvt_parse_dex(struct dvt *dvt, struct dex_file *file)
{
	struct dex_header *header = (struct dex_header *)file->raw.base;
	int rc;

	if (!valid_magic(header)) {
		err("dex: invalid magic number\n");
		return -1;
	}

	if (header->endian_tag != ENDIAN_TAG) {
		err("dex: unsupported endianness\n");
		return -1;
	}

	if (header->header_size != sizeof(*header)) {
		err("dex: header size incorrect\n");
		return -1;
	}

	if (header->file_size != file->raw.size) {
		err("dex: stated file size does not match loaded file size\n");
		return -1;
	}

	dbg("checksum: %x, file_size: %x (%x), header_size: %x (%x), endian: %x\n", header->checksum, header->file_size, file->raw.size, header->header_size, sizeof(*header), header->endian_tag);

	rc = load_types(dvt, header);
	if (rc) {
		err("dex: unable to load type descriptors\n");
		return rc;
	}
	
	rc = load_classes(dvt, header);
	if (rc) {
		err("dex: unable to load classes\n");
		return rc;
	}
	
	rc = load_methods(dvt, header);
	if (rc) {
		err("dex: unable to load methods\n");
		return rc;
	}

	return 0;
}
