//
//  hb_mongoc_apm.c
//  hbmongoc
//
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc.h"

/* the APM code-block slots are declared in hb_mongoc.h (via hb_apm_slot())
   and defined exactly once here, shared by all translation units */
static PHB_ITEM s_hb_apm_slots[ 12 ] = { NULL };

PHB_ITEM * hb_apm_slot( int iSlot )
{
   return ( iSlot >= 0 && iSlot < ( int ) sizeof(s_hb_apm_slots)/sizeof(PHB_ITEM *) )
           ? & s_hb_apm_slots[ iSlot ]
           : NULL;
}

void hb_apm_store( PHB_ITEM * pSlot, PHB_ITEM pBlock )
{
   if ( pSlot )
   {
      if ( *pSlot )
      {
         hb_itemRelease( *pSlot );
      }

      *pSlot = hb_itemNew( pBlock );
   }
}

/* --------------------------------------------------------------------
 * APM callback shims.
 *
 * mongoc's APM callbacks are C function pointers; Harbour code supplies a
 * code block instead. Each shim invokes that block with the event pointer
 * as its single argument, so the MONGOC_APM_*_GET_* accessors below accept
 * PARAM(1) inside the block.
 *
 * The event pointer is valid only for the duration of the callback
 * (mongoc's contract) - do not retain it.
 *
 * NOTE: the callback runs on whichever thread mongoc emits it from.
 *       Harbour is not thread-safe; see hb_mongoc.h's HB_APM_SAFE flag.
 * ------------------------------------------------------------------ */

static void hbmongoc_apm_invoke( PHB_ITEM * pBlock, const void * event )
{
   if ( pBlock && *pBlock )
   {
      PHB_ITEM pEvent = hb_itemNew( NULL );
      hb_itemPutPtr( pEvent, (void *) event );
      hb_itemRelease( hb_vmEvalBlockV( *pBlock, 1, pEvent ) );
   }
}

static void mongoc_apm_cb_command_started(const mongoc_apm_command_started_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_COMMAND_STARTED ), event );
}

static void mongoc_apm_cb_command_succeeded(const mongoc_apm_command_succeeded_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_COMMAND_SUCCEEDED ), event );
}

static void mongoc_apm_cb_command_failed(const mongoc_apm_command_failed_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_COMMAND_FAILED ), event );
}

static void mongoc_apm_cb_server_changed(const mongoc_apm_server_changed_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_SERVER_CHANGED ), event );
}

static void mongoc_apm_cb_server_opening(const mongoc_apm_server_opening_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_SERVER_OPENING ), event );
}

static void mongoc_apm_cb_server_closed(const mongoc_apm_server_closed_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_SERVER_CLOSED ), event );
}

static void mongoc_apm_cb_topology_changed(const mongoc_apm_topology_changed_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_TOPOLOGY_CHANGED ), event );
}

static void mongoc_apm_cb_topology_opening(const mongoc_apm_topology_opening_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_TOPOLOGY_OPENING ), event );
}

static void mongoc_apm_cb_topology_closed(const mongoc_apm_topology_closed_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_TOPOLOGY_CLOSED ), event );
}

static void mongoc_apm_cb_server_heartbeat_started(const mongoc_apm_server_heartbeat_started_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_SERVER_HEARTBEAT_STARTED ), event );
}

static void mongoc_apm_cb_server_heartbeat_succeeded(const mongoc_apm_server_heartbeat_succeeded_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_SERVER_HEARTBEAT_SUCCEEDED ), event );
}

static void mongoc_apm_cb_server_heartbeat_failed(const mongoc_apm_server_heartbeat_failed_t *event)
{
   hbmongoc_apm_invoke( hb_apm_slot( HB_APM_SLOT_SERVER_HEARTBEAT_FAILED ), event );
}

/* --------------------------------------------------------------------
 * helpers (file-local)
 * ------------------------------------------------------------------ */

