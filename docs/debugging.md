# Debugging / Testing (Dev C)

## Valgrind
make debug
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./bin/connect4 < tests/test1.in

- Attach `valgrind.log` to the sprint report.
- Look for "ERROR SUMMARY: 0 errors" and "definitely lost: 0 bytes".

## GDB (batch)
make debug
gdb -batch -ex "set pagination 0" -ex "run < tests/test1.in" -ex "bt" -ex "info locals" --args ./bin/connect4 2>&1 | tee gdb_trace.txt

- `gdb_trace.txt` contains backtrace and local variables after crash; include in report if crash happened.

## Expect (automating interactive runs)
We may use Expect scripts to feed moves automatically for regression tests.
