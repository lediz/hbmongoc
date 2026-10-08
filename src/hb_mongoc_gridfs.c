//
//  hb_mongoc_gridfs.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//
//  Wraps mongoc-gridfs.h, mongoc-gridfs-file.h, mongoc-gridfs-file-list.h
//  and mongoc-gridfs-bucket.h (mongo-c-driver 2.x).
//

#include "hb_mongoc.h"

/* ------------------------------------------------------------------
 * mongoc_gridfs_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_GRIDFS_DESTROY )
{
   PHB_MONGOC gridfs = hbmongoc_param( 1, _hbmongoc_gridfs_t_ );

   if ( gridfs )
   {
      mongoc_gridfs_destroy( ( mongoc_gridfs_t * ) gridfs->p );
      gridfs->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_DROP )
{
   mongoc_gridfs_t * gridfs = mongoc_hbparam( 1, _hbmongoc_gridfs_t_ );

   if ( gridfs )
   {
      bson_error_t error;
      bool result = mongoc_gridfs_drop( gridfs, &error );
      bson_hbstor_byref_error( 2, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_GET_FILES )
{
   mongoc_gridfs_t * gridfs = mongoc_hbparam( 1, _hbmongoc_gridfs_t_ );

   if ( gridfs )
   {
      mongoc_collection_t * files = mongoc_gridfs_get_files( gridfs );

      if ( files )
      {
         PHB_MONGOC phFiles = hbmongoc_new_dataContainer( _hbmongoc_collection_t_, files );
         hb_retptrGC( phFiles );
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

HB_FUNC( MONGOC_GRIDFS_GET_CHUNKS )
{
   mongoc_gridfs_t * gridfs = mongoc_hbparam( 1, _hbmongoc_gridfs_t_ );

   if ( gridfs )
   {
      mongoc_collection_t * chunks = mongoc_gridfs_get_chunks( gridfs );

      if ( chunks )
      {
         PHB_MONGOC phChunks = hbmongoc_new_dataContainer( _hbmongoc_collection_t_, chunks );
         hb_retptrGC( phChunks );
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

HB_FUNC( MONGOC_GRIDFS_CREATE_FILE )
{
   mongoc_gridfs_t * gridfs = mongoc_hbparam( 1, _hbmongoc_gridfs_t_ );

   if ( gridfs )
   {
      // options are passed as separate Harbour params, not a C struct:
      //   2 = filename (string), 3 = chunk_size (int), 4 = metadata (BSON)
      // pOpt stays NULL when none of them are supplied.
      mongoc_gridfs_file_opt_t opt;
      mongoc_gridfs_file_opt_t * pOpt = NULL;
      bson_t * metadata = bson_hbparam( 4, HB_IT_ANY );

      if ( HB_IS_STRING( 2 ) || HB_ISNUM( 3 ) || metadata )
      {
         memset( &opt, 0, sizeof( opt ) );
         opt.filename   = hb_parc( 2 );
         opt.chunk_size = (uint32_t) hb_parnidef( 3, 0 );
         opt.metadata   = metadata;
         pOpt = & opt;
      }

      mongoc_gridfs_file_t * file = mongoc_gridfs_create_file( gridfs, pOpt );

      if ( file )
      {
         PHB_MONGOC phFile = hbmongoc_new_dataContainer( _hbmongoc_gridfs_file_t_, file );
         hb_retptrGC( phFile );
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

HB_FUNC( MONGOC_GRIDFS_CREATE_FILE_FROM_STREAM )
{
   mongoc_gridfs_t * gridfs = mongoc_hbparam( 1, _hbmongoc_gridfs_t_ );
   mongoc_stream_t * stream = mongoc_hbparam( 2, _hbmongoc_stream_t_ );

   if ( gridfs && stream )
   {
      mongoc_gridfs_file_opt_t opt;
      mongoc_gridfs_file_opt_t * pOpt = NULL;
      bson_t * metadata = bson_hbparam( 5, HB_IT_ANY );

      if ( HB_IS_STRING( 3 ) || HB_ISNUM( 4 ) || metadata )
      {
         memset( &opt, 0, sizeof( opt ) );
         opt.filename   = hb_parc( 3 );
         opt.chunk_size = (uint32_t) hb_parnidef( 4, 0 );
         opt.metadata   = metadata;
         pOpt = & opt;
      }

      mongoc_gridfs_file_t * file = mongoc_gridfs_create_file_from_stream( gridfs, stream, pOpt );

      if ( file )
      {
         PHB_MONGOC phFile = hbmongoc_new_dataContainer( _hbmongoc_gridfs_file_t_, file );
         hb_retptrGC( phFile );
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

HB_FUNC( MONGOC_GRIDFS_FIND_ONE_BY_FILENAME )
{
   mongoc_gridfs_t * gridfs = mongoc_hbparam( 1, _hbmongoc_gridfs_t_ );
   const char * filename = hb_parc( 2 );

   if ( gridfs && filename )
   {
      bson_error_t error;
      mongoc_gridfs_file_t * file = mongoc_gridfs_find_one_by_filename( gridfs, filename, &error );

      bson_hbstor_byref_error( 3, &error, file != NULL );

      if ( file )
      {
         PHB_MONGOC phFile = hbmongoc_new_dataContainer( _hbmongoc_gridfs_file_t_, file );
         hb_retptrGC( phFile );
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

HB_FUNC( MONGOC_GRIDFS_FIND_ONE_WITH_OPTS )
{
   mongoc_gridfs_t * gridfs = mongoc_hbparam( 1, _hbmongoc_gridfs_t_ );

   if ( gridfs )
   {
      bson_t * filter = bson_hbparam( 2, HB_IT_ANY );
      bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
      bson_error_t error;

      mongoc_gridfs_file_t * file = mongoc_gridfs_find_one_with_opts( gridfs, filter, opts, &error );

      bson_hbstor_byref_error( 4, &error, file != NULL );

      if ( file )
      {
         PHB_MONGOC phFile = hbmongoc_new_dataContainer( _hbmongoc_gridfs_file_t_, file );
         hb_retptrGC( phFile );
      }
      else
      {
         hb_ret();
      }

      if ( filter && ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( filter );
      }

      if ( opts && ! HB_ISPOINTER( 3 ) )
      {
         bson_destroy( opts );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FIND_WITH_OPTS )
{
   mongoc_gridfs_t * gridfs = mongoc_hbparam( 1, _hbmongoc_gridfs_t_ );

   if ( gridfs )
   {
      bson_t * filter = bson_hbparam( 2, HB_IT_ANY );
      bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

      mongoc_gridfs_file_list_t * list = mongoc_gridfs_find_with_opts( gridfs, filter, opts );

      if ( list )
      {
         PHB_MONGOC phList = hbmongoc_new_dataContainer( _hbmongoc_gridfs_file_list_t_, list );
         hb_retptrGC( phList );
      }
      else
      {
         hb_ret();
      }

      if ( filter && ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( filter );
      }

      if ( opts && ! HB_ISPOINTER( 3 ) )
      {
         bson_destroy( opts );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_REMOVE_BY_FILENAME )
{
   mongoc_gridfs_t * gridfs = mongoc_hbparam( 1, _hbmongoc_gridfs_t_ );
   const char * filename = hb_parc( 2 );

   if ( gridfs && filename )
   {
      bson_error_t error;
      bool result = mongoc_gridfs_remove_by_filename( gridfs, filename, &error );

      bson_hbstor_byref_error( 3, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* ------------------------------------------------------------------
 * mongoc_gridfs_file_list_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_GRIDFS_FILE_LIST_DESTROY )
{
   PHB_MONGOC list = hbmongoc_param( 1, _hbmongoc_gridfs_file_list_t_ );

   if ( list )
   {
      mongoc_gridfs_file_list_destroy( ( mongoc_gridfs_file_list_t * ) list->p );
      list->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_LIST_ERROR )
{
   mongoc_gridfs_file_list_t * list = mongoc_hbparam( 1, _hbmongoc_gridfs_file_list_t_ );

   if ( list )
   {
      bson_error_t error;
      bool result = mongoc_gridfs_file_list_error( list, &error );

      bson_hbstor_byref_error( 2, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_LIST_NEXT )
{
   mongoc_gridfs_file_list_t * list = mongoc_hbparam( 1, _hbmongoc_gridfs_file_list_t_ );

   if ( list )
   {
      mongoc_gridfs_file_t * file = mongoc_gridfs_file_list_next( list );

      if ( file )
      {
         PHB_MONGOC phFile = hbmongoc_new_dataContainer( _hbmongoc_gridfs_file_t_, file );
         hb_retptrGC( phFile );
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
 * mongoc_gridfs_file_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_GRIDFS_FILE_DESTROY )
{
   PHB_MONGOC file = hbmongoc_param( 1, _hbmongoc_gridfs_file_t_ );

   if ( file )
   {
      mongoc_gridfs_file_destroy( ( mongoc_gridfs_file_t * ) file->p );
      file->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_ERROR )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );

   if ( file )
   {
      bson_error_t error;
      bool result = mongoc_gridfs_file_error( file, &error );

      bson_hbstor_byref_error( 2, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_GET_CHUNK_SIZE )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );

   if ( file )
   {
      hb_retni( mongoc_gridfs_file_get_chunk_size( file ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_GET_LENGTH )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );

   if ( file )
   {
      hb_retnll( (HB_LONGLONG) mongoc_gridfs_file_get_length( file ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_GET_UPLOAD_DATE )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );

   if ( file )
   {
      hb_retnll( (HB_LONGLONG) mongoc_gridfs_file_get_upload_date( file ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_SAVE )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );

   if ( file )
   {
      hb_retl( mongoc_gridfs_file_save( file ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_REMOVE )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );

   if ( file )
   {
      bson_error_t error;
      bool result = mongoc_gridfs_file_remove( file, &error );

      bson_hbstor_byref_error( 2, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_SET_ID )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );
   const bson_value_t * id = bson_value_hbparam( 2 );

   if ( file && id )
   {
      bson_error_t error;
      bool result = mongoc_gridfs_file_set_id( file, id, &error );

      bson_hbstor_byref_error( 3, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_SEEK )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );

   if ( file && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) )
   {
      hb_retni( mongoc_gridfs_file_seek( file, (int64_t) hb_parnll( 2 ), (int) hb_parni( 3 ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_FILE_TELL )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );

   if ( file )
   {
      hb_retnll( (HB_LONGLONG) mongoc_gridfs_file_tell( file ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* readv / writev take a mongoc_iovec_t array. Harbour passes the buffers as
   an array of strings; we build the iovec on the stack for the common
   single-buffer case and fall back to a heap array for larger ones. */