static PHB_ITEM hbmongoc_apm_new_host_hash( const mongoc_host_list_t * host )
{
   if ( host )
   {
      PHB_ITEM pItemHash = hb_itemNew( NULL );
      hb_hashNew( pItemHash );

      PHB_ITEM pItemKey = hb_itemNew( NULL );
      PHB_ITEM pItemValue = hb_itemNew( NULL );

      hb_itemPutC( pItemKey, "host" );
      hb_itemPutC( pItemValue, host->host );
      hb_hashAdd( pItemHash, pItemKey, pItemValue );

      hb_itemPutC( pItemKey, "hostAndPort" );
      hb_itemPutC( pItemValue, host->host_and_port );
      hb_hashAdd( pItemHash, pItemKey, pItemValue );

      hb_itemPutC( pItemKey, "port" );
      hb_itemPutNI( pItemValue, host->port );
      hb_hashAdd( pItemHash, pItemKey, pItemValue );

      hb_itemPutC( pItemKey, "family" );
      hb_itemPutNI( pItemValue, host->family );
      hb_hashAdd( pItemHash, pItemKey, pItemValue );

      hb_itemRelease( pItemKey );
      hb_itemRelease( pItemValue );

      return pItemHash;
   }

   return NULL;
}

/* --------------------------------------------------------------------
 * callbacks object
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_CALLBACKS_NEW )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_apm_callbacks_new();

   if ( callbacks )
   {
      PHB_MONGOC phCallbacks = hbmongoc_new_dataContainer( _hbmongoc_apm_callbacks_t_, callbacks );
      hb_retptrGC( phCallbacks );
   }
   else
   {
      hb_ret();
   }
}

HB_FUNC( MONGOC_APM_CALLBACKS_DESTROY )
{
   PHB_MONGOC callbacks = hbmongoc_param( 1, _hbmongoc_apm_callbacks_t_ );

   if ( callbacks )
   {
      mongoc_apm_callbacks_destroy( callbacks->p );
      callbacks->p = NULL;
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * callback registration
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_SET_COMMAND_STARTED_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_COMMAND_STARTED ), pBlock );
      mongoc_apm_set_command_started_cb( callbacks, mongoc_apm_cb_command_started );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_COMMAND_SUCCEEDED_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_COMMAND_SUCCEEDED ), pBlock );
      mongoc_apm_set_command_succeeded_cb( callbacks, mongoc_apm_cb_command_succeeded );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_COMMAND_FAILED_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_COMMAND_FAILED ), pBlock );
      mongoc_apm_set_command_failed_cb( callbacks, mongoc_apm_cb_command_failed );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_SERVER_CHANGED_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_SERVER_CHANGED ), pBlock );
      mongoc_apm_set_server_changed_cb( callbacks, mongoc_apm_cb_server_changed );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_SERVER_OPENING_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_SERVER_OPENING ), pBlock );
      mongoc_apm_set_server_opening_cb( callbacks, mongoc_apm_cb_server_opening );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_SERVER_CLOSED_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_SERVER_CLOSED ), pBlock );
      mongoc_apm_set_server_closed_cb( callbacks, mongoc_apm_cb_server_closed );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_TOPOLOGY_CHANGED_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_TOPOLOGY_CHANGED ), pBlock );
      mongoc_apm_set_topology_changed_cb( callbacks, mongoc_apm_cb_topology_changed );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_TOPOLOGY_OPENING_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_TOPOLOGY_OPENING ), pBlock );
      mongoc_apm_set_topology_opening_cb( callbacks, mongoc_apm_cb_topology_opening );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_TOPOLOGY_CLOSED_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_TOPOLOGY_CLOSED ), pBlock );
      mongoc_apm_set_topology_closed_cb( callbacks, mongoc_apm_cb_topology_closed );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_SERVER_HEARTBEAT_STARTED_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_SERVER_HEARTBEAT_STARTED ), pBlock );
      mongoc_apm_set_server_heartbeat_started_cb( callbacks, mongoc_apm_cb_server_heartbeat_started );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_SERVER_HEARTBEAT_SUCCEEDED_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_SERVER_HEARTBEAT_SUCCEEDED ), pBlock );
      mongoc_apm_set_server_heartbeat_succeeded_cb( callbacks, mongoc_apm_cb_server_heartbeat_succeeded );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SET_SERVER_HEARTBEAT_FAILED_CB )
{
   mongoc_apm_callbacks_t * callbacks = mongoc_hbparam( 1, _hbmongoc_apm_callbacks_t_ );
   PHB_ITEM pBlock = hb_param( 2, HB_IT_BLOCK );

   if ( callbacks && pBlock )
   {
      hb_apm_store( hb_apm_slot( HB_APM_SLOT_SERVER_HEARTBEAT_FAILED ), pBlock );
      mongoc_apm_set_server_heartbeat_failed_cb( callbacks, mongoc_apm_cb_server_heartbeat_failed );
      hb_retl( true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * command_started event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_COMMAND_STARTED_GET_COMMAND )
{
   const mongoc_apm_command_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_started_t_ );

   if ( event )
   {
      const bson_t * command = mongoc_apm_command_started_get_command( event );

      if ( command )
      {
         PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( command ) );
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

HB_FUNC( MONGOC_APM_COMMAND_STARTED_GET_DATABASE_NAME )
{
   const mongoc_apm_command_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_started_t_ );

   if ( event )
   {
      hb_retc( mongoc_apm_command_started_get_database_name( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_STARTED_GET_COMMAND_NAME )
{
   const mongoc_apm_command_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_started_t_ );

   if ( event )
   {
      hb_retc( mongoc_apm_command_started_get_command_name( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_STARTED_GET_REQUEST_ID )
{
   const mongoc_apm_command_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_started_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_started_get_request_id( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_STARTED_GET_OPERATION_ID )
{
   const mongoc_apm_command_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_started_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_started_get_operation_id( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_STARTED_GET_HOST )
{
   const mongoc_apm_command_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_started_t_ );

   if ( event )
   {
      PHB_ITEM pItemHash = hbmongoc_apm_new_host_hash( mongoc_apm_command_started_get_host( event ) );

      if ( pItemHash )
      {
         hb_itemReturnRelease( pItemHash );
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

HB_FUNC( MONGOC_APM_COMMAND_STARTED_GET_SERVER_ID )
{
   const mongoc_apm_command_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_started_t_ );

   if ( event )
   {
      hb_retni( ( int ) mongoc_apm_command_started_get_server_id( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_STARTED_GET_SERVICE_ID )
{
   const mongoc_apm_command_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_started_t_ );

   if ( event )
   {
      const bson_oid_t * service_id = mongoc_apm_command_started_get_service_id( event );

      if ( service_id )
      {
         char szOid[25];
         bson_oid_to_string( service_id, szOid );
         hb_retc( szOid );
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

HB_FUNC( MONGOC_APM_COMMAND_STARTED_GET_SERVER_CONNECTION_ID_INT64 )
{
   const mongoc_apm_command_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_started_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_started_get_server_connection_id_int64( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_STARTED_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_command_started_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_command_started_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * command_succeeded event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_DURATION )
{
   const mongoc_apm_command_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_succeeded_get_duration( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_REPLY )
{
   const mongoc_apm_command_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      const bson_t * reply = mongoc_apm_command_succeeded_get_reply( event );

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

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_COMMAND_NAME )
{
   const mongoc_apm_command_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      hb_retc( mongoc_apm_command_succeeded_get_command_name( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_DATABASE_NAME )
{
   const mongoc_apm_command_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      hb_retc( mongoc_apm_command_succeeded_get_database_name( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_REQUEST_ID )
{
   const mongoc_apm_command_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_succeeded_get_request_id( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_OPERATION_ID )
{
   const mongoc_apm_command_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_succeeded_get_operation_id( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_HOST )
{
   const mongoc_apm_command_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      PHB_ITEM pItemHash = hbmongoc_apm_new_host_hash( mongoc_apm_command_succeeded_get_host( event ) );

      if ( pItemHash )
      {
         hb_itemReturnRelease( pItemHash );
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

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_SERVER_ID )
{
   const mongoc_apm_command_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      hb_retni( ( int ) mongoc_apm_command_succeeded_get_server_id( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_SERVICE_ID )
{
   const mongoc_apm_command_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      const bson_oid_t * service_id = mongoc_apm_command_succeeded_get_service_id( event );

      if ( service_id )
      {
         char szOid[25];
         bson_oid_to_string( service_id, szOid );
         hb_retc( szOid );
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

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_SERVER_CONNECTION_ID_INT64 )
{
   const mongoc_apm_command_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_succeeded_get_server_connection_id_int64( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_SUCCEEDED_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_command_succeeded_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_command_succeeded_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * command_failed event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_DURATION )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_failed_get_duration( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_COMMAND_NAME )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      hb_retc( mongoc_apm_command_failed_get_command_name( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_DATABASE_NAME )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      hb_retc( mongoc_apm_command_failed_get_database_name( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_ERROR )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      bson_error_t error;
      memset( &error, 0, sizeof(bson_error_t) );
      mongoc_apm_command_failed_get_error( event, &error );
      bson_hbstor_byref_error( 2, &error, true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_REPLY )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      const bson_t * reply = mongoc_apm_command_failed_get_reply( event );

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

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_REQUEST_ID )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_failed_get_request_id( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_OPERATION_ID )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_failed_get_operation_id( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_HOST )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      PHB_ITEM pItemHash = hbmongoc_apm_new_host_hash( mongoc_apm_command_failed_get_host( event ) );

      if ( pItemHash )
      {
         hb_itemReturnRelease( pItemHash );
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

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_SERVER_ID )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      hb_retni( ( int ) mongoc_apm_command_failed_get_server_id( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_SERVICE_ID )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      const bson_oid_t * service_id = mongoc_apm_command_failed_get_service_id( event );

      if ( service_id )
      {
         char szOid[25];
         bson_oid_to_string( service_id, szOid );
         hb_retc( szOid );
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

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_SERVER_CONNECTION_ID_INT64 )
{
   const mongoc_apm_command_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_command_failed_get_server_connection_id_int64( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_COMMAND_FAILED_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_command_failed_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_command_failed_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * server_changed event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_SERVER_CHANGED_GET_HOST )
{
   const mongoc_apm_server_changed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_changed_t_ );

   if ( event )
   {
      PHB_ITEM pItemHash = hbmongoc_apm_new_host_hash( mongoc_apm_server_changed_get_host( event ) );

      if ( pItemHash )
      {
         hb_itemReturnRelease( pItemHash );
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

HB_FUNC( MONGOC_APM_SERVER_CHANGED_GET_TOPOLOGY_ID )
{
   const mongoc_apm_server_changed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_changed_t_ );

   if ( event )
   {
      bson_oid_t topology_id;
      memset( &topology_id, 0, sizeof(bson_oid_t) );
      mongoc_apm_server_changed_get_topology_id( event, &topology_id );

      char szOid[25];
      bson_oid_to_string( &topology_id, szOid );
      hb_retc( szOid );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SERVER_CHANGED_GET_PREVIOUS_DESCRIPTION )
{
   const mongoc_apm_server_changed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_changed_t_ );

   if ( event )
   {
      const mongoc_server_description_t * description = mongoc_apm_server_changed_get_previous_description( event );

      if ( description )
      {
         mongoc_server_description_t * copy = mongoc_server_description_new_copy( description );

         if ( copy )
         {
            PHB_MONGOC phDescription = hbmongoc_new_dataContainer( _hbmongoc_server_description_t_, copy );
            hb_retptrGC( phDescription );
         }
         else
         {
            hb_ret();
         }
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

HB_FUNC( MONGOC_APM_SERVER_CHANGED_GET_NEW_DESCRIPTION )
{
   const mongoc_apm_server_changed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_changed_t_ );

   if ( event )
   {
      const mongoc_server_description_t * description = mongoc_apm_server_changed_get_new_description( event );

      if ( description )
      {
         mongoc_server_description_t * copy = mongoc_server_description_new_copy( description );

         if ( copy )
         {
            PHB_MONGOC phDescription = hbmongoc_new_dataContainer( _hbmongoc_server_description_t_, copy );
            hb_retptrGC( phDescription );
         }
         else
         {
            hb_ret();
         }
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

HB_FUNC( MONGOC_APM_SERVER_CHANGED_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_server_changed_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_server_changed_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * server_opening event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_SERVER_OPENING_GET_HOST )
{
   const mongoc_apm_server_opening_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_opening_t_ );

   if ( event )
   {
      PHB_ITEM pItemHash = hbmongoc_apm_new_host_hash( mongoc_apm_server_opening_get_host( event ) );

      if ( pItemHash )
      {
         hb_itemReturnRelease( pItemHash );
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

HB_FUNC( MONGOC_APM_SERVER_OPENING_GET_TOPOLOGY_ID )
{
   const mongoc_apm_server_opening_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_opening_t_ );

   if ( event )
   {
      bson_oid_t topology_id;
      memset( &topology_id, 0, sizeof(bson_oid_t) );
      mongoc_apm_server_opening_get_topology_id( event, &topology_id );

      char szOid[25];
      bson_oid_to_string( &topology_id, szOid );
      hb_retc( szOid );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SERVER_OPENING_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_server_opening_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_server_opening_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * server_closed event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_SERVER_CLOSED_GET_HOST )
{
   const mongoc_apm_server_closed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_closed_t_ );

   if ( event )
   {
      PHB_ITEM pItemHash = hbmongoc_apm_new_host_hash( mongoc_apm_server_closed_get_host( event ) );

      if ( pItemHash )
      {
         hb_itemReturnRelease( pItemHash );
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

HB_FUNC( MONGOC_APM_SERVER_CLOSED_GET_TOPOLOGY_ID )
{
   const mongoc_apm_server_closed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_closed_t_ );

   if ( event )
   {
      bson_oid_t topology_id;
      memset( &topology_id, 0, sizeof(bson_oid_t) );
      mongoc_apm_server_closed_get_topology_id( event, &topology_id );

      char szOid[25];
      bson_oid_to_string( &topology_id, szOid );
      hb_retc( szOid );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SERVER_CLOSED_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_server_closed_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_server_closed_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * topology_changed event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_TOPOLOGY_CHANGED_GET_TOPOLOGY_ID )
{
   const mongoc_apm_topology_changed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_topology_changed_t_ );

   if ( event )
   {
      bson_oid_t topology_id;
      memset( &topology_id, 0, sizeof(bson_oid_t) );
      mongoc_apm_topology_changed_get_topology_id( event, &topology_id );

      char szOid[25];
      bson_oid_to_string( &topology_id, szOid );
      hb_retc( szOid );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_TOPOLOGY_CHANGED_GET_PREVIOUS_DESCRIPTION )
{
   const mongoc_apm_topology_changed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_topology_changed_t_ );

   if ( event )
   {
      const mongoc_topology_description_t * description = mongoc_apm_topology_changed_get_previous_description( event );

      if ( description )
      {
         mongoc_topology_description_t * copy = mongoc_topology_description_new_copy( description );

         if ( copy )
         {
            PHB_MONGOC phDescription = hbmongoc_new_dataContainer( _hbmongoc_topology_description_t_, copy );
            hb_retptrGC( phDescription );
         }
         else
         {
            hb_ret();
         }
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

HB_FUNC( MONGOC_APM_TOPOLOGY_CHANGED_GET_NEW_DESCRIPTION )
{
   const mongoc_apm_topology_changed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_topology_changed_t_ );

   if ( event )
   {
      const mongoc_topology_description_t * description = mongoc_apm_topology_changed_get_new_description( event );

      if ( description )
      {
         mongoc_topology_description_t * copy = mongoc_topology_description_new_copy( description );

         if ( copy )
         {
            PHB_MONGOC phDescription = hbmongoc_new_dataContainer( _hbmongoc_topology_description_t_, copy );
            hb_retptrGC( phDescription );
         }
         else
         {
            hb_ret();
         }
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

HB_FUNC( MONGOC_APM_TOPOLOGY_CHANGED_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_topology_changed_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_topology_changed_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * topology_opening event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_TOPOLOGY_OPENING_GET_TOPOLOGY_ID )
{
   const mongoc_apm_topology_opening_t * event = mongoc_hbparam( 1, _hbmongoc_apm_topology_opening_t_ );

   if ( event )
   {
      bson_oid_t topology_id;
      memset( &topology_id, 0, sizeof(bson_oid_t) );
      mongoc_apm_topology_opening_get_topology_id( event, &topology_id );

      char szOid[25];
      bson_oid_to_string( &topology_id, szOid );
      hb_retc( szOid );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_TOPOLOGY_OPENING_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_topology_opening_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_topology_opening_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * topology_closed event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_TOPOLOGY_CLOSED_GET_TOPOLOGY_ID )
{
   const mongoc_apm_topology_closed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_topology_closed_t_ );

   if ( event )
   {
      bson_oid_t topology_id;
      memset( &topology_id, 0, sizeof(bson_oid_t) );
      mongoc_apm_topology_closed_get_topology_id( event, &topology_id );

      char szOid[25];
      bson_oid_to_string( &topology_id, szOid );
      hb_retc( szOid );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_TOPOLOGY_CLOSED_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_topology_closed_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_topology_closed_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * server_heartbeat_started event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_STARTED_GET_HOST )
{
   const mongoc_apm_server_heartbeat_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_heartbeat_started_t_ );

   if ( event )
   {
      PHB_ITEM pItemHash = hbmongoc_apm_new_host_hash( mongoc_apm_server_heartbeat_started_get_host( event ) );

      if ( pItemHash )
      {
         hb_itemReturnRelease( pItemHash );
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

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_STARTED_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_server_heartbeat_started_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_server_heartbeat_started_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_STARTED_GET_AWAITED )
{
   const mongoc_apm_server_heartbeat_started_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_heartbeat_started_t_ );

   if ( event )
   {
      hb_retl( mongoc_apm_server_heartbeat_started_get_awaited( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * server_heartbeat_succeeded event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_SUCCEEDED_GET_DURATION )
{
   const mongoc_apm_server_heartbeat_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_heartbeat_succeeded_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_server_heartbeat_succeeded_get_duration( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_SUCCEEDED_GET_REPLY )
{
   const mongoc_apm_server_heartbeat_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_heartbeat_succeeded_t_ );

   if ( event )
   {
      const bson_t * reply = mongoc_apm_server_heartbeat_succeeded_get_reply( event );

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

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_SUCCEEDED_GET_HOST )
{
   const mongoc_apm_server_heartbeat_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_heartbeat_succeeded_t_ );

   if ( event )
   {
      PHB_ITEM pItemHash = hbmongoc_apm_new_host_hash( mongoc_apm_server_heartbeat_succeeded_get_host( event ) );

      if ( pItemHash )
      {
         hb_itemReturnRelease( pItemHash );
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

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_SUCCEEDED_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_server_heartbeat_succeeded_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_server_heartbeat_succeeded_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_SUCCEEDED_GET_AWAITED )
{
   const mongoc_apm_server_heartbeat_succeeded_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_heartbeat_succeeded_t_ );

   if ( event )
   {
      hb_retl( mongoc_apm_server_heartbeat_succeeded_get_awaited( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

/* --------------------------------------------------------------------
 * server_heartbeat_failed event
 * ------------------------------------------------------------------ */

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_FAILED_GET_DURATION )
{
   const mongoc_apm_server_heartbeat_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_heartbeat_failed_t_ );

   if ( event )
   {
      hb_retnll( (HB_LONGLONG) mongoc_apm_server_heartbeat_failed_get_duration( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_FAILED_GET_ERROR )
{
   const mongoc_apm_server_heartbeat_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_heartbeat_failed_t_ );

   if ( event )
   {
      bson_error_t error;
      memset( &error, 0, sizeof(bson_error_t) );
      mongoc_apm_server_heartbeat_failed_get_error( event, &error );
      bson_hbstor_byref_error( 2, &error, true );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_FAILED_GET_HOST )
{
   const mongoc_apm_server_heartbeat_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_heartbeat_failed_t_ );

   if ( event )
   {
      PHB_ITEM pItemHash = hbmongoc_apm_new_host_hash( mongoc_apm_server_heartbeat_failed_get_host( event ) );

      if ( pItemHash )
      {
         hb_itemReturnRelease( pItemHash );
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

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_FAILED_GET_CONTEXT )
{
   PHB_MONGOC event = hbmongoc_param( 1, _hbmongoc_apm_server_heartbeat_failed_t_ );

   if ( event )
   {
      hb_retptr( hbmongoc_new_dataContainer( _hbmongoc_apm_context_t_, mongoc_apm_server_heartbeat_failed_get_context( event->p ) ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}

HB_FUNC( MONGOC_APM_SERVER_HEARTBEAT_FAILED_GET_AWAITED )
{
   const mongoc_apm_server_heartbeat_failed_t * event = mongoc_hbparam( 1, _hbmongoc_apm_server_heartbeat_failed_t_ );

   if ( event )
   {
      hb_retl( mongoc_apm_server_heartbeat_failed_get_awaited( event ) );
   }
   else
   {
      HBMONGOC_ERR_ARGS();
   }
}
