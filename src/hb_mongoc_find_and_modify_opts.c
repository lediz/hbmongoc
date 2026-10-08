//
//  hb_mongoc_find_and_modify_opts.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc.h"

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_NEW )
{
    mongoc_find_and_modify_opts_t * opts = mongoc_find_and_modify_opts_new();
    if ( opts ) {
        PHB_MONGOC phOpts = hbmongoc_new_dataContainer( _hbmongoc_find_and_modify_opts_t_, opts );
        hb_retptrGC( phOpts );
    } else {
        hb_ret();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_DESTROY )
{
    PHB_MONGOC opts = hbmongoc_param( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts ) {
        mongoc_find_and_modify_opts_destroy( opts->p );
        opts->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_SET_SORT )
{
    mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );
    bson_t * sort = bson_hbparam( 2, HB_IT_ANY );

    if ( opts && sort ) {
        mongoc_find_and_modify_opts_set_sort( opts, sort );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( sort && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( sort );
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_GET_SORT )
{
    const mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts && HB_ISBYREF( 2 ) ) {
        bson_t sort;
        bson_init( &sort );
        mongoc_find_and_modify_opts_get_sort( opts, &sort );
        hbmongoc_return_byref_bson( 2, bson_copy( &sort ) );
        bson_destroy( &sort );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_SET_UPDATE )
{
    mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );
    bson_t * update = bson_hbparam( 2, HB_IT_ANY );

    if ( opts && update ) {
        mongoc_find_and_modify_opts_set_update( opts, update );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( update && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( update );
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_GET_UPDATE )
{
    const mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts && HB_ISBYREF( 2 ) ) {
        bson_t update;
        bson_init( &update );
        mongoc_find_and_modify_opts_get_update( opts, &update );
        hbmongoc_return_byref_bson( 2, bson_copy( &update ) );
        bson_destroy( &update );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_SET_FIELDS )
{
    mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );
    bson_t * fields = bson_hbparam( 2, HB_IT_ANY );

    if ( opts && fields ) {
        mongoc_find_and_modify_opts_set_fields( opts, fields );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( fields && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( fields );
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_GET_FIELDS )
{
    const mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts && HB_ISBYREF( 2 ) ) {
        bson_t fields;
        bson_init( &fields );
        mongoc_find_and_modify_opts_get_fields( opts, &fields );
        hbmongoc_return_byref_bson( 2, bson_copy( &fields ) );
        bson_destroy( &fields );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_SET_FLAGS )
{
    mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts && HB_ISNUM( 2 ) ) {
        mongoc_find_and_modify_flags_t flags = ( mongoc_find_and_modify_flags_t ) hb_parni( 2 );
        mongoc_find_and_modify_opts_set_flags( opts, flags );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_GET_FLAGS )
{
    const mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts ) {
        mongoc_find_and_modify_flags_t flags = mongoc_find_and_modify_opts_get_flags( opts );
        hb_retni( ( int ) flags );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_SET_BYPASS_DOCUMENT_VALIDATION )
{
    mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts && HB_ISLOG( 2 ) ) {
        mongoc_find_and_modify_opts_set_bypass_document_validation( opts, hb_parl( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_GET_BYPASS_DOCUMENT_VALIDATION )
{
    const mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts ) {
        hb_retl( mongoc_find_and_modify_opts_get_bypass_document_validation( opts ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_SET_MAX_TIME_MS )
{
    mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts && HB_ISNUM( 2 ) ) {
        uint32_t max_time_ms = ( uint32_t ) hb_parni( 2 );
        mongoc_find_and_modify_opts_set_max_time_ms( opts, max_time_ms );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_GET_MAX_TIME_MS )
{
    const mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts ) {
        hb_retni( ( int ) mongoc_find_and_modify_opts_get_max_time_ms( opts ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_APPEND )
{
    mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );
    bson_t * extra = bson_hbparam( 2, HB_IT_ANY );

    if ( opts && extra ) {
        bool result = mongoc_find_and_modify_opts_append( opts, extra );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( extra && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( extra );
    }
}

HB_FUNC( MONGOC_FIND_AND_MODIFY_OPTS_GET_EXTRA )
{
    const mongoc_find_and_modify_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_find_and_modify_opts_t_ );

    if ( opts && HB_ISBYREF( 2 ) ) {
        bson_t extra;
        bson_init( &extra );
        mongoc_find_and_modify_opts_get_extra( opts, &extra );
        hbmongoc_return_byref_bson( 2, bson_copy( &extra ) );
        bson_destroy( &extra );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