static bool hbmongoc_apm_build_iovec( PHB_ITEM pBuffers, mongoc_iovec_t ** ppIov,
                                      size_t * pIovcnt )
{
   PHB_ITEM pItem = NULL;
   size_t cnt = 0;
   size_t i;

   if ( ! pBuffers )
   {
      return false;
   }

   cnt = hb_arrayLen( pBuffers );

   if ( cnt == 0 || cnt > 1024 )
   {
      return false;
   }

   *ppIov = hb_xgrab( cnt * sizeof( mongoc_iovec_t ) );

   if ( ! *ppIov )
   {
      return false;
   }

   for ( i = 0; i < cnt; ++i )
   {
      pItem = hb_arrayGetC( pBuffers, i + 1 );

      if ( ! pItem )
      {
         hb_xfree( *ppIov );
         return false;
      }

      ( *ppIov )[ i ].iov_base = pItem;
      ( *ppIov )[ i ].iov_len = hb_itemSize( pItem );
   }

   *pIovcnt = cnt;
   return true;
}

HB_FUNC( MONGOC_GRIDFS_FILE_READV )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );
   PHB_ITEM pBuffers = hb_param( 2, HB_IT_ARRAY );

   if ( file && pBuffers )
   {
      mongoc_iovec_t * iov = NULL;
      size_t iovcnt = 0;

      if ( hbmongoc_apm_build_iovec( pBuffers, &iov, &iovcnt ) )
      {
         ssize_t rc = mongoc_gridfs_file_readv( file, iov, iovcnt,
                                                HB_ISNIL( 3 ) ? 0 : (size_t) hb_parnll( 3 ),
                                                (uint32_t) hb_parnidef( 4, 0 ) );
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

HB_FUNC( MONGOC_GRIDFS_FILE_WRITEV )
{
   mongoc_gridfs_file_t * file = mongoc_hbparam( 1, _hbmongoc_gridfs_file_t_ );
   PHB_ITEM pBuffers = hb_param( 2, HB_IT_ARRAY );

   if ( file && pBuffers )
   {
      mongoc_iovec_t * iov = NULL;
      size_t iovcnt = 0;

      if ( hbmongoc_apm_build_iovec( pBuffers, &iov, &iovcnt ) )
      {
         ssize_t rc = mongoc_gridfs_file_writev( file, (const mongoc_iovec_t *) iov,
                                                 iovcnt,
                                                 (uint32_t) hb_parnidef( 3, 0 ) );
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
 * mongoc_gridfs_bucket_t
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_GRIDFS_BUCKET_NEW )
{
   mongoc_database_t * db = mongoc_hbparam( 1, _hbmongoc_database_t_ );

   if ( db )
   {
      bson_t * opts = bson_hbparam( 2, HB_IT_ANY );
      mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
      bson_error_t error;

      mongoc_gridfs_bucket_t * bucket = mongoc_gridfs_bucket_new( db, opts, read_prefs, &error );

      if ( bucket )
      {
         PHB_MONGOC phBucket = hbmongoc_new_dataContainer( _hbmongoc_gridfs_bucket_t_, bucket );
         hb_retptrGC( phBucket );
      }
      else
      {
         bson_hbstor_byref_error( 4, &error, false );
         hb_ret();
      }

      if ( opts && ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( opts );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_BUCKET_DESTROY )
{
   PHB_MONGOC bucket = hbmongoc_param( 1, _hbmongoc_gridfs_bucket_t_ );

   if ( bucket )
   {
      mongoc_gridfs_bucket_destroy( ( mongoc_gridfs_bucket_t * ) bucket->p );
      bucket->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_BUCKET_FIND )
{
   mongoc_gridfs_bucket_t * bucket = mongoc_hbparam( 1, _hbmongoc_gridfs_bucket_t_ );

   if ( bucket )
   {
      bson_t * filter = bson_hbparam( 2, HB_IT_ANY );
      bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

      mongoc_cursor_t * cursor = mongoc_gridfs_bucket_find( bucket, filter, opts );

      if ( cursor )
      {
         PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );
         hb_retptrGC( phCursor );
      }
      else
      {
         hb_ret();
      }

      if ( filter && ! HB_ISPOINTER( 2 ) )
      {
         bson_destroy( filter );
      }

      if ( opts && ! HB_ISPOINTER( 3 ) )
      {
         bson_destroy( opts );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_BUCKET_DELETE_BY_ID )
{
   mongoc_gridfs_bucket_t * bucket = mongoc_hbparam( 1, _hbmongoc_gridfs_bucket_t_ );
   const bson_value_t * file_id = bson_value_hbparam( 2 );

   if ( bucket && file_id )
   {
      bson_error_t error;
      bool result = mongoc_gridfs_bucket_delete_by_id( bucket, file_id, &error );

      bson_hbstor_byref_error( 3, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_BUCKET_OPEN_DOWNLOAD_STREAM )
{
   mongoc_gridfs_bucket_t * bucket = mongoc_hbparam( 1, _hbmongoc_gridfs_bucket_t_ );
   const bson_value_t * file_id = bson_value_hbparam( 2 );

   if ( bucket && file_id )
   {
      bson_error_t error;
      mongoc_stream_t * stream = mongoc_gridfs_bucket_open_download_stream( bucket, file_id, &error );

      bson_hbstor_byref_error( 3, &error, stream != NULL );

      if ( stream )
      {
         PHB_MONGOC phStream = hbmongoc_new_dataContainer( _hbmongoc_stream_t_, stream );
         hb_retptrGC( phStream );
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

HB_FUNC( MONGOC_GRIDFS_BUCKET_OPEN_UPLOAD_STREAM )
{
   mongoc_gridfs_bucket_t * bucket = mongoc_hbparam( 1, _hbmongoc_gridfs_bucket_t_ );
   const char * filename = hb_parc( 2 );

   if ( bucket && filename )
   {
      bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
      bson_value_t * file_id = bson_value_hbparam( 4 );
      bson_error_t error;

      mongoc_stream_t * stream = mongoc_gridfs_bucket_open_upload_stream( bucket, filename, opts, file_id, &error );

      bson_hbstor_byref_error( 5, &error, stream != NULL );

      if ( stream )
      {
         PHB_MONGOC phStream = hbmongoc_new_dataContainer( _hbmongoc_stream_t_, stream );
         hb_retptrGC( phStream );
      }
      else
      {
         hb_ret();
      }

      if ( opts && ! HB_ISPOINTER( 3 ) )
      {
         bson_destroy( opts );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_BUCKET_OPEN_UPLOAD_STREAM_WITH_ID )
{
   mongoc_gridfs_bucket_t * bucket = mongoc_hbparam( 1, _hbmongoc_gridfs_bucket_t_ );
   const bson_value_t * file_id = bson_value_hbparam( 2 );
   const char * filename = hb_parc( 3 );

   if ( bucket && file_id && filename )
   {
      bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
      bson_error_t error;

      mongoc_stream_t * stream = mongoc_gridfs_bucket_open_upload_stream_with_id(
                                    bucket, file_id, filename, opts, &error );

      bson_hbstor_byref_error( 5, &error, stream != NULL );

      if ( stream )
      {
         PHB_MONGOC phStream = hbmongoc_new_dataContainer( _hbmongoc_stream_t_, stream );
         hb_retptrGC( phStream );
      }
      else
      {
         hb_ret();
      }

      if ( opts && ! HB_ISPOINTER( 4 ) )
      {
         bson_destroy( opts );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_BUCKET_DOWNLOAD_TO_STREAM )
{
   mongoc_gridfs_bucket_t * bucket = mongoc_hbparam( 1, _hbmongoc_gridfs_bucket_t_ );
   const bson_value_t * file_id = bson_value_hbparam( 2 );
   mongoc_stream_t * destination = mongoc_hbparam( 3, _hbmongoc_stream_t_ );

   if ( bucket && file_id && destination )
   {
      bson_error_t error;
      bool result = mongoc_gridfs_bucket_download_to_stream( bucket, file_id, destination, &error );

      bson_hbstor_byref_error( 4, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_BUCKET_UPLOAD_FROM_STREAM )
{
   mongoc_gridfs_bucket_t * bucket = mongoc_hbparam( 1, _hbmongoc_gridfs_bucket_t_ );
   const char * filename = hb_parc( 2 );
   mongoc_stream_t * source = mongoc_hbparam( 3, _hbmongoc_stream_t_ );

   if ( bucket && filename && source )
   {
      bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
      bson_value_t * file_id_out = bson_value_hbparam( 5 );
      bson_error_t error;

      bool result = mongoc_gridfs_bucket_upload_from_stream(
                       bucket, filename, source, opts, file_id_out, &error );

      bson_hbstor_byref_error( 6, &error, result );
      hb_retl( result );

      if ( opts && ! HB_ISPOINTER( 4 ) )
      {
         bson_destroy( opts );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_BUCKET_UPLOAD_FROM_STREAM_WITH_ID )
{
   mongoc_gridfs_bucket_t * bucket = mongoc_hbparam( 1, _hbmongoc_gridfs_bucket_t_ );
   const bson_value_t * file_id = bson_value_hbparam( 2 );
   const char * filename = hb_parc( 3 );
   mongoc_stream_t * source = mongoc_hbparam( 4, _hbmongoc_stream_t_ );

   if ( bucket && file_id && filename && source )
   {
      bson_t * opts = bson_hbparam( 5, HB_IT_ANY );
      bson_error_t error;

      bool result = mongoc_gridfs_bucket_upload_from_stream_with_id(
                       bucket, file_id, filename, source, opts, &error );

      bson_hbstor_byref_error( 6, &error, result );
      hb_retl( result );

      if ( opts && ! HB_ISPOINTER( 5 ) )
      {
         bson_destroy( opts );
      }
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_BUCKET_ABORT_UPLOAD )
{
   mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );

   if ( stream )
   {
      hb_retl( mongoc_gridfs_bucket_abort_upload( stream ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_GRIDFS_BUCKET_STREAM_ERROR )
{
   mongoc_stream_t * stream = mongoc_hbparam( 1, _hbmongoc_stream_t_ );

   if ( stream )
   {
      bson_error_t error;
      bool result = mongoc_gridfs_bucket_stream_error( stream, &error );

      bson_hbstor_byref_error( 2, &error, result );
      hb_retl( result );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}
