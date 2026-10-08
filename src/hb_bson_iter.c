//
//  hb_bson_iter.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/17/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_bson_iter.h"

#include "hb_bson.h"

HB_FUNC( BSON_ITER_ARRAY )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_ARRAY( iter ) ) {

        uint32_t array_len;
        const uint8_t * array;
        bson_iter_array( iter, &array_len, &array );
        bson_t *doc = bson_new_from_data(array, array_len);

        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( array_len, 2 );
        }

        if ( HB_ISBYREF( 3 ) ) {
            hb_storclen( ( const char * ) array, array_len, 3 );
        }

        if (doc) {
            PHB_BSON phBson = hbbson_new_dataContainer(_hbbson_t_, doc);
            hb_retptrGC(phBson);
        } else {
            hb_ret();
        }

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_AS_BOOL )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        bool result = bson_iter_as_bool( iter );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_AS_DOUBLE )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        double result = bson_iter_as_double( iter );
        hb_retnd( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_AS_INT64 )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        HB_LONGLONG result = bson_iter_as_int64( iter );
        hb_retnll( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_BINARY )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_BINARY( iter ) ) {
        bson_subtype_t subtype;
        uint32_t binary_len;
        const uint8_t * binary;

        bson_iter_binary( iter, &subtype, &binary_len, &binary );

        if ( HB_ISBYREF( 2 ) ) {
            hb_storni( subtype, 2 );
        }
        if ( HB_ISBYREF( 3 ) ) {
            hb_stornl( binary_len, 3 );
        }

        if (HB_ISBYREF( 4 )) {
            hb_storclen( ( const char * ) binary, binary_len, 4 );
        }

        hb_retclen((const char *) binary, binary_len);

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_BOOL )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_BOOL( iter ) ) {
        bool result = bson_iter_bool( iter );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_CODE )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_CODE( iter ) ) {
        uint32_t length;
        const char * code = bson_iter_code( iter, &length );
        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( length, 2 );
        }
        hb_retclen(code, length);
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_CODEWSCOPE )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_CODEWSCOPE( iter ) ) {
        uint32_t length;
        uint32_t scope_len;
        const uint8_t * scope;
        const char * code = bson_iter_codewscope(iter, &length, &scope_len, &scope);
        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( length, 2 );
        }
        if (HB_ISBYREF(3)) {
            hb_stornl(scope_len, 3);
        }
        if (HB_ISBYREF(4)) {
            bson_t *doc = bson_new_from_data(scope, scope_len);
            if (doc) {
                PHB_BSON phBson = hbbson_new_dataContainer(_hbbson_t_, doc);
                hb_storptrGC(phBson, 4);
            } else {
                hb_stor(4);
            }
        }
        hb_retclen(code, length);
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_DATE_TIME )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_DATE_TIME( iter ) ) {
        HB_LONGLONG dt = bson_iter_date_time( iter );
        hb_retnll( dt );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_DECIMAL128 )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_DECIMAL128( iter ) && HB_ISBYREF( 2 ) ) {
        bson_decimal128_t * dec = hb_xgrab( sizeof( bson_decimal128_t ) );
        bool result = bson_iter_decimal128( iter, dec );
        if (result) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_decimal128_t_, dec );
            hb_storptrGC( phBson, 2 );
        } else {
            hb_stor(2);
        }
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_DOCUMENT )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if (iter && BSON_ITER_HOLDS_DOCUMENT(iter)) {
        uint32_t document_len;
        const uint8_t * document;
        bson_iter_document(iter, &document_len, &document);
        bson_t *doc = bson_new_from_data(document, document_len);

        if (HB_ISBYREF(2)) {
            hb_stornl(document_len, 2);
        }

        if (HB_ISBYREF(3)) {
            hb_storclen((const char * ) document, document_len, 3);
        }

        if (doc) {
            PHB_BSON phBson = hbbson_new_dataContainer(_hbbson_t_, doc);
            hb_retptrGC(phBson);
        } else {
            hb_ret();
        }

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_DOUBLE )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_DOUBLE( iter ) ) {
        double result = bson_iter_double( iter );
        hb_retnd( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_FIND )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    const char * key = hb_parc( 2 );

    if ( iter && key ) {
        bool result = bson_iter_find( iter, key );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_FIND_CASE )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    const char * key = hb_parc( 2 );

    if ( iter && key ) {
        bool result = bson_iter_find_case( iter, key );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_FIND_DESCENDANT )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    const char * key = hb_parc( 2 );
    bson_iter_t * descendant = bson_iter_hbparam( 3 );

    if ( iter && key && descendant ) {
        bool result = bson_iter_find_descendant( iter, key, descendant );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }

}

