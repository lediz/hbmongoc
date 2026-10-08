//
//  hb_mongoc_bulkwrite.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//
//  Wraps mongoc-bulkwrite.h (mongo-c-driver 2.x): the bulkwrite object,
//  its per-operation option objects, the execute options, the result and
//  the exception accessors.
//
//  NOTE: the *_new / *_set_* / *_destroy accessors for the option objects
//        return void or a pointer; the "append_*" and "execute" calls take
//        an optional opts pointer that may be NULL.
// ------------------------------------------------------------------

#include "hb_mongoc.h"

/* ------------------------------------------------------------------
 * mongoc_bulkwrite_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_BULKWRITE_NEW )
{
   mongoc_bulkwrite_t * self = mongoc_bulkwrite_new();

   if ( self )
   {
      PHB_MONGOC phSelf = hbmongoc_new_dataContainer( _hbmongoc_bulkwrite_t_, self );
      hb_retptrGC( phSelf );
   }
   else
   {
      hb_ret();
   }
}

HB_FUNC( MONGOC_CLIENT_BULKWRITE_NEW )
{
   mongoc_client_t * client = mongoc_hbparam( 1, _hbmongoc_client_t_ );

   if ( client )
   {
      mongoc_bulkwrite_t * self = mongoc_client_bulkwrite_new( client );

      if ( self )
      {
         PHB_MONGOC phSelf = hbmongoc_new_dataContainer( _hbmongoc_bulkwrite_t_, self );
         hb_retptrGC( phSelf );
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

HB_FUNC( MONGOC_BULKWRITE_DESTROY )
{
   PHB_MONGOC self = hbmongoc_param( 1, _hbmongoc_bulkwrite_t_ );

   if ( self )
   {
      mongoc_bulkwrite_destroy( ( mongoc_bulkwrite_t * ) self->p );
      self->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_SET_CLIENT )
{
   mongoc_bulkwrite_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_t_ );
   mongoc_client_t * client = mongoc_hbparam( 2, _hbmongoc_client_t_ );

   if ( self && client )
   {
      mongoc_bulkwrite_set_client( self, client );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_SET_SESSION )
{
   mongoc_bulkwrite_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_t_ );
   mongoc_client_session_t * session = mongoc_hbparam( 2, _hbmongoc_client_session_t_ );

   if ( self && session )
   {
      mongoc_bulkwrite_set_session( self, session );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_EXECUTE )
{
   mongoc_bulkwrite_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_t_ );

   if ( self )
   {
      mongoc_bulkwriteopts_t * opts = mongoc_hbparam( 2, _hbmongoc_bulkwriteopts_t_ );
      mongoc_bulkwritereturn_t ret = mongoc_bulkwrite_execute( self, opts );

      // the return struct owns a result and an exception pointer; hand
      // each back as a wrapped object (NULL when not present).
      PHB_ITEM pHash = hb_itemNew( NULL );
      hb_hashNew( pHash );

      PHB_ITEM pKey = hb_itemNew( NULL );
      PHB_ITEM pVal = hb_itemNew( NULL );

      hb_itemPutC( pKey, "result" );
      if ( ret.res )
      {
         PHB_ITEM pResItem = hb_itemNew( NULL );
         hb_itemPutPtrGC( pResItem, hbmongoc_new_dataContainer( _hbmongoc_bulkwriteresult_t_, ret.res ) );
         hb_hashAdd( pHash, pKey, pResItem );
      }
      else
      {
         hb_hashAdd( pHash, pKey, hb_itemNew( NULL ) );
      }

      hb_itemRelease( pKey );
      pKey = hb_itemNew( NULL );

      hb_itemPutC( pKey, "exception" );
      if ( ret.exc )
      {
         PHB_ITEM pExcItem = hb_itemNew( NULL );
         hb_itemPutPtrGC( pExcItem, hbmongoc_new_dataContainer( _hbmongoc_bulkwriteexception_t_, ret.exc ) );
         hb_hashAdd( pHash, pKey, pExcItem );
      }
      else
      {
         hb_hashAdd( pHash, pKey, hb_itemNew( NULL ) );
      }

      hb_itemRelease( pKey );
      hb_itemRelease( pVal );

      hb_itemReturnRelease( pHash );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_CHECK_ACKNOWLEDGED )
{
   const mongoc_bulkwrite_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_t_ );

   if ( self )
   {
      bson_error_t error;
      mongoc_bulkwrite_check_acknowledged_t ack = mongoc_bulkwrite_check_acknowledged( self, &error );

      bson_hbstor_byref_error( 2, &error, ack.is_ok );

      PHB_ITEM pHash = hb_itemNew( NULL );
      hb_hashNew( pHash );
      PHB_ITEM pKey = hb_itemNew( NULL );
      PHB_ITEM pVal = hb_itemNew( NULL );

      hb_itemPutC( pKey, "is_ok" );
      hb_itemPutL( pVal, ack.is_ok );
      hb_hashAdd( pHash, pKey, pVal );

      hb_itemRelease( pKey ); pKey = hb_itemNew( NULL );
      hb_itemRelease( pVal ); pVal = hb_itemNew( NULL );
      hb_itemPutC( pKey, "is_acknowledged" );
      hb_itemPutL( pVal, ack.is_acknowledged );
      hb_hashAdd( pHash, pKey, pVal );

      hb_itemRelease( pKey );
      hb_itemRelease( pVal );

      hb_itemReturnRelease( pHash );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* ------------------------------------------------------------------
 * append_* operations
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_BULKWRITE_APPEND_INSERTONE )
{
   mongoc_bulkwrite_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_t_ );
   const char * ns = hb_parc( 2 );
   bson_t * document = bson_hbparam( 3, HB_IT_ANY );
   mongoc_bulkwrite_insertoneopts_t * opts = mongoc_hbparam( 4, _hbmongoc_bulkwrite_insertoneopts_t_ );

   if ( self && ns && document )
   {
      bson_error_t error;
      bool result = mongoc_bulkwrite_append_insertone( self, ns, document, opts, &error );

      bson_hbstor_byref_error( 5, &error, result );
      hb_retl( result );

      if ( document && ! HB_ISPOINTER( 3 ) )
      {
         bson_destroy( document );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_APPEND_UPDATEONE )
{
   mongoc_bulkwrite_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_t_ );
   const char * ns = hb_parc( 2 );
   bson_t * filter = bson_hbparam( 3, HB_IT_ANY );
   bson_t * update = bson_hbparam( 4, HB_IT_ANY );
   mongoc_bulkwrite_updateoneopts_t * opts = mongoc_hbparam( 5, _hbmongoc_bulkwrite_updateoneopts_t_ );

   if ( self && ns && filter && update )
   {
      bson_error_t error;
      bool result = mongoc_bulkwrite_append_updateone( self, ns, filter, update, opts, &error );

      bson_hbstor_byref_error( 6, &error, result );
      hb_retl( result );

      if ( filter && ! HB_ISPOINTER( 3 ) )
      {
         bson_destroy( filter );
      }

      if ( update && ! HB_ISPOINTER( 4 ) )
      {
         bson_destroy( update );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_APPEND_UPDATEMANY )
{
   mongoc_bulkwrite_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_t_ );
   const char * ns = hb_parc( 2 );
   bson_t * filter = bson_hbparam( 3, HB_IT_ANY );
   bson_t * update = bson_hbparam( 4, HB_IT_ANY );
   mongoc_bulkwrite_updatemanyopts_t * opts = mongoc_hbparam( 5, _hbmongoc_bulkwrite_updatemanyopts_t_ );

   if ( self && ns && filter && update )
   {
      bson_error_t error;
      bool result = mongoc_bulkwrite_append_updatemany( self, ns, filter, update, opts, &error );

      bson_hbstor_byref_error( 6, &error, result );
      hb_retl( result );

      if ( filter && ! HB_ISPOINTER( 3 ) )
      {
         bson_destroy( filter );
      }

      if ( update && ! HB_ISPOINTER( 4 ) )
      {
         bson_destroy( update );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_APPEND_REPLACEONE )
{
   mongoc_bulkwrite_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_t_ );
   const char * ns = hb_parc( 2 );
   bson_t * filter = bson_hbparam( 3, HB_IT_ANY );
   bson_t * replacement = bson_hbparam( 4, HB_IT_ANY );
   mongoc_bulkwrite_replaceoneopts_t * opts = mongoc_hbparam( 5, _hbmongoc_bulkwrite_replaceoneopts_t_ );

   if ( self && ns && filter && replacement )
   {
      bson_error_t error;
      bool result = mongoc_bulkwrite_append_replaceone( self, ns, filter, replacement, opts, &error );

      bson_hbstor_byref_error( 6, &error, result );
      hb_retl( result );

      if ( filter && ! HB_ISPOINTER( 3 ) )
      {
         bson_destroy( filter );
      }

      if ( replacement && ! HB_ISPOINTER( 4 ) )
      {
         bson_destroy( replacement );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_APPEND_DELETEONE )
{
   mongoc_bulkwrite_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_t_ );
   const char * ns = hb_parc( 2 );
   bson_t * filter = bson_hbparam( 3, HB_IT_ANY );
   mongoc_bulkwrite_deleteoneopts_t * opts = mongoc_hbparam( 4, _hbmongoc_bulkwrite_deleteoneopts_t_ );

   if ( self && ns && filter )
   {
      bson_error_t error;
      bool result = mongoc_bulkwrite_append_deleteone( self, ns, filter, opts, &error );

      bson_hbstor_byref_error( 5, &error, result );
      hb_retl( result );

      if ( filter && ! HB_ISPOINTER( 3 ) )
      {
         bson_destroy( filter );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_APPEND_DELETENANY )
{
   mongoc_bulkwrite_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_t_ );
   const char * ns = hb_parc( 2 );
   bson_t * filter = bson_hbparam( 3, HB_IT_ANY );
   mongoc_bulkwrite_deletemanyopts_t * opts = mongoc_hbparam( 4, _hbmongoc_bulkwrite_deletemanyopts_t_ );

   if ( self && ns && filter )
   {
      bson_error_t error;
      bool result = mongoc_bulkwrite_append_deletemany( self, ns, filter, opts, &error );

      bson_hbstor_byref_error( 5, &error, result );
      hb_retl( result );

      if ( filter && ! HB_ISPOINTER( 3 ) )
      {
         bson_destroy( filter );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* ------------------------------------------------------------------
 * mongoc_bulkwriteopts_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_BULKWRITEOPTS_NEW )
{
   mongoc_bulkwriteopts_t * self = mongoc_bulkwriteopts_new();

   if ( self )
   {
      PHB_MONGOC phSelf = hbmongoc_new_dataContainer( _hbmongoc_bulkwriteopts_t_, self );
      hb_retptrGC( phSelf );
   }
   else
   {
      hb_ret();
   }
}

HB_FUNC( MONGOC_BULKWRITEOPTS_DESTROY )
{
   PHB_MONGOC self = hbmongoc_param( 1, _hbmongoc_bulkwriteopts_t_ );

   if ( self )
   {
      mongoc_bulkwriteopts_destroy( ( mongoc_bulkwriteopts_t * ) self->p );
      self->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITEOPTS_SET_ORDERED )
{
   mongoc_bulkwriteopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteopts_t_ );

   if ( self && HB_ISLOG( 2 ) )
   {
      mongoc_bulkwriteopts_set_ordered( self, hb_parl( 2 ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITEOPTS_SET_BYPASS_DOCUMENT_VALIDATION )
{
   mongoc_bulkwriteopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteopts_t_ );

   if ( self && HB_ISLOG( 2 ) )
   {
      mongoc_bulkwriteopts_set_bypassdocumentvalidation( self, hb_parl( 2 ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITEOPTS_SET_LET )
{
   mongoc_bulkwriteopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteopts_t_ );
   bson_t * let = bson_hbparam( 2, HB_IT_ANY );

   if ( self && let )
   {
      mongoc_bulkwriteopts_set_let( self, let );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( let );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITEOPTS_SET_WRITE_CONCERN )
{
   mongoc_bulkwriteopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteopts_t_ );
   const mongoc_write_concern_t * wc = mongoc_hbparam( 2, _hbmongoc_write_concern_t_ );

   if ( self && wc )
   {
      mongoc_bulkwriteopts_set_writeconcern( self, wc );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITEOPTS_SET_COMMENT )
{
   mongoc_bulkwriteopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteopts_t_ );
   const bson_value_t * comment = bson_value_hbparam( 2 );

   if ( self && comment )
   {
      mongoc_bulkwriteopts_set_comment( self, comment );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITEOPTS_SET_VERBOSERESULTS )
{
   mongoc_bulkwriteopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteopts_t_ );

   if ( self && HB_ISLOG( 2 ) )
   {
      mongoc_bulkwriteopts_set_verboseresults( self, hb_parl( 2 ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITEOPTS_SET_EXTRA )
{
   mongoc_bulkwriteopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteopts_t_ );
   bson_t * extra = bson_hbparam( 2, HB_IT_ANY );

   if ( self && extra )
   {
      mongoc_bulkwriteopts_set_extra( self, extra );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( extra );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITEOPTS_SET_SERVERID )
{
   mongoc_bulkwriteopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteopts_t_ );

   if ( self && HB_ISNUM( 2 ) )
   {
      mongoc_bulkwriteopts_set_serverid( self, (uint32_t) hb_parni( 2 ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* ------------------------------------------------------------------
 * mongoc_bulkwriteresult_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_BULKWRITERESULT_DESTROY )
{
   PHB_MONGOC self = hbmongoc_param( 1, _hbmongoc_bulkwriteresult_t_ );

   if ( self )
   {
      mongoc_bulkwriteresult_destroy( ( mongoc_bulkwriteresult_t * ) self->p );
      self->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

#define HB_BW_RESULT_I64( name, call ) \
   HB_FUNC( name ) \
   { \
      const mongoc_bulkwriteresult_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteresult_t_ ); \
      if ( self ) { hb_retnll( (HB_LONGLONG) call ( self ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

HB_BW_RESULT_I64( MONGOC_BULKWRITERESULT_INSERTEDCOUNT, mongoc_bulkwriteresult_insertedcount )
HB_BW_RESULT_I64( MONGOC_BULKWRITERESULT_UPSERTEDCOUNT, mongoc_bulkwriteresult_upsertedcount )
HB_BW_RESULT_I64( MONGOC_BULKWRITERESULT_MATCHEDCOUNT, mongoc_bulkwriteresult_matchedcount )
HB_BW_RESULT_I64( MONGOC_BULKWRITERESULT_MODIFIEDCOUNT, mongoc_bulkwriteresult_modifiedcount )
HB_BW_RESULT_I64( MONGOC_BULKWRITERESULT_DELETEDCOUNT, mongoc_bulkwriteresult_deletedcount )

HB_FUNC( MONGOC_BULKWRITERESULT_SERVERID )
{
   const mongoc_bulkwriteresult_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteresult_t_ );

   if ( self )
   {
      hb_retni( ( int ) mongoc_bulkwriteresult_serverid( self ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

#define HB_BW_RESULT_BSON( name, call ) \
   HB_FUNC( name ) \
   { \
      const mongoc_bulkwriteresult_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteresult_t_ ); \
      if ( self ) \
      { \
         const bson_t * doc = call ( self ); \
         if ( doc ) { PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( doc ) ); hb_retptrGC( phBson ); } \
         else { hb_ret(); } \
      } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

HB_BW_RESULT_BSON( MONGOC_BULKWRITERESULT_INSERTRESULTS, mongoc_bulkwriteresult_insertresults )
HB_BW_RESULT_BSON( MONGOC_BULKWRITERESULT_UPDATERESULTS, mongoc_bulkwriteresult_updateresults )
HB_BW_RESULT_BSON( MONGOC_BULKWRITERESULT_DELETERESULTS, mongoc_bulkwriteresult_deleteresults )

/* ------------------------------------------------------------------
 * mongoc_bulkwriteexception_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_BULKWRITEEXCEPTION_DESTROY )
{
   PHB_MONGOC self = hbmongoc_param( 1, _hbmongoc_bulkwriteexception_t_ );

   if ( self )
   {
      mongoc_bulkwriteexception_destroy( ( mongoc_bulkwriteexception_t * ) self->p );
      self->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITEEXCEPTION_ERROR )
{
   const mongoc_bulkwriteexception_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteexception_t_ );

   if ( self )
   {
      bson_error_t error;
      bool result = mongoc_bulkwriteexception_error( self, &error );

      bson_hbstor_byref_error( 2, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITEEXCEPTION_ERRORREPLY )
{
   const mongoc_bulkwriteexception_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteexception_t_ );

   if ( self )
   {
      const bson_t * reply = mongoc_bulkwriteexception_errorreply( self );

      if ( reply )
      {
         PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( reply ) );
         hb_retptrGC( phBson );
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

HB_FUNC( MONGOC_BULKWRITEEXCEPTION_WRITEERRORS )
{
   const mongoc_bulkwriteexception_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteexception_t_ );

   if ( self )
   {
      const bson_t * errors = mongoc_bulkwriteexception_writeerrors( self );

      if ( errors )
      {
         PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( errors ) );
         hb_retptrGC( phBson );
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

HB_FUNC( MONGOC_BULKWRITEEXCEPTION_WRITECONCERNERRORS )
{
   const mongoc_bulkwriteexception_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwriteexception_t_ );

   if ( self )
   {
      const bson_t * errors = mongoc_bulkwriteexception_writeconcernerrors( self );

      if ( errors )
      {
         PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( errors ) );
         hb_retptrGC( phBson );
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

/* ------------------------------------------------------------------
 * per-operation option objects
 * ------------------------------------------------------------------ */

