# Building hbmongoc (mongo-c-driver 2.x)

The package now targets **mongo-c-driver / libbson 2.x**. The driver is vendored
into `.deps/` inside this folder — nothing is installed system-wide.

## 1. Vendored driver (already present after the port)

```
.deps/src/     mongo-c-driver 2.5.5 sources (release tarball)
.deps/build/   CMake build tree
.deps/usr/local/lib/       libmongoc2.so.2, libbson2.so.2 (+ static .a, .pc, cmake)
.deps/usr/local/include/   bson-2.5.5/bson/*.h, mongoc-2.5.5/mongoc/*.h
```

To rebuild it from scratch:

```sh
mkdir -p .deps/usr/local
cmake -S .deps/src -B .deps/build \
  -DCMAKE_INSTALL_PREFIX=$PWD/.deps/usr/local \
  -DENABLE_SHARED=ON -DENABLE_PIC=ON -DENABLE_MONGOC=ON \
  -DENABLE_TESTS=OFF -DENABLE_EXAMPLES=OFF -DENABLE_SRV=OFF -DENABLE_TRACING=OFF \
  -DENABLE_SSL=OPENSSL -DENABLE_CRYPTO_SYSTEM_PROFILE=ON \
  -DENABLE_SASL=OFF
cmake --build .deps/build -j"$(nproc)"
cmake --build .deps/build --target install
```

Notes: SSL/cryptography come from the system OpenSSL 3; SASL, HTTP and
compression are off (the Harbour wrapper does not expose them). CMake 4.x
builds fine against the `3.15...4.0` minimum.

## 2. The Harbour package

Requires Harbour 3.2.x (`hbmk2`) on `PATH`.

```sh
hbmk2 -hbdyn hbmongoc.hbp     # -> bin/linux/gcc/libhbmongoc.so (+ .so.3.2, .so.3.2.1)
hbmk2 hbmongoc.hbp            # -> lib/linux/gcc/libhbmongoc.a (static)
```

`hbmongoc.hbp` links the built library against `-lmongoc2 -lbson2` from
`.deps/usr/local/lib` and stamps an rpath, so the shared object resolves at
runtime without `LD_LIBRARY_PATH`:

```
$ objdump -p bin/linux/gcc/libhbmongoc.so.3.2.1 | grep -E 'NEEDED|RUNPATH'
  NEEDED   libmongoc2.so.2
  NEEDED   libbson2.so.2
  RUNPATH  <project>/.deps/usr/local/lib
```

## 3. Building the tests / examples

`tests/hbmk.hbm` and `examples/libbson/hbmk.hbm` point at the package one/two
directories up and add the library search + rpath paths, so a test builds with:

```sh
cd tests && hbmk2 bson_test_01.prg && ./bson_test_01
```

Tests that open a connection (`test_00..03`, `bson_iter_binary_00`,
`client_command_01`, `collection_find_01`, `bulk_operation_01`,
`insert_docs`, `import_docs_from_dbf`) need a `mongod` to talk to; without one
they run and report a mongoc topology error instead of crashing.

BSON-only tests, verified working against 2.5.5: `bson_test_01`,
`bson_test_02`, `bson_iter_01`, `bson_iter_array_00`, `bson_oid_01`,
plus `examples/libbson/bcon-speed.prg` (exercises the nested
`BSON_APPEND_ARRAY_BEGIN` / `BSON_APPEND_DOCUMENT_BEGIN` path).
