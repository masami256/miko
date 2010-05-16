#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/elf.h>
#include <mikoOS/string.h>
#include <mikoOS/kmalloc.h>

static const char test_file[] = "/home/masami/experiment/miko/test/test_program/hello";

struct string_tables {
	unsigned char *string_tbl;
	unsigned char *symbol_str_tbl;
};

struct section {
	unsigned char *data;
	size_t size;
};
struct sections {
	struct section *text;
	struct section *bss;
};

struct elf {
	Elf32_Ehdr e_hdr;
	Elf32_Phdr *p_hdr;
	Elf32_Shdr *s_hdr;
	Elf32_Sym  *sym;
	int sym_count;
	Elf32_Rel  *rel;
	Elf32_Rela *rela;
	struct string_tables str_table;
	struct sections section_data;
};

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////

static inline unsigned long get_program_table_size(const struct elf *data);
static inline unsigned long get_section_size(const struct elf *data);
static int read_section_header(struct elf *data, const unsigned char *file);
static int read_program_header(struct elf *data, const unsigned char *file);
static int read_string_table(struct elf *data, const unsigned char *file);
static int read_symbol_string_table(struct elf *data, const unsigned char *file);
static int read_text_section(struct elf *data, const unsigned char *file);
static int read_bss_section(struct elf *data, const unsigned char *file);
static struct section *read_section(struct elf *data, const unsigned char *file, const char *name);
static int is_elf(const struct elf *data);
static int read_header(struct elf *data, const unsigned char *file);
static int read_symbol_table(struct elf *data, const unsigned char *file);
static void get_section_name(const Elf32_Shdr *data, const unsigned char *table, char *buf);
static void get_symbol_string_name(const Elf32_Sym *data, const unsigned char *table, char *buf);
static const Elf32_Shdr *get_section_header(const struct elf *data, const char *section);
static void free_elf_data(struct elf *data);
static void free_string_tables(struct string_tables *tables);
static void free_section(struct section *section);
static void free_sections(struct sections *sections);

static void print_symbol_table(const struct elf *data, const unsigned char *file);
static void print_section_header(const struct elf *data, const unsigned char *file);
static void print_program_header(const struct elf *data);
static void print_header(const struct elf *data);
static void print_text_section(const struct elf *data);
static void print_bss_section(const struct elf *data);

static void free_string_tables(struct string_tables *tables)
{
	if (tables->string_tbl)
		kfree(tables->string_tbl);
	if (tables->symbol_str_tbl)
		kfree(tables->symbol_str_tbl);
}

static void free_section(struct section *section)
{
	if (!section)
		return ;
	if (section->data) 
		kfree(section->data);
	section->size = 0;
	kfree(section);
}

static void free_sections(struct sections *sections)
{
	free_section(sections->text);
	free_section(sections->bss);
}

static void free_elf_data(struct elf *data)
{
	if (!data)
		return ;

	if (data->p_hdr)
		kfree(data->p_hdr);
	if (data->s_hdr)
		kfree(data->s_hdr);
	if (data->sym)
		kfree(data->sym);
	if (data->rel)
		kfree(data->rel);
	if (data->rela)
		kfree(data->rela);
	
	free_string_tables(&data->str_table);
	free_sections(&data->section_data);

	kfree(data);
}


static const Elf32_Shdr *get_section_header(const struct elf *data, const char *section)
{
	const Elf32_Shdr *sym = NULL;
	unsigned char *str_tbl = data->str_table.string_tbl;
	int i;
	
	for (i = 0; i < data->e_hdr.e_shnum; i++) {
		const Elf32_Shdr *p = data->s_hdr + i;
		char buf[64] = { 0 };
		get_section_name(p, str_tbl, buf);
		if (!strcmp(buf, section)) {
			sym = p;
			break;
		}
	}

	return sym;

}

static inline unsigned long get_program_table_size(const struct elf *data)
{
	return data->e_hdr.e_phentsize * data->e_hdr.e_phnum;
}


static inline unsigned long get_section_size(const struct elf *data)
{
	return data->e_hdr.e_shentsize * data->e_hdr.e_shnum;
}

static int read_section_header(struct elf *data, const unsigned char *file)
{
	unsigned long section_size = get_section_size(data);
	
	// Is there any section header?
	if (!data->e_hdr.e_shnum)
		return 1;
	
	data->s_hdr = kmalloc(section_size);
	if (!data->s_hdr)
		return -1;
	
	memcpy(data->s_hdr, file + data->e_hdr.e_shoff, section_size);
	
	return 0;
}

static struct section *read_section(struct elf *data, const unsigned char *file, const char *name)
{
	const Elf32_Shdr *sym = get_section_header(data, name);
	struct section *ret;

	if (!sym)
		return NULL;

