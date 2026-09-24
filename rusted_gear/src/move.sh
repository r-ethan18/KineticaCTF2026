#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
DEST_DIR="$SCRIPT_DIR/../challenge/public"

cp -- "$SCRIPT_DIR/corrupted-gears.pyc" "$DEST_DIR/gears.pyc"
cp -- "$SCRIPT_DIR/machine.py" "$DEST_DIR/machine.py"
