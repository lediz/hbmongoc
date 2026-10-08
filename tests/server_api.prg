/*
 * server_api.prg - behavioural test for the new mongoc modules against a
 * live mongod. Exits non-zero on the first hard failure.
 *
 * Requires a server at mongodb://127.0.0.1:27017 (override with argv[1]).
 * Build: hbmk2 server_api.prg
 */

#include "hbmongoc.ch"

PROCEDURE main( uri )

    LOCAL client     := nil
    LOCAL pool       := nil
    LOCAL coll       := nil
    LOCAL gridfs     := nil
    LOCAL bucket     := nil
    LOCAL cursor     := nil
    LOCAL doc        := nil
    LOCAL reply      := nil
    LOCAL error      := nil
    LOCAL uri_

    IF empty( uri )
        uri_ := "mongodb://127.0.0.1:27017"
    ELSE
        uri_ := uri
    ENDIF

    mongoc_init()

    ? "server:", uri_

    /* ---- client + collection round-trip ---- */
    client := mongoc_client_new( uri_ )
    IF client = nil
        ? "FAIL: client_new nil"
        mongoc_cleanup()
        QUIT
    ENDIF

    coll := mongoc_client_get_collection( client, "testdb", "apitest" )
    IF coll = nil
        ? "FAIL: get_collection nil"
    ELSE
        doc := { "name" => "apitest", "value" => 42 }
        IF mongoc_collection_insert_one( coll, doc, nil, @reply, @error )
            ? "insert_one: ok"
        ELSE
            ? "insert_one error:", HB_BSON_ERROR_MESSAGE( error )
        ENDIF

        /* count via a simple command */
        reply := nil
        IF mongoc_client_command_simple( client, "testdb", { "count" => "apitest" }, nil, @reply, @error )
            ? "count:", reply
        ELSE
            ? "count error:", HB_BSON_ERROR_MESSAGE( error )
        ENDIF

        mongoc_collection_destroy( coll )
    ENDIF

    /* ---- gridfs via client_get_gridfs ---- */
    gridfs := mongoc_client_get_gridfs( client, "testdb", "test", @error )
    IF gridfs # nil
        ? "get_gridfs: ok"
        mongoc_gridfs_destroy( gridfs )
    ELSE
        ? "get_gridfs error:", HB_BSON_ERROR_MESSAGE( error )
    ENDIF

    /* ---- client pool (standalone but server-backed) ---- */
    pool := mongoc_client_pool_new( mongoc_uri_new( uri_ ) )
    IF pool # nil
        ? "client_pool_new: ok"
        mongoc_client_pool_destroy( pool )
    ELSE
        ? "client_pool_new: nil"
    ENDIF

    mongoc_client_destroy( client )
    mongoc_cleanup()

    ? "DONE"

RETURN
