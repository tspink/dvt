#ifndef __DEX_H__
#define __DEX_H__

struct dex_table {
	unsigned int size;
	unsigned int offset;
};

#define ENDIAN_TAG 0x12345678

struct dex_header {
	unsigned char magic[8];		// 64 65 78 0a 30 33 35 00
	unsigned int checksum;
	unsigned char signature[20];
	unsigned int file_size;
	unsigned int header_size;	// 0x70 == sizeof(struct dex_header)
	unsigned int endian_tag;	// ENDIAN_TAG

	struct dex_table link;
	unsigned int map_offset;
	struct dex_table string_ids;
	struct dex_table type_ids;
	struct dex_table proto_ids;
	struct dex_table field_ids;
	struct dex_table method_ids;
	struct dex_table class_defs;
	struct dex_table data;
};

struct string_id_item {
	unsigned int off;
};

struct type_id_item {
	unsigned int descriptor_idx;
};

struct proto_id_item {
	unsigned int shorty_idx;
	unsigned int return_type_idx;
	unsigned int params_off;
};

struct field_id_item {
	unsigned short class_idx;
	unsigned short type_idx;
	unsigned int name_idx;
};

struct method_id_item {
	unsigned short class_idx;
	unsigned short proto_idx;
	unsigned int name_idx;
};

struct class_def_item {
	unsigned int class_idx;
	unsigned int access_flags;
	unsigned int superclass_idx;
	unsigned int interfaces_off;
	unsigned int source_file_idx;
	unsigned int annotations_off;
	unsigned int class_data_off;
	unsigned int static_values_off;
};

static inline unsigned int read_uleb128(char **pp)
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

static inline struct string_id_item *get_string_id_item(struct dex_header *header, unsigned int index)
{
	return &((struct string_id_item *)((char *)header + header->string_ids.offset))[index];
}

static inline struct type_id_item *get_type_id_item(struct dex_header *header, unsigned int index)
{
	return &((struct type_id_item *)((char *)header + header->type_ids.offset))[index];
}

static inline struct proto_id_item *get_proto_id_item(struct dex_header *header, unsigned int index)
{
	return &((struct proto_id_item *)((char *)header + header->proto_ids.offset))[index];
}

static inline struct field_id_item *get_field_id_item(struct dex_header *header, unsigned int index)
{
	return &((struct field_id_item *)((char *)header + header->field_ids.offset))[index];
}

static inline struct method_id_item *get_method_id_item(struct dex_header *header, unsigned int index)
{
	return &((struct method_id_item *)((char *)header + header->method_ids.offset))[index];
}

static inline struct class_def_item *get_class_def_item(struct dex_header *header, unsigned int index)
{
	return &((struct class_def_item *)((char *)header + header->class_defs.offset))[index];
}

static char *get_string_data(struct dex_header *header, unsigned int index)
{
	struct string_id_item *str;
	char *string_item;

	if (index >= header->string_ids.size)
		return NULL;

	str = get_string_id_item(header, index);
	string_item = (char *)header + str->off;

	read_uleb128(&string_item);
	return string_item;
}

#endif
