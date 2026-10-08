//
//  hb_bson_array_builder.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//
//  Wraps the bson_array_builder_t API from bson.h (mongo-c-driver 2.x).
//
//  NOT wrapped: the bson_vector_*_view_t family (init / from_iter /
//  append_vector_elements). Those take/receive bson_vector_*_view_t by
//  VALUE (a small struct wrapping a pointer + length), which Harbour's
//  pointer marshalling does not model. They need a dedicated Harbour box
//  type to be exposed safely.
// ------------------------------------------------------------------

#include "hb_bson.h"
#include "hb_mongoc.h"

/* ------------------------------------------------------------------
 * bson_array_builder_t lifecycle
 * ------------------------------------------------------------------ */

HB_FUNC( BSON_ARRAY_BUILDER_NEW )
{
   bson_array_builder_t * bab = bson_array_builder_new();

   if ( bab )
   {
      PHB_BSON ph = hbbson_new_dataContainer( _hbbson_array_builder_t_, bab );
      hb_retptrGC( ph );
   }
   else
   {
      hb_ret();
   }
}

HB_FUNC( BSON_ARRAY_BUILDER_DESTROY )
{
   PHB_BSON bab = hbbson_param( 1, _hbbson_array_builder_t_ );

   if ( bab )
   {
      bson_array_builder_destroy( ( bson_array_builder_t * ) bab->p );
      bab->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( BSON_ARRAY_BUILDER_BUILD )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );
   bson_t * out = bson_hbparam( 2, HB_IT_ANY );

   if ( bab && out )
   {
      hb_retl( bson_array_builder_build( bab, out ) );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( out );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* ------------------------------------------------------------------
 * bson_append_array_builder_begin / end
 * ------------------------------------------------------------------ */

HB_FUNC( BSON_APPEND_ARRAY_BUILDER_BEGIN )
{
   bson_t * bson = bson_hbparam( 1, HB_IT_ANY );
   const char * key = hb_parc( 2 );

   if ( bson && key )
   {
      bson_array_builder_t * child = bson_array_builder_new();

      if ( child && bson_append_array_builder_begin( bson, key, -1, &child ) )
      {
         PHB_BSON ph = hbbson_new_dataContainer( _hbbson_array_builder_t_, child );
         hb_retptrGC( ph );
      }
      else
      {
         if ( child ) bson_array_builder_destroy( child );
         hb_ret();
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( BSON_APPEND_ARRAY_BUILDER_END )
{
   bson_t * bson = bson_hbparam( 1, HB_IT_ANY );
   bson_array_builder_t * child = bson_array_builder_hbparam( 2 );

   if ( bson && child )
   {
      hb_retl( bson_append_array_builder_end( bson, child ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* ------------------------------------------------------------------
 * array_builder append_*
 * ------------------------------------------------------------------ */

#define HB_AB_BSON( fname, call ) \
   HB_FUNC( fname ) \
   { \
      bson_array_builder_t * bab = bson_array_builder_hbparam( 1 ); \
      const bson_t * v = bson_hbparam( 2, HB_IT_ANY ); \
      if ( bab && v ) { hb_retl( call ( bab, v ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_AB_BOOL( fname, call ) \
   HB_FUNC( fname ) \
   { \
      bson_array_builder_t * bab = bson_array_builder_hbparam( 1 ); \
      if ( bab && HB_ISLOG( 2 ) ) { hb_retl( call ( bab, hb_parl( 2 ) ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_AB_I64( fname, call ) \
   HB_FUNC( fname ) \
   { \
      bson_array_builder_t * bab = bson_array_builder_hbparam( 1 ); \
      if ( bab && HB_ISNUM( 2 ) ) { hb_retl( call ( bab, (int64_t) hb_parnll( 2 ) ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_AB_DBL( fname, call ) \
   HB_FUNC( fname ) \
   { \
      bson_array_builder_t * bab = bson_array_builder_hbparam( 1 ); \
      if ( bab && HB_ISNUM( 2 ) ) { hb_retl( call ( bab, hb_parnd( 2 ) ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_AB_I32( fname, call ) \
   HB_FUNC( fname ) \
   { \
      bson_array_builder_t * bab = bson_array_builder_hbparam( 1 ); \
      if ( bab && HB_ISNUM( 2 ) ) { hb_retl( call ( bab, (int32_t) hb_parni( 2 ) ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_AB_STR( fname, call ) \
   HB_FUNC( fname ) \
   { \
      bson_array_builder_t * bab = bson_array_builder_hbparam( 1 ); \
      const char * v = hb_parc( 2 ); \
      if ( bab && v ) { hb_retl( call ( bab, v ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_AB_NOARG( fname, call ) \
   HB_FUNC( fname ) \
   { \
      bson_array_builder_t * bab = bson_array_builder_hbparam( 1 ); \
      if ( bab ) { hb_retl( call ( bab ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

HB_AB_BSON( BSON_ARRAY_BUILDER_APPEND_ARRAY, bson_array_builder_append_array )
HB_AB_BSON( BSON_ARRAY_BUILDER_APPEND_DOCUMENT, bson_array_builder_append_document )
HB_AB_BOOL( BSON_ARRAY_BUILDER_APPEND_BOOL, bson_array_builder_append_bool )
HB_AB_I64( BSON_ARRAY_BUILDER_APPEND_DATE_TIME, bson_array_builder_append_date_time )
HB_AB_I64( BSON_ARRAY_BUILDER_APPEND_INT64, bson_array_builder_append_int64 )
HB_AB_I64( BSON_ARRAY_BUILDER_APPEND_TIME_T, bson_array_builder_append_time_t )
HB_AB_DBL( BSON_ARRAY_BUILDER_APPEND_DOUBLE, bson_array_builder_append_double )
HB_AB_I32( BSON_ARRAY_BUILDER_APPEND_INT32, bson_array_builder_append_int32 )
HB_AB_STR( BSON_ARRAY_BUILDER_APPEND_CODE, bson_array_builder_append_code )

HB_AB_NOARG( BSON_ARRAY_BUILDER_APPEND_MAXKEY, bson_array_builder_append_maxkey )
HB_AB_NOARG( BSON_ARRAY_BUILDER_APPEND_MINKEY, bson_array_builder_append_minkey )
HB_AB_NOARG( BSON_ARRAY_BUILDER_APPEND_NOW_UTC, bson_array_builder_append_now_utc )
HB_AB_NOARG( BSON_ARRAY_BUILDER_APPEND_NULL, bson_array_builder_append_null )
HB_AB_NOARG( BSON_ARRAY_BUILDER_APPEND_UNDEFINED, bson_array_builder_append_undefined )

HB_FUNC( BSON_ARRAY_BUILDER_APPEND_UTF8 )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );
   const char * v = hb_parc( 2 );
   if ( bab && v ) { hb_retl( bson_array_builder_append_utf8( bab, v, (int) strlen( v ) ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( BSON_ARRAY_BUILDER_APPEND_REGEX )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );
   const char * regex = hb_parc( 2 );
   const char * options = hb_parc( 3 );
   if ( bab && regex ) { hb_retl( bson_array_builder_append_regex( bab, regex, options ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( BSON_ARRAY_BUILDER_APPEND_OID )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );
   const bson_oid_t * oid = bson_oid_hbparam( 2 );
   if ( bab && oid ) { hb_retl( bson_array_builder_append_oid( bab, oid ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( BSON_ARRAY_BUILDER_APPEND_VALUE )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );
   const bson_value_t * v = bson_value_hbparam( 2 );
   if ( bab && v ) { hb_retl( bson_array_builder_append_value( bab, v ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( BSON_ARRAY_BUILDER_APPEND_ITER )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );
   const bson_iter_t * iter = bson_iter_hbparam( 2 );
   if ( bab && iter ) { hb_retl( bson_array_builder_append_iter( bab, iter ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( BSON_ARRAY_BUILDER_APPEND_ARRAY_BUILDER_BEGIN )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );

   if ( bab )
   {
      bson_array_builder_t * child = bson_array_builder_new();

      if ( child && bson_array_builder_append_array_builder_begin( bab, &child ) )
      {
         PHB_BSON ph = hbbson_new_dataContainer( _hbbson_array_builder_t_, child );
         hb_retptrGC( ph );
      }
      else
      {
         if ( child ) bson_array_builder_destroy( child );
         hb_ret();
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( BSON_ARRAY_BUILDER_APPEND_ARRAY_BUILDER_END )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );
   bson_array_builder_t * child = bson_array_builder_hbparam( 2 );

   if ( bab && child )
   {
      hb_retl( bson_array_builder_append_array_builder_end( bab, child ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( BSON_ARRAY_BUILDER_APPEND_DOCUMENT_BEGIN )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );

   if ( bab )
   {
      bson_t * child = bson_new();

      if ( child && bson_array_builder_append_document_begin( bab, child ) )
      {
         PHB_BSON ph = hbbson_new_dataContainer( _hbbson_t_, child );
         hb_retptrGC( ph );
      }
      else
      {
         if ( child ) bson_destroy( child );
         hb_ret();
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( BSON_ARRAY_BUILDER_APPEND_DOCUMENT_END )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );
   bson_t * child = bson_hbparam( 2, HB_IT_ANY );

   if ( bab && child )
   {
      hb_retl( bson_array_builder_append_document_end( bab, child ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( BSON_ARRAY_BUILDER_APPEND_ARRAY_FROM_VECTOR )
{
   bson_array_builder_t * bab = bson_array_builder_hbparam( 1 );
   const bson_iter_t * iter = bson_iter_hbparam( 2 );

   if ( bab && iter )
   {
      hb_retl( bson_array_builder_append_array_from_vector( bab, iter ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}
