//
//  hb_mongoc_client_pool.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//
//  Wraps the client-pool API (mongoc-client-pool.h) and the remaining
//  mongoc_client_* setters that were not already wrapped: append_metadata,
//  get_gridfs, apm/ssl/structured-log/oidc/stream-initiator setters.
//
//  NOTE: the *_set_apm_callbacks / *_set_oidc_callback / *_set_stream_initiator
//        / *_set_usleep_impl functions take a C function pointer + context.
//        Harbour cannot supply a C function pointer, so those four are NOT
//        wrapped here - see the APM callback shims in hb_mongoc_apm.c for the
//        pattern that would be needed.
// ------------------------------------------------------------------

#include "hb_mongoc.h"

/* ------------------------------------------------------------------
 * mongoc_client_pool_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_CLIENT_POOL_NEW )
{
   const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

   if ( uri )
   {
      mongoc_client_pool_t * pool = mongoc_client_pool_new( uri );

      if ( pool )
      {
         PHB_MONGOC phPool = hbmongoc_new_dataContainer( _hbmongoc_client_pool_t_, pool );
         hb_retptrGC( phPool );
      }
      else
      {
         hb_ret();
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_NEW_WITH_ERROR )
{
   const mongoc_uri_t * uri = mongoc_hbparam( 1, _hbmongoc_uri_t_ );

   if ( uri )
   {
      bson_error_t error;
      mongoc_client_pool_t * pool = mongoc_client_pool_new_with_error( uri, &error );

      bson_hbstor_byref_error( 2, &error, pool != NULL );

      if ( pool )
      {
         PHB_MONGOC phPool = hbmongoc_new_dataContainer( _hbmongoc_client_pool_t_, pool );
         hb_retptrGC( phPool );
      }
      else
      {
         hb_ret();
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_DESTROY )
{
   PHB_MONGOC pool = hbmongoc_param( 1, _hbmongoc_client_pool_t_ );

   if ( pool )
   {
      mongoc_client_pool_destroy( ( mongoc_client_pool_t * ) pool->p );
      pool->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_POP )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );

   if ( pool )
   {
      mongoc_client_t * client = mongoc_client_pool_pop( pool );

      if ( client )
      {
         PHB_MONGOC phClient = hbmongoc_new_dataContainer( _hbmongoc_client_t_, client );
         hb_retptrGC( phClient );
      }
      else
      {
         hb_ret();
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_TRY_POP )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );

   if ( pool )
   {
      mongoc_client_t * client = mongoc_client_pool_try_pop( pool );

      if ( client )
      {
         PHB_MONGOC phClient = hbmongoc_new_dataContainer( _hbmongoc_client_t_, client );
         hb_retptrGC( phClient );
      }
      else
      {
         hb_ret();
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_PUSH )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );
   mongoc_client_t * client = mongoc_hbparam( 2, _hbmongoc_client_t_ );

   if ( pool && client )
   {
      mongoc_client_pool_push( pool, client );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_MAX_SIZE )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );

   if ( pool && HB_ISNUM( 2 ) )
   {
      mongoc_client_pool_max_size( pool, (uint32_t) hb_parni( 2 ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_SET_APPNAME )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );
   const char * appname = hb_parc( 2 );

   if ( pool && appname )
   {
      hb_retl( mongoc_client_pool_set_appname( pool, appname ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_SET_ERROR_API )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );

   if ( pool && HB_ISNUM( 2 ) )
   {
      hb_retl( mongoc_client_pool_set_error_api( pool, (int32_t) hb_parni( 2 ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_APPEND_METADATA )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );
   const char * name = hb_parc( 2 );
   const char * version = hb_parc( 3 );
   const char * platform = hb_parc( 4 );

   if ( pool && name && version && platform )
   {
      hb_retl( mongoc_client_pool_append_metadata( pool, name, version, platform ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_SET_SERVER_API )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );
   const mongoc_server_api_t * api = mongoc_hbparam( 2, _hbmongoc_server_api_t_ );

   if ( pool && api )
   {
      bson_error_t error;
      bool result = mongoc_client_pool_set_server_api( pool, api, &error );

      bson_hbstor_byref_error( 3, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_ENABLE_AUTO_ENCRYPTION )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );
   mongoc_auto_encryption_opts_t * opts = mongoc_hbparam( 2, _hbmongoc_auto_encryption_opts_t_ );

   if ( pool && opts )
   {
      bson_error_t error;
      bool result = mongoc_client_pool_enable_auto_encryption( pool, opts, &error );

      bson_hbstor_byref_error( 3, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_POOL_SET_STRUCTURED_LOG_OPTS )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );
   const mongoc_structured_log_opts_t * opts = mongoc_hbparam( 2, _hbmongoc_structured_log_opts_t_ );

   if ( pool && opts )
   {
      hb_retl( mongoc_client_pool_set_structured_log_opts( pool, opts ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

#ifdef MONGOC_ENABLE_SSL
HB_FUNC( MONGOC_CLIENT_POOL_SET_SSL_OPTS )
{
   mongoc_client_pool_t * pool = mongoc_hbparam( 1, _hbmongoc_client_pool_t_ );
   const mongoc_ssl_opt_t * opts = mongoc_hbparam( 2, _hbmongoc_ssl_opt_t_ );

   if ( pool && opts )
   {
      mongoc_client_pool_set_ssl_opts( pool, opts );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}
#endif

/* ------------------------------------------------------------------
 * mongoc_client_* setters not already wrapped
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_CLIENT_APPEND_METADATA )
{
   mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
   const char * name = hb_parc( 2 );
   const char * version = hb_parc( 3 );
   const char * platform = hb_parc( 4 );

   if ( client && name && version && platform )
   {
      hb_retl( mongoc_client_append_metadata( client, name, version, platform ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_GET_GRIDFS )
{
   mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
   const char * db = hb_parc( 2 );
   const char * prefix = hb_parc( 3 );

   if ( client && db && prefix )
   {
      bson_error_t error;
      mongoc_gridfs_t * gridfs = mongoc_client_get_gridfs( client, db, prefix, &error );

      bson_hbstor_byref_error( 4, &error, gridfs != NULL );

      if ( gridfs )
      {
         PHB_MONGOC phGridfs = hbmongoc_new_dataContainer( _hbmongoc_gridfs_t_, gridfs );
         hb_retptrGC( phGridfs );
      }
      else
      {
         hb_ret();
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_CLIENT_SET_STRUCTURED_LOG_OPTS )
{
   mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
   const mongoc_structured_log_opts_t * opts = mongoc_hbparam( 2, _hbmongoc_structured_log_opts_t_ );

   if ( client && opts )
   {
      hb_retl( mongoc_client_set_structured_log_opts( client, opts ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

#ifdef MONGOC_ENABLE_SSL
HB_FUNC( MONGOC_CLIENT_SET_SSL_OPTS )
{
   mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );
   const mongoc_ssl_opt_t * opts = mongoc_hbparam( 2, _hbmongoc_ssl_opt_t_ );

   if ( client && opts )
   {
      mongoc_client_set_ssl_opts( client, opts );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}
#endif
