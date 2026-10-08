# hbmongoc — status vs. upstream MongoDB

Generated: 2026-10-07 (repo HEAD `a710773`, upstream data fetched live from GitHub).

**Project:** Harbour-language dynamic library/wrapper over `mongo-c-driver` + `libbson`.
It is *not* a fork of [`mongodb/mongo`](https://github.com/mongodb/mongo) (the C++ server);
the relevant upstream is [`mongodb/mongo-c-driver`](https://github.com/mongodb/mongo-c-driver),
which relates to the server only through the BSON/wire protocol.

## Repo state (local)

| Item | Value |
|---|---|
| HEAD | `a710773` "+ functions added", **2018-11-26** |
| Branches | `enhance` == `origin/master` == `origin/enhance` (no divergence, no tags) |
| Remote | `https://github.com/lediz/hbmongoc.git` (README still points at `tfonrouge/…`) |
| Size | 29 files in `src/`, ~3,808 LOC, 15 test `.prg`, 1 example, 708 KB |
| API surface | 162 `HB_FUNC` in `src/*.c`, 163 `DYNAMIC` in `hbmongoc.hbx` |
| Missing | no `LICENSE`, no CI/`.github`, no docs (`README.md` is untouched GitHub-Pages boilerplate), `_config.yml` still Jekyll default |

## Upstream state (fetched 2026-10-07)

| Upstream | State |
|---|---|
| `mongodb/mongo` | HEAD `f969c5973` (2026-09-30), pushed 2026-10-07, latest release tag `r9.1.0-alpha0` — the **9.x** server line |
| `mongo-c-driver` | latest release **2.5.5** (2026-09-24); the 1.x ABI line is still maintained at **1.30.12** (same day) |
| Project targets | conditional-compilation guards stop at `MONGOC_CHECK_VERSION(1, 9, 0)`; `hbmongoc.hbc` hard-pins `libs=mongoc-1.0` / `bson-1.0` |

The project is ~8 years stale and sits at the **1.9.x driver / MongoDB 4.x era**, while upstream is at 2.5.5 / server 9.x.

## API coverage (measured against real upstream headers)

| Baseline | Public fns | Covered | Coverage |
|---|---|---|---|
| `mongo-c-driver` 1.30.12 headers | 1,023 | 153 | **15.0%** |
| `mongo-c-driver` 2.5.5 headers | 1,038 | 149 | **14.4%** |

- 9 project-only names: 8 genuine Harbour helpers (`hb_bson_as_hash`, `hb_bson_as_json`,
  `hb_dttounix`, `hb_unixtot`, `hb_numtype`, `hb_bson_version`,
  `hb_bson_set_return_json_type`, `hb_mongoc_set_return_bson_value_type`) plus `bson_new`,
  which is not a libbson symbol at all.
- **4 covered functions no longer exist in 2.x** → will not compile against the modern driver:
  `bson_append_array_begin`, `bson_array_as_json`, `bson_as_json`, `mongoc_collection_find`.
- **3 covered functions are deprecated even in 1.30.12**: `bson_as_json`, `bson_init`, `mongoc_collection_find`.
- 2.x delta vs 1.30: **118 new / 103 removed** APIs (new `bson_vector_float32/int8/packed_bit`,
  `bson_error_clear`, array-builder family) — none wrapped.

## Largest unwrapped modules (vs 1.30.12)

`mongoc-apm` 82 · `mongoc-bulkwrite` 66 · `bson.h` 64 · `client-side-encryption` 60 ·
`mongoc-uri` 53 · `mongoc-client` 41 · `mongoc-collection` 40 · `mongoc-client-session` 38 ·
`bson-iter` 36 · `bson-atomic` 27 · `mongoc-bulk-operation` 26 · `mongoc-database` 24 ·
`mongoc-cursor` 20 — plus entire absence of `mongoc-ssl`, `mongoc-read-concern`,
`mongoc-topology-description`, `mongoc-server-api`, `mongoc-structured-log`, client-pool, and GridFS.

Full per-module lists: see [`docs/upstream-api-delta.md`](docs/upstream-api-delta.md).

## Buildability on this machine

Harbour is present (`/home/jack/Projects/harbour/bin/linux/gcc/hbmk2`, Harbour 3.2.1dev r2609180937),
but **no `libmongoc`/`libbson` is installed** (`pkg-config` finds neither), so the package
cannot currently build or run its tests here.

## Port to mongo-c-driver 2.x (done 2026-10-07)

The wrapper was moved onto the vendored **mongo-c-driver / libbson 2.5.5** in
`.deps/` (see [`BUILD.md`](BUILD.md)). What changed:

| Area | Change |
|---|---|
| Removed in 2.x | `bson_as_json` → `bson_as_legacy_extended_json` (keeps the old "SIMPLE" output), `bson_array_as_json` → `bson_array_as_legacy_extended_json` |
| Removed in 2.x | `mongoc_collection_find` no longer exists; `MONGOC_COLLECTION_FIND` is now a shim onto `mongoc_collection_find_with_opts` (skip/limit forwarded, `fields` → `projection`; legacy `flags` and `batch_size` ignored) |
| Deprecated in 2.x | `bson_append_array_begin` → `bson_append_array_unsafe_begin` (same semantics, non-deprecated) |
| Version guards | all `#if BSON/MONGOC_CHECK_VERSION( 1, x, y )` conditionals removed; `hb_bson.h` / `hb_mongoc.h` now `#error` unless the headers are 2.0.0+ |
| `hbmongoc.hbc` | link flags `mongoc-1.0`/`bson-1.0` → `mongoc2`/`bson2`; lib paths + rpath point at `.deps/usr/local/lib` |
| `hbmongoc.hbp` | include paths for the 2.x header layout (`bson-2.5.5/bson`, `mongoc-2.5.5/mongoc`); `-lmongoc2 -lbson2` + rpath for the package's own `.so` |
| `hbmongoc.ch` | `MONGOC_QUERY_SLAVE_OK` → `MONGOC_QUERY_SECONDARY_OK`; dropped `MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED` (gone upstream); added `BSON_VALIDATE_CORRUPT`, `BSON_SUBTYPE_ENCRYPTED/COLUMN/SENSITIVE/VECTOR`, `BSON_APPEND_ARRAY_UNSAFE_BEGIN` |
| Bug fix | `HB_BSON_SET_RETURN_JSON_TYPE` set CANONICAL when asked for RELAXED |

Verified: `bin/linux/gcc/libhbmongoc.so` links `libmongoc2.so.2` / `libbson2.so.2`,
and the BSON-only tests (`bson_test_01/02`, `bson_iter_01`, `bson_iter_array_00`,
`bson_oid_01`) plus `examples/libbson/bcon-speed.prg` run correctly on 2.5.5.

## APM wrapper added (2026-10-08)

`mongoc-apm.h` was the largest unwrapped module (82 functions). It is now fully
wrapped in `src/hb_mongoc_apm.c` — all 82 upstream functions have a Harbour
interface, taking coverage from 149/1038 (14.4%) to **231/1038 (22.3%)**.

| Area | Change |
|---|---|
| New type tags | `_hbmongoc_apm_*_t_` (12 event kinds), `_hbmongoc_apm_callbacks_t_`, `_hbmongoc_apm_context_t_`, `_hbmongoc_topology_description_t_` added to `hbmongoc_t_` |
| Garbage collection | `hb_mongoc.c`'s destroy switch handles `topology_description` and `apm_callbacks`; APM **event** and **context** objects are deliberately not destroyed — mongoc owns them and they are only valid during a callback |
| Callbacks | mongoc wants C function pointers; Harbour supplies a code block. A trampoline per event kind invokes the stashed block via `hb_vmEvalBlockV()`, passing the event pointer as the block's single argument, so the accessors accept `PARAM(1)` |
| Slot storage | the 12 code-block slots live in one `static` array in `hb_mongoc_apm.c`, reached through `hb_apm_slot()`; declaring them in `hb_mongoc.h` would have emitted a definition in every translation unit and failed to link |

Also fixed while building: `MONGOC_CURSOR_GET_HINT` / `SET_HINT` called
`mongoc_cursor_get_hint` / `set_hint`, which no longer exist in 2.x — they now
map onto `mongoc_cursor_get_server_id` / `set_server_id`.

Caveats on the APM layer, not yet verified against a live `mongod`:

- The callback runs on whichever thread mongoc emits it from; Harbour is not
  thread-safe, so cross-thread callbacks need a runtime guard.
- The event pointer must not be retained past the callback.
- Only one code block can be registered per event kind process-wide.

Still open: the remaining unwrapped 2.x functions — regenerate the accurate
count with `tools/api-delta.sh` (the checked-in delta doc drifts). Most of the
remainder is genuinely not wrappable in Harbour: C function pointers (apm /
oidc / stream-initiator setters), value-semantics structs (bson_vector_*_view_t,
struct sockaddr, oidc_callback_params_t), and bcon varargs macros.

## Client-side encryption: not buildable in this environment

`mongoc_client_encryption_*` compiles and links, but at runtime reports
"libmongoc is not built with support for Client-Side Field Level Encryption".
Forcing `ENABLE_CLIENT_SIDE_ENCRYPTION=ON` in the `.deps` cmake fails:

```
CMake Error at src/libmongoc/CMakeLists.txt:517 (message):
  Required library (libmongocrypt) not found.
```

Client-side encryption needs **libmongocrypt**, a separate MongoDB library
(https://github.com/mongodb/libmongocrypt) that is not part of mongo-c-driver
and is not present in `.deps/` or on the system. The encryption path therefore
cannot be exercised here regardless of the cmake flag. The wrapper handles the
absence cleanly (returns the error, no crash) — verified by tests/server_api.prg.

To get a real encryption round-trip you must vendor libmongocrypt and point
`-DENABLE_CLIENT_SIDE_ENCRYPTION=ON` at it.

Note: `hbmongoc.hbx` is **stale** — it still lists 498 `DYNAMIC` entries while
`src/` now defines 606 `HB_FUNC`. hbmk2 regenerates `.hbx` only as an install
step (`/usr/local/share/harbour/addons/…`), which is not writable here, so the
79 new `MONGOC_APM_*` names are absent from it. Regenerate on a machine where
the package can be installed, or hand-append them.

