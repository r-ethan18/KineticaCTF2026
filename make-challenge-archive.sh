#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
CHALLENGES_DIR="$SCRIPT_DIR/challenges"

mkdir -p "$CHALLENGES_DIR"

for name in exceptional_lock jump_around_find_out rusted_gear; do
    cp -R -- "$SCRIPT_DIR/$name/challenge" \
        "$CHALLENGES_DIR/${name}_challenge"
done

(
    cd -- "$SCRIPT_DIR"
    zip -r -- "challenges.zip" "challenges"
)
