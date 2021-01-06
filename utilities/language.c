/*
 * Description: Language module of the Chaos Programming Language's source
 *
 * Copyright (c) 2019-2020 Chaos Language Development Authority <info@chaos-lang.org>
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

#include "language.h"
#include "helpers.h"

extern int kaos_lineno;

void greet() {
#if !defined(_WIN32) && !defined(_WIN64) && !defined(__CYGWIN__)
    if (!isatty(fileno(stdin)))
        return;
#else
    if (!_isatty(_fileno(stdin)))
        return;
#endif

    char lang[__KAOS_MSG_LINE_LENGTH__ / 2 - __KAOS_INDENT_LENGTH__ * 2];
    char compiler[__KAOS_MSG_LINE_LENGTH__ / 2 - __KAOS_INDENT_LENGTH__ * 2];
    char lang_compiler[__KAOS_MSG_LINE_LENGTH__];
    char motto[__KAOS_MSG_LINE_LENGTH__];
    char copyright[__KAOS_MSG_LINE_LENGTH__];
    char notice1[__KAOS_MSG_LINE_LENGTH__];
    char notice2[__KAOS_MSG_LINE_LENGTH__];
    char license[__KAOS_MSG_LINE_LENGTH__];

    sprintf(
        lang,
        "%s Language %d.%d.%d (%s %s)",
        __KAOS_LANGUAGE_NAME__,
        __KAOS_VERSION_MAJOR__,
        __KAOS_VERSION_MINOR__,
        __KAOS_VERSION_PATCHLEVEL__,
        __DATE__,
        __TIME__
    );
#if defined(__clang__)
    sprintf(
        compiler,
        "Clang version: %d.%d.%d on %s",
        __clang_major__,
        __clang_minor__,
        __clang_patchlevel__,
        __KAOS_PLATFORM_NAME__
    );
#elif defined(__GNUC__) || defined(__GNUG__)
    sprintf(
        compiler,
        "GCC version: %d.%d.%d on %s",
        __GNUC__,
        __GNUC_MINOR__,
        __GNUC_PATCHLEVEL__,
        __KAOS_PLATFORM_NAME__
    );
#endif
    sprintf(lang_compiler, "%*c%s, %s", __KAOS_INDENT_LENGTH__ * 2, ' ', lang, compiler);
    sprintf(
        copyright,
        "%*c%s",
        __KAOS_INDENT_LENGTH__,
        ' ',
        "Copyright (C) 2019-2021 Chaos Language Development Authority <info@chaos-lang.org>"
    );
    sprintf(notice1, "%*c%s", __KAOS_INDENT_LENGTH__, ' ', "This program comes with");
    sprintf(
        notice2,
        "%*c%s",
        __KAOS_INDENT_LENGTH__,
        ' ',
        "and you are welcome to redistribute it under certain conditions."
    );
    sprintf(license, "%*c%s", __KAOS_INDENT_LENGTH__, ' ', "License:");

    int cols[4];
    cols[0] = (int) strlen(lang_compiler) + 1;
    cols[1] = (int) strlen(copyright) + 1;
    cols[2] = (int) strlen(notice1) + 1;
    cols[3] = (int) strlen(notice2) + 1;
    int ws_col = largest(cols, 2) + 4;

    sprintf(motto, "%*c%s", ws_col / 2 - 12, ' ', __KAOS_LANGUAGE_MOTTO__);

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;45m");
#endif
    printf("%-*s", ws_col, "");
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[1;45m");
#endif
    printf("%-*s", ws_col, lang_compiler);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[1;45m");
    printf("\033[5;45m");
#endif
    printf("%-*s", ws_col, motto);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;45m");
#endif
    printf("%-*s", ws_col, "");
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;45m");
#endif
    printf("%-*s", ws_col, copyright);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;45m");
#endif
    printf("%-*s", 28, notice1);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[1;45m");
#endif
    printf("%-*s", 21, "ABSOLUTELY NO WARRANTY");
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;45m");
#endif
    printf("%-*s", ws_col - 50, ". This is free software,");
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;45m");
#endif
    printf("%-*s", ws_col, notice2);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;45m");
#endif
    printf("%-*s", 13, license);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[4;45m");
#endif
    printf("%-*s", 31, "GNU General Public License v3.0");
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;45m");
#endif
    printf("%-*s", ws_col - 44, " (run `chaos -l` to print the license)");
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;45m");
#endif
    printf("%-*s", ws_col, "");
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif

    printf("\n\n");
}

void yyerror_msg(char* error_name, char* current_module, char* cause) {
    char error_name_msg[__KAOS_MSG_LINE_LENGTH__];
    char current_module_msg[__KAOS_MSG_LINE_LENGTH__];
    char line_no_msg[__KAOS_MSG_LINE_LENGTH__];
    char cause_msg[__KAOS_MSG_LINE_LENGTH__];

    sprintf(error_name_msg, "  %s:", error_name);
    sprintf(current_module_msg, "    Module: %s", current_module);
    sprintf(line_no_msg, "    Line: %d", kaos_lineno);
    sprintf(cause_msg, "    Cause: %s", cause);

    char* new_cause_msg = str_replace(cause_msg, "\n", "\\n");

    int cols[4];
    cols[0] = (int) strlen(error_name_msg) + 1;
    cols[1] = (int) strlen(current_module_msg) + 1;
    cols[2] = (int) strlen(line_no_msg) + 1;
    cols[3] = (int) strlen(new_cause_msg) + 1;
    int ws_col = largest(cols, 3) + 4;

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[1;46m");
#endif
    printf("%-*s", ws_col, error_name_msg);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;46m");
#endif
    printf("%-*s", ws_col, current_module_msg);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;46m");
#endif
    printf("%-*s", ws_col, line_no_msg);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0;46m");
#endif
    printf("%-*s", ws_col, new_cause_msg);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");
    free(new_cause_msg);
}

void print_bye_bye() {
#if !defined(_WIN32) && !defined(_WIN64) && !defined(__CYGWIN__)
    if (!isatty(fileno(stdin)))
        return;
#else
    if (!_isatty(_fileno(stdin)))
        return;
#endif
    int ws_col = (int) strlen(__KAOS_BYE_BYE__) + 8 + 1;
    char bye_bye_msg[ws_col];
    sprintf(bye_bye_msg, "    %s    ", __KAOS_BYE_BYE__);

#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[5;42m");
#endif
    printf("%-*s", ws_col, bye_bye_msg);
#if defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    printf("\033[0m");
#endif
    printf("\n");
}

unsigned char help_txt[] = {
    0x55, 0x73, 0x61, 0x67, 0x65, 0x3a, 0x20, 0x63, 0x68, 0x61, 0x6f, 0x73,
    0x20, 0x5b, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x5d, 0x20, 0x3c,
    0x66, 0x69, 0x6c, 0x65, 0x3e, 0x20, 0x5b, 0x2d, 0x2d, 0x5d, 0x20, 0x5b,
    0x61, 0x72, 0x67, 0x73, 0x2e, 0x2e, 0x2e, 0x5d, 0x0a, 0x0a, 0x20, 0x20,
    0x20, 0x20, 0x2d, 0x68, 0x2c, 0x20, 0x2d, 0x2d, 0x68, 0x65, 0x6c, 0x70,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x50, 0x72,
    0x69, 0x6e, 0x74, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x68, 0x65, 0x6c,
    0x70, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2e, 0x0a, 0x20, 0x20, 0x20, 0x20,
    0x2d, 0x76, 0x2c, 0x20, 0x2d, 0x2d, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f,
    0x6e, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x56, 0x65, 0x72, 0x73,
    0x69, 0x6f, 0x6e, 0x20, 0x6e, 0x75, 0x6d, 0x62, 0x65, 0x72, 0x2e, 0x0a,
    0x20, 0x20, 0x20, 0x20, 0x2d, 0x6c, 0x2c, 0x20, 0x2d, 0x2d, 0x6c, 0x69,
    0x63, 0x65, 0x6e, 0x73, 0x65, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x50, 0x72, 0x69, 0x6e, 0x74, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6c, 0x69,
    0x63, 0x65, 0x6e, 0x73, 0x65, 0x2e, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d,
    0x64, 0x2c, 0x20, 0x2d, 0x2d, 0x64, 0x65, 0x62, 0x75, 0x67, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x44, 0x65, 0x62, 0x75, 0x67,
    0x20, 0x74, 0x68, 0x65, 0x20, 0x65, 0x78, 0x65, 0x63, 0x75, 0x74, 0x69,
    0x6f, 0x6e, 0x20, 0x6f, 0x66, 0x20, 0x61, 0x20, 0x70, 0x72, 0x6f, 0x67,
    0x72, 0x61, 0x6d, 0x20, 0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x41, 0x53,
    0x54, 0x20, 0x6e, 0x6f, 0x64, 0x65, 0x73, 0x2e, 0x0a, 0x20, 0x20, 0x20,
    0x20, 0x2d, 0x63, 0x2c, 0x20, 0x2d, 0x2d, 0x63, 0x6f, 0x6d, 0x70, 0x69,
    0x6c, 0x65, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x43, 0x6f, 0x6d,
    0x70, 0x69, 0x6c, 0x65, 0x20, 0x61, 0x20, 0x43, 0x68, 0x61, 0x6f, 0x73,
    0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 0x61, 0x6d, 0x20, 0x69, 0x6e, 0x74,
    0x6f, 0x20, 0x6d, 0x61, 0x63, 0x68, 0x69, 0x6e, 0x65, 0x20, 0x63, 0x6f,
    0x64, 0x65, 0x2e, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x6f, 0x2c, 0x20,
    0x2d, 0x2d, 0x6f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x42, 0x69, 0x6e, 0x61, 0x72, 0x79, 0x20, 0x6f,
    0x75, 0x74, 0x70, 0x75, 0x74, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x6e, 0x61,
    0x6d, 0x65, 0x2e, 0x20, 0x4d, 0x75, 0x73, 0x74, 0x20, 0x62, 0x65, 0x20,
    0x75, 0x73, 0x65, 0x64, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x74, 0x68,
    0x65, 0x20, 0x2d, 0x63, 0x20, 0x2f, 0x20, 0x2d, 0x2d, 0x63, 0x6f, 0x6d,
    0x70, 0x69, 0x6c, 0x65, 0x20, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x2e,
    0x0a, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x65, 0x2c, 0x20, 0x2d, 0x2d, 0x65,
    0x78, 0x74, 0x72, 0x61, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x45, 0x78, 0x74, 0x72, 0x61, 0x20, 0x66, 0x6c, 0x61, 0x67, 0x73,
    0x20, 0x74, 0x6f, 0x20, 0x69, 0x6e, 0x6a, 0x65, 0x63, 0x74, 0x20, 0x69,
    0x6e, 0x74, 0x6f, 0x20, 0x43, 0x20, 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c,
    0x65, 0x72, 0x20, 0x63, 0x6f, 0x6d, 0x6d, 0x61, 0x6e, 0x64, 0x2e, 0x0a,
    0x20, 0x20, 0x20, 0x20, 0x2d, 0x6b, 0x2c, 0x20, 0x2d, 0x2d, 0x6b, 0x65,
    0x65, 0x70, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x44, 0x6f, 0x6e, 0x27, 0x74, 0x20, 0x72, 0x65, 0x6d, 0x6f, 0x76, 0x65,
    0x20, 0x74, 0x68, 0x65, 0x20, 0x43, 0x20, 0x73, 0x6f, 0x75, 0x72, 0x63,
    0x65, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x68, 0x65, 0x61, 0x64, 0x65, 0x72,
    0x20, 0x66, 0x69, 0x6c, 0x65, 0x73, 0x20, 0x28, 0x74, 0x65, 0x6d, 0x70,
    0x6f, 0x72, 0x61, 0x72, 0x79, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x73, 0x29,
    0x20, 0x61, 0x66, 0x74, 0x65, 0x72, 0x20, 0x63, 0x6f, 0x6d, 0x70, 0x69,
    0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2e, 0x0a, 0x0a
};
unsigned int help_txt_len = 585;

void print_help() {
    char lang[__KAOS_MSG_LINE_LENGTH__];
    sprintf(
        lang,
        "%s Language %d.%d.%d (%s %s) ",
        __KAOS_LANGUAGE_NAME__,
        __KAOS_VERSION_MAJOR__,
        __KAOS_VERSION_MINOR__,
        __KAOS_VERSION_PATCHLEVEL__,
        __DATE__,
        __TIME__
    );
    printf("%s\n\n", lang);
    printf("%s", help_txt);
}

void print_license() {
    char license_path[PATH_MAX];
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#   if defined(__clang__)
    sprintf(license_path, "%%programfiles%%/LLVM/lib/clang/%d.%d.%d/include/chaos/LICENSE", __clang_major__, __clang_minor__, __clang_patchlevel__);
#   elif defined(__GNUC__) || defined(__GNUG__)
    sprintf(license_path, "%%programdata%%/Chocolatey/lib/mingw/tools/install/mingw64/lib/gcc/x86_64-w64-mingw32/%d.%d.%d/include/chaos/LICENSE", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#   endif
#else
    sprintf(license_path, "/usr/local/include/chaos/LICENSE");
#endif

    char *license_text = fileGetContents(license_path);
    printf("%s", license_text);
    free(license_text);
}
