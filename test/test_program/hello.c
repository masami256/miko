#define UNUSED __attribute__((unused))

int main(UNUSED int argc, UNUSED char **argv)
{
	__asm__("mov $1, %eax\n\t"
		"int $0x80\n\t"
		);

	return 0;
}
