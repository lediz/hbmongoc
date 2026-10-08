//
//  hb_mongoc_collection.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_collection.h"
#include "hb_mongoc.h"


HB_FUNC( MONGOC_COLLECTION_AGGREGATE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * pipeline = bson_hbparam( 3, HB_IT_ANY );

    if (collection && pipeline) {
        mongoc_query_flags_t flags = hb_parnidef( 2, MONGOC_QUERY_NONE );
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        const mongoc_read_prefs_t *read_prefs = mongoc_hbparam( 5, _hbmongoc_read_prefs_t_ );

        mongoc_cursor_t * cursor = mongoc_collection_aggregate(collection, flags, pipeline, opts, read_prefs);

        PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );

        hb_retptrGC( phCursor );

        if (opts && !HB_ISPOINTER(4)) {
            bson_destroy(opts);
        }

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if (pipeline && !HB_ISPOINTER(3)) {
        bson_destroy(pipeline);
    }
}

HB_FUNC( MONGOC_COLLECTION_CREATE_BULK_OPERATION_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

        mongoc_bulk_operation_t * bulk = mongoc_collection_create_bulk_operation_with_opts(collection, opts);
        PHB_MONGOC phMongo = hbmongoc_new_dataContainer( _hbmongoc_bulk_operation_t_, bulk );
        hb_retptrGC( phMongo );

        if ( opts && ! HB_ISPOINTER( 2 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_COMMAND_SIMPLE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && command && HB_ISBYREF( 4 ) ) {

        const mongoc_read_prefs_t *read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_command_simple( collection, command, read_prefs, &reply, &error);

        hbmongoc_return_byref_bson( 4, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_COLLECTION_DESTROY )
{
    PHB_MONGOC collection = hbmongoc_param( 1, _hbmongoc_collection_t_ );

    if( collection ) {
        mongoc_collection_destroy( collection->p );
        collection->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_DROP )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        bson_error_t error;

        bool result = mongoc_collection_drop( collection, &error );

        bson_hbstor_byref_error( 2, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_DROP_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

        bson_error_t error;

        bool result = mongoc_collection_drop_with_opts( collection, opts, &error );

        bson_hbstor_byref_error( 3, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 2 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_FIND )
{
    /* mongoc_collection_find() was dropped from the 2.x driver. Kept as a
       compatibility shim onto mongoc_collection_find_with_opts(): 'skip' and
       'limit' are forwarded as find options, 'fields' as the projection.
       The legacy 'flags' (param 2) and 'batch_size' (param 5) have no 2.x
       equivalent and are ignored. */
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * query = bson_hbparam( 6, HB_IT_ANY );

    if ( collection && query ) {
        u_int32_t skip = hb_parnidef( 3, 0 );
        u_int32_t limit = hb_parnidef( 4, 0 );
        bson_t * fields = bson_hbparam( 7, HB_IT_ANY );
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 8, _hbmongoc_read_prefs_t_ );

        bson_t opts;
        bson_init( &opts );

        if ( skip ) {
            bson_append_int32( &opts, "skip", 4, ( int32_t ) skip );
        }

        if ( limit ) {
            bson_append_int32( &opts, "limit", 5, ( int32_t ) limit );
        }

        if ( fields ) {
            bson_append_document( &opts, "projection", 10, fields );
        }

        mongoc_cursor_t * cursor = mongoc_collection_find_with_opts( collection, query, &opts, read_prefs );

        bson_destroy( &opts );

        PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );

        hb_retptrGC( phCursor );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( query && ! HB_ISPOINTER( 6 ) ) {
        bson_destroy( query );
    }
}

HB_FUNC( MONGOC_COLLECTION_FIND_INDEXES_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if (collection) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

        mongoc_cursor_t * cursor = mongoc_collection_find_indexes_with_opts(collection, opts);

        PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );

        hb_retptrGC( phCursor );

        if ( opts && ! HB_ISPOINTER( 2 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_FIND_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * filter = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && filter ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        const mongoc_read_prefs_t *read_prefs = mongoc_hbparam( 4, _hbmongoc_read_prefs_t_ );

        mongoc_cursor_t * cursor = mongoc_collection_find_with_opts( collection, filter, opts, read_prefs );

        PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );

        hb_retptrGC( phCursor );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( filter && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( filter );
    }
}

HB_FUNC( MONGOC_COLLECTION_INSERT )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * document = bson_hbparam( 3, HB_IT_ANY );

    if ( collection && document ) {
        mongoc_insert_flags_t flags = hb_parnidef( 2, MONGOC_INSERT_NONE );
        const mongoc_write_concern_t * write_concern = mongoc_hbparam( 4, _hbmongoc_write_concern_t_ );
        bson_error_t error;

        bool result = mongoc_collection_insert( collection, flags, document, write_concern, &error );

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( document && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( document );
    }
}

/**
 * param 3 (array size) not used
 */
HB_FUNC( MONGOC_COLLECTION_INSERT_MANY )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    PHB_ITEM pArray = hb_param( 2, HB_IT_ARRAY );

    if (collection && pArray) {
        HB_SIZE arrayLen = hb_arrayLen(pArray);
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;
        bson_t * docs[arrayLen];
        bson_t * noBsonDocs[arrayLen];

        for (HB_SIZE i = 0; i < arrayLen; ++i) {
            PHB_ITEM pItem = hb_itemArrayGet(pArray, i + 1);
            docs[i] = get_bson_item(pItem);
            if (docs[i] && ! HB_IS_POINTER(pItem)) {
                noBsonDocs[i] = docs[i];
            } else {
                noBsonDocs[i] = NULL;
            }
            hb_itemRelease(pItem);
        }

        bool result = mongoc_collection_insert_many(collection, (const bson_t **) docs, arrayLen, opts, &reply, &error);

        for (HB_SIZE i = 0; i < arrayLen; ++i) {
            if (noBsonDocs[i] != NULL) {
                bson_free(noBsonDocs[i]);
            }
        }

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

        if (HB_ISBYREF(4)) {
            hbmongoc_return_byref_bson(4, bson_copy(&reply));
        }
        bson_destroy(&reply);

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl(result);

    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_INSERT_ONE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * document = bson_hbparam( 2, HB_IT_ANY );

    if (collection && document) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_insert_one(collection, document, opts, &reply, &error);

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

        if (HB_ISBYREF(4)) {
            hbmongoc_return_byref_bson(4, bson_copy(&reply));
        }
        bson_destroy(&reply);

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl(result);

    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_KEYS_TO_INDEX_STRING )
{
    bson_t * document = bson_hbparam( 1, HB_IT_ANY );

    if ( document ) {
        char * indexName = mongoc_collection_keys_to_index_string( document );
        hb_retc( indexName );
        bson_free( indexName );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( document && ! HB_ISPOINTER( 1 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_COLLECTION_REMOVE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * selector = bson_hbparam( 3, HB_IT_ANY );

    if ( collection && selector ) {
        int flags = hb_parnidef( 2, MONGOC_REMOVE_NONE );
        const mongoc_write_concern_t * write_concern = mongoc_hbparam( 4, _hbmongoc_write_concern_t_ );
        bson_error_t error;

        bool result = mongoc_collection_remove( collection, flags, selector, write_concern, &error );

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( selector );
    }
}

HB_FUNC( MONGOC_COLLECTION_UPDATE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * selector = bson_hbparam( 3, HB_IT_ANY );
    bson_t * update = bson_hbparam( 4, HB_IT_ANY );

    if ( collection && HB_ISNUM( 2 ) && selector && update ) {
        int flags = hb_parni( 2 );
        const mongoc_write_concern_t * write_concern = mongoc_hbparam( 5, _hbmongoc_write_concern_t_ );
        bson_error_t error;

        bool result = mongoc_collection_update( collection, flags, selector, update, write_concern, &error );

        bson_hbstor_byref_error( 6, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( selector );
    }

    if ( update && ! HB_ISPOINTER( 4 ) ) {
        bson_destroy( update );
    }
}

HB_FUNC(MONGOC_COLLECTION_UPDATE_ONE)
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );
    bson_t * update = bson_hbparam( 3, HB_IT_ANY );

    if (collection && selector && update) {
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_update_one(collection, selector, update, opts, &reply, &error);

        if ( opts && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( opts );
        }

        if (HB_ISBYREF(5)) {
            hbmongoc_return_byref_bson(5, bson_copy(&reply));
        }
        bson_destroy(&reply);

        bson_hbstor_byref_error( 6, &error, result );

        hb_retl(result);

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }

    if ( update && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( update );
    }
}

HB_FUNC( MONGOC_COLLECTION_UPDATE_MANY )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );
    bson_t * update = bson_hbparam( 3, HB_IT_ANY );

    if ( collection && selector && update ) {

        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );

        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_update_many(collection, selector, update, opts, &reply, &error);

        hbmongoc_return_byref_bson( 5, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 6, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( opts );
        }

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( selector );
    }

    if ( update && ! HB_ISPOINTER( 4 ) ) {
        bson_destroy( update );
    }
}

HB_FUNC( MONGOC_COLLECTION_WRITE_COMMAND_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && command ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_write_command_with_opts( collection, command, opts, &reply, &error );

        hbmongoc_return_byref_bson( 4, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_COLLECTION_COPY )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        mongoc_collection_t * copy = mongoc_collection_copy( collection );
        if ( copy ) {
            PHB_MONGOC phCopy = hbmongoc_new_dataContainer( _hbmongoc_collection_t_, copy );
            hb_retptrGC( phCopy );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_GET_NAME )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        hb_retc( mongoc_collection_get_name( collection ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_GET_READ_PREFS )
{
    const mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        mongoc_read_prefs_t * copy = mongoc_read_prefs_copy( mongoc_collection_get_read_prefs( collection ) );
        PHB_MONGOC phPrefs = hbmongoc_new_dataContainer( _hbmongoc_read_prefs_t_, copy );
        hb_retptrGC( phPrefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_SET_READ_PREFS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    const mongoc_read_prefs_t * prefs = mongoc_hbparam( 2, _hbmongoc_read_prefs_t_ );

    if ( collection && prefs ) {
        mongoc_collection_set_read_prefs( collection, prefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_GET_READ_CONCERN )
{
    const mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        mongoc_read_concern_t * copy = mongoc_read_concern_copy( mongoc_collection_get_read_concern( collection ) );
        PHB_MONGOC phRC = hbmongoc_new_dataContainer( _hbmongoc_read_concern_t_, copy );
        hb_retptrGC( phRC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_SET_READ_CONCERN )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    const mongoc_read_concern_t * rc = mongoc_hbparam( 2, _hbmongoc_read_concern_t_ );

    if ( collection && rc ) {
        mongoc_collection_set_read_concern( collection, rc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_GET_WRITE_CONCERN )
{
    const mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        mongoc_write_concern_t * copy = mongoc_write_concern_copy( mongoc_collection_get_write_concern( collection ) );
        PHB_MONGOC phWC = hbmongoc_new_dataContainer( _hbmongoc_write_concern_t_, copy );
        hb_retptrGC( phWC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_SET_WRITE_CONCERN )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    const mongoc_write_concern_t * wc = mongoc_hbparam( 2, _hbmongoc_write_concern_t_ );

    if ( collection && wc ) {
        mongoc_collection_set_write_concern( collection, wc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_DELETE_ONE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && selector ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_delete_one( collection, selector, opts, &reply, &error );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

        if (HB_ISBYREF(4)) {
            hbmongoc_return_byref_bson(4, bson_copy(&reply));
        }
        bson_destroy(&reply);

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl(result);
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }
}

HB_FUNC( MONGOC_COLLECTION_DELETE_MANY )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && selector ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_delete_many( collection, selector, opts, &reply, &error );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

        if (HB_ISBYREF(4)) {
            hbmongoc_return_byref_bson(4, bson_copy(&reply));
        }
        bson_destroy(&reply);

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl(result);
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }
}

HB_FUNC( MONGOC_COLLECTION_REPLACE_ONE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );
    bson_t * replacement = bson_hbparam( 3, HB_IT_ANY );

    if ( collection && selector && replacement ) {
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_replace_one( collection, selector, replacement, opts, &reply, &error );

        if ( opts && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( opts );
        }

        if (HB_ISBYREF(5)) {
            hbmongoc_return_byref_bson(5, bson_copy(&reply));
        }
        bson_destroy(&reply);

        bson_hbstor_byref_error( 6, &error, result );

        hb_retl(result);
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }

    if ( replacement && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( replacement );
    }
}

HB_FUNC( MONGOC_COLLECTION_DROP_INDEX )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    const char * index_name = hb_parc( 2 );

    if ( collection && index_name ) {
        bson_error_t error;

        bool result = mongoc_collection_drop_index( collection, index_name, &error );

        bson_hbstor_byref_error( 3, &error, result );

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_DROP_INDEX_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    const char * index_name = hb_parc( 2 );

    if ( collection && index_name ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_error_t error;

        bool result = mongoc_collection_drop_index_with_opts( collection, index_name, opts, &error );

        bson_hbstor_byref_error( 4, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_COUNT_DOCUMENTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * filter = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && filter ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 4, _hbmongoc_read_prefs_t_ );
        bson_t reply;
        bson_error_t error;

        int64_t count = mongoc_collection_count_documents( collection, filter, opts, read_prefs, &reply, &error );

        if ( HB_ISBYREF( 5 ) ) {
            hbmongoc_return_byref_bson( 5, bson_copy( &reply ) );
        }
        bson_destroy( &reply );

        bson_hbstor_byref_error( 6, &error, count != -1 );

        if ( count != -1 ) {
            hb_retnll( (HB_LONGLONG) count );
        } else {
            hb_ret();
        }

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( filter && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( filter );
    }
}

HB_FUNC( MONGOC_COLLECTION_ESTIMATED_DOCUMENT_COUNT )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t reply;
        bson_error_t error;

        int64_t count = mongoc_collection_estimated_document_count( collection, opts, read_prefs, &reply, &error );

        if ( HB_ISBYREF( 4 ) ) {
            hbmongoc_return_byref_bson( 4, bson_copy( &reply ) );
        }
        bson_destroy( &reply );

        bson_hbstor_byref_error( 5, &error, count != -1 );

        if ( count != -1 ) {
            hb_retnll( (HB_LONGLONG) count );
        } else {
            hb_ret();
        }

        if ( opts && ! HB_ISPOINTER( 2 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_FIND_AND_MODIFY )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * query = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && query && HB_ISBYREF( 8 ) ) {
        bson_t * sort = bson_hbparam( 3, HB_IT_ANY );
        bson_t * update = bson_hbparam( 4, HB_IT_ANY );
        bson_t * fields = bson_hbparam( 5, HB_IT_ANY );
        bool remove = hb_parldef( 6, false );
        bool upsert = hb_parldef( 7, false );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_find_and_modify( collection, query, sort, update, fields, remove, upsert, false, &reply, &error );

        hbmongoc_return_byref_bson( 8, bson_copy( &reply ) );
        bson_destroy( &reply );
        bson_hbstor_byref_error( 9, &error, result );

        hb_retl( result );

        if ( sort && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( sort );
        }

        if ( update && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( update );
        }

        if ( fields && ! HB_ISPOINTER( 5 ) ) {
            bson_destroy( fields );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( query && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( query );
    }
}

HB_FUNC( MONGOC_COLLECTION_FIND_AND_MODIFY_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * query = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && query ) {
        const mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 3, _hbmongoc_find_and_modify_opts_t_ );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_find_and_modify_with_opts( collection, query, opts, &reply, &error );

        if ( HB_ISBYREF( 4 ) ) {
            hbmongoc_return_byref_bson( 4, bson_copy( &reply ) );
        }
        bson_destroy( &reply );

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( query && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( query );
    }
}

HB_FUNC( MONGOC_COLLECTION_RENAME )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    const char * new_db = hb_parc( 2 );
    const char * new_name = hb_parc( 3 );

    if ( collection && new_db && new_name && HB_ISLOG( 4 ) ) {
        bson_error_t error;

        bool result = mongoc_collection_rename( collection, new_db, new_name, hb_parl( 4 ), &error );

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_RENAME_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    const char * new_db = hb_parc( 2 );
    const char * new_name = hb_parc( 3 );

    if ( collection && new_db && new_name && HB_ISLOG( 4 ) ) {
        bson_t * opts = bson_hbparam( 5, HB_IT_ANY );
        bson_error_t error;

        bool result = mongoc_collection_rename_with_opts( collection, new_db, new_name, hb_parl( 4 ), opts, &error );

        bson_hbstor_byref_error( 6, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 5 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_READ_COMMAND_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && command && HB_ISBYREF( 5 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_read_command_with_opts( collection, command, read_prefs, opts, &reply, &error );

        hbmongoc_return_byref_bson( 5, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 6, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_COLLECTION_READ_WRITE_COMMAND_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && command && HB_ISBYREF( 5 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_read_write_command_with_opts( collection, command, read_prefs, opts, &reply, &error );

        hbmongoc_return_byref_bson( 5, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 6, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_COLLECTION_COMMAND_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && command && HB_ISBYREF( 5 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_command_with_opts( collection, command, read_prefs, opts, &reply, &error );

        hbmongoc_return_byref_bson( 5, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 6, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_COLLECTION_WATCH )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * pipeline = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && pipeline ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

        mongoc_change_stream_t * stream = mongoc_collection_watch( collection, pipeline, opts );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

        if ( stream ) {
            PHB_MONGOC phStream = hbmongoc_new_dataContainer( _hbmongoc_change_stream_t_, stream );
            hb_retptrGC( phStream );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( pipeline && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( pipeline );
    }
}

HB_FUNC( MONGOC_COLLECTION_CREATE_INDEXES_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    PHB_ITEM pArray = hb_param( 2, HB_IT_ARRAY );

    if ( collection && pArray ) {
        HB_SIZE len = hb_arrayLen( pArray );
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;
        bool result = false;
        mongoc_index_model_t ** models = NULL;

        if ( len > 0 && len < (HB_SIZE) 1024 * 1024 ) {
            models = ( mongoc_index_model_t ** ) hb_xgrab( len * sizeof( mongoc_index_model_t * ) );
        }

        if ( models ) {
            result = true;

            for ( HB_SIZE i = 0; i < len && result; ++i ) {
                PHB_ITEM pItem = hb_itemArrayGet( pArray, i + 1 );
                bson_t * keys = get_bson_item( pItem );

                if ( keys ) {
                    models[ i ] = mongoc_index_model_new( keys, NULL );
                    result = models[ i ] != NULL;

                    if ( !( hb_itemType( pItem ) & HB_IT_POINTER ) ) {
                        bson_destroy( keys );
                    }
                } else {
                    models[ i ] = NULL;
                    result = false;
                }

                hb_itemRelease( pItem );
            }

            if ( result ) {
                result = mongoc_collection_create_indexes_with_opts( collection, models, ( size_t ) len, opts, &reply, &error );
            }

            for ( HB_SIZE i = 0; i < len; ++i ) {
                if ( models[ i ] ) {
                    mongoc_index_model_destroy( models[ i ] );
                }
            }

            hb_xfree( models );
        }

        if ( HB_ISBYREF( 4 ) ) {
            hbmongoc_return_byref_bson( 4, bson_copy( &reply ) );
        }
        bson_destroy( &reply );

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_INDEX_MODEL_NEW )
{
    bson_t * keys = bson_hbparam( 1, HB_IT_ANY );

    if ( keys ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );
        mongoc_index_model_t * model = mongoc_index_model_new( keys, opts );

        if ( model ) {
            PHB_MONGOC phModel = hbmongoc_new_dataContainer( _hbmongoc_index_model_t_, model );
            hb_retptrGC( phModel );
        } else {
            hb_ret();
        }

        if ( opts && ! HB_ISPOINTER( 2 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( keys && ! HB_ISPOINTER( 1 ) ) {
        bson_destroy( keys );
    }
}

HB_FUNC( MONGOC_INDEX_MODEL_DESTROY )
{
    PHB_MONGOC model = hbmongoc_param( 1, _hbmongoc_index_model_t_ );

    if ( model ) {
        mongoc_index_model_destroy( model->p );
        model->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
