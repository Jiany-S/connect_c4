#!/usr/bin/env bash
set -e
make debug
mkdir -p tests/results
for t in tests/*.in; do
  echo "Valgrind run for $t"
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=tests/results/valgrind_$(basename $t).log ./bin/connect4 < "$t" || true
done
echo "Valgrind logs written to tests/results/"