// insertone: no setters, only new/destroy
HB_FUNC( MONGOC_BULKWRITE_INSERTONEOPTS_NEW )
{
   mongoc_bulkwrite_insertoneopts_t * self = mongoc_bulkwrite_insertoneopts_new();

   if ( self )
   {
      PHB_MONGOC phSelf = hbmongoc_new_dataContainer( _hbmongoc_bulkwrite_insertoneopts_t_, self );
      hb_retptrGC( phSelf );
   }
   else
   {
      hb_ret();
   }
}

// the remaining option objects share the same new/destroy + collation/hint
// shape; updatemany/replaceone add upsert/sort.

HB_FUNC( MONGOC_BULKWRITE_UPDATEONEOPTS_NEW )
{
   mongoc_bulkwrite_updateoneopts_t * self = mongoc_bulkwrite_updateoneopts_new();

   if ( self )
   {
      PHB_MONGOC phSelf = hbmongoc_new_dataContainer( _hbmongoc_bulkwrite_updateoneopts_t_, self );
      hb_retptrGC( phSelf );
   }
   else
   {
      hb_ret();
   }
}

HB_FUNC( MONGOC_BULKWRITE_UPDATEONEOPTS_SET_COLLATION )
{
   mongoc_bulkwrite_updateoneopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_updateoneopts_t_ );
   bson_t * collation = bson_hbparam( 2, HB_IT_ANY );

   if ( self && collation )
   {
      mongoc_bulkwrite_updateoneopts_set_collation( self, collation );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( collation );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_UPDATEONEOPTS_SET_HINT )
{
   mongoc_bulkwrite_updateoneopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_updateoneopts_t_ );
   const bson_value_t * hint = bson_value_hbparam( 2 );

   if ( self && hint )
   {
      mongoc_bulkwrite_updateoneopts_set_hint( self, hint );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_UPDATEMANYOPTS_NEW )
{
   mongoc_bulkwrite_updatemanyopts_t * self = mongoc_bulkwrite_updatemanyopts_new();

   if ( self )
   {
      PHB_MONGOC phSelf = hbmongoc_new_dataContainer( _hbmongoc_bulkwrite_updatemanyopts_t_, self );
      hb_retptrGC( phSelf );
   }
   else
   {
      hb_ret();
   }
}

