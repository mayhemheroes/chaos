/*
 * Description: CPU module of the Chaos Programming Language's source
 *
 * Copyright (c) 2019-2021 Chaos Language Development Authority <info@chaos-lang.org>
 *
 * License: GNU General Public License v3.0
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>
 *
 * Authors: M. Mert Yildiran <me@mertyildiran.com>
 */

#include "../myjit/myjit/jitlib.h"

typedef long (*plfl)(long);
struct jit *_jit;

#include "cpu.h"

char *reg_names[] = {
    "R0", "R1", "R2",  "R3",  "R4",  "R5",  "R6",  "R7",
    "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"
};

i64* ast_stack = NULL;
i64 ast_stack_p = 0;

cpu *new_cpu(KaosIR* program, unsigned short debug_level)
{
    cpu *c = malloc(sizeof(cpu));
    c->program = program;
    c->ic = 0;

    ast_stack = (i64*)malloc(USHRT_MAX * 256 * sizeof(i64));
    return c;
}

void free_cpu(cpu *c)
{
    free(c);
    // free(ast_stack);
    // ast_stack = NULL;
    // ast_stack_p = 0;
}

void run_cpu(cpu *c)
{
    _jit = jit_init();

	plfl foo;

    jit_prolog(_jit, &foo);
    jit_declare_arg(_jit, JIT_SIGNED_NUM, sizeof(long));
    jit_getarg(_jit, R(0), 0);

    do {
        fetch(c);
        execute(c);
    } while (c->inst->op_code != HLT);

    jit_retr(_jit, R(0));
    jit_generate_code(_jit);

    printf("%li\n", foo(0));
}

void eat_until_hlt(cpu *c)
{
    do {
    } while (c->inst->op_code != HLT);
}

void fetch(cpu *c)
{
    c->inst = c->program->arr[c->ic++];
}

void execute(cpu *c)
{
    // i64 ic_start = c->ic;

    switch (c->inst->op_code) {
    case MOVI:
        jit_movi(_jit, R(c->inst->op1->reg), c->inst->op2->value.i);
        break;
    default:
        break;
    }
}
