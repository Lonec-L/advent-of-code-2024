#include <assert.h>
#include <stdio.h>
#include <string.h>

enum reg_name {
	A,
	B,
	C,
	REGISTER_COUNT
};

struct instruction {
	long long opcode, operand;
};

void get_combo_operand(long long mem[3], struct instruction *ins)
{
	if (ins->operand <= 3) {
		return;
	}
	ins->operand = mem[ins->operand - 4];
}

void regdv(long long mem[3], enum reg_name out, struct instruction ins)
{
	get_combo_operand(mem, &ins);
	mem[out] = mem[A] / (1 << ins.operand);
}

void bxl(long long mem[3], struct instruction ins)
{
	mem[B] = mem[B] ^ ins.operand;
}

void bst(long long mem[3], struct instruction ins)
{
	get_combo_operand(mem, &ins);
	mem[B] = ins.operand % 8;
}

void jnz(long long mem[3], struct instruction ins, long long *instr_p)
{
	if (mem[A] == 0) {
		*instr_p += 2;
		return;
	} else {
		*instr_p = ins.operand;
	}
}

void bxc(long long mem[3])
{
	mem[B] = mem[B] ^ mem[C];
}

long long out(long long mem[3], struct instruction ins, long long *out_count,
	      long long out_buffer[40], long long program[40])
{
	get_combo_operand(mem, &ins);
	out_buffer[*out_count] = ins.operand % 8;
	*out_count += 1;
	return 0;
}

void run(long long n, long long program[40], long long mem[3], long long *out_count,
	 long long out_buffer[40])
{
	struct instruction ins;
	long long instr_p = 0;
	while (instr_p < n) {
		ins.opcode = program[instr_p];
		ins.operand = program[instr_p + 1];
		if (ins.opcode == 0) {
			regdv(mem, A, ins);
		} else if (ins.opcode == 6) {
			regdv(mem, B, ins);
		} else if (ins.opcode == 7) {
			regdv(mem, C, ins);
		} else if (ins.opcode == 1) {
			bxl(mem, ins);
		} else if (ins.opcode == 2) {
			bst(mem, ins);
		} else if (ins.opcode == 3) {
			jnz(mem, ins, &instr_p);
			continue;
		} else if (ins.opcode == 4) {
			bxc(mem);
		} else if (ins.opcode == 5) {
			if (out(mem, ins, out_count, out_buffer, program) != 0) {
				instr_p = n + 1;
				continue;
			}
		}
		instr_p += 2;
		if (*out_count >= n) {
			break;
		}
	}
}

int main()
{
	FILE *in_file = fopen("in", "r");
	long long mem[3];
	long long instr_p = 0;
	fscanf(in_file, "Register A: %lli\n", &mem[A]);
	fscanf(in_file, "Register B: %lli\n", &mem[B]);
	fscanf(in_file, "Register C: %lli\n", &mem[C]);

	long long program[40];
	long long out[40];
	fscanf(in_file, "Program: ");
	long long x;
	long long n = 0;
	while (fscanf(in_file, "%lli,", &x) != EOF) {
		program[n++] = x;
	}

	printf("pt1: ");
	long long out_count = 0;
	run(n, program, mem, &out_count, out);
	for (long long i = 0; i < out_count; i++) {
		if (i > 0) {
			printf(",");
		}
		printf("%lli", out[i]);
	}
	printf("\n");

	// pt2
	long long res2 = 0;
	long long tmp = 0;
	long long found = 0;
	long long counters[n];
	memset(counters, 0, sizeof(counters));
	while (found < n) {
		// rebuild partial solution
		tmp = counters[0];
		assert(counters[0] < 8);
		for (int i = 1; i <= found; i++) {
			tmp = tmp << 3;
			tmp += counters[i];
		}
		res2 = tmp;

		mem[A] = tmp;
		mem[B] = 0;
		mem[C] = 0;
		out_count = 0;
		run(n, program, mem, &out_count, out);
		if (out[0] == program[n - 1 - found]) {
			found++;
		} else {
			if (found > 0 && counters[found] > 7) {
				counters[found--] = 0;
			}
			counters[found]++;
		}
	}
	printf("pt2: %lli\n", res2);
}
