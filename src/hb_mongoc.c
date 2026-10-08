//
//  hb_mongoc.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc.h"
#include "hbjson.h"

enum hb_return_bson_value_type { _HBRETVAL_BSON_, _HBRETVAL_JSON_, _HBRETVAL_HASH_ };

static bool s_mongoc_inited = false;
static enum hb_return_bson_value_type s_hbmongoc_return_bson_value_type = _HBRETVAL_BSON_;

static const char * _STR_BSON_ = "BSON";
static const char * _STR_JSON_ = "JSON";
static const char * _STR_HASH_ = "HASH";

static HB_GARBAGE_FUNC( hbmongoc_funcs_destroy )
{
    PHB_MONGOC phMongoc = Cargo;

    if ( phMongoc && phMongoc->p ) {
        switch (phMongoc->type) {
            case _hbmongoc_client_t_:
                mongoc_client_destroy( ( mongoc_client_t * ) phMongoc->p );
                break;
            case _hbmongoc_database_t_:
                mongoc_database_destroy( ( mongoc_database_t * ) phMongoc->p );
                break;
            case _hbmongoc_collection_t_:
                mongoc_collection_destroy( ( mongoc_collection_t * ) phMongoc->p );
                break;
            case _hbmongoc_uri_t_:
                mongoc_uri_destroy( ( mongoc_uri_t * ) phMongoc->p );
                break;
            case _hbmongoc_cursor_t_:
                mongoc_cursor_destroy( ( mongoc_cursor_t * ) phMongoc->p );
                break;
            case _hbmongoc_write_concern_t_:
                mongoc_write_concern_destroy( ( mongoc_write_concern_t * ) phMongoc->p );
                break;
            case _hbmongoc_read_prefs_t_:
                mongoc_read_prefs_destroy( ( mongoc_read_prefs_t * ) phMongoc->p );
                break;
            case _hbmongoc_bulk_operation_t_:
                mongoc_bulk_operation_destroy( ( mongoc_bulk_operation_t * ) phMongoc->p );
                break;
            case _hbmongoc_read_concern_t_:
                mongoc_read_concern_destroy( ( mongoc_read_concern_t * ) phMongoc->p );
                break;
            case _hbmongoc_client_session_t_:
                mongoc_client_session_destroy( ( mongoc_client_session_t * ) phMongoc->p );
                break;
            case _hbmongoc_transaction_opts_t_:
                mongoc_transaction_opts_destroy( ( mongoc_transaction_opt_t * ) phMongoc->p );
                break;
            case _hbmongoc_session_opts_t_:
                mongoc_session_opts_destroy( ( mongoc_session_opt_t * ) phMongoc->p );
                break;
            case _hbmongoc_find_and_modify_opts_t_:
                mongoc_find_and_modify_opts_destroy( ( mongoc_find_and_modify_opts_t * ) phMongoc->p );
                break;
            case _hbmongoc_change_stream_t_:
                mongoc_change_stream_destroy( ( mongoc_change_stream_t * ) phMongoc->p );
                break;
            case _hbmongoc_server_description_t_:
                mongoc_server_description_destroy( ( mongoc_server_description_t * ) phMongoc->p );
                break;
            case _hbmongoc_server_api_t_:
                mongoc_server_api_destroy( ( mongoc_server_api_t * ) phMongoc->p );
                break;
            case _hbmongoc_index_model_t_:
                mongoc_index_model_destroy( ( mongoc_index_model_t * ) phMongoc->p );
                break;
            case _hbmongoc_topology_description_t_:
                mongoc_topology_description_destroy( ( mongoc_topology_description_t * ) phMongoc->p );
                break;
            case _hbmongoc_apm_callbacks_t_:
                mongoc_apm_callbacks_destroy( ( mongoc_apm_callbacks_t * ) phMongoc->p );
                break;
            /* APM event and context objects are owned by mongoc and are
               only valid during a callback - never destroy them here. */
            case _hbmongoc_apm_context_t_:
            case _hbmongoc_apm_command_started_t_:
            case _hbmongoc_apm_command_succeeded_t_:
            case _hbmongoc_apm_command_failed_t_:
            case _hbmongoc_apm_server_changed_t_:
            case _hbmongoc_apm_server_opening_t_:
            case _hbmongoc_apm_server_closed_t_:
            case _hbmongoc_apm_topology_changed_t_:
            case _hbmongoc_apm_topology_opening_t_:
            case _hbmongoc_apm_topology_closed_t_:
            case _hbmongoc_apm_server_heartbeat_started_t_:
            case _hbmongoc_apm_server_heartbeat_succeeded_t_:
            case _hbmongoc_apm_server_heartbeat_failed_t_:
                break;
            /* gridfs objects are owned by the caller */
            case _hbmongoc_gridfs_t_:
                mongoc_gridfs_destroy( ( mongoc_gridfs_t * ) phMongoc->p );
                break;
            case _hbmongoc_gridfs_file_t_:
                mongoc_gridfs_file_destroy( ( mongoc_gridfs_file_t * ) phMongoc->p );
                break;
            case _hbmongoc_gridfs_file_list_t_:
                mongoc_gridfs_file_list_destroy( ( mongoc_gridfs_file_list_t * ) phMongoc->p );
                break;
            case _hbmongoc_gridfs_bucket_t_:
                mongoc_gridfs_bucket_destroy( ( mongoc_gridfs_bucket_t * ) phMongoc->p );
                break;
            /* mongoc_stream_t has no single destroy entry point; the
               concrete stream type decides. Leave it to the caller. */
            case _hbmongoc_stream_t_:
                break;
            /* bulkwrite family */
            case _hbmongoc_bulkwrite_t_:
                mongoc_bulkwrite_destroy( ( mongoc_bulkwrite_t * ) phMongoc->p );
                break;
            case _hbmongoc_bulkwriteopts_t_:
                mongoc_bulkwriteopts_destroy( ( mongoc_bulkwriteopts_t * ) phMongoc->p );
                break;
            case _hbmongoc_bulkwriteresult_t_:
                mongoc_bulkwriteresult_destroy( ( mongoc_bulkwriteresult_t * ) phMongoc->p );
                break;
            case _hbmongoc_bulkwriteexception_t_:
                mongoc_bulkwriteexception_destroy( ( mongoc_bulkwriteexception_t * ) phMongoc->p );
                break;
            case _hbmongoc_bulkwrite_insertoneopts_t_:
                mongoc_bulkwrite_insertoneopts_destroy( ( mongoc_bulkwrite_insertoneopts_t * ) phMongoc->p );
                break;
            case _hbmongoc_bulkwrite_updateoneopts_t_:
                mongoc_bulkwrite_updateoneopts_destroy( ( mongoc_bulkwrite_updateoneopts_t * ) phMongoc->p );
                break;
            case _hbmongoc_bulkwrite_updatemanyopts_t_:
                mongoc_bulkwrite_updatemanyopts_destroy( ( mongoc_bulkwrite_updatemanyopts_t * ) phMongoc->p );
                break;
            case _hbmongoc_bulkwrite_replaceoneopts_t_:
                mongoc_bulkwrite_replaceoneopts_destroy( ( mongoc_bulkwrite_replaceoneopts_t * ) phMongoc->p );
                break;
            case _hbmongoc_bulkwrite_deleteoneopts_t_:
                mongoc_bulkwrite_deleteoneopts_destroy( ( mongoc_bulkwrite_deleteoneopts_t * ) phMongoc->p );
                break;
            case _hbmongoc_bulkwrite_deletemanyopts_t_:
                mongoc_bulkwrite_deletemanyopts_destroy( ( mongoc_bulkwrite_deletemanyopts_t * ) phMongoc->p );
                break;
            case _hbmongoc_client_pool_t_:
                mongoc_client_pool_destroy( ( mongoc_client_pool_t * ) phMongoc->p );
                break;
            /* ssl_opt / structured_log_opts / auto_encryption_opts are
               plain option structs; the caller owns them. */
            case _hbmongoc_ssl_opt_t_:
            case _hbmongoc_structured_log_opts_t_:
            case _hbmongoc_auto_encryption_opts_t_:
                break;
        }
        phMongoc->p = NULL;
    }
}

