/*
 * dex/parse.c
 * 
 * Copyright (C) University of Edinburgh 2013
 * Tom Spink <t.spink@sms.ed.ac.uk>
 */
#include <dvt.h>

struct dex_section_ptr {
	unsigned int size;
	unsigned int offset;
};

#define ENDIAN_TAG 0x12345678

struct dex_header {
	unsigned char magic[8];		// 64 65 78 0a 30 33 35 00
	unsigned int checksum;
	unsigned char signature[20];
	unsigned int file_size;
	unsigned int header_size;	// 0x70
	unsigned int endian_tag;	// 0x78563412

	struct dex_section_ptr link;
	unsigned int map_offset;
	struct dex_section_ptr string_ids;
	struct dex_section_ptr type_ids;
	struct dex_section_ptr proto_ids;
	struct dex_section_ptr field_ids;
	struct dex_section_ptr method_ids;
	struct dex_section_ptr class_defs;
	struct dex_section_ptr data;
};

struct dex_string {
	unsigned int offset;
};

struct dex_method {
	unsigned short class_index;
	unsigned short method_desc_string_index;
	unsigned int method_name_string_index;
};

struct dex_proto {
	unsigned int shorty_index;
	unsigned int return_type_index;
	unsigned int params_offset;
};

inline char read_s8(char **p)
{
	char r = **p;
	*p += sizeof(r);
	
	return r;
}

inline short read_s16(char **p)
{
	short r = *(short *)*p;
	*p += sizeof(r);
	
	return r;
}

inline int read_s32(char **p)
{
	int r = *(int *)*p;
	*p += sizeof(r);
	
	return r;
}

inline unsigned int read_uleb128(char **pp)
{
	unsigned int total = 0;
	char *p = *pp;
	int i = 0;
	
	do {
		total |= (*p & 0x7F) << (i * 7);
	} while (i++ < 5 && (*p++ & 0x80));

	*pp = *pp + i;
	
	return total;
}

static inline int valid_magic(struct dex_header *header)
{
	return 1;
}

static char *dex_get_string(struct dex_header *header, unsigned int index)
{
	struct dex_string *string_table = (struct dex_string *)((char *)header + header->string_ids.offset);
	char *string_item;
	unsigned int length;
	
	if (index >= header->string_ids.size)
		return NULL;
	
	string_item = (char *)header + string_table[index].offset;
	
	length = read_uleb128(&string_item);
	//dbg("length: %d\n", length);
	
	return string_item;
}

static int dex_read_methods(struct dex_header *header)
{
	struct dex_method *method = (struct dex_method *)((char *)header + header->method_ids.offset);
	int i;
	
	for (i = 0; i < header->method_ids.size; i++) {
		dbg("method: %s\n", dex_get_string(header, method[i].method_name_string_index));
	}
	
	return -1;
}

int dvt_parse_dex(struct dvt *dvt, struct dex_file *file)
{
	struct dex_header *header = (struct dex_header *)file->raw.base;

	if (!valid_magic(header)) {
		err("dex: invalid magic number\n");
		return -1;
	}
	
	if (header->header_size != sizeof(*header)) {
		err("dex: header size incorrect\n");
		return -1;
	}
	
	if (header->endian_tag != ENDIAN_TAG) {
		err("dex: unsupported endianness\n");
		return -1;
	}

	dbg("checksum: %x, file_size: %x, header_size: %x (%x), endian: %x\n", header->checksum, header->file_size, header->header_size, sizeof(*header), header->endian_tag);

	return dex_read_methods(header);
}
