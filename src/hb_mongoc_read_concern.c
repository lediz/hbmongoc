//
//  hb_mongoc_read_concern.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc.h"

HB_FUNC( MONGOC_READ_CONCERN_NEW )
{
    mongoc_read_concern_t * rc = mongoc_read_concern_new();
    PHB_MONGOC phRC = hbmongoc_new_dataContainer( _hbmongoc_read_concern_t_, rc );
    hb_retptrGC( phRC );
}

HB_FUNC( MONGOC_READ_CONCERN_COPY )
{
    const mongoc_read_concern_t * rc = mongoc_hbparam( 1, _hbmongoc_read_concern_t_ );

    if ( rc ) {
        mongoc_read_concern_t * copy = mongoc_read_concern_copy( rc );
        PHB_MONGOC phRC = hbmongoc_new_dataContainer( _hbmongoc_read_concern_t_, copy );
        hb_retptrGC( phRC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_CONCERN_DESTROY )
{
    PHB_MONGOC rc = hbmongoc_param( 1, _hbmongoc_read_concern_t_ );

    if ( rc ) {
        mongoc_read_concern_destroy( rc->p );
        rc->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_CONCERN_GET_LEVEL )
{
    const mongoc_read_concern_t * rc = mongoc_hbparam( 1, _hbmongoc_read_concern_t_ );

    if ( rc ) {
        hb_retc( mongoc_read_concern_get_level( rc ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_CONCERN_SET_LEVEL )
{
    mongoc_read_concern_t * rc = mongoc_hbparam( 1, _hbmongoc_read_concern_t_ );
    const char * level = hb_parc( 2 );

    if ( rc && level ) {
        hb_retl( mongoc_read_concern_set_level( rc, level ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_CONCERN_APPEND )
{
    mongoc_read_concern_t * rc = mongoc_hbparam( 1, _hbmongoc_read_concern_t_ );
    bson_t * doc = bson_hbparam( 2, HB_IT_POINTER );

    if ( rc && doc ) {
        hb_retl( mongoc_read_concern_append( rc, doc ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_CONCERN_IS_DEFAULT )
{
    const mongoc_read_concern_t * rc = mongoc_hbparam( 1, _hbmongoc_read_concern_t_ );

    if ( rc ) {
        hb_retl( mongoc_read_concern_is_default( rc ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
