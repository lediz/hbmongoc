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
    LOCAL bw         := nil
    LOCAL ins        := nil
    LOCAL ret        := nil
    LOCAL res        := nil
    LOCAL exc        := nil
    LOCAL encOpts    := nil
    LOCAL enc        := nil
    LOCAL eopts      := nil
    LOCAL cipher     := nil
    LOCAL plaintext  := nil

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

    /* ---- bulkwrite: append + execute against the live server ---- */
    bw := mongoc_client_bulkwrite_new( client )

    IF bw # nil
            ins := { "name" => "bw1", "v" => 1 }
            IF mongoc_bulkwrite_append_insertone( bw, "testdb.apitest", ins, nil, @error )
                ? "bulkwrite_append_insertone: ok"
                ret := mongoc_bulkwrite_execute( bw, nil )
                IF ret # nil
                    res := ret["result"]
                    exc := ret["exception"]
                    IF res # nil
                        ? "bulkwrite result: insertedCount =", mongoc_bulkwriteresult_insertedcount( res )
                        mongoc_bulkwriteresult_destroy( res )
                    ELSE
                        IF exc # nil
                            ? "bulkwrite exception error:", HB_BSON_ERROR_MESSAGE( exc )
                            mongoc_bulkwriteexception_destroy( exc )
                        ENDIF
                    ENDIF
                ENDIF
            ELSE
                ? "bulkwrite_append error:", HB_BSON_ERROR_MESSAGE( error )
            ENDIF
    ELSE
        ? "bulkwrite_new: nil"
    ENDIF

    /* ---- client-side encryption: construct + teardown ---- */
    encOpts := mongoc_client_encryption_opts_new()

    IF encOpts # nil
            /* keyvault client + namespace are required to construct the
               encryption object; point them at this same client. */
            mongoc_client_encryption_opts_set_keyvault_client( encOpts, client )
            mongoc_client_encryption_opts_set_keyvault_namespace( encOpts, "testdb", "keys" )
            /* local KMS provider: { local : { key : <96-byte binary> } } */
            mongoc_client_encryption_opts_set_local_kms_key( encOpts, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456" )
            enc := mongoc_client_encryption_new( encOpts, @error )
            IF enc # nil
                ? "client_encryption_new: ok"
                eopts := mongoc_client_encryption_encrypt_opts_new()
                IF eopts # nil
                    mongoc_client_encryption_encrypt_opts_set_algorithm( eopts, "AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic" )
                    mongoc_client_encryption_encrypt_opts_set_keyaltname( eopts, "k1" )
                    plaintext := bson_value_new_str( "secret-value" )
                    IF plaintext # nil
                        IF mongoc_client_encryption_encrypt( enc, plaintext, eopts, @cipher, @error )
                            ? "encrypt: ok, cipher =", bson_value_get_str( cipher )
                        ELSE
                            ? "encrypt error:", HB_BSON_ERROR_MESSAGE( error )
                        ENDIF
                        bson_value_destroy( plaintext )
                    ENDIF
                    mongoc_client_encryption_encrypt_opts_destroy( eopts )
                ENDIF
                mongoc_client_encryption_destroy( enc )
            ELSE
                ? "client_encryption_new error:", HB_BSON_ERROR_MESSAGE( error )
            ENDIF
    ELSE
        ? "client_encryption_opts_new: nil"
    ENDIF

    mongoc_client_destroy( client )
    mongoc_cleanup()

    ? "DONE"

RETURN
