//
//  hb_mongoc_change_stream.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc.h"

HB_FUNC( MONGOC_CHANGE_STREAM_DESTROY )
{
    PHB_MONGOC stream = hbmongoc_param( 1, _hbmongoc_change_stream_t_ );

    if ( stream ) {
        mongoc_change_stream_destroy( stream->p );
        stream->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CHANGE_STREAM_NEXT )
{
    mongoc_change_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_change_stream_t_ );

    if ( stream && HB_ISBYREF( 2 ) ) {
        const bson_t * doc;

        bool result = mongoc_change_stream_next( stream, &doc );

        if ( result && doc ) {
            hbmongoc_return_byref_bson( 2, bson_copy( doc ) );
        } else {
            hb_stor( 2 );
        }

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CHANGE_STREAM_ERROR_DOCUMENT )
{
    mongoc_change_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_change_stream_t_ );

    if ( stream && HB_ISBYREF( 2 ) ) {
        bson_error_t error;
        const bson_t * doc = NULL;

        bool result = mongoc_change_stream_error_document( stream, &error, &doc );

        if ( HB_ISBYREF( 3 ) && result && doc ) {
            hbmongoc_return_byref_bson( 3, bson_copy( doc ) );
        }

        bson_hbstor_byref_error( 2, &error, ! result );

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CHANGE_STREAM_GET_RESUME_TOKEN )
{
    const mongoc_change_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_change_stream_t_ );

    if ( stream ) {
        const bson_t * token = mongoc_change_stream_get_resume_token( stream );
        if ( token ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( token ) );
            hb_retptrGC( phBson );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
