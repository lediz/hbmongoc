//
//  hb_bson.h
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#ifndef hb_bson_h
#define hb_bson_h

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbapifs.h"
#include "hbapistr.h"
#include "hbstack.h"
#include "hbvm.h"

#include <bson.h>

#if ! BSON_CHECK_VERSION( 2, 0, 0 )
#error "hbmongoc targets mongo-c-driver / libbson 2.x (2.0.0 or later)."
#endif

#define HBBSON_ERR_ARGS()  ( hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS ) )
#define HBBSON_ERR_NOFUNC()  ( hb_errRT_BASE_SubstR( EG_NOFUNC, 1001, "Undefined function", HB_ERR_FUNCNAME, 0 ) )

typedef enum
{
    _hbbson_t_,
    _hbbson_oid_t_,
    _hbbson_iter_t_,
    _hbbson_context_t_,
    _hbbson_decimal128_t_,
    _hbbson_value_t_,
    _hbbson_reader_t_,
    _hbbson_json_opts_t_,
    _hbbson_json_reader_t_,
    _hbbson_writer_t_,
    _hbbson_array_builder_t_
} hbbson_t_;

typedef struct _HB_BSON_
{
    hbbson_t_ hbbson_type;
    void * p;
} HB_BSON, * PHB_BSON;

bson_context_t *    bson_context_hbparam( int iParam );
bson_decimal128_t * bson_decimal128_hbparam( int iParam );
bson_t *            bson_hbparam( int iParam, long lMask );
void                bson_hbstor_byref_error( int iParam, bson_error_t * error, HB_BOOL valid );
bson_iter_t *       bson_iter_hbparam( int iParam );
bson_oid_t *        bson_oid_hbparam( int iParam );
bson_value_t *      bson_value_hbparam( int iParam );
bson_reader_t *     bson_reader_hbparam( int iParam );
bson_json_opts_t *  bson_json_opts_hbparam( int iParam );
bson_json_reader_t *bson_json_reader_hbparam( int iParam );
bson_writer_t *     bson_writer_hbparam( int iParam );
bson_array_builder_t * bson_array_builder_hbparam( int iParam );
bson_t *            get_bson_item(PHB_ITEM pItem);
char *              hbbson_as_json( const bson_t * bson );
PHB_BSON            hbbson_new_dataContainer( hbbson_t_ hbbson_type, void * p );
PHB_BSON            hbbson_param( int iParam, hbbson_t_ hbbson_type );
PHB_BSON            hbbson_hbparam( PHB_ITEM pItem, hbbson_t_ hbbson_type );
HB_LONGLONG         hb_dtToUnix(double dTimeStamp);

#endif /* hb_bson_h */
