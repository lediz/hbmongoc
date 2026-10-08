//
//  hb_mongoc_database.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_database.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_DATABASE_CREATE_COLLECTION )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    const char * name = hb_parc( 2 );
    bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

    if ( database && name ) {
        bson_error_t error;

        mongoc_collection_t * collection = mongoc_database_create_collection( database, name, opts, &error );

        bson_hbstor_byref_error( 4, &error, collection != NULL );

        PHB_MONGOC phCollection = hbmongoc_new_dataContainer( _hbmongoc_collection_t_, collection );
        hb_retptrGC( phCollection );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( opts && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( opts );
    }
}

HB_FUNC( MONGOC_DATABASE_DESTROY )
{
    PHB_MONGOC database = hbmongoc_param( 1, _hbmongoc_database_t_ );

    if( database ) {
        mongoc_database_destroy( database->p );
        database->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_GET_COLLECTION )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    const char * name = hb_parc( 2 );

    if ( database && name ) {
        mongoc_collection_t * collection = mongoc_database_get_collection( database, name );
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

HB_FUNC( MONGOC_DATABASE_GET_COLLECTION_NAMES_WITH_OPTS )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

        bson_error_t error;
        bson_set_error( &error, 0, 0, "%s", "" );

        char ** names = mongoc_database_get_collection_names_with_opts(database, opts, &error);

        bson_hbstor_byref_error( 3, &error, false );

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

HB_FUNC( MONGOC_DATABASE_WRITE_COMMAND_WITH_OPTS )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( database && command ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

        bson_t reply;
        bson_error_t error;

        bool result = mongoc_database_write_command_with_opts( database, command, opts, &reply, &error );

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

HB_FUNC( MONGOC_DATABASE_COPY )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        mongoc_database_t * copy = mongoc_database_copy( database );
        if ( copy ) {
            PHB_MONGOC phCopy = hbmongoc_new_dataContainer( _hbmongoc_database_t_, copy );
            hb_retptrGC( phCopy );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_GET_NAME )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        hb_retc( mongoc_database_get_name( database ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_HAS_COLLECTION )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    const char * name = hb_parc( 2 );

    if ( database && name && HB_ISBYREF( 3 ) ) {
        bson_error_t error;

        bool result = mongoc_database_has_collection( database, name, &error );

        bson_hbstor_byref_error( 3, &error, result );

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_DROP )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        bson_error_t error;

        bool result = mongoc_database_drop( database, &error );

        bson_hbstor_byref_error( 2, &error, result );

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_DROP_WITH_OPTS )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );
        bson_error_t error;

        bool result = mongoc_database_drop_with_opts( database, opts, &error );

        bson_hbstor_byref_error( 3, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 2 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_AGGREGATE )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    bson_t * pipeline = bson_hbparam( 2, HB_IT_ANY );

    if ( database && pipeline ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 4, _hbmongoc_read_prefs_t_ );

        mongoc_cursor_t * cursor = mongoc_database_aggregate( database, pipeline, opts, read_prefs );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
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

    if ( pipeline && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( pipeline );
    }
}

HB_FUNC( MONGOC_DATABASE_FIND_COLLECTIONS_WITH_OPTS )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

        mongoc_cursor_t * cursor = mongoc_database_find_collections_with_opts( database, opts );

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

HB_FUNC( MONGOC_DATABASE_GET_READ_PREFS )
{
    const mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        mongoc_read_prefs_t * copy = mongoc_read_prefs_copy( mongoc_database_get_read_prefs( database ) );
        PHB_MONGOC phPrefs = hbmongoc_new_dataContainer( _hbmongoc_read_prefs_t_, copy );
        hb_retptrGC( phPrefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_SET_READ_PREFS )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    const mongoc_read_prefs_t * prefs = mongoc_hbparam( 2, _hbmongoc_read_prefs_t_ );

    if ( database && prefs ) {
        mongoc_database_set_read_prefs( database, prefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_GET_READ_CONCERN )
{
    const mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        mongoc_read_concern_t * copy = mongoc_read_concern_copy( mongoc_database_get_read_concern( database ) );
        PHB_MONGOC phRC = hbmongoc_new_dataContainer( _hbmongoc_read_concern_t_, copy );
        hb_retptrGC( phRC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_SET_READ_CONCERN )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    const mongoc_read_concern_t * rc = mongoc_hbparam( 2, _hbmongoc_read_concern_t_ );

    if ( database && rc ) {
        mongoc_database_set_read_concern( database, rc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_GET_WRITE_CONCERN )
{
    const mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        mongoc_write_concern_t * copy = mongoc_write_concern_copy( mongoc_database_get_write_concern( database ) );
        PHB_MONGOC phWC = hbmongoc_new_dataContainer( _hbmongoc_write_concern_t_, copy );
        hb_retptrGC( phWC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_SET_WRITE_CONCERN )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    const mongoc_write_concern_t * wc = mongoc_hbparam( 2, _hbmongoc_write_concern_t_ );

    if ( database && wc ) {
        mongoc_database_set_write_concern( database, wc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_COMMAND_SIMPLE )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( database && command && HB_ISBYREF( 4 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_database_command_simple( database, command, read_prefs, &reply, &error );

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

HB_FUNC( MONGOC_DATABASE_COMMAND_WITH_OPTS )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( database && command && HB_ISBYREF( 5 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_database_command_with_opts( database, command, read_prefs, opts, &reply, &error );

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

HB_FUNC( MONGOC_DATABASE_READ_COMMAND_WITH_OPTS )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( database && command && HB_ISBYREF( 5 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_database_read_command_with_opts( database, command, read_prefs, opts, &reply, &error );

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

HB_FUNC( MONGOC_DATABASE_READ_WRITE_COMMAND_WITH_OPTS )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( database && command && HB_ISBYREF( 5 ) ) {
        const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_database_read_write_command_with_opts( database, command, read_prefs, opts, &reply, &error );

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

HB_FUNC( MONGOC_DATABASE_WATCH )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    bson_t * pipeline = bson_hbparam( 2, HB_IT_ANY );

    if ( database && pipeline ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

        mongoc_change_stream_t * stream = mongoc_database_watch( database, pipeline, opts );

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

HB_FUNC( MONGOC_DATABASE_ADD_USER )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    const char * username = hb_parc( 2 );
    const char * password = hb_parc( 3 );

    if ( database && username && password ) {
        bson_t * roles = bson_hbparam( 4, HB_IT_ANY );
        bson_t * custom_data = bson_hbparam( 5, HB_IT_ANY );
        bson_error_t error;

        bool result = mongoc_database_add_user( database, username, password, roles, custom_data, &error );

        bson_hbstor_byref_error( 6, &error, result );

        hb_retl( result );

        if ( roles && ! HB_ISPOINTER( 4 ) ) {
            bson_destroy( roles );
        }

        if ( custom_data && ! HB_ISPOINTER( 5 ) ) {
            bson_destroy( custom_data );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_REMOVE_USER )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    const char * username = hb_parc( 2 );

    if ( database && username ) {
        bson_error_t error;

        bool result = mongoc_database_remove_user( database, username, &error );

        bson_hbstor_byref_error( 3, &error, result );

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_REMOVE_ALL_USERS )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        bson_error_t error;

        bool result = mongoc_database_remove_all_users( database, &error );

        bson_hbstor_byref_error( 2, &error, result );

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
