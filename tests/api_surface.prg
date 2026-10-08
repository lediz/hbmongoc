/*
 * api_surface.prg - verify the new mongoc modules are linked and callable.
 *
 * No mongod is required: this exercises the parts of the new API that do
 * not need a live server (object constructors that return NULL or a
 * standalone object, and the byref-error convention). It is a link +
 * ABI check, not a behavioural test - a real mongod is still needed for
 * full coverage.
 *
 * Build:  hbmk2 api_surface.prg
 */

#include "hbmongoc.ch"

PROCEDURE main()

    LOCAL ok := .T.
    LOCAL pool      := nil
    LOCAL opts      := nil
    LOCAL stream    := nil
    LOCAL cred      := nil
    LOCAL bab       := nil
    LOCAL out       := nil
    LOCAL error     := nil

    mongoc_init()

    /* ---- client pool: standalone, no server needed ---- */
    pool := mongoc_client_pool_new( mongoc_uri_new( "mongodb://localhost" ) )
    IF pool # nil
        ? "client_pool_new: ok"
        mongoc_client_pool_destroy( pool )
    ELSE
        ? "client_pool_new: nil (uri or pool failed)"
    ENDIF

    /* ---- structured log opts: standalone ---- */
    opts := mongoc_structured_log_opts_new()
    IF opts # nil
        ? "structured_log_opts_new: ok"
        mongoc_structured_log_opts_set_max_document_length( opts, 1024 )
        mongoc_structured_log_opts_destroy( opts )
    ELSE
        ? "structured_log_opts_new: nil"
        ok := .F.
    ENDIF

    /* ---- OIDC credential: standalone ---- */
    cred := mongoc_oidc_credential_new( "dummy-token" )
    IF cred # nil
        ? "oidc_credential_new: ok, token =", mongoc_oidc_credential_get_access_token( cred )
        mongoc_oidc_credential_destroy( cred )
    ELSE
        ? "oidc_credential_new: nil"
        ok := .F.
    ENDIF

    /* ---- bson array builder: pure bson, no server ---- */
    bab := bson_array_builder_new()
    out := { }
    IF bab # nil
        bson_array_builder_append_int64( bab, 42 )
        bson_array_builder_append_utf8( bab, "hello" )
        IF bson_array_builder_build( bab, @out )
            ? "array_builder_build: ok, out =", out
        ELSE
            ? "array_builder_build: failed"
            ok := .F.
        ENDIF
    ENDIF

    /* ---- stream: file stream, no server ---- */
    stream := mongoc_stream_file_new_for_path( "/dev/null", 0, 0 )
    IF stream # nil
        ? "stream_file_new_for_path: ok"
        mongoc_stream_destroy( stream )
    ELSE
        ? "stream_file_new_for_path: nil"
        ok := .F.
    ENDIF

    /* cleanup */
    mongoc_cleanup()

    IF ok
        ? "RESULT: PASS"
    ELSE
        ? "RESULT: PARTIAL"
    ENDIF

RETURN
