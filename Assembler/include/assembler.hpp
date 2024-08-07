#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "void_stack.hpp"
#include "file_processing.hpp"

enum AsmError
{
    ASM_NO_ERROR              = 0,
    ASM_ERROR_NULL_PTR_FILE   = 1,
    ASM_ERROR_FOPEN           = 2,
    ASM_ERROR_NULL_PTR_LOG    = 3,
    ASM_ERROR_SSCANF          = 4,
    ASM_ERROR_FSEEK           = 5,
    ASM_ERROR_FTELL           = 6,
    ASM_ERROR_STAT            = 7,
    ASM_ERROR_STRCHR          = 8,
    ASM_ERROR_CALLOC          = 9,
    ASM_ERROR_NOT_CMD         = 10,
    ASM_ERROR_READ            = 11,
    ASM_ERROR_STK             = 12,
    ASM_ERROR_SETVBUF         = 13,
    ASM_ERROR_NULL_PTR_STRUCT = 14,
    ASM_ERROR_PROC_FILE       = 15,
    ASM_ERROR_FWRITE          = 16,
    ASM_ERROR_RAM             = 17
};

enum Cmd
{
    NOT_CMD = 0,
    PUSH    = 1,
    POP     = 2,
    ADD     = 3,
    SUB     = 4,
    MUL     = 5,
    DIV     = 6,
    OUT     = 7,
    IN      = 8,
    JMP     = 9,
    CALL    = 10,
    RET     = 11,
    HLT     = 12
};

enum Reg
{
    NOT_REG = 0,
    RAX     = 1,
    RBX     = 2,
    RCX     = 3,
    RDX     = 4
};

struct Regs
{
    enum Reg reg_enum;
    const char* name;
};

const Regs REG_ARRAY[] =
{
    {NOT_REG},
    {RAX, "RAX"},
    {RBX, "RBX"},
    {RCX, "RCX"},
    {RDX, "RDX"}
};

struct Cmds
{
    enum Cmd cmd_enum;
    const char* name;
    bool has_digit;
    bool has_reg;
    bool has_label;
    bool has_ram;
    size_t length;
};

const Cmds CMD_ARRAY[] =
{
    {NOT_CMD},
    {PUSH, "PUSH", true,  true,  true,  true,  4},
    {POP,  "POP",  false, true,  false, true,  3},
    {ADD,  "ADD",  false, false, false, false, 3},
    {SUB,  "SUB",  false, false, false, false, 3},
    {MUL,  "MUL",  false, false, false, false, 3},
    {DIV,  "DIV",  false, false, false, false, 3},
    {OUT,  "OUT",  false, false, false, false, 3},
    {IN,   "IN",   false, false, false, false, 2},
    {JMP,  "JMP",  true,  false, true,  true,  3},
    {CALL, "CALL", true,  false, true,  true,  4},
    {RET,  "RET",  false, false, false, false, 3},
    {HLT,  "HLT",  false, false, false, false, 3}
};

enum Mask_arg
{
    MASK_RAM    = (char) (1 << 7),
    MASK_REG    = (char) (1 << 6),
    MASK_NUMBER = (char) (1 << 5)
};

struct Label
{
    char* name;
    size_t len;
    size_t ip;
};

struct Assem
{
    FILE* input_file;
    char** ptr_to_lines;
    size_t number_of_lines;
    char* output_buffer;
    FILE* output_file;
    Stack labels;
    double* ram;
};

void          asm_set_log_file  (FILE* file);
void          asm_print_error   (AsmError error);
const char*   asm_get_error     (AsmError error);
AsmError      make_assem_file   (Stack* labels, char** lines, size_t num_line, char* buffer, size_t* position);
int           search_label      (const char* line, size_t size, Label* labels, int number_of_labels);
Reg           search_reg        (const char* line, size_t size);
Cmd           search_command    (const char* line, size_t size);
int           search_label      (const char* line, size_t size, Stack* labels);
bool          try_command       (Stack* labels, const char* cur_line, char* buffer, size_t* position, AsmError* error);
bool          try_label         (Stack* labels, const char* cur_line, size_t position, StkError* error);
bool          try_command_label (Stack* labels, const char* cur_line, char* buffer, size_t* position);
bool          try_command_digit (const char* cur_line, char* buffer, size_t* position);
bool          try_command_reg   (const char* cur_line, char* buffer, size_t* position);
bool          try_command_ram   (const char* cur_line, char* buffer, size_t* position, AsmError* error);
void          labels_name_dtor  (Stack* labels);
AsmError      asm_ctor          (Assem* asm_struct, const char* name_of_input_file);
AsmError      assembly          (Assem* asm_struct);
AsmError      asm_dtor          (Assem* asm_struct);

#endif // ASSEMBLER_HPP
