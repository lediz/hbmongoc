//
//  hb_mongoc_server_description.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc.h"

HB_FUNC( MONGOC_SERVER_DESCRIPTION_DESTROY )
{
    PHB_MONGOC description = hbmongoc_param( 1, _hbmongoc_server_description_t_ );

    if ( description ) {
        mongoc_server_description_destroy( description->p );
        description->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_DESCRIPTION_NEW_COPY )
{
    const mongoc_server_description_t * description = mongoc_hbparam( 1, _hbmongoc_server_description_t_ );

    if ( description ) {
        mongoc_server_description_t * copy = mongoc_server_description_new_copy( description );
        if ( copy ) {
            PHB_MONGOC phDescription = hbmongoc_new_dataContainer( _hbmongoc_server_description_t_, copy );
            hb_retptrGC( phDescription );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_DESCRIPTION_ID )
{
    const mongoc_server_description_t * description = mongoc_hbparam( 1, _hbmongoc_server_description_t_ );

    if ( description ) {
        hb_retni( ( int ) mongoc_server_description_id( description ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_DESCRIPTION_TYPE )
{
    const mongoc_server_description_t * description = mongoc_hbparam( 1, _hbmongoc_server_description_t_ );

    if ( description ) {
        hb_retc( mongoc_server_description_type( description ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_DESCRIPTION_HELLO_RESPONSE )
{
    const mongoc_server_description_t * description = mongoc_hbparam( 1, _hbmongoc_server_description_t_ );

    if ( description ) {
        const bson_t * hello = mongoc_server_description_hello_response( description );
        if ( hello ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( hello ) );
            hb_retptrGC( phBson );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_DESCRIPTION_COMPRESSOR_ID )
{
    const mongoc_server_description_t * description = mongoc_hbparam( 1, _hbmongoc_server_description_t_ );

    if ( description ) {
        hb_retni( mongoc_server_description_compressor_id( description ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_DESCRIPTION_LAST_UPDATE_TIME )
{
    const mongoc_server_description_t * description = mongoc_hbparam( 1, _hbmongoc_server_description_t_ );

    if ( description ) {
        hb_retnll( (HB_LONGLONG) mongoc_server_description_last_update_time( description ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_DESCRIPTION_ROUND_TRIP_TIME )
{
    const mongoc_server_description_t * description = mongoc_hbparam( 1, _hbmongoc_server_description_t_ );

    if ( description ) {
        hb_retnll( (HB_LONGLONG) mongoc_server_description_round_trip_time( description ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_DESCRIPTIONS_DESTROY_ALL )
{
    PHB_ITEM pArray = hb_param( 1, HB_IT_ARRAY );

    if ( pArray ) {
        HB_SIZE len = hb_arrayLen( pArray );
        mongoc_server_description_t ** sds = NULL;

        if ( len > 0 && len < (HB_SIZE) 1024 * 1024 ) {
            sds = ( mongoc_server_description_t ** ) hb_xgrab( len * sizeof( mongoc_server_description_t * ) );
        }

        if ( sds ) {
            for ( HB_SIZE i = 0; i < len; ++i ) {
                PHB_MONGOC phDescription = NULL;
                PHB_ITEM pItem = hb_itemArrayGet( pArray, i + 1 );

                if ( pItem && ( hb_itemType( pItem ) & HB_IT_POINTER ) ) {
                    phDescription = hbmongoc_hbparam( pItem, _hbmongoc_server_description_t_ );
                }
                sds[ i ] = phDescription ? phDescription->p : NULL;
                hb_itemRelease( pItem );
            }

            mongoc_server_descriptions_destroy_all( sds, len );
            hb_xfree( sds );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
