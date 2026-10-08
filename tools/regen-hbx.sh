#!/bin/bash
# Regenerate <pkg>.hbx from the HB_FUNC names actually defined in src/.
#
# hbmk2's own -hbx= writes only to the package install path
# (/usr/local/share/harbour/addons/<pkg>/), which is not writable in a
# checkout-only environment, so the .hbx goes stale after every new module.
# This reproduces hbmk2's output format directly from src/ instead.
#
# Usage: tools/regen-hbx.sh [pkg-name]     (default: hbmongoc)
set -euo pipefail

PKG="${1:-hbmongoc}"
HBX="${PKG}.hbx"
CH="${PKG}.ch"

cd "$(dirname "$0")/.."

[ -f "$HBX" ] || { echo "no $HBX to regenerate"; exit 1; }

# 1) every HB_FUNC name defined in src/ (upper-cased)
grep -rh 'HB_FUNC' src/ \
  | grep -oE 'HB_FUNC[[:space:]]*\([[:space:]]*[A-Za-z0-9_]+' \
  | grep -oE '[A-Za-z0-9_]+$' | tr '[:lower:]' '[:upper:]' | sort -u > /tmp/hbx_fns.txt

# 2) preserve any manual HB_FUNC_INCLUDE / HB_FUNC_EXCLUDE overrides in src/
grep -rhE 'HB_FUNC_(INCLUDE|EXCLUDE)' src/ > /tmp/hbx_over.txt || true

# 3) header (everything before the first DYNAMIC line) and footer
#    (everything from the last DYNAMIC line onward) stay intact.
awk '/^DYNAMIC /{exit} {print}' "$HBX" > /tmp/hbx_head.txt
last=$(awk '/^DYNAMIC /{n=NR} END{print n}' "$HBX")
tail -n +"$last" "$HBX" > /tmp/hbx_tail.txt

{
  cat /tmp/hbx_head.txt
  sed 's/^/DYNAMIC /' /tmp/hbx_fns.txt
  [ -s /tmp/hbx_over.txt ] && cat /tmp/hbx_over.txt
  cat /tmp/hbx_tail.txt
} > /tmp/hbx_new.txt

mv /tmp/hbx_new.txt "$HBX"

echo "$PKG.hbx: $(grep -c '^DYNAMIC' "$HBX") DYNAMIC entries ($(wc -l < /tmp/hbx_fns.txt) HB_FUNCs in src/)"
