//
//  hb_mongoc.h
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/25/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#ifndef hb_mongoc_h
#define hb_mongoc_h

#include "hb_bson.h"

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbapifs.h"
#include "hbapistr.h"
#include "hbstack.h"
#include "hbvm.h"

#include <mongoc.h>
#include <bson.h>

#if ! MONGOC_CHECK_VERSION( 2, 0, 0 )
#error "hbmongoc targets mongo-c-driver / libbson 2.x (2.0.0 or later)."
#endif

typedef enum
{
    _hbmongoc_client_t_,
    _hbmongoc_database_t_,
    _hbmongoc_collection_t_,
    _hbmongoc_uri_t_,
    _hbmongoc_cursor_t_,
    _hbmongoc_write_concern_t_,
    _hbmongoc_read_prefs_t_,
    _hbmongoc_bulk_operation_t_,
    _hbmongoc_read_concern_t_,
    _hbmongoc_client_session_t_,
    _hbmongoc_transaction_opts_t_,
    _hbmongoc_session_opts_t_,
    _hbmongoc_find_and_modify_opts_t_,
    _hbmongoc_change_stream_t_,
    _hbmongoc_server_description_t_,
    _hbmongoc_server_api_t_,
    _hbmongoc_index_model_t_,
    _hbmongoc_topology_description_t_,
    _hbmongoc_apm_callbacks_t_,
    _hbmongoc_apm_context_t_,
    _hbmongoc_apm_command_started_t_,
    _hbmongoc_apm_command_succeeded_t_,
    _hbmongoc_apm_command_failed_t_,
    _hbmongoc_apm_server_changed_t_,
    _hbmongoc_apm_server_opening_t_,
    _hbmongoc_apm_server_closed_t_,
    _hbmongoc_apm_topology_changed_t_,
    _hbmongoc_apm_topology_opening_t_,
    _hbmongoc_apm_topology_closed_t_,
    _hbmongoc_apm_server_heartbeat_started_t_,
    _hbmongoc_apm_server_heartbeat_succeeded_t_,
    _hbmongoc_apm_server_heartbeat_failed_t_
} hbmongoc_t_;

typedef struct _HB_MONGOC_
{
    hbmongoc_t_ type;
    void * p;

} HB_MONGOC, * PHB_MONGOC;

PHB_MONGOC  hbmongoc_param( int iParam, hbmongoc_t_ type );
PHB_MONGOC  hbmongoc_hbparam( PHB_ITEM pItem, hbmongoc_t_ type );
PHB_MONGOC  hbmongoc_new_dataContainer( hbmongoc_t_ type, void * p );
PHB_BSON    hbmongoc_return_byref_bson( int iParam, bson_t * bson );
void        hbmongoc_stor_byref_value( int iParam, const bson_value_t * value );
void *      mongoc_hbparam( int iParam, hbmongoc_t_ type );
void *      mongoc_hbparam_any( int iParam );

/* APM callback code blocks kept alive so mongoc can invoke them.
   A NULL slot means the corresponding mongoc callback is never fired.
   Accessed through these accessors because the single instance lives in
   hb_mongoc_apm.c - declaring it in a header would emit a definition in
   every translation unit. */
PHB_ITEM *  hb_apm_slot( int iSlot );

/* iSlot values */
#define HB_APM_SLOT_COMMAND_STARTED            0
#define HB_APM_SLOT_COMMAND_SUCCEEDED          1
#define HB_APM_SLOT_COMMAND_FAILED             2
#define HB_APM_SLOT_SERVER_CHANGED             3
#define HB_APM_SLOT_SERVER_OPENING             4
#define HB_APM_SLOT_SERVER_CLOSED              5
#define HB_APM_SLOT_TOPOLOGY_CHANGED           6
#define HB_APM_SLOT_TOPOLOGY_OPENING           7
#define HB_APM_SLOT_TOPOLOGY_CLOSED            8
#define HB_APM_SLOT_SERVER_HEARTBEAT_STARTED   9
#define HB_APM_SLOT_SERVER_HEARTBEAT_SUCCEEDED 10
#define HB_APM_SLOT_SERVER_HEARTBEAT_FAILED    11

/* store a code block into one of the slots above, releasing any previous
   block so the Harbour refcount stays balanced */
void        hb_apm_store( PHB_ITEM * pSlot, PHB_ITEM pBlock );

#define HBMONGOC_ERR_ARGS()  ( hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS ) )
#define HBMONGOC_ERR_NOFUNC()  ( hb_errRT_BASE_SubstR( EG_NOFUNC, 1001, "Undefined function", HB_ERR_FUNCNAME, 0 ) )
#define HBMONGOC_ERR_DEPRECATEDFUNC() ( hb_errRT_BASE_SubstR( EG_NOFUNC, 1001, "Deprecated function", HB_ERR_FUNCNAME, 0 ) )

#endif /* hb_mongoc_h */
