//
//  hb_mongoc_stream.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//
//  Wraps the mongoc_stream_t API (mongoc-stream.h and the buffered / file /
//  socket / gridfs / tls stream constructors).
//
//  NOT wrapped: mongoc_stream_poll (takes an array of mongoc_stream_poll_t,
//  a value struct), mongoc_stream_setsockopt (raw socket opt marshalling),
//  and the *_new constructors that take a mongoc_ssl_opt_t by value.
// ------------------------------------------------------------------

#include "hb_mongoc.h"

/* ------------------------------------------------------------------
 * lifecycle
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_STREAM_DESTROY )
{
   PHB_MONGOC stream = hbmongoc_param( 1, _hbmongoc_stream_t_ );

   if ( stream )
   {
      mongoc_stream_destroy( ( mongoc_stream_t * ) stream->p );
      stream->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_STREAM_BUFFERED_NEW )
{
   mongoc_stream_t * base = mongoc_hbparam( 1, _hbmongoc_stream_t_ );

   if ( base && HB_ISNUM( 2 ) )
   {
      mongoc_stream_t * s = mongoc_stream_buffered_new( base, (size_t) hb_parnll( 2 ) );

      if ( s ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_stream_t_, s ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_STREAM_FILE_NEW )
{
   if ( HB_ISNUM( 1 ) )
   {
      mongoc_stream_t * s = mongoc_stream_file_new( (int) hb_parni( 1 ) );

      if ( s ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_stream_t_, s ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_STREAM_FILE_NEW_FOR_PATH )
{
   const char * path = hb_parc( 1 );

   if ( path )
   {
      mongoc_stream_t * s = mongoc_stream_file_new_for_path( path,
                                 (int) hb_parnidef( 2, 0 ),
                                 (int) hb_parnidef( 3, 0 ) );

      if ( s ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_stream_t_, s ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_STREAM_GRIDFS_NEW )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );

   if ( file )
   {
      mongoc_stream_t * s = mongoc_stream_gridfs_new( file );

      if ( s ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_stream_t_, s ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_STREAM_SOCKET_NEW )
{
   mongoc_socket_t * sock = mongoc_hbparam( 1, _hbmongoc_socket_t_ );

   if ( sock )
   {
      mongoc_stream_t * s = mongoc_stream_socket_new( sock );

      if ( s ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_stream_t_, s ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* ------------------------------------------------------------------
 * predicates / simple int-returning ops
 * ------------------------------------------------------------------ */

#define HB_STREAM_BOOL( fname, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ ); \
      if ( stream ) { hb_retl( call ( stream ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

#define HB_STREAM_INT( fname, call ) \
   HB_FUNC( fname ) \
   { \
      mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ ); \
      if ( stream ) { hb_retni( call ( stream ) ); } \
      else { HBMONGOC_ERR_ARGS(); } \
   }

HB_STREAM_BOOL( MONGOC_STREAM_CHECK_CLOSED, mongoc_stream_check_closed )
HB_STREAM_BOOL( MONGOC_STREAM_TIMED_OUT, mongoc_stream_timed_out )
HB_STREAM_BOOL( MONGOC_STREAM_SHOULD_RETRY, mongoc_stream_should_retry )
HB_STREAM_INT( MONGOC_STREAM_CLOSE, mongoc_stream_close )
HB_STREAM_INT( MONGOC_STREAM_FLUSH, mongoc_stream_flush )

HB_FUNC( MONGOC_STREAM_FAILED )
{
   mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );
   if ( stream ) { mongoc_stream_failed( stream ); }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_STREAM_GET_BASE_STREAM )
{
   mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );
   if ( stream )
   {
      mongoc_stream_t * base = mongoc_stream_get_base_stream( stream );
      if ( base ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_stream_t_, base ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_STREAM_GET_TLS_STREAM )
{
   mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );
   if ( stream )
   {
      mongoc_stream_t * tls = mongoc_stream_get_tls_stream( stream );
      if ( tls ) { PHB_MONGOC ph = hbmongoc_new_dataContainer( _hbmongoc_stream_t_, tls ); hb_retptrGC( ph ); }
      else { hb_ret(); }
   }
   else { HBMONGOC_ERR_ARGS(); }
}

HB_FUNC( MONGOC_STREAM_FILE_GET_FD )
{
   mongoc_stream_file_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );
   if ( stream ) { hb_retni( mongoc_stream_file_get_fd( stream ) ); }
   else { HBMONGOC_ERR_ARGS(); }
}

