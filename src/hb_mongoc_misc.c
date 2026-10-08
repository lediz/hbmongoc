//
//  hb_mongoc_misc.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//
//  Wraps the remaining mongoc_socket_*, mongoc_oidc_* and
//  mongoc_structured_log_* functions whose arguments Harbour can marshal
//  (pointers, strings, integers).
//
//  NOT wrapped, and why:
//   - mongoc_oidc_callback_new / _new_with_user_data /
//     mongoc_structured_log_opts_set_handler: take a C function pointer
//     (mongoc_oidc_callback_fn_t / mongoc_structured_log_func_t).
//   - mongoc_socket_bind / connect / getsockname / setsockopt / inet_ntop /
//     poll: take struct sockaddr / struct addrinfo / mongoc_socket_poll_t
//     value structs.
//   - mongoc_structured_log_entry_get_level / get_component,
//     get_level_name / get_component_name / get_named_level /
//     get_named_component, opts_get/set_max_level_for_*: take/return
//     mongoc_structured_log_component_t / level_t value enums with no
//     Harbour box type.
//   - mongoc_oidc_callback_get_fn / get_user_data / set_user_data,
//     oidc_callback_params_*: operate on oidc_callback / params value
//     structs not exposed as Harbour objects.
// ------------------------------------------------------------------

#include "hb_mongoc.h"