HB_FUNC( MONGOC_BULKWRITE_UPDATEMANYOPTS_SET_COLLATION )
{
   mongoc_bulkwrite_updatemanyopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_updatemanyopts_t_ );
   bson_t * collation = bson_hbparam( 2, HB_IT_ANY );

   if ( self && collation )
   {
      mongoc_bulkwrite_updatemanyopts_set_collation( self, collation );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( collation );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_UPDATEMANYOPTS_SET_HINT )
{
   mongoc_bulkwrite_updatemanyopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_updatemanyopts_t_ );
   const bson_value_t * hint = bson_value_hbparam( 2 );

   if ( self && hint )
   {
      mongoc_bulkwrite_updatemanyopts_set_hint( self, hint );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_UPDATEMANYOPTS_SET_ARRAYFILTERS )
{
   mongoc_bulkwrite_updatemanyopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_updatemanyopts_t_ );
   bson_t * arrayfilters = bson_hbparam( 2, HB_IT_ANY );

   if ( self && arrayfilters )
   {
      mongoc_bulkwrite_updatemanyopts_set_arrayfilters( self, arrayfilters );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( arrayfilters );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_UPDATEONEOPTS_SET_ARRAYFILTERS )
{
   mongoc_bulkwrite_updateoneopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_updateoneopts_t_ );
   bson_t * arrayfilters = bson_hbparam( 2, HB_IT_ANY );

   if ( self && arrayfilters )
   {
      mongoc_bulkwrite_updateoneopts_set_arrayfilters( self, arrayfilters );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( arrayfilters );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_REPLACEONEOPTS_NEW )
{
   mongoc_bulkwrite_replaceoneopts_t * self = mongoc_bulkwrite_replaceoneopts_new();

   if ( self )
   {
      PHB_MONGOC phSelf = hbmongoc_new_dataContainer( _hbmongoc_bulkwrite_replaceoneopts_t_, self );
      hb_retptrGC( phSelf );
   }
   else
   {
      hb_ret();
   }
}

