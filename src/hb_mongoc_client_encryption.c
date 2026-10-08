//
//  hb_mongoc_client_encryption.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//
//  Wraps mongoc-client-side-encryption.h (mongo-c-driver 2.x): the
//  client_encryption object, auto_encryption / client_encryption option
//  objects, the per-operation option objects and their setters, and the
//  key / encrypt / decrypt / datakey / rewrap operations.
//
//  NOT wrapped (need a C function pointer Harbour cannot supply):
//    mongoc_auto_encryption_opts_set_kms_credential_provider_callback
//    mongoc_client_encryption_opts_set_kms_credential_provider_callback
// ------------------------------------------------------------------

#include "hb_mongoc.h"

/* ------------------------------------------------------------------
 * option-object new / destroy
 * ------------------------------------------------------------------ */

#define HB_ENC_NEW( fname, tag, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_##tag##_t * self = call (); \
      if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_##tag##_t_, self ); hb_retptrGC( ph ); } \
      else { hb_ret(); } \
   }

// (manual expansions below because the tag names contain underscores)

HB_FUNC( MONGOC_AUTO_ENCRYPTION_OPTS_NEW )
{
   mongoc_auto_encryption_opts_t * self = mongoc_auto_encryption_opts_new();
   if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_auto_encryption_opts_t_, self ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_OPTS_NEW )
{
   mongoc_client_encryption_opts_t * self = mongoc_client_encryption_opts_new();
   if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_client_encryption_opts_t_, self ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_DATAKEY_OPTS_NEW )
{
   mongoc_client_encryption_datakey_opts_t * self = mongoc_client_encryption_datakey_opts_new();
   if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_client_encryption_datakey_opts_t_, self ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_NEW )
{
   mongoc_client_encryption_encrypt_opts_t * self = mongoc_client_encryption_encrypt_opts_new();
   if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_client_encryption_encrypt_opts_t_, self ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_DESTROY )
{
   PHB_MONGOC self = hbmongoc_param( 1, _hbmongoc_client_encryption_encrypt_opts_t_ );
   if ( self ) { mongoc_client_encryption_encrypt_opts_destroy( ( mongoc_client_encryption_encrypt_opts_t * ) self->p ); self->p = NULL; }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_RANGE_OPTS_NEW )
{
   mongoc_client_encryption_encrypt_range_opts_t * self = mongoc_client_encryption_encrypt_range_opts_new();
   if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_client_encryption_encrypt_range_opts_t_, self ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_OPTS_NEW )
{
   mongoc_client_encryption_encrypt_string_opts_t * self = mongoc_client_encryption_encrypt_string_opts_new();
   if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_client_encryption_encrypt_string_opts_t_, self ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_PREFIX_OPTS_NEW )
{
   mongoc_client_encryption_encrypt_string_prefix_opts_t * self = mongoc_client_encryption_encrypt_string_prefix_opts_new();
   if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_client_encryption_encrypt_string_prefix_opts_t_, self ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_SUBSTRING_OPTS_NEW )
{
   mongoc_client_encryption_encrypt_string_substring_opts_t * self = mongoc_client_encryption_encrypt_string_substring_opts_new();
   if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_client_encryption_encrypt_string_substring_opts_t_, self ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_SUFFIX_OPTS_NEW )
{
   mongoc_client_encryption_encrypt_string_suffix_opts_t * self = mongoc_client_encryption_encrypt_string_suffix_opts_new();
   if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_client_encryption_encrypt_string_suffix_opts_t_, self ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_REWRAP_MANY_DATAKEY_RESULT_NEW )
{
   mongoc_client_encryption_rewrap_many_datakey_result_t * self = mongoc_client_encryption_rewrap_many_datakey_result_new();
   if ( self ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_client_encryption_rewrap_many_datakey_result_t_, self ); hb_retptrGC( ph ); }
   else { hb_ret(); }
}

/* ------------------------------------------------------------------
 * auto_encryption_opts setters
 * ------------------------------------------------------------------ */

