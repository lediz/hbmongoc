//
//  hb_mongoc_bulk_operation.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/7/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_bulk_operation.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_BULK_OPERATION_DESTROY )
{
    PHB_MONGOC phBulk = hbmongoc_param( 1, _hbmongoc_bulk_operation_t_ );

    if ( phBulk ) {
        mongoc_bulk_operation_destroy( ( mongoc_bulk_operation_t * ) phBulk->p );
        phBulk->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_EXECUTE )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );

    if ( bulk && HB_ISBYREF( 2 ) ) {
        bson_t reply;
        bson_error_t error;

        uint32_t server_id = mongoc_bulk_operation_execute( bulk, &reply, &error );

        hbmongoc_return_byref_bson( 2, bson_copy(&reply) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 3, &error, server_id != 0 );

        hb_retni( server_id );

    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_INSERT )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * document = bson_hbparam( 2, HB_IT_ANY );

    if ( bulk && document ) {
        mongoc_bulk_operation_insert( bulk, document );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( document && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_INSERT_WITH_OPTS )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * document = bson_hbparam( 2, HB_IT_ANY );

    if ( bulk && document ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_error_t error;

        HB_BOOL result = mongoc_bulk_operation_insert_with_opts( bulk, document, opts, &error );

        bson_hbstor_byref_error( 4, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( document && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_NEW )
{
    bool ordered = hb_parldef( 1, false );
    mongoc_bulk_operation_t * bulk = mongoc_bulk_operation_new( ordered );

    if ( bulk ) {
        PHB_MONGOC phBulk = hbmongoc_new_dataContainer( _hbmongoc_bulk_operation_t_, bulk );
        hb_retptrGC( phBulk );
    } else {
        hb_ret();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_REMOVE )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );

    if ( bulk && selector ) {
        mongoc_bulk_operation_remove( bulk, selector );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_REMOVE_MANY_WITH_OPTS )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );

    if ( bulk && selector ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_error_t error;

        bool result = mongoc_bulk_operation_remove_many_with_opts( bulk, selector, opts, &error );

        bson_hbstor_byref_error( 4, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_REMOVE_ONE )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );

    if ( bulk && selector ) {
        mongoc_bulk_operation_remove_one( bulk, selector );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_REMOVE_ONE_WITH_OPTS )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );

    if ( bulk && selector ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_error_t error;

        bool result = mongoc_bulk_operation_remove_one_with_opts( bulk, selector, opts, &error );

        bson_hbstor_byref_error( 4, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_REPLACE_ONE )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );
    bson_t * document = bson_hbparam( 3, HB_IT_ANY );

    if ( bulk && selector && document && HB_ISLOG( 4 ) ) {
        bool upsert = hb_parl( 4 );
        mongoc_bulk_operation_replace_one( bulk, selector, document, upsert );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }

    if ( document && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_REPLACE_ONE_WITH_OPTS )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );
    bson_t * document = bson_hbparam( 3, HB_IT_ANY );

    if ( bulk && selector && document ) {
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_error_t error;

        bool result = mongoc_bulk_operation_replace_one_with_opts( bulk, selector, document, opts, &error );

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }

    if ( document && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_UPDATE )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );
    bson_t * document = bson_hbparam( 3, HB_IT_ANY );

    if ( bulk && selector && document && HB_ISLOG( 4 ) ) {
        bool upsert = hb_parl( 4 );
        mongoc_bulk_operation_update( bulk, selector, document, upsert );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }

    if ( document && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_UPDATE_MANY_WITH_OPTS )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );
    bson_t * document = bson_hbparam( 3, HB_IT_ANY );

    if ( bulk && selector && document ) {
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_error_t error;

        bool result = mongoc_bulk_operation_update_many_with_opts( bulk, selector, document, opts, &error );

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }

    if ( document && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_UPDATE_ONE )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );
    bson_t * document = bson_hbparam( 3, HB_IT_ANY );

    if ( bulk && selector && document && HB_ISLOG( 4 ) ) {
        bool upsert = hb_parl( 4 );
        mongoc_bulk_operation_update_one( bulk, selector, document, upsert );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }

    if ( document && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_UPDATE_ONE_WITH_OPTS )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * selector = bson_hbparam( 2, HB_IT_ANY );
    bson_t * document = bson_hbparam( 3, HB_IT_ANY );

    if ( bulk && selector && document ) {
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_error_t error;

        bool result = mongoc_bulk_operation_update_one_with_opts( bulk, selector, document, opts, &error );

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( selector );
    }

    if ( document && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_SET_BYPASS_DOCUMENT_VALIDATION )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );

    if ( bulk && HB_ISLOG( 2 ) ) {
        mongoc_bulk_operation_set_bypass_document_validation( bulk, hb_parl( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_GET_BYPASS_DOCUMENT_VALIDATION )
{
    const mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );

    if ( bulk ) {
        hb_retl( mongoc_bulk_operation_get_bypass_document_validation( bulk ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_SET_COMMENT )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    const bson_value_t * comment = bson_value_hbparam( 2 );

    if ( bulk && comment ) {
        mongoc_bulk_operation_set_comment( bulk, comment );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_GET_COMMENT )
{
    const mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );

    if ( bulk ) {
        const bson_value_t * comment = mongoc_bulk_operation_get_comment( bulk );
        if ( comment ) {
            bson_value_t * copy = hb_xgrab( sizeof( bson_value_t ) );
            bson_value_copy( comment, copy );
            PHB_BSON phValue = hbbson_new_dataContainer( _hbbson_value_t_, copy );
            hb_retptrGC( phValue );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_SET_LET )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    bson_t * let = bson_hbparam( 2, HB_IT_ANY );

    if ( bulk && let ) {
        mongoc_bulk_operation_set_let( bulk, let );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( let && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( let );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_GET_LET )
{
    const mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );

    if ( bulk ) {
        const bson_t * let = mongoc_bulk_operation_get_let( bulk );
        if ( let ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( let ) );
            hb_retptrGC( phBson );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_SET_DATABASE )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    const char * database = hb_parc( 2 );

    if ( bulk && database ) {
        mongoc_bulk_operation_set_database( bulk, database );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_GET_DATABASE )
{
    const mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );

    if ( bulk ) {
        hb_retc( mongoc_bulk_operation_get_database( bulk ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_SET_COLLECTION )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    const char * collection = hb_parc( 2 );

    if ( bulk && collection ) {
        mongoc_bulk_operation_set_collection( bulk, collection );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_GET_COLLECTION )
{
    const mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );

    if ( bulk ) {
        hb_retc( mongoc_bulk_operation_get_collection( bulk ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_SET_SERVER_ID )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );

    if ( bulk && HB_ISNUM( 2 ) ) {
        mongoc_bulk_operation_set_server_id( bulk, ( uint32_t ) hb_parni( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_GET_SERVER_ID )
{
    const mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );

    if ( bulk ) {
        hb_retni( ( int ) mongoc_bulk_operation_get_server_id( bulk ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_SET_WRITE_CONCERN )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    const mongoc_write_concern_t * wc = mongoc_hbparam( 2, _hbmongoc_write_concern_t_ );

    if ( bulk && wc ) {
        mongoc_bulk_operation_set_write_concern( bulk, wc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_GET_WRITE_CONCERN )
{
    const mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );

    if ( bulk ) {
        mongoc_write_concern_t * copy = mongoc_write_concern_copy( mongoc_bulk_operation_get_write_concern( bulk ) );
        PHB_MONGOC phWC = hbmongoc_new_dataContainer( _hbmongoc_write_concern_t_, copy );
        hb_retptrGC( phWC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_SET_CLIENT )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    mongoc_client_t * client = mongoc_hbparam( 2, _hbmongoc_client_t_ );

    if ( bulk && client ) {
        mongoc_bulk_operation_set_client( bulk, client );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_SET_CLIENT_SESSION )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bulk_operation_t_ );
    mongoc_client_session_t * session = mongoc_hbparam( 2, _hbmongoc_client_session_t_ );

    if ( bulk && session ) {
        mongoc_bulk_operation_set_client_session( bulk, session );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