	ret = kmalloc(sizeof(*ret));
	ret->size = sym->sh_size;
	if (!sym->sh_size) {
		printk("%s section size is 0\n", name);
		return NULL;
	}

	ret->data = kmalloc(sym->sh_size);
	if (!ret->data)
		return NULL;

 	memcpy(ret->data, file + sym->sh_offset, sym->sh_size);

	return ret;
}

static int read_text_section(struct elf *data, const unsigned char *file)
{
	data->section_data.text = read_section(data, file, ".text");
	if (!data->section_data.text)
		return -1;

	return 0;
}

static int read_bss_section(struct elf *data, const unsigned char *file)
{
	data->section_data.bss = read_section(data, file, ".bss");
	if (!data->section_data.bss)
		return -1;

	return 0;
}


static int read_program_header(struct elf *data, const unsigned char *file)
{
	unsigned long size = get_program_table_size(data);
	// Is there program header.
	if (!data->e_hdr.e_phnum)
		return 1;
	
	data->p_hdr = kmalloc(size);
	if (!data->p_hdr)
		return -1;
	
	memcpy(data->p_hdr, file + data->e_hdr.e_phoff, size);
	
	return 0;
}

static int read_string_table(struct elf *data, const unsigned char *file)
{
	unsigned long offset = data->s_hdr[data->e_hdr.e_shstrndx].sh_offset;
	unsigned long size = data->s_hdr[data->e_hdr.e_shstrndx].sh_size;

	data->str_table.string_tbl = kmalloc(size);
	if (!data->str_table.string_tbl)
		return -1;

	memcpy(data->str_table.string_tbl, file + offset, size);

	return 0;
}

static int read_symbol_string_table(struct elf *data, const unsigned char *file)
{
	const Elf32_Shdr *sym = get_section_header(data, ".strtab");

	if (!sym)
		return -2;

	data->str_table.symbol_str_tbl = kmalloc(sym->sh_size);
	if (!data->str_table.symbol_str_tbl)
		return -1;

	memcpy(data->str_table.symbol_str_tbl, file + sym->sh_offset, sym->sh_size);
	return 0;
	
}


static int is_elf(const struct elf *data)
{
	if (data->e_hdr.e_ident[0] == 0x7f &&
		data->e_hdr.e_ident[1] == 'E' &&
		data->e_hdr.e_ident[2] == 'L' &&
		data->e_hdr.e_ident[3] == 'F')
		return 1;
		
	return 0;
}

static int read_header(struct elf *data, const unsigned char *file)
{
	memcpy(&data->e_hdr, file, sizeof(data->e_hdr));
	
	return is_elf(data) == 1 ? 0 : -1;
}

static int read_symbol_table(struct elf *data, const unsigned char *file)
{
	const Elf32_Shdr *sym = get_section_header(data, ".symtab");

	if (!sym)
		return -2;
	
	data->sym = kmalloc(sym->sh_size);
	if (!data->sym)
		return -1;

	memcpy(data->sym, file + sym->sh_offset, sym->sh_size);	
	data->sym_count = sym->sh_size / sizeof(Elf32_Sym);

	return 0;
}

static void get_section_name(const Elf32_Shdr *data, const unsigned char *table, char *buf)
{
	strcpy(buf, (const char *) table + data->sh_name);
}

static void get_symbol_string_name(const Elf32_Sym *data, const unsigned char *table, char *buf)
{
	strcpy(buf, (const char *) table + data->st_name);
}

#define MIKO_ELF_DEBUG 1
#ifdef MIKO_ELF_DEBUG 
static void print_text_section(const struct elf *data)
{
	size_t i;

	if (!data->section_data.text)
		return ;
	printk("[-------Text section size(0x%lx)-------]\n", data->section_data.text->size);

	for (i = 0; i < data->section_data.text->size; i++) {
		printk("0x%x ", data->section_data.text->data[i]);
		if (i != 0 && (i % 16) == 0)
			printk("\n");
	}
}
static void print_bss_section(const struct elf *data)
{
	size_t i;

	if (!data->section_data.bss)
		return ;

	printk("[-------BSS section size(0x%lx)-------]\n", data->section_data.bss->size);

	for (i = 0; i < data->section_data.bss->size; i++) {
		printk("0x%x ", data->section_data.bss->data[i]);
		if (i != 0 && (i % 16) == 0)
			printk("\n");
	}
}

static void print_symbol_table(const struct elf *data, const unsigned char *file)
{
	int i;
	const Elf32_Sym *p;
	unsigned char *sym_str_tbl = data->str_table.symbol_str_tbl;
	
	for (i = 0; i < data->sym_count; i++) {
		p = data->sym + i;
		char buf[128] = { 0 };
		get_symbol_string_name(p, sym_str_tbl, buf);
		printk("[-------Symbol Table %d-------]\nst_name: %s\nst_value: 0x%x\n" 
		       "st_size: 0x%x\nst_info: 0x%x\nst_other: 0x%x\nst_shndx: 0x%x\n",
		       i, buf, p->st_value,
		       p->st_size, p->st_info, p->st_other,
		       p->st_shndx);
	}

}

