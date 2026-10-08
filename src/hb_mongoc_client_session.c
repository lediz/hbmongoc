//
//  hb_mongoc_client_session.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc.h"

HB_FUNC( MONGOC_CLIENT_START_SESSION )
{
    mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
    const mongoc_session_opt_t * opts = mongoc_hbparam( 2, _hbmongoc_session_opts_t_ );

    bson_error_t error;
    mongoc_client_session_t * session = mongoc_client_start_session( client, opts, &error );

    bson_hbstor_byref_error( 3, &error, session != NULL );

    if ( session ) {
        PHB_MONGOC phSession = hbmongoc_new_dataContainer( _hbmongoc_client_session_t_, session );
        hb_retptrGC( phSession );
    } else {
        hb_ret();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_DESTROY )
{
    PHB_MONGOC session = hbmongoc_param( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        mongoc_client_session_destroy( session->p );
        session->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_GET_OPTS )
{
    const mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        const mongoc_session_opt_t * opts = mongoc_client_session_get_opts( session );
        PHB_MONGOC phOpts = hbmongoc_new_dataContainer( _hbmongoc_session_opts_t_, mongoc_session_opts_clone( opts ) );
        hb_retptrGC( phOpts );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_GET_LSID )
{
    const mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        const bson_t * lsid = mongoc_client_session_get_lsid( session );
        PHB_BSON phLsid = hbbson_new_dataContainer( _hbbson_t_, bson_copy( lsid ) );
        hb_retptrGC( phLsid );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_GET_CLUSTER_TIME )
{
    const mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        const bson_t * cluster_time = mongoc_client_session_get_cluster_time( session );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( cluster_time ) );
        hb_retptrGC( phBson );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_ADVANCE_CLUSTER_TIME )
{
    mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );
    bson_t * cluster_time = bson_hbparam( 2, HB_IT_ANY );

    if ( session && cluster_time ) {
        mongoc_client_session_advance_cluster_time( session, cluster_time );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( cluster_time && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( cluster_time );
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_ADVANCE_OPERATION_TIME )
{
    mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) ) {
        mongoc_client_session_advance_operation_time( session, ( uint32_t ) hb_parni( 2 ), ( uint32_t ) hb_parni( 3 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_GET_OPERATION_TIME )
{
    const mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        uint32_t timestamp = 0;
        uint32_t increment = 0;
        mongoc_client_session_get_operation_time( session, &timestamp, &increment );
        if ( HB_ISBYREF( 2 ) ) {
            hb_storni( timestamp, 2 );
        }
        if ( HB_ISBYREF( 3 ) ) {
            hb_storni( increment, 3 );
        }
        hb_retni( timestamp );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_GET_SNAPSHOT_TIME )
{
    mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        uint32_t timestamp = 0;
        uint32_t increment = 0;
        bson_error_t error;
        bool result = mongoc_client_session_get_snapshot_time( session, &timestamp, &increment, &error );

        if ( result ) {
            if ( HB_ISBYREF( 2 ) ) {
                hb_stornl( timestamp, 2 );
            }
            if ( HB_ISBYREF( 3 ) ) {
                hb_stornl( increment, 3 );
            }
        }

        bson_hbstor_byref_error( 4, &error, result );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_GET_SERVER_ID )
{
    const mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        hb_retni( ( int ) mongoc_client_session_get_server_id( session ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_GET_DIRTY )
{
    mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        hb_retl( mongoc_client_session_get_dirty( session ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_IN_TRANSACTION )
{
    const mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        hb_retl( mongoc_client_session_in_transaction( session ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_GET_TRANSACTION_STATE )
{
    const mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        hb_retni( ( int ) mongoc_client_session_get_transaction_state( session ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_START_TRANSACTION )
{
    mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );
    const mongoc_transaction_opt_t * txn_opts = mongoc_hbparam( 2, _hbmongoc_transaction_opts_t_ );

    if ( session ) {
        bson_error_t error;
        bool result = mongoc_client_session_start_transaction( session, txn_opts, &error );
        bson_hbstor_byref_error( 3, &error, result );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_COMMIT_TRANSACTION )
{
    mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session && HB_ISBYREF( 2 ) ) {
        bson_t reply;
        bson_error_t error;
        bool result = mongoc_client_session_commit_transaction( session, &reply, &error );

        hbmongoc_return_byref_bson( 2, bson_copy( &reply ) );
        bson_destroy( &reply );
        bson_hbstor_byref_error( 3, &error, result );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_ABORT_TRANSACTION )
{
    mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        bson_error_t error;
        bool result = mongoc_client_session_abort_transaction( session, &error );
        bson_hbstor_byref_error( 2, &error, result );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_APPEND )
{
    mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );
    bson_t * opts = bson_hbparam( 2, HB_IT_POINTER );

    if ( session && opts ) {
        bson_error_t error;
        bool result = mongoc_client_session_append( session, opts, &error );
        bson_hbstor_byref_error( 3, &error, result );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

/* session options */

HB_FUNC( MONGOC_SESSION_OPTS_NEW )
{
    mongoc_session_opt_t * opts = mongoc_session_opts_new();
    PHB_MONGOC phOpts = hbmongoc_new_dataContainer( _hbmongoc_session_opts_t_, opts );
    hb_retptrGC( phOpts );
}

HB_FUNC( MONGOC_SESSION_OPTS_CLONE )
{
    const mongoc_session_opt_t * opts = mongoc_hbparam( 1, _hbmongoc_session_opts_t_ );

    if ( opts ) {
        mongoc_session_opt_t * copy = mongoc_session_opts_clone( opts );
        PHB_MONGOC phOpts = hbmongoc_new_dataContainer( _hbmongoc_session_opts_t_, copy );
        hb_retptrGC( phOpts );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SESSION_OPTS_DESTROY )
{
    PHB_MONGOC opts = hbmongoc_param( 1, _hbmongoc_session_opts_t_ );

    if ( opts ) {
        mongoc_session_opts_destroy( opts->p );
        opts->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SESSION_OPTS_SET_CAUSAL_CONSISTENCY )
{
    mongoc_session_opt_t * opts = mongoc_hbparam( 1, _hbmongoc_session_opts_t_ );

    if ( opts && HB_ISLOG( 2 ) ) {
        mongoc_session_opts_set_causal_consistency( opts, hb_parl( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SESSION_OPTS_GET_CAUSAL_CONSISTENCY )
{
    const mongoc_session_opt_t * opts = mongoc_hbparam( 1, _hbmongoc_session_opts_t_ );

    if ( opts ) {
        hb_retl( mongoc_session_opts_get_causal_consistency( opts ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SESSION_OPTS_SET_SNAPSHOT )
{
    mongoc_session_opt_t * opts = mongoc_hbparam( 1, _hbmongoc_session_opts_t_ );

    if ( opts && HB_ISLOG( 2 ) ) {
        mongoc_session_opts_set_snapshot( opts, hb_parl( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SESSION_OPTS_GET_SNAPSHOT )
{
    const mongoc_session_opt_t * opts = mongoc_hbparam( 1, _hbmongoc_session_opts_t_ );

    if ( opts ) {
        hb_retl( mongoc_session_opts_get_snapshot( opts ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SESSION_OPTS_SET_SNAPSHOT_TIME )
{
    mongoc_session_opt_t * opts = mongoc_hbparam( 1, _hbmongoc_session_opts_t_ );

    if ( opts && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) ) {
        mongoc_session_opts_set_snapshot_time( opts, ( uint32_t ) hb_parni( 2 ), ( uint32_t ) hb_parni( 3 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SESSION_OPTS_GET_SNAPSHOT_TIME )
{
    const mongoc_session_opt_t * opts = mongoc_hbparam( 1, _hbmongoc_session_opts_t_ );

    if ( opts ) {
        uint32_t timestamp = 0;
        uint32_t increment = 0;
        mongoc_session_opts_get_snapshot_time( opts, &timestamp, &increment );
        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( timestamp, 2 );
        }
        if ( HB_ISBYREF( 3 ) ) {
            hb_stornl( increment, 3 );
        }
        hb_retni( timestamp );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SESSION_OPTS_SET_DEFAULT_TRANSACTION_OPTS )
{
    mongoc_session_opt_t * opts = mongoc_hbparam( 1, _hbmongoc_session_opts_t_ );
    const mongoc_transaction_opt_t * txn_opts = mongoc_hbparam( 2, _hbmongoc_transaction_opts_t_ );

    if ( opts && txn_opts ) {
        mongoc_session_opts_set_default_transaction_opts( opts, txn_opts );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SESSION_OPTS_GET_DEFAULT_TRANSACTION_OPTS )
{
    const mongoc_session_opt_t * opts = mongoc_hbparam( 1, _hbmongoc_session_opts_t_ );

    if ( opts ) {
        const mongoc_transaction_opt_t * txn = mongoc_session_opts_get_default_transaction_opts( opts );
        PHB_MONGOC phTxn = hbmongoc_new_dataContainer( _hbmongoc_transaction_opts_t_, mongoc_transaction_opts_clone( txn ) );
        hb_retptrGC( phTxn );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_SESSION_OPTS_GET_TRANSACTION_OPTS )
{
    const mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session ) {
        mongoc_transaction_opt_t * txn = mongoc_session_opts_get_transaction_opts( session );
        PHB_MONGOC phTxn = hbmongoc_new_dataContainer( _hbmongoc_transaction_opts_t_, txn );
        hb_retptrGC( phTxn );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

/* transaction options */

HB_FUNC( MONGOC_TRANSACTION_OPTS_NEW )
{
    mongoc_transaction_opt_t * txn = mongoc_transaction_opts_new();
    PHB_MONGOC phTxn = hbmongoc_new_dataContainer( _hbmongoc_transaction_opts_t_, txn );
    hb_retptrGC( phTxn );
}

HB_FUNC( MONGOC_TRANSACTION_OPTS_CLONE )
{
    const mongoc_transaction_opt_t * txn = mongoc_hbparam( 1, _hbmongoc_transaction_opts_t_ );

    if ( txn ) {
        mongoc_transaction_opt_t * copy = mongoc_transaction_opts_clone( txn );
        PHB_MONGOC phTxn = hbmongoc_new_dataContainer( _hbmongoc_transaction_opts_t_, copy );
        hb_retptrGC( phTxn );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_TRANSACTION_OPTS_DESTROY )
{
    PHB_MONGOC txn = hbmongoc_param( 1, _hbmongoc_transaction_opts_t_ );

    if ( txn ) {
        mongoc_transaction_opts_destroy( txn->p );
        txn->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_TRANSACTION_OPTS_SET_MAX_COMMIT_TIME_MS )
{
    mongoc_transaction_opt_t * txn = mongoc_hbparam( 1, _hbmongoc_transaction_opts_t_ );

    if ( txn && HB_ISNUM( 2 ) ) {
        mongoc_transaction_opts_set_max_commit_time_ms( txn, hb_parnll( 2 ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_TRANSACTION_OPTS_GET_MAX_COMMIT_TIME_MS )
{
    mongoc_transaction_opt_t * txn = mongoc_hbparam( 1, _hbmongoc_transaction_opts_t_ );

    if ( txn ) {
        hb_retnll( mongoc_transaction_opts_get_max_commit_time_ms( txn ) );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_TRANSACTION_OPTS_SET_READ_CONCERN )
{
    mongoc_transaction_opt_t * txn = mongoc_hbparam( 1, _hbmongoc_transaction_opts_t_ );
    const mongoc_read_concern_t * rc = mongoc_hbparam( 2, _hbmongoc_read_concern_t_ );

    if ( txn && rc ) {
        mongoc_transaction_opts_set_read_concern( txn, rc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_TRANSACTION_OPTS_GET_READ_CONCERN )
{
    const mongoc_transaction_opt_t * txn = mongoc_hbparam( 1, _hbmongoc_transaction_opts_t_ );

    if ( txn ) {
        const mongoc_read_concern_t * rc = mongoc_transaction_opts_get_read_concern( txn );
        PHB_MONGOC phRC = hbmongoc_new_dataContainer( _hbmongoc_read_concern_t_, mongoc_read_concern_copy( rc ) );
        hb_retptrGC( phRC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_TRANSACTION_OPTS_SET_WRITE_CONCERN )
{
    mongoc_transaction_opt_t * txn = mongoc_hbparam( 1, _hbmongoc_transaction_opts_t_ );
    const mongoc_write_concern_t * wc = mongoc_hbparam( 2, _hbmongoc_write_concern_t_ );

    if ( txn && wc ) {
        mongoc_transaction_opts_set_write_concern( txn, wc );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_TRANSACTION_OPTS_GET_WRITE_CONCERN )
{
    const mongoc_transaction_opt_t * txn = mongoc_hbparam( 1, _hbmongoc_transaction_opts_t_ );

    if ( txn ) {
        const mongoc_write_concern_t * wc = mongoc_transaction_opts_get_write_concern( txn );
        PHB_MONGOC phWC = hbmongoc_new_dataContainer( _hbmongoc_write_concern_t_, mongoc_write_concern_copy( wc ) );
        hb_retptrGC( phWC );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_TRANSACTION_OPTS_SET_READ_PREFS )
{
    mongoc_transaction_opt_t * txn = mongoc_hbparam( 1, _hbmongoc_transaction_opts_t_ );
    const mongoc_read_prefs_t * prefs = mongoc_hbparam( 2, _hbmongoc_read_prefs_t_ );

    if ( txn && prefs ) {
        mongoc_transaction_opts_set_read_prefs( txn, prefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_TRANSACTION_OPTS_GET_READ_PREFS )
{
    const mongoc_transaction_opt_t * txn = mongoc_hbparam( 1, _hbmongoc_transaction_opts_t_ );

    if ( txn ) {
        const mongoc_read_prefs_t * prefs = mongoc_transaction_opts_get_read_prefs( txn );
        PHB_MONGOC phPrefs = hbmongoc_new_dataContainer( _hbmongoc_read_prefs_t_, mongoc_read_prefs_copy( prefs ) );
        hb_retptrGC( phPrefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_SESSION_GET_CLIENT )
{
    const mongoc_client_session_t * session = mongoc_hbparam( 1, _hbmongoc_client_session_t_ );

    if ( session && HB_ISBYREF( 2 ) ) {
        const mongoc_client_t * client = mongoc_client_session_get_client( session );
        hb_stor( 2 );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
