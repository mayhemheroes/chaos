#!/usr/bin/env bash
#
# mayhem/build.sh — build the chaos interpreter fuzz target and the upstream test oracle. Runs
# inside the commit image (mayhem/Dockerfile) as `mayhem` in /mayhem.
#
# Produces:
#   /mayhem/chaos             — the FUZZ TARGET (file-input CLI: `chaos <file.kaos>`), the WHOLE
#                               interpreter compiled WITH $SANITIZER_FLAGS (ASan+UBSan, halting) +
#                               DWARF-3. The Mayhemfile runs `/mayhem/chaos @@`, feeding the fuzz bytes
#                               as a .kaos program (lexer → parser → AST → interpreter/JIT). This is
#                               also its own single-input reproducer (run it on a crashing file).
#                               WHY this shape: chaos is an exit()-on-every-error batch interpreter
#                               (syntax/runtime errors call exit()), so an in-process libFuzzer harness
#                               dies on the first input — not viable. The proven pattern for a
#                               sanitized file-input interpreter on this base image is the sibling
#                               integration mayhemheroes/LISP (`komplott`): an ASan+UBSan binary driven
#                               with the `@@` input-file placeholder, from which Mayhem derives edge
#                               coverage (komplott: 7.4k edges). A raw/uninstrumented binary with a
#                               fixed `filepath:` input records edges live but finalizes to 0 on this
#                               base image (measured here: plain gcc/clang → 0 final edges, runs
#                               14/16/17; AFL-instrumented → line-coverage-incompatible, run 15).
#                               -fcommon is required: chaos has upstream ODR-violating file-scope
#                               globals in shared headers (e.g. _ast_root, symbol_cursor) that only
#                               link when merged into a single common symbol.
#   /mayhem/build-test/bin/chaos — the TEST ORACLE: the same sources built with the project's NORMAL
#                               flags (no sanitizers), used by mayhem/test.sh to run upstream's
#                               golden-output suites.
set -euo pipefail

[ -n "${SOURCE_DATE_EPOCH:-}" ] || unset SOURCE_DATE_EPOCH

: "${SANITIZER_FLAGS=-fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer}"
: "${DEBUG_FLAGS:=-g -gdwarf-3}"
: "${CC:=clang}"
: "${MAYHEM_JOBS:=$(nproc)}"
: "${COVERAGE_FLAGS=}"
export SANITIZER_FLAGS DEBUG_FLAGS CC MAYHEM_JOBS COVERAGE_FLAGS

cd "$SRC"

# myjit is a git submodule (gitlink only in the CI build context). A pinned copy of the
# sources we need (myjit @ a4401e4, the SHA the gitlink records) is vendored under
# mayhem/vendor/myjit so the build is hermetic — no network at image build or re-run.
if [ ! -f myjit/myjit/jitlib-core.c ]; then
    mkdir -p myjit
    cp -r mayhem/vendor/myjit/myjit myjit/
fi

# Generated parser/lexer (upstream's Makefile recipe, minus the report files).
bison -Wconflicts-rr -Wno-conflicts-sr -d parser/parser.y
flex lexer/lexer.l

CHAOS_SRCS=(parser.tab.c lex.yy.c parser/*.c utilities/*.c ast/*.c vm/*.c interpreter/*.c compiler/*.c Chaos.c)
# (-Werror dropped from upstream's recipe: it pins it for its own gcc/clang; newer clang warns differently.)

# 1) Fuzz target — the WHOLE interpreter compiled WITH $SANITIZER_FLAGS (ASan+UBSan, halting) so the
#    fuzzed code (lexer, parser, AST, interpreter, JIT backend) is instrumented; driven via `@@`.
#    asan_default_options.c bakes detect_leaks=0 (chaos is an allocate-and-exit batch interpreter).
#    -fcommon merges chaos's shared-header globals into single common symbols (see header).
mkdir -p build-fuzz
( cd build-fuzz
  $CC -c $SANITIZER_FLAGS $DEBUG_FLAGS -Winline -Wall -std=c99 -D_XOPEN_SOURCE=600 \
      -o jitlib-core.o "$SRC/myjit/myjit/jitlib-core.c"
  $CC -c $SANITIZER_FLAGS $DEBUG_FLAGS -Wall -fcommon -DCHAOS_INTERPRETER \
      "${CHAOS_SRCS[@]/#/$SRC/}" "$SRC/mayhem/asan_default_options.c"
  $CC $SANITIZER_FLAGS $DEBUG_FLAGS -o "$SRC/chaos" ./*.o -lreadline -lm -ldl )

# 2) Test oracle — independent NORMAL build (upstream default flags) for mayhem/test.sh.
#    myjit backend object built with upstream recipe flags.
$CC -c -g -Winline -Wall -std=c99 -pedantic -D_XOPEN_SOURCE=600 \
    -o myjit/jitlib-core.o myjit/myjit/jitlib-core.c
mkdir -p build-test/bin build-test/obj
( cd build-test/obj
  $CC -c -g -Wall -fcommon -DCHAOS_INTERPRETER -O2 $COVERAGE_FLAGS "${CHAOS_SRCS[@]/#/$SRC/}"
  $CC -g -Wall $COVERAGE_FLAGS -o "$SRC/build-test/bin/chaos" ./*.o \
      "$SRC/myjit/jitlib-core.o" -lreadline -lm -ldl )

echo "build.sh: built /mayhem/chaos (sanitized fuzz target + reproducer) and /mayhem/build-test/bin/chaos (oracle)"