static const HB_GC_FUNCS s_gc_mongoc_funcs = {
    hbmongoc_funcs_destroy,
    hb_gcDummyMark
};

PHB_MONGOC hbmongoc_new_dataContainer( hbmongoc_t_ type, void * p )
{
    if ( p ) {
        PHB_MONGOC phMongo = hb_gcAllocate( sizeof( HB_MONGOC ), &s_gc_mongoc_funcs );

        phMongo->type = type;
        phMongo->p = p;

        return phMongo;
    } else {
        HBMONGOC_ERR_ARGS();
    }
    return NULL;
}

static void hbmongoc_check_inited()
{
    if ( ! s_mongoc_inited ) {
        mongoc_init();
        s_mongoc_inited = true;
    }
}

PHB_MONGOC hbmongoc_param( int iParam, hbmongoc_t_ type )
{
    PHB_MONGOC phMongo = hb_parptrGC( &s_gc_mongoc_funcs, iParam );

    return phMongo && phMongo->type == type ? phMongo : NULL;
}

PHB_MONGOC hbmongoc_hbparam( PHB_ITEM pItem, hbmongoc_t_ type )
{
    PHB_MONGOC phMongo = hb_itemGetPtrGC( pItem, &s_gc_mongoc_funcs );

    return phMongo && phMongo->type == type ? phMongo : NULL;
}

