//
//  hb_mongoc_client.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_client.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_CLIENT_COMMAND_SIMPLE )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    bson_t * command = bson_hbparam( 3, HB_IT_ANY );
    const char * db_name = hb_parc( 2 );

    if ( client && db_name && command && HB_ISBYREF( 5 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 4, _hbmongoc_read_prefs_t_ );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_client_command_simple( client, db_name, command, read_prefs, &reply, &error);

        hbmongoc_return_byref_bson( 5, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 6, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_CLIENT_DESTROY )
{
    PHB_MONGOC client = hbmongoc_param( 1, _hbmongoc_client_t_ );

    if( client ) {
        mongoc_client_destroy( client->p );
        client->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_COLLECTION )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const char * db = hb_parc( 2 );
    const char * szCollection = hb_parc( 3 );

    if ( client && db && szCollection ) {
        mongoc_collection_t * collection = mongoc_client_get_collection( client, db, szCollection);
        if ( collection ) {
            PHB_MONGOC phCollection = hbmongoc_new_dataContainer( _hbmongoc_collection_t_, collection );
            hb_retptrGC( phCollection );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_DATABASE )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const char * name = hb_parc( 2 );

    if ( client && name ) {
        mongoc_database_t * database = mongoc_client_get_database( client, name );
        if ( database ) {
            PHB_MONGOC phDatabase = hbmongoc_new_dataContainer( _hbmongoc_database_t_, database );
            hb_retptrGC( phDatabase );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_NEW )
{
    const char *uri_string = hb_parc( 1 );

    if ( uri_string ) {
        mongoc_client_t * client = mongoc_client_new( uri_string );
        if ( client ) {
            PHB_MONGOC phClient = hbmongoc_new_dataContainer( _hbmongoc_client_t_, client );
            hb_retptrGC( phClient );
        } else {
            hb_ret();
        }
    } else {
//        HBMONGOC_ERR_NOFUNC();
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_NEW_FROM_URI )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri ) {
        mongoc_client_t * client = mongoc_client_new_from_uri( uri );
        if ( client ) {
            PHB_MONGOC phClient = hbmongoc_new_dataContainer( _hbmongoc_client_t_, client );
            hb_retptrGC( phClient );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SET_APPNAME )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const char * appname = hb_parc( 2 );

    if ( client && appname ) {
        bool result = mongoc_client_set_appname( client, appname );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_NEW_FROM_URI_WITH_ERROR )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

    if ( uri && HB_ISBYREF( 2 ) ) {
        bson_error_t error;
        mongoc_client_t * client = mongoc_client_new_from_uri_with_error( uri, &error );

        bson_hbstor_byref_error( 2, &error, client != NULL );

        if ( client ) {
            PHB_MONGOC phClient = hbmongoc_new_dataContainer( _hbmongoc_client_t_, client );
            hb_retptrGC( phClient );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_DEFAULT_DATABASE )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        mongoc_database_t * database = mongoc_client_get_default_database( client );
        if ( database ) {
            PHB_MONGOC phDatabase = hbmongoc_new_dataContainer( _hbmongoc_database_t_, database );
            hb_retptrGC( phDatabase );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_DATABASE_NAMES_WITH_OPTS )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );
        bson_error_t error;

        char ** names = mongoc_client_get_database_names_with_opts( client, opts, &error );

        bson_hbstor_byref_error( 3, &error, names != NULL );

        if ( names ) {
            PHB_ITEM pItemArray = hb_itemNew( NULL );
            hb_arrayNew( pItemArray, 0 );

            for ( int i = 0; names[ i ]; ++i ) {
                PHB_ITEM pItem = hb_itemNew( NULL );
                hb_itemPutC( pItem, names[ i ] );
                hb_arrayAdd( pItemArray, pItem );
                hb_itemRelease( pItem );
            }

            bson_strfreev( names );

            hb_itemReturnRelease( pItemArray );
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

HB_FUNC( MONGOC_CLIENT_FIND_DATABASES_WITH_OPTS )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

        mongoc_cursor_t * cursor = mongoc_client_find_databases_with_opts( client, opts );

        if ( opts && ! HB_ISPOINTER( 2 ) ) {
            bson_destroy( opts );
        }

        if ( cursor ) {
            PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );
            hb_retptrGC( phCursor );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_COMMAND_SIMPLE_WITH_SERVER_ID )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const char * db_name = hb_parc( 2 );
    bson_t * command = bson_hbparam( 3, HB_IT_ANY );

    if ( client && db_name && command && HB_ISNUM( 5 ) && HB_ISBYREF( 6 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 4, _hbmongoc_read_prefs_t_ );
        uint32_t server_id = ( uint32_t ) hb_parni( 5 );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_client_command_simple_with_server_id( client, db_name, command, read_prefs, server_id, &reply, &error );

        hbmongoc_return_byref_bson( 6, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 7, &error, result );

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_CLIENT_COMMAND_WITH_OPTS )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const char * db_name = hb_parc( 2 );
    bson_t * command = bson_hbparam( 3, HB_IT_ANY );

    if ( client && db_name && command && HB_ISBYREF( 6 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 4, _hbmongoc_read_prefs_t_ );
        bson_t * opts = bson_hbparam( 5, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_client_command_with_opts( client, db_name, command, read_prefs, opts, &reply, &error );

        hbmongoc_return_byref_bson( 6, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 7, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 5 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_CLIENT_READ_COMMAND_WITH_OPTS )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const char * db_name = hb_parc( 2 );
    bson_t * command = bson_hbparam( 3, HB_IT_ANY );

    if ( client && db_name && command && HB_ISBYREF( 6 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 4, _hbmongoc_read_prefs_t_ );
        bson_t * opts = bson_hbparam( 5, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_client_read_command_with_opts( client, db_name, command, read_prefs, opts, &reply, &error );

        hbmongoc_return_byref_bson( 6, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 7, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 5 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_CLIENT_WRITE_COMMAND_WITH_OPTS )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const char * db_name = hb_parc( 2 );
    bson_t * command = bson_hbparam( 3, HB_IT_ANY );

    if ( client && db_name && command && HB_ISBYREF( 5 ) ) {
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_client_write_command_with_opts( client, db_name, command, opts, &reply, &error );

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

    if ( command && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_CLIENT_READ_WRITE_COMMAND_WITH_OPTS )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const char * db_name = hb_parc( 2 );
    bson_t * command = bson_hbparam( 3, HB_IT_ANY );

    if ( client && db_name && command && HB_ISBYREF( 6 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 4, _hbmongoc_read_prefs_t_ );
        bson_t * opts = bson_hbparam( 5, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_client_read_write_command_with_opts( client, db_name, command, read_prefs, opts, &reply, &error );

        hbmongoc_return_byref_bson( 6, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 7, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 5 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_CLIENT_GET_URI )
{
    const mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        const mongoc_uri_t * uri = mongoc_client_get_uri( client );
        if ( uri ) {
            PHB_MONGOC phURI = hbmongoc_new_dataContainer( _hbmongoc_uri_t_, mongoc_uri_copy( uri ) );
            hb_retptrGC( phURI );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_WRITE_CONCERN )
{
    const mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        mongoc_write_concern_t * copy = mongoc_write_concern_copy( mongoc_client_get_write_concern( client ) );
        PHB_MONGOC phWC = hbmongoc_new_dataContainer( _hbmongoc_write_concern_t_, copy );
        hb_retptrGC( phWC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SET_WRITE_CONCERN )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const mongoc_write_concern_t * wc = mongoc_hbparam( 2, _hbmongoc_write_concern_t_ );

    if ( client && wc ) {
        mongoc_client_set_write_concern( client, wc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_READ_CONCERN )
{
    const mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        mongoc_read_concern_t * copy = mongoc_read_concern_copy( mongoc_client_get_read_concern( client ) );
        PHB_MONGOC phRC = hbmongoc_new_dataContainer( _hbmongoc_read_concern_t_, copy );
        hb_retptrGC( phRC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SET_READ_CONCERN )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const mongoc_read_concern_t * rc = mongoc_hbparam( 2, _hbmongoc_read_concern_t_ );

    if ( client && rc ) {
        mongoc_client_set_read_concern( client, rc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_READ_PREFS )
{
    const mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        mongoc_read_prefs_t * copy = mongoc_read_prefs_copy( mongoc_client_get_read_prefs( client ) );
        PHB_MONGOC phPrefs = hbmongoc_new_dataContainer( _hbmongoc_read_prefs_t_, copy );
        hb_retptrGC( phPrefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SET_READ_PREFS )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const mongoc_read_prefs_t * prefs = mongoc_hbparam( 2, _hbmongoc_read_prefs_t_ );

    if ( client && prefs ) {
        mongoc_client_set_read_prefs( client, prefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SET_SOCKETTIMEOUTMS )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client && HB_ISNUM( 2 ) ) {
        mongoc_client_set_sockettimeoutms( client, hb_parni( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SET_ERROR_API )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client && HB_ISNUM( 2 ) ) {
        bool result = mongoc_client_set_error_api( client, hb_parni( 2 ) );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SET_SERVER_API )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const mongoc_server_api_t * api = mongoc_hbparam( 2, _hbmongoc_server_api_t_ );

    if ( client && api ) {
        bson_error_t error;
        bool result = mongoc_client_set_server_api( client, api, &error );
        bson_hbstor_byref_error( 3, &error, result );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_RESET )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        mongoc_client_reset( client );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SELECT_SERVER )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client && HB_ISLOG( 2 ) ) {
        const mongoc_read_prefs_t * prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_error_t error;

        mongoc_server_description_t * description = mongoc_client_select_server( client, hb_parl( 2 ), prefs, &error );

        bson_hbstor_byref_error( 4, &error, description != NULL );

        if ( description ) {
            PHB_MONGOC phDescription = hbmongoc_new_dataContainer( _hbmongoc_server_description_t_, description );
            hb_retptrGC( phDescription );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_SERVER_DESCRIPTION )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client && HB_ISNUM( 2 ) ) {
        bson_error_t error;
        uint32_t server_id = ( uint32_t ) hb_parni( 2 );

        mongoc_server_description_t * description = mongoc_client_get_server_description( client, server_id );

        bson_hbstor_byref_error( 3, &error, description != NULL );

        if ( description ) {
            PHB_MONGOC phDescription = hbmongoc_new_dataContainer( _hbmongoc_server_description_t_, description );
            hb_retptrGC( phDescription );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_SERVER_DESCRIPTIONS )
{
    const mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        size_t n = 0;
        mongoc_server_description_t ** sds = mongoc_client_get_server_descriptions( client, &n );

        if ( sds ) {
            PHB_ITEM pItemArray = hb_itemNew( NULL );
            hb_arrayNew( pItemArray, 0 );

            for ( size_t i = 0; i < n; ++i ) {
                PHB_MONGOC phDescription = hbmongoc_new_dataContainer( _hbmongoc_server_description_t_, sds[ i ] );
                hb_arrayAdd( pItemArray, phDescription );
            }

            hb_itemReturnRelease( pItemArray );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_WATCH )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    bson_t * pipeline = bson_hbparam( 2, HB_IT_ANY );

    if ( client && pipeline ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

        mongoc_change_stream_t * stream = mongoc_client_watch( client, pipeline, opts );

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

HB_FUNC( MONGOC_CLIENT_GET_DATABASE_NAMES )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        bson_error_t error;
        char ** names = mongoc_client_get_database_names( client, &error );

        bson_hbstor_byref_error( 2, &error, names != NULL );

        if ( names ) {
            PHB_ITEM pItemArray = hb_itemNew( NULL );
            hb_arrayNew( pItemArray, 0 );

            for ( int i = 0; names[ i ]; ++i ) {
                PHB_ITEM pItem = hb_itemNew( NULL );
                hb_itemPutC( pItem, names[ i ] );
                hb_arrayAdd( pItemArray, pItem );
                hb_itemRelease( pItem );
            }

            bson_strfreev( names );

            hb_itemReturnRelease( pItemArray );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_FIND_DATABASES )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        bson_error_t error;
        mongoc_cursor_t * cursor = mongoc_client_find_databases( client, &error );

        bson_hbstor_byref_error( 2, &error, cursor != NULL );

        if ( cursor ) {
            PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );
            hb_retptrGC( phCursor );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_CRYPT_SHARED_VERSION )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client ) {
        hb_retc( mongoc_client_get_crypt_shared_version( client ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_HANDSHAKE_DESCRIPTION )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

    if ( client && HB_ISNUM( 2 ) ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_error_t error;
        uint32_t server_id = ( uint32_t ) hb_parni( 2 );

        mongoc_server_description_t * description = mongoc_client_get_handshake_description( client, server_id, opts, &error );

        bson_hbstor_byref_error( 4, &error, description != NULL );

        if ( description ) {
            PHB_MONGOC phDescription = hbmongoc_new_dataContainer( _hbmongoc_server_description_t_, description );
            hb_retptrGC( phDescription );
        } else {
            hb_ret();
        }

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
