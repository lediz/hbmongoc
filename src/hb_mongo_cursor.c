//
//  hb_mongo_cursor.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/5/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongo_cursor.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_CURSOR_ERROR )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if (cursor && HB_ISBYREF(2)) {
        bson_error_t error;
        bool result = mongoc_cursor_error(cursor, &error);
        bson_hbstor_byref_error( 2, &error, ! result );
        hb_retl(result);
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_NEXT )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor && HB_ISBYREF( 2 ) ) {

        const bson_t * doc;

        bool result = mongoc_cursor_next( cursor, &doc );

        if ( result ) {
            hbmongoc_return_byref_bson( 2, bson_copy( doc ) );
        } else {
            hb_stor( 2 );
        }

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_DESTROY )
{
    PHB_MONGOC cursor = hbmongoc_param( 1, _hbmongoc_cursor_t_ );

    if ( cursor ) {
        mongoc_cursor_destroy( cursor->p );
        cursor->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_ERROR_DOCUMENT )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor && HB_ISBYREF( 2 ) ) {
        bson_error_t error;
        const bson_t * doc = NULL;

        bool result = mongoc_cursor_error_document( cursor, &error, &doc );

        if ( HB_ISBYREF( 3 ) && result && doc ) {
            hbmongoc_return_byref_bson( 3, bson_copy( doc ) );
        }

        bson_hbstor_byref_error( 2, &error, ! result );

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_CURRENT )
{
    const mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor ) {
        const bson_t * doc = mongoc_cursor_current( cursor );
        if ( doc ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( doc ) );
            hb_retptrGC( phBson );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_MORE )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor ) {
        hb_retl( mongoc_cursor_more( cursor ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_CLONE )
{
    const mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor ) {
        mongoc_cursor_t * copy = mongoc_cursor_clone( cursor );
        if ( copy ) {
            PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, copy );
            hb_retptrGC( phCursor );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_GET_BATCH_SIZE )
{
    const mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor ) {
        hb_retni( ( int ) mongoc_cursor_get_batch_size( cursor ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_SET_BATCH_SIZE )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor && HB_ISNUM( 2 ) ) {
        mongoc_cursor_set_batch_size( cursor, ( uint32_t ) hb_parni( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_GET_LIMIT )
{
    const mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor ) {
        hb_retnll( (HB_LONGLONG) mongoc_cursor_get_limit( cursor ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_SET_LIMIT )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor && HB_ISNUM( 2 ) ) {
        bool result = mongoc_cursor_set_limit( cursor, ( int64_t ) hb_parnll( 2 ) );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_GET_HINT )
{
    const mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor ) {
        // removed in mongo-c-driver 2.x; the closest remaining accessor is
        // the server id, which is what the deprecated hint now maps to
        hb_retni( ( int ) mongoc_cursor_get_server_id( cursor ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_SET_HINT )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor && HB_ISNUM( 2 ) ) {
        bool result = mongoc_cursor_set_server_id( cursor, ( uint32_t ) hb_parni( 2 ) );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_GET_ID )
{
    const mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor ) {
        hb_retnll( (HB_LONGLONG) mongoc_cursor_get_id( cursor ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_GET_MAX_AWAIT_TIME_MS )
{
    const mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor ) {
        hb_retni( ( int ) mongoc_cursor_get_max_await_time_ms( cursor ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_SET_MAX_AWAIT_TIME_MS )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor && HB_ISNUM( 2 ) ) {
        mongoc_cursor_set_max_await_time_ms( cursor, ( uint32_t ) hb_parni( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_GET_SERVER_ID )
{
    const mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor ) {
        hb_retni( ( int ) mongoc_cursor_get_server_id( cursor ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_SET_SERVER_ID )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor && HB_ISNUM( 2 ) ) {
        bool result = mongoc_cursor_set_server_id( cursor, ( uint32_t ) hb_parni( 2 ) );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_GET_HOST )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hbmongoc_cursor_t_ );

    if ( cursor && HB_ISBYREF( 2 ) ) {
        mongoc_host_list_t host;
        memset( &host, 0, sizeof( host ) );
        mongoc_cursor_get_host( cursor, &host );

        PHB_ITEM pItemHash = hb_itemNew( NULL );
        hb_hashNew( pItemHash );

        PHB_ITEM pItemKey = hb_itemNew( NULL );
        PHB_ITEM pItemValue = hb_itemNew( NULL );
        hb_itemPutC( pItemKey, "host" );
        hb_itemPutC( pItemValue, host.host );
        hb_hashAdd( pItemHash, pItemKey, pItemValue );
        hb_itemRelease( pItemKey );
        hb_itemRelease( pItemValue );

        pItemKey = hb_itemNew( NULL );
        pItemValue = hb_itemNew( NULL );
        hb_itemPutC( pItemKey, "port" );
        hb_itemPutNI( pItemValue, host.port );
        hb_hashAdd( pItemHash, pItemKey, pItemValue );
        hb_itemRelease( pItemKey );
        hb_itemRelease( pItemValue );

        hb_itemParamStoreRelease( 2, pItemHash );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CURSOR_NEW_FROM_COMMAND_REPLY_WITH_OPTS )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    bson_t * reply = bson_hbparam( 2, HB_IT_ANY );

    if ( client && reply ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

        mongoc_cursor_t * cursor = mongoc_cursor_new_from_command_reply_with_opts( client, reply, opts );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

        if ( cursor ) {
            PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );
            hb_retptrGC( phCursor );
        } else {
            if ( ! HB_ISPOINTER( 2 ) ) {
                bson_destroy( reply );
            }
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