PHB_BSON hbmongoc_return_byref_bson( int iParam, bson_t * bson )
{
    PHB_BSON phBson = NULL;
    char * szJSON = NULL;

    switch ( s_hbmongoc_return_bson_value_type ) {
        case _HBRETVAL_JSON_:
            szJSON = hbbson_as_json( bson );
            if ( szJSON ) {
                hb_storc( szJSON, iParam );
                bson_free( szJSON );
            } else {
                hb_stor( iParam );
            }
            bson_destroy( bson );
            break;
        case _HBRETVAL_BSON_:
            phBson = hbbson_new_dataContainer( _hbbson_t_, bson );
            hb_storptrGC( phBson, iParam );
            break;
        case _HBRETVAL_HASH_:
            szJSON = hbbson_as_json( bson );
            PHB_ITEM pItem = hb_itemNew( NULL );
            if ( szJSON ) {
                hb_jsonDecode( szJSON, pItem );
                bson_free( szJSON );
                hb_itemParamStoreRelease( iParam, pItem );
            } else {
                hb_stor( iParam );
            }
            bson_destroy( bson );
            break;
    }
    return phBson;
}

void * mongoc_hbparam( int iParam, hbmongoc_t_ type )
{
    PHB_MONGOC phMongoc = hbmongoc_param( iParam, type );

    if ( phMongoc && phMongoc->p ) {
        return phMongoc->p;
    }
    return NULL;
}