HB_FUNC( MONGOC_BULKWRITE_REPLACEONEOPTS_SET_COLLATION )
{
   mongoc_bulkwrite_replaceoneopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_replaceoneopts_t_ );
   bson_t * collation = bson_hbparam( 2, HB_IT_ANY );

   if ( self && collation )
   {
      mongoc_bulkwrite_replaceoneopts_set_collation( self, collation );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( collation );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_REPLACEONEOPTS_SET_HINT )
{
   mongoc_bulkwrite_replaceoneopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_replaceoneopts_t_ );
   const bson_value_t * hint = bson_value_hbparam( 2 );

   if ( self && hint )
   {
      mongoc_bulkwrite_replaceoneopts_set_hint( self, hint );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_REPLACEONEOPTS_SET_UPSERT )
{
   mongoc_bulkwrite_replaceoneopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_replaceoneopts_t_ );

   if ( self && HB_ISLOG( 2 ) )
   {
      mongoc_bulkwrite_replaceoneopts_set_upsert( self, hb_parl( 2 ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_REPLACEONEOPTS_SET_SORT )
{
   mongoc_bulkwrite_replaceoneopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_replaceoneopts_t_ );
   bson_t * sort = bson_hbparam( 2, HB_IT_ANY );

   if ( self && sort )
   {
      mongoc_bulkwrite_replaceoneopts_set_sort( self, sort );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( sort );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_DELETEONEOPTS_NEW )
{
   mongoc_bulkwrite_deleteoneopts_t * self = mongoc_bulkwrite_deleteoneopts_new();

   if ( self )
   {
      PHB_MONGOC phSelf = hbmongoc_new_dataContainer( _hbmongoc_bulkwrite_deleteoneopts_t_, self );
      hb_retptrGC( phSelf );
   }
   else
   {
      hb_ret();
   }
}

HB_FUNC( MONGOC_BULKWRITE_DELETEONEOPTS_SET_COLLATION )
{
   mongoc_bulkwrite_deleteoneopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_deleteoneopts_t_ );
   bson_t * collation = bson_hbparam( 2, HB_IT_ANY );

   if ( self && collation )
   {
      mongoc_bulkwrite_deleteoneopts_set_collation( self, collation );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( collation );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_DELETEONEOPTS_SET_HINT )
{
   mongoc_bulkwrite_deleteoneopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_deleteoneopts_t_ );
   const bson_value_t * hint = bson_value_hbparam( 2 );

   if ( self && hint )
   {
      mongoc_bulkwrite_deleteoneopts_set_hint( self, hint );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_DELETENANYOPTS_NEW )
{
   mongoc_bulkwrite_deletemanyopts_t * self = mongoc_bulkwrite_deletemanyopts_new();

   if ( self )
   {
      PHB_MONGOC phSelf = hbmongoc_new_dataContainer( _hbmongoc_bulkwrite_deletemanyopts_t_, self );
      hb_retptrGC( phSelf );
   }
   else
   {
      hb_ret();
   }
}

HB_FUNC( MONGOC_BULKWRITE_DELETENANYOPTS_SET_COLLATION )
{
   mongoc_bulkwrite_deletemanyopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_deletemanyopts_t_ );
   bson_t * collation = bson_hbparam( 2, HB_IT_ANY );

   if ( self && collation )
   {
      mongoc_bulkwrite_deletemanyopts_set_collation( self, collation );

      if ( ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( collation );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_BULKWRITE_DELETENANYOPTS_SET_HINT )
{
   mongoc_bulkwrite_deletemanyopts_t * self = mongoc_hbparam( 1, _hbmongoc_bulkwrite_deletemanyopts_t_ );
   const bson_value_t * hint = bson_value_hbparam( 2 );

   if ( self && hint )
   {
      mongoc_bulkwrite_deletemanyopts_set_hint( self, hint );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}
