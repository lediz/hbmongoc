#!/bin/bash
# Regenerate docs/upstream-api-delta.md from the live 2.x headers and the
# HB_FUNC names actually wrapped in src/. The committed doc drifts badly
# (it double-counts already-wrapped and static-inline functions), so
# recompute it rather than trusting the checked-in copy.
#
# Usage: tools/api-delta.sh > docs/upstream-api-delta.md
set -euo pipefail
cd "$(dirname "$0")/.."

M=.deps/usr/local/include/mongoc-2.5.5/mongoc
B=.deps/usr/local/include/bson-2.5.5/bson

# public functions = a "name(...)" line preceded by MONGOC_EXPORT(...) / BSON_EXPORT(...)
{ grep -rhA1 'MONGOC_EXPORT(\|BSON_EXPORT(' "$M" "$B" 2>/dev/null || true; } \
  | awk '/EXPORT\(/{t=1;next} t&&/^[a-z_][a-z_0-9]*\(/{sub(/\(.*/,"");print;t=0}' \
  | sort -u > /tmp/public.txt

# already wrapped (upper-cased HB_FUNC names)
grep -rh 'HB_FUNC' src/ \
  | grep -oE 'HB_FUNC[[:space:]]*\([[:space:]]*[A-Za-z0-9_]+' \
  | grep -oE '[A-Za-z0-9_]+$' | tr '[:lower:]' '[:upper:]' | sort -u > /tmp/wrapped.txt

: > /tmp/missing.txt
while read -r fn; do
  up=$(tr '[:lower:]' '[:upper:]' <<< "$fn")
  grep -qxF "$up" /tmp/wrapped.txt || echo "$fn" >> /tmp/missing.txt
done < /tmp/public.txt

pub=$(wc -l < /tmp/public.txt)
wr=$(wc -l < /tmp/wrapped.txt)
miss=$(wc -l < /tmp/missing.txt)

cat <<EOF
# Upstream API delta — hbmongoc vs mongo-c-driver 2.5.5

Regenerated from the live headers by \`tools/api-delta.sh\` (do not hand-edit).

- public API in 2.x headers: **$pub**
- wrapped (HB_FUNC in src/): **$wr**
- not wrapped: **$miss**

## Not wrapped, grouped by header

EOF

# group the missing names by their declaring header
while read -r fn; do
  hdr=$(grep -rl "^${fn}(" "$M" "$B" 2>/dev/null | head -1 | sed 's#.*/##')
  printf '%s\t%s\n' "${hdr:-unknown}" "$fn"
done < /tmp/missing.txt | sort | awk '
  { if ($1!=prev) { if(prev) print ""; print "### " $1; prev=$1 } printf "%s, ", $2 }
  END{ print "" }'