void * mongoc_hbparam_any( int iParam )
{
    PHB_MONGOC phMongoc = hbmongoc_param( iParam, _hbmongoc_client_t_ );

    if ( phMongoc == NULL ) {
        phMongoc = hbmongoc_param( iParam, _hbmongoc_database_t_ );
        if ( phMongoc == NULL ) {
            phMongoc = hbmongoc_param( iParam, _hbmongoc_collection_t_ );
            if ( phMongoc == NULL ) {
                phMongoc = hbmongoc_param( iParam, _hbmongoc_uri_t_ );
                if ( phMongoc == NULL ) {
                    phMongoc = hbmongoc_param( iParam, _hbmongoc_cursor_t_ );
                    if ( phMongoc == NULL ) {
                        phMongoc = hbmongoc_param( iParam, _hbmongoc_write_concern_t_ );
                        if ( phMongoc == NULL ) {
                            phMongoc = hbmongoc_param( iParam, _hbmongoc_read_prefs_t_ );
                            if ( phMongoc == NULL ) {
                                phMongoc = hbmongoc_param( iParam, _hbmongoc_bulk_operation_t_ );
                                if ( phMongoc == NULL ) {
                                    phMongoc = hbmongoc_param( iParam, _hbmongoc_read_concern_t_ );
                                    if ( phMongoc == NULL ) {
                                        phMongoc = hbmongoc_param( iParam, _hbmongoc_client_session_t_ );
                                        if ( phMongoc == NULL ) {
                                            phMongoc = hbmongoc_param( iParam, _hbmongoc_transaction_opts_t_ );
                                            if ( phMongoc == NULL ) {
                                                phMongoc = hbmongoc_param( iParam, _hbmongoc_session_opts_t_ );
                                                if ( phMongoc == NULL ) {
                                                    phMongoc = hbmongoc_param( iParam, _hbmongoc_find_and_modify_opts_t_ );
                                                    if ( phMongoc == NULL ) {
                                                        phMongoc = hbmongoc_param( iParam, _hbmongoc_change_stream_t_ );
                                                        if ( phMongoc == NULL ) {
                                                            phMongoc = hbmongoc_param( iParam, _hbmongoc_server_description_t_ );
                                                            if ( phMongoc == NULL ) {
                                                                phMongoc = hbmongoc_param( iParam, _hbmongoc_server_api_t_ );
                                                                if ( phMongoc == NULL ) {
                                                                    phMongoc = hbmongoc_param( iParam, _hbmongoc_index_model_t_ );
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return phMongoc ? phMongoc->p : NULL;
}


/* Harbour API */

void hbmongoc_stor_byref_value( int iParam, const bson_value_t * value )
{
    if ( HB_ISBYREF( iParam ) ) {
        if ( value ) {
            switch ( value->value_type ) {
                case BSON_TYPE_NULL:
                    hb_stor( iParam );
                    break;
                case BSON_TYPE_BOOL:
                    hb_storl( value->value.v_bool, iParam );
                    break;
                case BSON_TYPE_INT32:
                    hb_storni( value->value.v_int32, iParam );
                    break;
                case BSON_TYPE_INT64:
                    hb_stornll( (HB_LONGLONG) value->value.v_int64, iParam );
                    break;
                case BSON_TYPE_DOUBLE:
                    hb_stornd( value->value.v_double, iParam );
                    break;
                case BSON_TYPE_UTF8:
                    hb_storclen( value->value.v_utf8.str, value->value.v_utf8.len, iParam );
                    break;
                case BSON_TYPE_SYMBOL:
                    hb_storclen( value->value.v_symbol.symbol, value->value.v_symbol.len, iParam );
                    break;
                case BSON_TYPE_OID:
                    {
                        char szOID[ 25 ];
                        bson_oid_to_string( ( const bson_oid_t * ) &value->value.v_oid, szOID );
                        hb_storc( szOID, iParam );
                    }
                    break;
                case BSON_TYPE_DATE_TIME:
                    hb_storclen( ( const char * ) &value->value.v_datetime, sizeof( int64_t ), iParam );
                    break;
                default:
                    hb_stor( iParam );
                    break;
            }
        } else {
            hb_stor( iParam );
        }
    }
}

HB_FUNC( HB_NUMTYPE )
{
    PHB_ITEM pItem = hb_param( 1, HB_IT_NUMERIC );

    if ( pItem ) {
        switch ( HB_ITEM_TYPE( pItem ) ) {
            case HB_IT_INTEGER:
                hb_retc( "I" );
                break;
            case HB_IT_LONG:
                hb_retc( "L" );
                break;
            case HB_IT_DOUBLE:
                hb_retc( "D" );
                break;
        }
    }
}

HB_FUNC( HB_MONGOC_SET_RETURN_BSON_VALUE_TYPE )
{
    if ( hb_pcount() > 0 ) {
        if ( hb_stricmp( hb_parc( 1 ), _STR_JSON_ ) == 0 ) {
            s_hbmongoc_return_bson_value_type = _HBRETVAL_JSON_;
        } else if ( hb_stricmp( hb_parc( 1 ), _STR_BSON_ ) == 0 ) {
            s_hbmongoc_return_bson_value_type = _HBRETVAL_BSON_;
        } else if ( hb_stricmp( hb_parc( 1 ), _STR_HASH_ ) == 0 ) {
            s_hbmongoc_return_bson_value_type = _HBRETVAL_HASH_;
        } else {
            HBMONGOC_ERR_ARGS();
        }
    }
    switch ( s_hbmongoc_return_bson_value_type ) {
        case _HBRETVAL_JSON_:
            hb_retc( _STR_JSON_ );
            break;
        case _HBRETVAL_BSON_:
            hb_retc( _STR_BSON_ );
            break;
        case _HBRETVAL_HASH_:
            hb_retc( _STR_HASH_ );
            break;
    }
}

HB_FUNC( MONGOC_CHECK_VERSION )
{
    if ( HB_ISNUM( 1 ) && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) ) {
        hb_retl( mongoc_check_version( hb_parni( 1 ), hb_parni( 2 ), hb_parni( 3 ) ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLEANUP )
{
    if( s_mongoc_inited ) {
        mongoc_cleanup();
    }
}

HB_FUNC( MONGOC_GET_MAJOR_VERSION )
{
    hb_retni( mongoc_get_major_version() );
}

HB_FUNC( MONGOC_GET_MICRO_VERSION )
{
    hb_retni( mongoc_get_micro_version() );
}

HB_FUNC( MONGOC_GET_MINOR_VERSION )
{
    hb_retni( mongoc_get_minor_version() );
}

HB_FUNC( MONGOC_GET_VERSION )
{
    hb_retc( mongoc_get_version() );
}

HB_FUNC( MONGOC_INIT )
{
    hbmongoc_check_inited();
}

/* server api */

HB_FUNC( MONGOC_SERVER_API_NEW )
{
    mongoc_server_api_version_t version;

    if ( HB_ISNUM( 1 ) ) {
        version = ( mongoc_server_api_version_t ) hb_parni( 1 );
    } else if ( HB_IS_STRING( 1 ) ) {
        if ( ! mongoc_server_api_version_from_string( hb_parc( 1 ), &version ) ) {
            HBMONGOC_ERR_ARGS();
            return;
        }
    } else {
        HBMONGOC_ERR_ARGS();
        return;
    }

    mongoc_server_api_t * api = mongoc_server_api_new( version );
    if ( api ) {
        PHB_MONGOC phApi = hbmongoc_new_dataContainer( _hbmongoc_server_api_t_, api );
        hb_retptrGC( phApi );
    } else {
        hb_ret();
    }
}

HB_FUNC( MONGOC_SERVER_API_DEPRECATION_ERRORS )
{
    mongoc_server_api_t * api = mongoc_hbparam( 1, _hbmongoc_server_api_t_ );

    if ( api && HB_ISLOG( 2 ) ) {
        mongoc_server_api_deprecation_errors( api, hb_parl( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_API_STRICT )
{
    mongoc_server_api_t * api = mongoc_hbparam( 1, _hbmongoc_server_api_t_ );

    if ( api && HB_ISLOG( 2 ) ) {
        mongoc_server_api_strict( api, hb_parl( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_API_GET_DEPRECATION_ERRORS )
{
    const mongoc_server_api_t * api = mongoc_hbparam( 1, _hbmongoc_server_api_t_ );

    if ( api ) {
        const mongoc_optional_t * opt = mongoc_server_api_get_deprecation_errors( api );
        hb_retl( opt ? mongoc_optional_value( opt ) : false );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_API_GET_STRICT )
{
    const mongoc_server_api_t * api = mongoc_hbparam( 1, _hbmongoc_server_api_t_ );

    if ( api ) {
        const mongoc_optional_t * opt = mongoc_server_api_get_strict( api );
        hb_retl( opt ? mongoc_optional_value( opt ) : false );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_API_GET_VERSION )
{
    const mongoc_server_api_t * api = mongoc_hbparam( 1, _hbmongoc_server_api_t_ );

    if ( api ) {
        hb_retni( ( int ) mongoc_server_api_get_version( api ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_API_COPY )
{
    const mongoc_server_api_t * api = mongoc_hbparam( 1, _hbmongoc_server_api_t_ );

    if ( api ) {
        mongoc_server_api_t * copy = mongoc_server_api_copy( api );
        PHB_MONGOC phApi = hbmongoc_new_dataContainer( _hbmongoc_server_api_t_, copy );
        hb_retptrGC( phApi );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_API_DESTROY )
{
    PHB_MONGOC api = hbmongoc_param( 1, _hbmongoc_server_api_t_ );

    if ( api ) {
        mongoc_server_api_destroy( api->p );
        api->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_API_VERSION_TO_STRING )
{
    if ( HB_ISNUM( 1 ) ) {
        mongoc_server_api_version_t version = ( mongoc_server_api_version_t ) hb_parni( 1 );
        hb_retc( mongoc_server_api_version_to_string( version ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SERVER_API_VERSION_FROM_STRING )
{
    const char * version = hb_parc( 1 );

    if ( version ) {
        mongoc_server_api_version_t out;
        if ( mongoc_server_api_version_from_string( version, &out ) ) {
            hb_retni( ( int ) out );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

/* log */

HB_FUNC( MONGOC_LOG_TRACE_ENABLE )
{
    mongoc_log_trace_enable();
}

HB_FUNC( MONGOC_LOG_TRACE_DISABLE )
{
    mongoc_log_trace_disable();
}

HB_FUNC( MONGOC_LOG_LEVEL_STR )
{
    if ( HB_ISNUM( 1 ) ) {
        hb_retc( mongoc_log_level_str( ( mongoc_log_level_t ) hb_parni( 1 ) ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_LOG_DEFAULT_HANDLER )
{
    if ( HB_ISNUM( 1 ) && HB_IS_STRING( 2 ) && HB_IS_STRING( 3 ) ) {
        mongoc_log_default_handler( ( mongoc_log_level_t ) hb_parni( 1 ), hb_parc( 2 ), hb_parc( 3 ), NULL );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_LOG )
{
    if ( HB_ISNUM( 1 ) && HB_IS_STRING( 2 ) && HB_IS_STRING( 3 ) ) {
        mongoc_log( ( mongoc_log_level_t ) hb_parni( 1 ), hb_parc( 2 ), "%s", hb_parc( 3 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
