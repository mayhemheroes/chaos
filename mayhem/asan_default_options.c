/* Baked-in ASan defaults for the fuzz build only (linked into /mayhem/chaos).
 * chaos is an allocate-and-exit batch interpreter: it makes no attempt to free
 * interpreter state on exit, so end-of-process leak reports would flood every
 * run with non-actionable "defects". Memory-safety checks stay fully enabled. */
const char *__asan_default_options(void) { return "detect_leaks=0"; }
