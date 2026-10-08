//
//  hb_mongoc_uri.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_uri.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_URI_NEW )
{
    const char * uri_string = hb_parc( 1 );

    if ( uri_string ) {
        mongoc_uri_t * uri = mongoc_uri_new( uri_string );
        if ( uri ) {
            PHB_MONGOC phURI = hbmongoc_new_dataContainer( _hbmongoc_uri_t_, uri );
            hb_retptrGC( phURI );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_NEW_WITH_ERROR )
{
    const char * uri_string = hb_parc( 1 );

    if ( uri_string ) {
        bson_error_t error;
        mongoc_uri_t * uri = mongoc_uri_new_with_error( uri_string, &error );

        bson_hbstor_byref_error( 2, &error, uri != NULL );

        if ( uri ) {
            PHB_MONGOC phURI = hbmongoc_new_dataContainer( _hbmongoc_uri_t_, uri );
            hb_retptrGC( phURI );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_NEW_FOR_HOST_PORT )
{
    const char * hostname = hb_parc( 1 );

    if ( hostname && HB_ISNUM( 2 ) ) {
        mongoc_uri_t * uri = mongoc_uri_new_for_host_port( hostname, ( uint16_t ) hb_parni( 2 ) );
        if ( uri ) {
            PHB_MONGOC phURI = hbmongoc_new_dataContainer( _hbmongoc_uri_t_, uri );
            hb_retptrGC( phURI );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_COPY )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        mongoc_uri_t * copy = mongoc_uri_copy( uri );
        PHB_MONGOC phURI = hbmongoc_new_dataContainer( _hbmongoc_uri_t_, copy );
        hb_retptrGC( phURI );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_DESTROY )
{
    PHB_MONGOC uri = hbmongoc_param( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        mongoc_uri_destroy( uri->p );
        uri->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_SRV_HOSTNAME )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_srv_hostname( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_SRV_SERVICE_NAME )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_srv_service_name( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_DATABASE )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_database( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_DATABASE )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * database = hb_parc( 2 );

    if ( uri && database ) {
        hb_retl( mongoc_uri_set_database( uri, database ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_PASSWORD )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_password( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_PASSWORD )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * password = hb_parc( 2 );

    if ( uri && password ) {
        hb_retl( mongoc_uri_set_password( uri, password ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_USERNAME )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_username( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_USERNAME )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * username = hb_parc( 2 );

    if ( uri && username ) {
        hb_retl( mongoc_uri_set_username( uri, username ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_CREDENTIALS )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        const bson_t * credentials = mongoc_uri_get_credentials( uri );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( credentials ) );
        hb_retptrGC( phBson );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_AUTH_SOURCE )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_auth_source( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_AUTH_SOURCE )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * value = hb_parc( 2 );

    if ( uri && value ) {
        hb_retl( mongoc_uri_set_auth_source( uri, value ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_APPNAME )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_appname( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_APPNAME )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * value = hb_parc( 2 );

    if ( uri && value ) {
        hb_retl( mongoc_uri_set_appname( uri, value ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_AUTH_MECHANISM )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_auth_mechanism( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_AUTH_MECHANISM )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * value = hb_parc( 2 );

    if ( uri && value ) {
        hb_retl( mongoc_uri_set_auth_mechanism( uri, value ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_REPLICA_SET )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_replica_set( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_STRING )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_string( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_COMPRESSORS )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        const bson_t * compressors = mongoc_uri_get_compressors( uri );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( compressors ) );
        hb_retptrGC( phBson );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_COMPRESSORS )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * value = hb_parc( 2 );

    if ( uri && value ) {
        hb_retl( mongoc_uri_set_compressors( uri, value ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_SERVER_MONITORING_MODE )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retc( mongoc_uri_get_server_monitoring_mode( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_SERVER_MONITORING_MODE )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * value = hb_parc( 2 );

    if ( uri && value ) {
        hb_retl( mongoc_uri_set_server_monitoring_mode( uri, value ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_TLS )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        hb_retl( mongoc_uri_get_tls( uri ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_HAS_OPTION )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * key = hb_parc( 2 );

    if ( uri && key ) {
        hb_retl( mongoc_uri_has_option( uri, key ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_OPTION_IS_INT32 )
{
    const char * key = hb_parc( 1 );

    if ( key ) {
        hb_retl( mongoc_uri_option_is_int32( key ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_OPTION_IS_INT64 )
{
    const char * key = hb_parc( 1 );

    if ( key ) {
        hb_retl( mongoc_uri_option_is_int64( key ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_OPTION_IS_BOOL )
{
    const char * key = hb_parc( 1 );

    if ( key ) {
        hb_retl( mongoc_uri_option_is_bool( key ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_OPTION_IS_UTF8 )
{
    const char * key = hb_parc( 1 );

    if ( key ) {
        hb_retl( mongoc_uri_option_is_utf8( key ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_OPTION_AS_INT32 )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * option = hb_parc( 2 );

    if ( uri && option ) {
        hb_retni( mongoc_uri_get_option_as_int32( uri, option, hb_parni( 3 ) ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_OPTION_AS_INT64 )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * option = hb_parc( 2 );

    if ( uri && option ) {
        hb_retnll( mongoc_uri_get_option_as_int64( uri, option, hb_parnll( 3 ) ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_OPTION_AS_BOOL )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * option = hb_parc( 2 );

    if ( uri && option ) {
        bool fallback = HB_ISLOG( 3 ) ? hb_parl( 3 ) : false;
        hb_retl( mongoc_uri_get_option_as_bool( uri, option, fallback ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_OPTION_AS_UTF8 )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * option = hb_parc( 2 );

    if ( uri && option ) {
        const char * value = mongoc_uri_get_option_as_utf8( uri, option, hb_parc( 3 ) );
        if ( value ) {
            hb_retc( value );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_OPTION_AS_INT32 )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * option = hb_parc( 2 );

    if ( uri && option && HB_ISNUM( 3 ) ) {
        hb_retl( mongoc_uri_set_option_as_int32( uri, option, hb_parni( 3 ) ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_OPTION_AS_INT64 )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * option = hb_parc( 2 );

    if ( uri && option && HB_ISNUM( 3 ) ) {
        hb_retl( mongoc_uri_set_option_as_int64( uri, option, hb_parnll( 3 ) ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_OPTION_AS_BOOL )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * option = hb_parc( 2 );

    if ( uri && option && HB_ISLOG( 3 ) ) {
        hb_retl( mongoc_uri_set_option_as_bool( uri, option, hb_parl( 3 ) ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_OPTION_AS_UTF8 )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const char * option = hb_parc( 2 );
    const char * value = hb_parc( 3 );

    if ( uri && option && value ) {
        hb_retl( mongoc_uri_set_option_as_utf8( uri, option, value ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_OPTIONS )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        const bson_t * options = mongoc_uri_get_options( uri );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( options ) );
        hb_retptrGC( phBson );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_MECHANISM_PROPERTIES )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri && HB_ISBYREF( 2 ) ) {
        bson_t properties;
        bool result = mongoc_uri_get_mechanism_properties( uri, &properties );

        if ( result ) {
            hbmongoc_return_byref_bson( 2, bson_copy( &properties ) );
            bson_destroy( &properties );
        } else {
            hb_stor( 2 );
        }

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_MECHANISM_PROPERTIES )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    bson_t * properties = bson_hbparam( 2, HB_IT_ANY );

    if ( uri && properties ) {
        hb_retl( mongoc_uri_set_mechanism_properties( uri, properties ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( properties && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( properties );
    }
}

HB_FUNC( MONGOC_URI_GET_READ_PREFS_T )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        mongoc_read_prefs_t * copy = mongoc_read_prefs_copy( mongoc_uri_get_read_prefs_t( uri ) );
        PHB_MONGOC phPrefs = hbmongoc_new_dataContainer( _hbmongoc_read_prefs_t_, copy );
        hb_retptrGC( phPrefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_READ_PREFS_T )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const mongoc_read_prefs_t * prefs = mongoc_hbparam( 2, _hbmongoc_read_prefs_t_ );

    if ( uri && prefs ) {
        mongoc_uri_set_read_prefs_t( uri, prefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_WRITE_CONCERN )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        mongoc_write_concern_t * copy = mongoc_write_concern_copy( mongoc_uri_get_write_concern( uri ) );
        PHB_MONGOC phWC = hbmongoc_new_dataContainer( _hbmongoc_write_concern_t_, copy );
        hb_retptrGC( phWC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_WRITE_CONCERN )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const mongoc_write_concern_t * wc = mongoc_hbparam( 2, _hbmongoc_write_concern_t_ );

    if ( uri && wc ) {
        mongoc_uri_set_write_concern( uri, wc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_READ_CONCERN )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        mongoc_read_concern_t * copy = mongoc_read_concern_copy( mongoc_uri_get_read_concern( uri ) );
        PHB_MONGOC phRC = hbmongoc_new_dataContainer( _hbmongoc_read_concern_t_, copy );
        hb_retptrGC( phRC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_SET_READ_CONCERN )
{
    mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );
    const mongoc_read_concern_t * rc = mongoc_hbparam( 2, _hbmongoc_read_concern_t_ );

    if ( uri && rc ) {
        mongoc_uri_set_read_concern( uri, rc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_UNESCAPE )
{
    const char * escaped = hb_parc( 1 );

    if ( escaped ) {
        char * unescaped = mongoc_uri_unescape( escaped );
        if ( unescaped ) {
            hb_retc( unescaped );
            bson_free( unescaped );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_URI_GET_HOSTS )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        const mongoc_host_list_t * host = mongoc_uri_get_hosts( uri );

        if ( host ) {
            PHB_ITEM pItemHash = hb_itemNew( NULL );
            hb_hashNew( pItemHash );

            PHB_ITEM pItemKey = hb_itemNew( NULL );
            PHB_ITEM pItemValue = hb_itemNew( NULL );
            hb_itemPutC( pItemKey, "host" );
            hb_itemPutC( pItemValue, host->host );
            hb_hashAdd( pItemHash, pItemKey, pItemValue );
            hb_itemRelease( pItemKey );
            hb_itemRelease( pItemValue );

            pItemKey = hb_itemNew( NULL );
            pItemValue = hb_itemNew( NULL );
            hb_itemPutC( pItemKey, "port" );
            hb_itemPutNI( pItemValue, host->port );
            hb_hashAdd( pItemHash, pItemKey, pItemValue );
            hb_itemRelease( pItemKey );
            hb_itemRelease( pItemValue );

            hb_itemReturnRelease( pItemHash );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
