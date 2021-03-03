/*
 * Description: Compiler module of the Chaos Programming Language's source
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

#include "compiler.h"

i64_array* compile(ASTRoot* ast_root)
{
    i64_array* program = initProgram();

    for (unsigned long i = 0; i < ast_root->file_count; i++) {
        compileStmtList(program, ast_root->files[i]->stmt_list);
    }

    push_instr(program, HLT);
    return program;
}

void compileStmtList(i64_array* program, StmtList* stmt_list)
{
    for (unsigned long i = stmt_list->stmt_count; 0 < i; i--) {
        compileStmt(program, stmt_list->stmts[i - 1]);
    }
}

void compileStmt(i64_array* program, Stmt* stmt)
{
    switch (stmt->kind) {
    case PrintStmt_kind:
        compileExpr(program, stmt->v.print_stmt->x);
        push_instr(program, PRNT);
        break;
    case DeclStmt_kind:
        compileDecl(program, stmt->v.decl_stmt->decl);
        break;
    default:
        break;
    }
}

unsigned short compileExpr(i64_array* program, Expr* expr)
{
    size_t len;
    Symbol* symbol;
    unsigned short type;
    i64 addr;
    switch (expr->kind) {
    case BasicLit_kind:
        switch (expr->v.basic_lit->value_type) {
        case V_BOOL:
            push_instr(program, LII);
            push_instr(program, R0);
            push_instr(program, V_BOOL);

            push_instr(program, LII);
            push_instr(program, R1);
            push_instr(program, expr->v.basic_lit->value.b ? 1 : 0);
            return 1;
            break;
        case V_INT:
            push_instr(program, LII);
            push_instr(program, R0);
            push_instr(program, V_INT);

            push_instr(program, LII);
            push_instr(program, R1);
            push_instr(program, expr->v.basic_lit->value.i);
            return 2;
            break;
        case V_FLOAT:
            push_instr(program, LII);
            push_instr(program, R0);
            push_instr(program, V_FLOAT);

            i64 ipart;
            i64 frac;
            char *buf = NULL;
            buf = snprintf_concat_float(buf, "%Lf", expr->v.basic_lit->value.f);
            sscanf(buf, "%lld.%lld", &ipart, &frac);

            push_instr(program, LII);
            push_instr(program, R1);
            push_instr(program, ipart);

            push_instr(program, LII);
            push_instr(program, R2);
            push_instr(program, frac);
            return 3;
            break;
        case V_STRING:
            len = strlen(expr->v.basic_lit->value.s);
            for (size_t i = len; i > 0; i--) {
                push_instr(program, LII);
                push_instr(program, R0);
                push_instr(program, expr->v.basic_lit->value.s[i - 1] - '0');

                push_instr(program, PUSH);
                push_instr(program, R0);
            }

            push_instr(program, LII);
            push_instr(program, R0);
            push_instr(program, V_STRING);

            push_instr(program, LII);
            push_instr(program, R1);
            push_instr(program, len);
            return 4;
            break;
        default:
            break;
        }
        break;
    case Ident_kind:
        symbol = getSymbol(expr->v.ident->name);
        addr = symbol->addr;
        switch (symbol->value_type) {
        case V_BOOL:
            push_instr(program, LDI);
            push_instr(program, R0);
            push_instr(program, addr++);

            push_instr(program, LDI);
            push_instr(program, R1);
            push_instr(program, addr++);
            break;
        case V_INT:
            push_instr(program, LDI);
            push_instr(program, R0);
            push_instr(program, addr++);

            push_instr(program, LDI);
            push_instr(program, R1);
            push_instr(program, addr++);
            break;
        case V_FLOAT:
            push_instr(program, LDI);
            push_instr(program, R0);
            push_instr(program, addr++);

            push_instr(program, LDI);
            push_instr(program, R1);
            push_instr(program, addr++);

            push_instr(program, LDI);
            push_instr(program, R2);
            push_instr(program, addr++);
            break;
        case V_STRING:
            push_instr(program, LDI);
            push_instr(program, R0);
            push_instr(program, addr++);

            push_instr(program, LDI);
            push_instr(program, R1);
            push_instr(program, addr++);

            len = strlen(symbol->value.s);
            addr += len - 1;
            for (size_t i = len; i > 0; i--) {
                push_instr(program, LDI);
                push_instr(program, R2);
                push_instr(program, addr--);

                push_instr(program, PUSH);
                push_instr(program, R2);
            }
            addr += len - 1;
            break;
        default:
            break;
        }
        break;
    case UnaryExpr_kind:
        type = compileExpr(program, expr->v.unary_expr->x);
        switch (expr->v.unary_expr->op) {
        case ADD_tok:
            push_instr(program, LII);
            push_instr(program, R3);
            push_instr(program, 1);

            push_instr(program, MUL);
            push_instr(program, R1);
            push_instr(program, R3);
            break;
        case SUB_tok:
            push_instr(program, LII);
            push_instr(program, R3);
            push_instr(program, -1);

            push_instr(program, MUL);
            push_instr(program, R1);
            push_instr(program, R3);
            break;
        case NOT_tok:
            push_instr(program, LNOT);
            push_instr(program, R1);
            break;
        case TILDE_tok:
            push_instr(program, BNOT);
            push_instr(program, R1);
            break;
        default:
            break;
        }
        return type;
        break;
    default:
        break;
    }

    return 0;
}

void compileDecl(i64_array* program, Decl* decl)
{
    size_t len;
    Symbol* symbol;
    unsigned short type;
    switch (decl->kind) {
    case VarDecl_kind:
        type = compileExpr(program, decl->v.var_decl->expr);

        switch (type) {
        case V_BOOL:
            symbol = addSymbolBool(
                decl->v.var_decl->ident->v.ident->name,
                decl->v.var_decl->expr->v.basic_lit->value.b
            );
            symbol->addr = program->heap;

            push_instr(program, STI);
            push_instr(program, program->heap++);
            push_instr(program, R0);

            push_instr(program, STI);
            push_instr(program, program->heap++);
            push_instr(program, R1);
            break;
        case V_INT:
            symbol = addSymbolInt(
                decl->v.var_decl->ident->v.ident->name,
                decl->v.var_decl->expr->v.basic_lit->value.i
            );
            symbol->addr = program->heap;

            push_instr(program, STI);
            push_instr(program, program->heap++);
            push_instr(program, R0);

            push_instr(program, STI);
            push_instr(program, program->heap++);
            push_instr(program, R1);
            break;
        case V_FLOAT:
            symbol = addSymbolFloat(
                decl->v.var_decl->ident->v.ident->name,
                decl->v.var_decl->expr->v.basic_lit->value.f
            );
            symbol->addr = program->heap;

            push_instr(program, STI);
            push_instr(program, program->heap++);
            push_instr(program, R0);

            push_instr(program, STI);
            push_instr(program, program->heap++);
            push_instr(program, R1);

            push_instr(program, STI);
            push_instr(program, program->heap++);
            push_instr(program, R2);
            break;
        case V_STRING:
            symbol = addSymbolString(
                decl->v.var_decl->ident->v.ident->name,
                decl->v.var_decl->expr->v.basic_lit->value.s
            );
            symbol->addr = program->heap;

            push_instr(program, STI);
            push_instr(program, program->heap++);
            push_instr(program, R0);

            push_instr(program, STI);
            push_instr(program, program->heap++);
            push_instr(program, R1);

            len = strlen(decl->v.var_decl->expr->v.basic_lit->value.s);
            for (size_t i = len; i > 0; i--) {
                push_instr(program, POP);
                push_instr(program, R0);

                push_instr(program, STI);
                push_instr(program, program->heap++);
                push_instr(program, R0);
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void push_instr(i64_array* program, i64 el)
{
    program->arr[program->size++] = el;
}

i64 popProgram(i64_array* program)
{
    return program->arr[program->size--];
}

void freeProgram(i64_array* program)
{
    free(program->arr);
    initProgram(program);
}

i64_array* initProgram()
{
    i64_array* program = malloc(sizeof *program);
    program->capacity = USHRT_MAX * 32;
    program->arr = (i64*)malloc(program->capacity * sizeof(i64));
    program->size = 0;
    program->heap = USHRT_MAX * 2;
    return program;
}