#define HB_ENC_SET_BOOL( fname, tag, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_##tag##_t * opts = mongoc_hbparam( 1, _hbmongoc_##tag##_t_ ); \
      if ( opts && HB_ISLOG( 2 ) ) { call ( opts, hb_parl( 2 ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_ENC_SET_BSON( fname, tag, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_##tag##_t * opts = mongoc_hbparam( 1, _hbmongoc_##tag##_t_ ); \
      bson_t * arg = bson_hbparam( 2, HB_IT_ANY ); \
      if ( opts && arg ) { call ( opts, arg ); if ( ! HB_ISPOINTER( 2 ) ) bson_destroy( arg ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_ENC_SET_U64( fname, tag, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_##tag##_t * opts = mongoc_hbparam( 1, _hbmongoc_##tag##_t_ ); \
      if ( opts && HB_ISNUM( 2 ) ) { call ( opts, (uint64_t) hb_parnll( 2 ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

HB_ENC_SET_BOOL( MONGOC_AUTO_ENCRYPTION_OPTS_SET_BYPASS_AUTO_ENCRYPTION, auto_encryption_opts, mongoc_auto_encryption_opts_set_bypass_auto_encryption )
HB_ENC_SET_BOOL( MONGOC_AUTO_ENCRYPTION_OPTS_SET_BYPASS_QUERY_ANALYSIS, auto_encryption_opts, mongoc_auto_encryption_opts_set_bypass_query_analysis )
HB_ENC_SET_BSON( MONGOC_AUTO_ENCRYPTION_OPTS_SET_ENCRYPTED_FIELDS_MAP, auto_encryption_opts, mongoc_auto_encryption_opts_set_encrypted_fields_map )
HB_ENC_SET_BSON( MONGOC_AUTO_ENCRYPTION_OPTS_SET_EXTRA, auto_encryption_opts, mongoc_auto_encryption_opts_set_extra )
HB_ENC_SET_BSON( MONGOC_AUTO_ENCRYPTION_OPTS_SET_KMS_PROVIDERS, auto_encryption_opts, mongoc_auto_encryption_opts_set_kms_providers )
HB_ENC_SET_BSON( MONGOC_AUTO_ENCRYPTION_OPTS_SET_SCHEMA_MAP, auto_encryption_opts, mongoc_auto_encryption_opts_set_schema_map )
HB_ENC_SET_BSON( MONGOC_AUTO_ENCRYPTION_OPTS_SET_TLS_OPTS, auto_encryption_opts, mongoc_auto_encryption_opts_set_tls_opts )
HB_ENC_SET_U64( MONGOC_AUTO_ENCRYPTION_OPTS_SET_KEY_EXPIRATION, auto_encryption_opts, mongoc_auto_encryption_opts_set_key_expiration )

HB_FUNC( MONGOC_AUTO_ENCRYPTION_OPTS_SET_KEYVAULT_CLIENT )
{
   mongoc_auto_encryption_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_auto_encryption_opts_t_ );
   mongoc_client_t * client = mongoc_hbparam( 2, _hbmongoc_client_t_ );
   if ( opts && client ) { mongoc_auto_encryption_opts_set_keyvault_client( opts, client ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_AUTO_ENCRYPTION_OPTS_SET_KEYVAULT_CLIENT_POOL )
{
   mongoc_auto_encryption_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_auto_encryption_opts_t_ );
   mongoc_client_pool_t * pool = mongoc_hbparam( 2, _hbmongoc_client_pool_t_ );
   if ( opts && pool ) { mongoc_auto_encryption_opts_set_keyvault_client_pool( opts, pool ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_AUTO_ENCRYPTION_OPTS_SET_KEYVAULT_NAMESPACE )
{
   mongoc_auto_encryption_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_auto_encryption_opts_t_ );
   const char * db = hb_parc( 2 );
   const char * coll = hb_parc( 3 );
   if ( opts && db && coll ) { mongoc_auto_encryption_opts_set_keyvault_namespace( opts, db, coll ); }
   else { HBMONGOC_ERR_ARGS(); }
}

/* ------------------------------------------------------------------
 * client_encryption_opts setters
 * ------------------------------------------------------------------ */

HB_ENC_SET_BSON( MONGOC_CLIENT_ENCRYPTION_OPTS_SET_KMS_PROVIDERS, client_encryption_opts, mongoc_client_encryption_opts_set_kms_providers )

/* Build the { local : { keyMaterial : <binary> } } KMS-providers document
   in C from a raw Harbour string, because a Harbour hash cannot carry a
   BSON binary subtype. keyMaterial must be exactly 16/24/32 bytes. */
HB_FUNC( MONGOC_CLIENT_ENCRYPTION_OPTS_SET_LOCAL_KMS_KEY )
{
   mongoc_client_encryption_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_client_encryption_opts_t_ );
   const char * key = hb_parc( 2 );

   if ( opts && key )
   {
      bson_t providers;
      bson_t local;
      bson_init( &providers );
      bson_init( &local );

      /* local KMS provider schema (libmongocrypt mongocrypt-opts.c):
         { local : { key : <binary, exactly 96 bytes> } } */
      bson_append_binary( &local, "key", -1, BSON_SUBTYPE_BINARY,
                          (const uint8_t *) key, (uint32_t) strlen( key ) );
      bson_append_document( &providers, "local", -1, &local );

      mongoc_client_encryption_opts_set_kms_providers( opts, &providers );

      bson_destroy( &providers );
      bson_destroy( &local );

      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}
HB_ENC_SET_BSON( MONGOC_CLIENT_ENCRYPTION_OPTS_SET_TLS_OPTS, client_encryption_opts, mongoc_client_encryption_opts_set_tls_opts )
HB_ENC_SET_U64( MONGOC_CLIENT_ENCRYPTION_OPTS_SET_KEY_EXPIRATION, client_encryption_opts, mongoc_client_encryption_opts_set_key_expiration )

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_OPTS_SET_KEYVAULT_CLIENT )
{
   mongoc_client_encryption_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_client_encryption_opts_t_ );
   mongoc_client_t * client = mongoc_hbparam( 2, _hbmongoc_client_t_ );
   if ( opts && client ) { mongoc_client_encryption_opts_set_keyvault_client( opts, client ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_OPTS_SET_KEYVAULT_NAMESPACE )
{
   mongoc_client_encryption_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_client_encryption_opts_t_ );
   const char * db = hb_parc( 2 );
   const char * coll = hb_parc( 3 );
   if ( opts && db && coll ) { mongoc_client_encryption_opts_set_keyvault_namespace( opts, db, coll ); }
   else { HBMONGOC_ERR_ARGS(); }
}

/* ------------------------------------------------------------------
 * datakey_opts setters
 * ------------------------------------------------------------------ */

HB_ENC_SET_BSON( MONGOC_CLIENT_ENCRYPTION_DATAKEY_OPTS_SET_MASTERKEY, client_encryption_datakey_opts, mongoc_client_encryption_datakey_opts_set_masterkey )

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_DATAKEY_OPTS_SET_KEYMATERIAL )
{
   mongoc_client_encryption_datakey_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_client_encryption_datakey_opts_t_ );
   const uint8_t * data = (const uint8_t *) hb_param( 2, HB_IT_STRING );
   if ( opts && data ) { mongoc_client_encryption_datakey_opts_set_keymaterial( opts, data, (uint32_t) hb_parni( 3 ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_DATAKEY_OPTS_SET_KEYALTAMES )
{
   mongoc_client_encryption_datakey_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_client_encryption_datakey_opts_t_ );
   PHB_ITEM pArr = hb_param( 2, HB_IT_ARRAY );

   if ( opts && pArr )
   {
      HB_SIZE n = hb_arrayLen( pArr );
      char ** names = hb_xgrab( ( n + 1 ) * sizeof( char * ) );
      HB_SIZE i;

      for ( i = 0; i < n; ++i )
      {
         names[ i ] = hb_arrayGetC( pArr, i + 1 );
      }
      names[ n ] = NULL;

      mongoc_client_encryption_datakey_opts_set_keyaltnames( opts, names, (uint32_t) n );
      hb_xfree( names );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* ------------------------------------------------------------------
 * encrypt_opts setters
 * ------------------------------------------------------------------ */

#define HB_ENC_SET_STR( fname, tag, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_##tag##_t * opts = mongoc_hbparam( 1, _hbmongoc_##tag##_t_ ); \
      const char * arg = hb_parc( 2 ); \
      if ( opts && arg ) { call ( opts, arg ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_ENC_SET_VAL( fname, tag, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_##tag##_t * opts = mongoc_hbparam( 1, _hbmongoc_##tag##_t_ ); \
      const bson_value_t * arg = bson_value_hbparam( 2 ); \
      if ( opts && arg ) { call ( opts, arg ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_ENC_SET_I64( fname, tag, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_##tag##_t * opts = mongoc_hbparam( 1, _hbmongoc_##tag##_t_ ); \
      if ( opts && HB_ISNUM( 2 ) ) { call ( opts, (int64_t) hb_parnll( 2 ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_ENC_SET_OBJ( fname, tag, argtag, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_##tag##_t * opts = mongoc_hbparam( 1, _hbmongoc_##tag##_t_ ); \
      const mongoc_##argtag##_t * arg = mongoc_hbparam( 2, _hbmongoc_##argtag##_t_ ); \
      if ( opts && arg ) { call ( opts, arg ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

HB_ENC_SET_STR( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_SET_ALGORITHM, client_encryption_encrypt_opts, mongoc_client_encryption_encrypt_opts_set_algorithm )
HB_ENC_SET_STR( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_SET_KEYALTNAME, client_encryption_encrypt_opts, mongoc_client_encryption_encrypt_opts_set_keyaltname )
HB_ENC_SET_STR( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_SET_QUERY_TYPE, client_encryption_encrypt_opts, mongoc_client_encryption_encrypt_opts_set_query_type )
HB_ENC_SET_VAL( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_SET_KEYID, client_encryption_encrypt_opts, mongoc_client_encryption_encrypt_opts_set_keyid )

/* keyid from a plain Harbour string: build a bson_value_t in C, because
   a Harbour string is not a wrapped bson_value_t. */
HB_FUNC( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_SET_KEYID_STR )
{
   mongoc_client_encryption_encrypt_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_client_encryption_encrypt_opts_t_ );
   const char * keyid = hb_parc( 2 );

   if ( opts && keyid )
   {
      bson_value_t val;
      memset( &val, 0, sizeof( val ) );
      val.value_type = BSON_TYPE_UTF8;
      val.value.v_utf8.str = (char *) keyid;
      val.value.v_utf8.len = (uint32_t) strlen( keyid );
      /* set_keyid copies the value into opts, so the stack val needs no
         destroy and must not free Harbour's keyid string. */
      mongoc_client_encryption_encrypt_opts_set_keyid( opts, &val );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}
HB_ENC_SET_I64( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_SET_CONTENTION_FACTOR, client_encryption_encrypt_opts, mongoc_client_encryption_encrypt_opts_set_contention_factor )
HB_ENC_SET_OBJ( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_SET_RANGE_OPTS, client_encryption_encrypt_opts, client_encryption_encrypt_range_opts, mongoc_client_encryption_encrypt_opts_set_range_opts )
HB_ENC_SET_OBJ( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_SET_STRING_OPTS, client_encryption_encrypt_opts, client_encryption_encrypt_string_opts, mongoc_client_encryption_encrypt_opts_set_string_opts )

/* ------------------------------------------------------------------
 * range_opts setters
 * ------------------------------------------------------------------ */

HB_ENC_SET_VAL( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_RANGE_OPTS_SET_MAX, client_encryption_encrypt_range_opts, mongoc_client_encryption_encrypt_range_opts_set_max )
HB_ENC_SET_VAL( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_RANGE_OPTS_SET_MIN, client_encryption_encrypt_range_opts, mongoc_client_encryption_encrypt_range_opts_set_min )

#define HB_ENC_SET_I32( fname, tag, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_##tag##_t * opts = mongoc_hbparam( 1, _hbmongoc_##tag##_t_ ); \
      if ( opts && HB_ISNUM( 2 ) ) { call ( opts, (int32_t) hb_parni( 2 ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

HB_ENC_SET_I32( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_RANGE_OPTS_SET_PRECISION, client_encryption_encrypt_range_opts, mongoc_client_encryption_encrypt_range_opts_set_precision )
HB_ENC_SET_I32( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_RANGE_OPTS_SET_TRIM_FACTOR, client_encryption_encrypt_range_opts, mongoc_client_encryption_encrypt_range_opts_set_trim_factor )
HB_ENC_SET_I64( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_RANGE_OPTS_SET_SPARSITY, client_encryption_encrypt_range_opts, mongoc_client_encryption_encrypt_range_opts_set_sparsity )

/* ------------------------------------------------------------------
 * string / prefix / substring / suffix opts setters
 * ------------------------------------------------------------------ */

HB_ENC_SET_BOOL( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_OPTS_SET_CASE_SENSITIVE, client_encryption_encrypt_string_opts, mongoc_client_encryption_encrypt_string_opts_set_case_sensitive )
HB_ENC_SET_BOOL( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_OPTS_SET_DIACRITIC_SENSITIVE, client_encryption_encrypt_string_opts, mongoc_client_encryption_encrypt_string_opts_set_diacritic_sensitive )
HB_ENC_SET_OBJ( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_OPTS_SET_PREFIX, client_encryption_encrypt_string_opts, client_encryption_encrypt_string_prefix_opts, mongoc_client_encryption_encrypt_string_opts_set_prefix )
HB_ENC_SET_OBJ( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_OPTS_SET_SUBSTRING, client_encryption_encrypt_string_opts, client_encryption_encrypt_string_substring_opts, mongoc_client_encryption_encrypt_string_opts_set_substring )
HB_ENC_SET_OBJ( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_OPTS_SET_SUFFIX, client_encryption_encrypt_string_opts, client_encryption_encrypt_string_suffix_opts, mongoc_client_encryption_encrypt_string_opts_set_suffix )

HB_ENC_SET_I32( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_PREFIX_OPTS_SET_STR_MAX_QUERY_LENGTH, client_encryption_encrypt_string_prefix_opts, mongoc_client_encryption_encrypt_string_prefix_opts_set_str_max_query_length )
HB_ENC_SET_I32( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_PREFIX_OPTS_SET_STR_MIN_QUERY_LENGTH, client_encryption_encrypt_string_prefix_opts, mongoc_client_encryption_encrypt_string_prefix_opts_set_str_min_query_length )

HB_ENC_SET_I32( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_SUBSTRING_OPTS_SET_STR_MAX_LENGTH, client_encryption_encrypt_string_substring_opts, mongoc_client_encryption_encrypt_string_substring_opts_set_str_max_length )
HB_ENC_SET_I32( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_SUBSTRING_OPTS_SET_STR_MAX_QUERY_LENGTH, client_encryption_encrypt_string_substring_opts, mongoc_client_encryption_encrypt_string_substring_opts_set_str_max_query_length )
HB_ENC_SET_I32( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_SUBSTRING_OPTS_SET_STR_MIN_QUERY_LENGTH, client_encryption_encrypt_string_substring_opts, mongoc_client_encryption_encrypt_string_substring_opts_set_str_min_query_length )

HB_ENC_SET_I32( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_SUFFIX_OPTS_SET_STR_MAX_QUERY_LENGTH, client_encryption_encrypt_string_suffix_opts, mongoc_client_encryption_encrypt_string_suffix_opts_set_str_max_query_length )
HB_ENC_SET_I32( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_STRING_SUFFIX_OPTS_SET_STR_MIN_QUERY_LENGTH, client_encryption_encrypt_string_suffix_opts, mongoc_client_encryption_encrypt_string_suffix_opts_set_str_min_query_length )

/* ------------------------------------------------------------------
 * client_encryption operations
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_NEW )
{
   mongoc_client_encryption_opts_t * opts = mongoc_hbparam( 1, _hbmongoc_client_encryption_opts_t_ );

   if ( opts )
   {
      bson_error_t error;
      mongoc_client_encryption_t * enc = mongoc_client_encryption_new( opts, &error );

      bson_hbstor_byref_error( 2, &error, enc != NULL );

      if ( enc )
      {
         PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_client_encryption_t_, enc );
         hb_retptrGC( ph );
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

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_DESTROY )
{
   PHB_MONGOC enc = hbmongoc_param( 1, _hbmongoc_client_encryption_t_ );
   if ( enc ) { mongoc_client_encryption_destroy( ( mongoc_client_encryption_t * ) enc->p ); enc->p = NULL; }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_GET_CRYPT_SHARED_VERSION )
{
   const mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   if ( enc ) { hb_retc( mongoc_client_encryption_get_crypt_shared_version( enc ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

// key accessors that return a bson_t *key_doc by out-param
#define HB_ENC_KEYOP( fname, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ ); \
      const bson_value_t * keyid = bson_value_hbparam( 2 ); \
      const char * alt = hb_parc( 3 ); \
      bson_t * key_doc = bson_hbparam( 4, HB_IT_ANY ); \
      if ( enc && keyid ) \
      { \
         bson_error_t error; \
         bool result = call ( enc, keyid, alt, key_doc, &error ); \
         bson_hbstor_byref_error( 5, &error, result ); \
         hb_retl( result ); \
         if ( key_doc && ! HB_ISPOINTER( 4 ) ) bson_destroy( key_doc ); \
      } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

HB_ENC_KEYOP( MONGOC_CLIENT_ENCRYPTION_ADD_KEY_ALT_NAME, mongoc_client_encryption_add_key_alt_name )
HB_ENC_KEYOP( MONGOC_CLIENT_ENCRYPTION_REMOVE_KEY_ALT_NAME, mongoc_client_encryption_remove_key_alt_name )

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_GET_KEY )
{
   mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   const bson_value_t * keyid = bson_value_hbparam( 2 );
   bson_t * key_doc = bson_hbparam( 3, HB_IT_ANY );
   if ( enc && keyid )
   {
      bson_error_t error;
      bool result = mongoc_client_encryption_get_key( enc, keyid, key_doc, &error );
      bson_hbstor_byref_error( 4, &error, result );
      hb_retl( result );
      if ( key_doc && ! HB_ISPOINTER( 3 ) ) bson_destroy( key_doc );
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_GET_KEY_BY_ALT_NAME )
{
   mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   const char * alt = hb_parc( 2 );
   bson_t * key_doc = bson_hbparam( 3, HB_IT_ANY );
   if ( enc && alt )
   {
      bson_error_t error;
      bool result = mongoc_client_encryption_get_key_by_alt_name( enc, alt, key_doc, &error );
      bson_hbstor_byref_error( 4, &error, result );
      hb_retl( result );
      if ( key_doc && ! HB_ISPOINTER( 3 ) ) bson_destroy( key_doc );
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_GET_KEYS )
{
   mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   if ( enc )
   {
      bson_error_t error;
      mongoc_cursor_t * cursor = mongoc_client_encryption_get_keys( enc, &error );
      bson_hbstor_byref_error( 2, &error, cursor != NULL );
      if ( cursor ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_DELETE_KEY )
{
   mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   const bson_value_t * keyid = bson_value_hbparam( 2 );
   bson_t * reply = bson_hbparam( 3, HB_IT_ANY );
   if ( enc && keyid )
   {
      bson_error_t error;
      bool result = mongoc_client_encryption_delete_key( enc, keyid, reply, &error );
      bson_hbstor_byref_error( 4, &error, result );
      hb_retl( result );
      if ( reply && ! HB_ISPOINTER( 3 ) ) bson_destroy( reply );
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_CREATE_DATAKEY )
{
   mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   const char * kms = hb_parc( 2 );
   const mongoc_client_encryption_datakey_opts_t * opts = mongoc_hbparam( 3, _hbmongoc_client_encryption_datakey_opts_t_ );
   bson_value_t * keyid = bson_value_hbparam( 4 );
   if ( enc && kms && opts && keyid )
   {
      bson_error_t error;
      bool result = mongoc_client_encryption_create_datakey( enc, kms, opts, keyid, &error );
      bson_hbstor_byref_error( 5, &error, result );
      hb_retl( result );
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_ENCRYPT )
{
   mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   const bson_value_t * value = bson_value_hbparam( 2 );
   mongoc_client_encryption_encrypt_opts_t * opts = mongoc_hbparam( 3, _hbmongoc_client_encryption_encrypt_opts_t_ );
   bson_value_t * ciphertext = bson_value_hbparam( 4 );
   if ( enc && value && opts && ciphertext )
   {
      bson_error_t error;
      bool result = mongoc_client_encryption_encrypt( enc, value, opts, ciphertext, &error );
      bson_hbstor_byref_error( 5, &error, result );
      hb_retl( result );
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_DECRYPT )
{
   mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   const bson_value_t * ciphertext = bson_value_hbparam( 2 );
   bson_value_t * value = bson_value_hbparam( 3 );
   if ( enc && ciphertext && value )
   {
      bson_error_t error;
      bool result = mongoc_client_encryption_decrypt( enc, ciphertext, value, &error );
      bson_hbstor_byref_error( 4, &error, result );
      hb_retl( result );
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_ENCRYPT_EXPRESSION )
{
   mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   bson_t * expr = bson_hbparam( 2, HB_IT_ANY );
   mongoc_client_encryption_encrypt_opts_t * opts = mongoc_hbparam( 3, _hbmongoc_client_encryption_encrypt_opts_t_ );
   bson_t * expr_out = bson_hbparam( 4, HB_IT_ANY );
   if ( enc && expr && opts && expr_out )
   {
      bson_error_t error;
      bool result = mongoc_client_encryption_encrypt_expression( enc, expr, opts, expr_out, &error );
      bson_hbstor_byref_error( 5, &error, result );
      hb_retl( result );
      if ( expr && ! HB_ISPOINTER( 2 ) ) bson_destroy( expr );
      if ( expr_out && ! HB_ISPOINTER( 4 ) ) bson_destroy( expr_out );
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_REWRAP_MANY_DATAKEY )
{
   mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   bson_t * filter = bson_hbparam( 2, HB_IT_ANY );
   const char * provider = hb_parc( 3 );
   bson_t * master_key = bson_hbparam( 4, HB_IT_ANY );
   mongoc_client_encryption_rewrap_many_datakey_result_t * result = mongoc_hbparam( 5, _hbmongoc_client_encryption_rewrap_many_datakey_result_t_ );
   if ( enc && filter && provider && result )
   {
      bson_error_t error;
      bool ok = mongoc_client_encryption_rewrap_many_datakey( enc, filter, provider, master_key, result, &error );
      bson_hbstor_byref_error( 6, &error, ok );
      hb_retl( ok );
      if ( filter && ! HB_ISPOINTER( 2 ) ) bson_destroy( filter );
      if ( master_key && ! HB_ISPOINTER( 4 ) ) bson_destroy( master_key );
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_REWRAP_MANY_DATAKEY_RESULT_GET_BULK_WRITE_RESULT )
{
   const mongoc_client_encryption_rewrap_many_datakey_result_t * result = mongoc_hbparam( 1, _hbmongoc_client_encryption_rewrap_many_datakey_result_t_ );
   if ( result )
   {
      const bson_t * doc = mongoc_client_encryption_rewrap_many_datakey_result_get_bulk_write_result( result );
      if ( doc ) { PHB_BSON ph = hbbson_new_dataContainer( _hbbson_t_, bson_copy( doc ) ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_CLIENT_ENCRYPTION_CREATE_ENCRYPTED_COLLECTION )
{
   mongoc_client_encryption_t * enc = mongoc_hbparam( 1, _hbmongoc_client_encryption_t_ );
   mongoc_database_t * database = mongoc_hbparam( 2, _hbmongoc_database_t_ );
   const char * name = hb_parc( 3 );
   bson_t * in_options = bson_hbparam( 4, HB_IT_ANY );
   bson_t * out_options = bson_hbparam( 5, HB_IT_ANY );
   const char * kms_provider = hb_parc( 6 );
   bson_t * masterkey = bson_hbparam( 7, HB_IT_ANY );

   if ( enc && database && name && in_options && out_options && kms_provider && masterkey )
   {
      bson_error_t error;
      mongoc_collection_t * coll = mongoc_client_encryption_create_encrypted_collection(
                                      enc, database, name, in_options, out_options, kms_provider, masterkey, &error );
      bson_hbstor_byref_error( 8, &error, coll != NULL );
      if ( coll ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_collection_t_, coll ); hb_retptrGC( ph ); }
      else { hb_ret(); }
      if ( in_options && ! HB_ISPOINTER( 4 ) ) bson_destroy( in_options );
      if ( out_options && ! HB_ISPOINTER( 5 ) ) bson_destroy( out_options );
      if ( masterkey && ! HB_ISPOINTER( 7 ) ) bson_destroy( masterkey );
   }
   else { HBMONGOC_ERR_ARGS(); }
}