HB_FUNC( BSON_ITER_INIT )
{
    const bson_t * bson = bson_hbparam( 2, HB_IT_POINTER );

    if ( HB_ISBYREF( 1 ) && bson ) {

        PHB_BSON phBson = hbbson_param( 1, _hbbson_iter_t_ );
        bson_iter_t * iter = NULL;

        if ( phBson ) {
            iter = phBson->p;
        } else {
            iter = hb_xgrab( sizeof( bson_iter_t ) );
        }

        bool result = bson_iter_init( iter, bson );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_iter_t_, iter );
            hb_storptrGC( phBson, 1 );
        }

        hb_retl( result );

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INIT_FIND )
{
    const bson_t * bson = bson_hbparam( 2, HB_IT_POINTER );
    const char * key = hb_parc( 3 );

    if ( HB_ISBYREF( 1 ) && bson && key ) {

        PHB_BSON phBson = hbbson_param( 1, _hbbson_iter_t_ );
        bson_iter_t * iter = NULL;

        if ( phBson ) {
            iter = phBson->p;
        } else {
            iter = hb_xgrab( sizeof( bson_iter_t ) );
        }

        bool result = bson_iter_init_find( iter, bson, key );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_iter_t_, iter );
            hb_storptrGC( phBson, 1 );
        }

        hb_retl( result );

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INIT_FIND_CASE )
{
    const bson_t * bson = bson_hbparam( 2, HB_IT_POINTER );
    const char * key = hb_parc( 3 );

    if ( HB_ISBYREF( 1 ) && bson && key ) {

        PHB_BSON phBson = hbbson_param( 1, _hbbson_iter_t_ );
        bson_iter_t * iter = NULL;

        if ( phBson ) {
            iter = phBson->p;
        } else {
            iter = hb_xgrab( sizeof( bson_iter_t ) );
        }

        bool result = bson_iter_init_find_case( iter, bson, key );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_iter_t_, iter );
            hb_storptrGC( phBson, 1 );
        }

        hb_retl( result );

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INT32 )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_INT32( iter ) ) {
        int32_t result = bson_iter_int32( iter );
        hb_retni( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INT64 )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_INT64( iter ) ) {
        HB_LONGLONG result = bson_iter_int64( iter );
        hb_retnll( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_KEY )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        const char * key = bson_iter_key( iter );
        hb_retc( key );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_NEXT )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        bool result = bson_iter_next( iter );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OID )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_OID( iter ) ) {
        const bson_oid_t * oid = bson_iter_oid( iter );
        bson_oid_t * new = hb_xgrab( sizeof( bson_oid_t ) );
        bson_oid_copy( oid, new );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_oid_t_, new );
        hb_retptrGC( phBson );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_REGEX )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if (iter && BSON_ITER_HOLDS_REGEX(iter)) {
        const char * options;
        const char * regex = bson_iter_regex(iter, &options);
        if (HB_ISBYREF(2)) {
            hb_storc(options, 2);
        }
        hb_retc(regex);
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_TYPE )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        int type = bson_iter_type( iter );
        hb_retni( type );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_UTF8 )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        uint32_t length;
        const char * utfBuffer = bson_iter_utf8(iter, &length);
        if (HB_ISBYREF(2)) {
            hb_storni(length, 2);
        }
        hb_retc( utfBuffer );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INIT_FROM_DATA )
{
    const char * data = hb_parc( 2 );

    if ( HB_ISBYREF( 1 ) && data ) {
        size_t length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );

        PHB_BSON phBson = hbbson_param( 1, _hbbson_iter_t_ );
        bson_iter_t * iter = NULL;

        if ( phBson ) {
            iter = phBson->p;
        } else {
            iter = hb_xgrab( sizeof( bson_iter_t ) );
        }

        bool result = bson_iter_init_from_data( iter, ( const uint8_t * ) data, length );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_iter_t_, iter );
            hb_storptrGC( phBson, 1 );
        }

        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INIT_FIND_W_LEN )
{
    const bson_t * bson = bson_hbparam( 2, HB_IT_POINTER );
    const char * key = hb_parc( 3 );

    if ( HB_ISBYREF( 1 ) && bson && key && HB_ISNUM( 4 ) ) {
        int keylen = hb_parni( 4 );

        PHB_BSON phBson = hbbson_param( 1, _hbbson_iter_t_ );
        bson_iter_t * iter = NULL;

        if ( phBson ) {
            iter = phBson->p;
        } else {
            iter = hb_xgrab( sizeof( bson_iter_t ) );
        }

        bool result = bson_iter_init_find_w_len( iter, bson, key, keylen );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_iter_t_, iter );
            hb_storptrGC( phBson, 1 );
        }

        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_FIND_W_LEN )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    const char * key = hb_parc( 2 );

    if ( iter && key && HB_ISNUM( 3 ) ) {
        bool result = bson_iter_find_w_len( iter, key, hb_parni( 3 ) );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_KEY_LEN )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        hb_retni( ( int ) bson_iter_key_len( iter ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OFFSET )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        hb_retni( ( int ) bson_iter_offset( iter ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_DUP_UTF8 )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        uint32_t length = 0;
        char * szValue = bson_iter_dup_utf8( iter, &length );

        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( (HB_LONG) length, 2 );
        }

        if ( szValue ) {
            hb_retc( szValue );
            bson_free( szValue );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_BINARY_SUBTYPE )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        hb_retni( ( int ) bson_iter_binary_subtype( iter ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_BINARY_EQUAL )
{
    const bson_iter_t * iter_a = bson_iter_hbparam( 1 );
    const bson_iter_t * iter_b = bson_iter_hbparam( 2 );

    if ( iter_a && iter_b ) {
        hb_retl( bson_iter_binary_equal( iter_a, iter_b ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_DBPOINTER )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        uint32_t collection_len = 0;
        const char * collection = NULL;
        const bson_oid_t * oid = NULL;

        bson_iter_dbpointer( iter, &collection_len, &collection, &oid );

        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( (HB_LONG) collection_len, 2 );
        }

        if ( HB_ISBYREF( 3 ) && collection ) {
            hb_storclen( collection, collection_len, 3 );
        }

        if ( HB_ISBYREF( 4 ) && oid ) {
            PHB_BSON phOid = hbbson_new_dataContainer( _hbbson_oid_t_, hb_xmemdup( ( void * ) oid, sizeof( bson_oid_t ) ) );
            hb_storptrGC( phOid, 4 );
        }

        hb_ret();
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_SYMBOL )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        uint32_t length = 0;
        const char * szSymbol = bson_iter_symbol( iter, &length );

        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( (HB_LONG) length, 2 );
        }

        if ( szSymbol ) {
            hb_retc( szSymbol );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_TIME_T )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        hb_retnll( (HB_LONGLONG) bson_iter_time_t( iter ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_TIMEVAL )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && HB_ISBYREF( 2 ) ) {
        struct timeval tv;
        memset( &tv, 0, sizeof( tv ) );
        bson_iter_timeval( iter, &tv );
        hb_stornd( (double) tv.tv_sec, 2 );
        hb_retnll( (HB_LONGLONG) tv.tv_sec );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_TIMESTAMP )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        uint32_t timestamp = 0;
        uint32_t increment = 0;
        bson_iter_timestamp( iter, &timestamp, &increment );

        if ( HB_ISBYREF( 3 ) ) {
            hb_stornl( (HB_LONG) increment, 3 );
        }

        hb_retni( ( int ) timestamp );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_RECURSE )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && HB_ISBYREF( 2 ) ) {
        PHB_BSON phBson = hbbson_param( 2, _hbbson_iter_t_ );
        bson_iter_t * child = NULL;

        if ( phBson ) {
            child = phBson->p;
        } else {
            child = hb_xgrab( sizeof( bson_iter_t ) );
        }

        bool result = bson_iter_recurse( iter, child );

        if ( ! phBson ) {
            phBson = hbbson_new_dataContainer( _hbbson_iter_t_, child );
            hb_storptrGC( phBson, 2 );
        }

        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OVERWRITE_BOOL )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && HB_ISLOG( 2 ) ) {
        bson_iter_overwrite_bool( iter, hb_parl( 2 ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OVERWRITE_INT32 )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && HB_ISNUM( 2 ) ) {
        bson_iter_overwrite_int32( iter, ( int32_t ) hb_parni( 2 ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OVERWRITE_INT64 )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && HB_ISNUM( 2 ) ) {
        bson_iter_overwrite_int64( iter, ( int64_t ) hb_parnll( 2 ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OVERWRITE_DOUBLE )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && HB_ISNUM( 2 ) ) {
        bson_iter_overwrite_double( iter, ( double ) hb_parnl( 2 ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OVERWRITE_DECIMAL128 )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    const bson_decimal128_t * dec = bson_decimal128_hbparam( 2 );

    if ( iter && dec ) {
        bson_iter_overwrite_decimal128( iter, dec );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OVERWRITE_OID )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    const bson_oid_t * oid = bson_oid_hbparam( 2 );

    if ( iter && oid ) {
        bson_iter_overwrite_oid( iter, oid );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OVERWRITE_TIMESTAMP )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) ) {
        bson_iter_overwrite_timestamp( iter, ( uint32_t ) hb_parni( 2 ), ( uint32_t ) hb_parni( 3 ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OVERWRITE_DATE_TIME )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    PHB_ITEM pItem = hb_param( 2, HB_IT_LONG | HB_IT_DATETIME );

    if ( iter && pItem ) {
        HB_LONGLONG value;
        if ( hb_itemType( pItem ) & HB_IT_DATETIME ) {
            value = hb_dtToUnix( hb_itemGetTD( pItem ) );
        } else {
            value = hb_itemGetNLL( pItem );
        }
        bson_iter_overwrite_date_time( iter, ( int64_t ) value );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OVERWRITE_BINARY )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    const char * binary = hb_parc( 3 );

    if ( iter && binary ) {
        bson_subtype_t subtype = hb_parnidef( 2, BSON_SUBTYPE_BINARY );
        uint32_t length = hb_parnidef( 4, ( int ) hb_parclen( 3 ) );
        uint8_t * data = ( uint8_t * ) hb_xgrab( length );
        memcpy( data, binary, length );
        bson_iter_overwrite_binary( iter, subtype, &length, &data );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_VALUE )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        const bson_value_t * value = bson_iter_value( iter );

        if ( value ) {
            bson_value_t * copy = hb_xgrab( sizeof( bson_value_t ) );
            bson_value_copy( value, copy );
            PHB_BSON phValue = hbbson_new_dataContainer( _hbbson_value_t_, copy );
            hb_retptrGC( phValue );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INIT_FROM_DATA_AT_OFFSET )
{
    const char * data = hb_parc( 2 );

    if ( HB_ISBYREF( 1 ) && data && HB_ISNUM( 3 ) && HB_ISNUM( 4 ) && HB_ISNUM( 5 ) ) {
        size_t length = ( size_t ) hb_parnll( 3 );
        uint32_t offset = ( uint32_t ) hb_parni( 4 );
        uint32_t keylen = ( uint32_t ) hb_parni( 5 );

        PHB_BSON phBson = hbbson_param( 1, _hbbson_iter_t_ );
        bson_iter_t * iter = NULL;

        if ( phBson ) {
            iter = phBson->p;
        } else {
            iter = hb_xgrab( sizeof( bson_iter_t ) );
        }

        bool result = bson_iter_init_from_data_at_offset( iter, ( const uint8_t * ) data, length, offset, keylen );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_iter_t_, iter );
            hb_storptrGC( phBson, 1 );
        }

        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}