/* ------------------------------------------------------------------
 * read / write (raw buffer)
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_STREAM_WRITE )
{
   mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );
   const char * buf = hb_parc( 2 );

   if ( stream && buf )
   {
      ssize_t rc = mongoc_stream_write( stream, (void *) buf, (size_t) strlen( buf ), (int32_t) hb_parnidef( 3, 0 ) );
      hb_retnll( (HB_LONGLONG) rc );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_STREAM_READ )
{
   mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );

   if ( stream && HB_ISNUM( 2 ) )
   {
      size_t count = (size_t) hb_parnll( 2 );
      char * buf = hb_xgrab( count + 1 );
      size_t min_bytes = HB_ISNIL( 3 ) ? 0 : (size_t) hb_parnll( 3 );

      if ( buf )
      {
         ssize_t rc = mongoc_stream_read( stream, buf, count, min_bytes, (int32_t) hb_parnidef( 4, 0 ) );

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
 * readv / writev (iovec)
 * ------------------------------------------------------------------ */

static bool hbmongoc_stream_build_iovec( PHB_ITEM pBuffers, mongoc_iovec_t ** ppIov, size_t * pIovcnt )
{
   size_t cnt = 0, i;

   if ( ! pBuffers ) return false;

   cnt = hb_arrayLen( pBuffers );
   if ( cnt == 0 || cnt > 1024 ) return false;

   *ppIov = hb_xgrab( cnt * sizeof( mongoc_iovec_t ) );
   if ( ! *ppIov ) return false;

   for ( i = 0; i < cnt; ++i )
   {
      PHB_ITEM pItem = hb_arrayGetC( pBuffers, i + 1 );
      if ( ! pItem ) { hb_xfree( *ppIov ); return false; }
      ( *ppIov )[ i ].iov_base = pItem;
      ( *ppIov )[ i ].iov_len = hb_itemSize( pItem );
   }

   *pIovcnt = cnt;
   return true;
}

HB_FUNC( MONGOC_STREAM_WRITEV )
{
   mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );
   PHB_ITEM pBuffers = hb_param( 2, HB_IT_ARRAY );

   if ( stream && pBuffers )
   {
      mongoc_iovec_t * iov = NULL;
      size_t iovcnt = 0;

      if ( hbmongoc_stream_build_iovec( pBuffers, &iov, &iovcnt ) )
      {
         ssize_t rc = mongoc_stream_writev( stream, iov, iovcnt, (int32_t) hb_parnidef( 3, 0 ) );
         hb_xfree( iov );
         hb_retnll( (HB_LONGLONG) rc );
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

HB_FUNC( MONGOC_STREAM_READV )
{
   mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );
   PHB_ITEM pBuffers = hb_param( 2, HB_IT_ARRAY );

   if ( stream && pBuffers )
   {
      mongoc_iovec_t * iov = NULL;
      size_t iovcnt = 0;

      if ( hbmongoc_stream_build_iovec( pBuffers, &iov, &iovcnt ) )
      {
         ssize_t rc = mongoc_stream_readv( stream, iov, iovcnt,
                                            HB_ISNIL( 3 ) ? 0 : (size_t) hb_parnll( 3 ),
                                            (int32_t) hb_parnidef( 4, 0 ) );
         hb_xfree( iov );
         hb_retnll( (HB_LONGLONG) rc );
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
 * TLS
 * ------------------------------------------------------------------ */

#ifdef MONGOC_ENABLE_SSL
HB_FUNC( MONGOC_STREAM_TLS_HANDSHAKE_BLOCK )
{
   mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );
   const char * host = hb_parc( 2 );

   if ( stream && host )
   {
      bson_error_t error;
      bool result = mongoc_stream_tls_handshake_block( stream, host, (int32_t) hb_parnidef( 3, 0 ), &error );

      bson_hbstor_byref_error( 4, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}
#endif