/* ------------------------------------------------------------------
 * mongoc_socket_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_SOCKET_NEW )
{
   if ( HB_ISNUM( 1 ) && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) )
   {
      mongoc_socket_t * sock = mongoc_socket_new( (int) hb_parni( 1 ), (int) hb_parni( 2 ), (int) hb_parni( 3 ) );

      if ( sock ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_socket_t_, sock ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_SOCKET_DESTROY )
{
   PHB_MONGOC sock = hbmongoc_param( 1, _hbmongoc_socket_t_ );
   if ( sock ) { mongoc_socket_destroy( ( mongoc_socket_t * ) sock->p ); sock->p = NULL; }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_SOCKET_CLOSE )
{
   mongoc_socket_t * sock = mongoc_hbparam( 1, _hbmongoc_socket_t_ );
   if ( sock ) { hb_retni( mongoc_socket_close( sock ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_SOCKET_CHECK_CLOSED )
{
   mongoc_socket_t * sock = mongoc_hbparam( 1, _hbmongoc_socket_t_ );
   if ( sock ) { hb_retl( mongoc_socket_check_closed( sock ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_SOCKET_ERRNO )
{
   mongoc_socket_t * sock = mongoc_hbparam( 1, _hbmongoc_socket_t_ );
   if ( sock ) { hb_retni( mongoc_socket_errno( sock ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_SOCKET_GETNAMEINFO )
{
   mongoc_socket_t * sock = mongoc_hbparam( 1, _hbmongoc_socket_t_ );
   if ( sock )
   {
      char * name = mongoc_socket_getnameinfo( sock );
      if ( name ) { hb_retc( name ); hb_xfree( name ); }
      else { hb_ret(); }
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_SOCKET_LISTEN )
{
   mongoc_socket_t * sock = mongoc_hbparam( 1, _hbmongoc_socket_t_ );
   if ( sock && HB_ISNUM( 2 ) ) { hb_retni( mongoc_socket_listen( sock, (unsigned int) hb_parni( 2 ) ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_SOCKET_SEND )
{
   mongoc_socket_t * sock = mongoc_hbparam( 1, _hbmongoc_socket_t_ );
   const char * buf = hb_parc( 2 );

   if ( sock && buf )
   {
      ssize_t rc = mongoc_socket_send( sock, buf, (size_t) strlen( buf ), HB_ISNIL( 3 ) ? 0 : (int64_t) hb_parnll( 3 ) );
      hb_retnll( (HB_LONGLONG) rc );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_SOCKET_RECV )
{
   mongoc_socket_t * sock = mongoc_hbparam( 1, _hbmongoc_socket_t_ );

   if ( sock && HB_ISNUM( 2 ) )
   {
      size_t buflen = (size_t) hb_parnll( 2 );
      char * buf = hb_xgrab( buflen + 1 );

      if ( buf )
      {
         ssize_t rc = mongoc_socket_recv( sock, buf, buflen, (int) hb_parnidef( 3, 0 ), HB_ISNIL( 4 ) ? 0 : (int64_t) hb_parnll( 4 ) );

         if ( rc >= 0 )
         {
            PHB_ITEM pStr = hb_itemNew( NULL );
            hb_itemPutC( pStr, buf );
            hb_xfree( buf );
            hb_itemReturnRelease( pStr );
         }
         else
         {
            hb_xfree( buf );
            hb_retnll( (HB_LONGLONG) rc );
         }
      }
      else
      {
         HBMONGOC_ERR_ARGS();
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* ------------------------------------------------------------------
 * mongoc_oidc_credential_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_OIDC_CREDENTIAL_NEW )
{
   const char * token = hb_parc( 1 );
   if ( token )
   {
      mongoc_oidc_credential_t * cred = mongoc_oidc_credential_new( token );
      if ( cred ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_oidc_credential_t_, cred ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_OIDC_CREDENTIAL_NEW_WITH_EXPIRES_IN )
{
   const char * token = hb_parc( 1 );
   if ( token && HB_ISNUM( 2 ) )
   {
      mongoc_oidc_credential_t * cred = mongoc_oidc_credential_new_with_expires_in( token, (int64_t) hb_parnll( 2 ) );
      if ( cred ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_oidc_credential_t_, cred ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_OIDC_CREDENTIAL_DESTROY )
{
   PHB_MONGOC cred = hbmongoc_param( 1, _hbmongoc_oidc_credential_t_ );
   if ( cred ) { mongoc_oidc_credential_destroy( ( mongoc_oidc_credential_t * ) cred->p ); cred->p = NULL; }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_OIDC_CREDENTIAL_GET_ACCESS_TOKEN )
{
   const mongoc_oidc_credential_t * cred = mongoc_hbparam( 1, _hbmongoc_oidc_credential_t_ );
   if ( cred ) { hb_retc( mongoc_oidc_credential_get_access_token( cred ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_OIDC_CREDENTIAL_GET_EXPIRES_IN )
{
   const mongoc_oidc_credential_t * cred = mongoc_hbparam( 1, _hbmongoc_oidc_credential_t_ );
   if ( cred )
   {
      const int64_t * p = mongoc_oidc_credential_get_expires_in( cred );
      if ( p ) { hb_retnll( (HB_LONGLONG) * p ); }
      else { hb_ret(); }
   }
   else { HBMONGOC_ERR_ARGS(); }
}

/* ------------------------------------------------------------------
 * mongoc_structured_log_opts_t (the subset with marshalable args)
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_STRUCTURED_LOG_OPTS_NEW )
{
   mongoc_structured_log_opts_t * opts = mongoc_structured_log_opts_new();
   if ( opts ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_structured_log_opts_t_, opts ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

HB_FUNC( MONGOC_STRUCTURED_LOG_OPTS_DESTROY )
{
   PHB_MONGOC opts = hbmongoc_param( 1, _hbmongoc_structured_log_opts_t_ );
   if ( opts ) { mongoc_structured_log_opts_destroy( ( mongoc_structured_log_opts_t * ) opts->p ); opts->p = NULL; }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_STRUCTURED_LOG_OPTS_GET_MAX_DOCUMENT_LENGTH )
{
   const mongoc_structured_log_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_structured_log_opts_t_ );
   if ( opts ) { hb_retnll( (HB_LONGLONG) mongoc_structured_log_opts_get_max_document_length( opts ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_STRUCTURED_LOG_OPTS_SET_MAX_DOCUMENT_LENGTH )
{
   mongoc_structured_log_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_structured_log_opts_t_ );
   if ( opts && HB_ISNUM( 2 ) ) { hb_retl( mongoc_structured_log_opts_set_max_document_length( opts, (size_t) hb_parnll( 2 ) ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_STRUCTURED_LOG_OPTS_SET_MAX_DOCUMENT_LENGTH_FROM_ENV )
{
   mongoc_structured_log_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_structured_log_opts_t_ );
   if ( opts ) { hb_retl( mongoc_structured_log_opts_set_max_document_length_from_env( opts ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_STRUCTURED_LOG_OPTS_SET_MAX_LEVELS_FROM_ENV )
{
   mongoc_structured_log_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_structured_log_opts_t_ );
   if ( opts ) { hb_retl( mongoc_structured_log_opts_set_max_levels_from_env( opts ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}