static void print_section_header(const struct elf *data, const unsigned char *file)
{
	int i;
	const unsigned char *str_tbl = data->str_table.string_tbl;

	printk("[-------Section Header-------]\nn:sh_name:sh_type:sh_flags:"
	       "sh_addr:sh_offset:sh_size:sh_link:"
	       "sh_info:sh_addralign:sh_entsize\n");
	       
	for (i = 0; i < data->e_hdr.e_shnum; i++) {
		const Elf32_Shdr *p = data->s_hdr + i;
		char buf[64] = { 0 };
		get_section_name(p, str_tbl, buf);
		printk("%d:%s:0x%x:0x%x:"
		       "0x%x:0x%x:0x%x:0x%x:"
		       "0x%x:0x%x:0x%x\n",
		       i, buf, p->sh_type, p->sh_flags, 
		       p->sh_addr, p->sh_offset, p->sh_size,
		       p->sh_link, p->sh_info, p->sh_addralign,
		       p->sh_entsize);
	}

}

static void print_program_header(const struct elf *data)
{
	int i;
	
	for (i = 0; i < data->e_hdr.e_phnum; i++) {
		const Elf32_Phdr *p = data->p_hdr + i;
		printk("[-------Program Header %d-------]\np_type: 0x%x\np_offset: 0x%x\n"
		       "p_vaddr: 0x%x\np_paddr: 0x%x\np_memsz: 0x%x\np_flags: 0x%x\np_align: 0x%x\n", 
			i, p->p_type, p->p_offset, p->p_vaddr,
			p->p_paddr, p->p_memsz, p->p_flags, p->p_align);
	}
}

static void print_header(const struct elf *data)
{
	printk("[-------ELF Header-------]\n");
	printk("EI_CLASS: 0x%x\n", data->e_hdr.e_ident[4]);
	printk("EI_DATA: 0x%x\n", data->e_hdr.e_ident[5]);
	printk("EI_VERSION: 0x%x\n", data->e_hdr.e_ident[6]);
	printk("EI_OSABI: 0x%x\n", data->e_hdr.e_ident[7]);
	
	printk("e_type: 0x%x\n", data->e_hdr.e_type);
	printk("e_machine: 0x%x\n", data->e_hdr.e_machine);
	printk("e_version: 0x%x\n", data->e_hdr.e_version);
	printk("e_entry: 0x%x\n", data->e_hdr.e_entry);
	printk("e_phoff: 0x%x\n", data->e_hdr.e_phoff);
	printk("e_shoff: 0x%x\n", data->e_hdr.e_shoff);
	printk("e_ehsize: 0x%x\n", data->e_hdr.e_ehsize);
	printk("e_phentsize: 0x%x\n", data->e_hdr.e_phentsize);
	printk("e_phnum: 0x%x\n", data->e_hdr.e_phnum);
	printk("e_shentsize: 0x%x\n", data->e_hdr.e_shentsize);
	printk("e_shnum: 0x%x\n", data->e_hdr.e_shnum);
	printk("e_shstrndx: 0x%x\n", data->e_hdr.e_shstrndx);
}
#else
static void print_symbol_table(UNUSED const struct elf *data, UNUSED const unsigned char *file)
{
}

static void print_section_header(UNUSED const struct elf *data, UNUSED const unsigned char *file)
{
}

static void print_program_header(UNUSED const struct elf *data)
{
}

static void print_header(UNUSED const struct elf *data)
{
}

static void print_text_section(UNUSED const struct elf *data)
{
}

static void print_bss_section(UNUSED const struct elf *data)
{
}

#endif // MIKO_ELF_DEBUG

struct elf *init_elf_data(void)
{
	struct elf *data;

	data = kmalloc(sizeof(*data));
	if (!data) {
		printk("couldn't allocate for data\n");
		return NULL;
	}

	memset(data, 0, sizeof(*data));

	return data;
}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////

/**
 * Execute ELF file.
 * @param ELF file.
 * @return 0 or negative value.
 */
int execute_elf(const unsigned char *file)
{
	struct elf *data;
	
	printk("%s\n", __FUNCTION__);

	data = init_elf_data();
	if (!data)
		return 0;

	if (read_header(data, file) < 0) {
		printk("this is not ELF file\n");
		return -1;
	}

	
	read_program_header(data, file);

	read_section_header(data, file);
	read_string_table(data, file);
		
	read_symbol_table(data, file);
	read_symbol_string_table(data, file);

	read_text_section(data, file);
	
	read_bss_section(data, file);

	print_text_section(data);

//	free_elf_data(data);
	
	return 0;
}
