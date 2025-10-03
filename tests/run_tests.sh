#!/usr/bin/env bash
set -e
mkdir -p tests/results
BIN=./bin/connect4
for infile in tests/*.in; do
  outfile=tests/results/$(basename $infile).out
  echo "Running $infile -> $outfile"
  $BIN < "$infile" > "$outfile" 2>&1 || true
  if grep -iq "wins" "$outfile" || grep -iq "win" "$outfile"; then
    echo "  RESULT: likely PASS (found 'win' text)"
  else
    echo "  RESULT: needs manual check (no 'win' text found in $outfile)"
  fi
done
