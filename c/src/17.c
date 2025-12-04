#include <stdio.h>

enum reg_name {
	A,
	B,
	C,
	REGISTER_COUNT
};

struct instruction {
	int opcode, operand;
};

void regdv(int mem[3], enum reg_name out, int operand)
{
}

void get_operand(int mem[3], struct instruction *ins)
{
	if (ins->operand <= 3) {
		return;
	}
}

int main()
{
	printf("Hello interpreter!\n");
	FILE *in_file = fopen("in_small", "r");
	int mem[3];
	int instr_p = 0;
	fscanf(in_file, "Register A: %i\n", &mem[A]);
	fscanf(in_file, "Register B: %i\n", &mem[B]);
	fscanf(in_file, "Register C: %i\n", &mem[C]);

	struct instruction program[20];
	fscanf(in_file, "Program: ");
	int opcode, operand;
	while (fscanf(in_file, "%i, %i,", &opcode, &operand) != EOF) {
        printf("%i, %i\n", opcode, operand);
	}
}
