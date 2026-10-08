//
//  hb_bson.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_bson.h"
#include "hbjson.h"
#include "hbdate.h"

enum hb_return_json_type
{
    _HBRETJSON_CANONICAL_,
    _HBRETJSON_RELAXED_,
    _HBRETJSON_SIMPLE_
};

static enum hb_return_json_type s_hbmongoc_return_json_type = _HBRETJSON_RELAXED_;

static const char * _STR_JSON_SIMPLE_    = "SIMPLE";
static const char * _STR_JSON_CANONICAL_ = "CANONICAL";
static const char * _STR_JSON_RELAXED_   = "RELAXED";

PHB_BSON hbbson_hbparam( PHB_ITEM pItem, hbbson_t_ hbbson_type );
/*
 hbbson_destroy
 */
static HB_GARBAGE_FUNC( hbbson_gc_func )
{
    PHB_BSON phBson = Cargo;

    if ( phBson ) {
        switch (phBson->hbbson_type) {
            case _hbbson_t_:
                if (phBson->p) {
                    bson_destroy( ( bson_t * ) phBson->p );
                    phBson->p = NULL;
                }
                break;
            case _hbbson_oid_t_:
                if ( phBson->p ) {
                    hb_xfree( phBson->p );
                    phBson->p = NULL;
                }
                break;
            case _hbbson_iter_t_:
                if ( phBson->p ) {
                    hb_xfree( phBson->p );
                    phBson->p = NULL;
                }
                break;
            case _hbbson_context_t_:
                if ( phBson->p ) {
                    bson_context_destroy( phBson->p );
                    phBson->p = NULL;
                }
                break;
            case _hbbson_decimal128_t_:
                if ( phBson->p ) {
                    hb_xfree( phBson->p );
                    phBson->p = NULL;
                }
                break;
            case _hbbson_value_t_:
                if ( phBson->p ) {
                    bson_value_destroy( phBson->p );
                    hb_xfree( phBson->p );
                    phBson->p = NULL;
                }
                break;
            case _hbbson_reader_t_:
                if ( phBson->p ) {
                    bson_reader_destroy( phBson->p );
                    phBson->p = NULL;
                }
                break;
            case _hbbson_json_opts_t_:
                if ( phBson->p ) {
                    bson_json_opts_destroy( phBson->p );
                    phBson->p = NULL;
                }
                break;
            case _hbbson_json_reader_t_:
                if ( phBson->p ) {
                    bson_json_reader_destroy( phBson->p );
                    phBson->p = NULL;
                }
                break;
            case _hbbson_writer_t_:
                if ( phBson->p ) {
                    bson_writer_destroy( phBson->p );
                    phBson->p = NULL;
                }
                break;
        }
    }
}

static const HB_GC_FUNCS s_gc_bson_funcs = {
    hbbson_gc_func,
    hb_gcDummyMark
};

bson_context_t * bson_context_hbparam( int iParam )
{
    PHB_BSON phBson = hbbson_param( iParam, _hbbson_context_t_ );

    if ( phBson ) {
        return phBson->p;
    }
    return NULL;
}

bson_decimal128_t * bson_decimal128_hbparam( int iParam )
{
    PHB_BSON phBson = hbbson_param( iParam, _hbbson_decimal128_t_ );

    if ( phBson ) {
        return phBson->p;
    }
    return NULL;
}

bson_iter_t * bson_iter_hbparam( int iParam )
{
    PHB_BSON phBson = hbbson_param( iParam, _hbbson_iter_t_ );

    if ( phBson ) {
        return phBson->p;
    }
    return NULL;
}

bson_oid_t * bson_oid_hbparam( int iParam )
{
    PHB_BSON phBson = hbbson_param( iParam, _hbbson_oid_t_ );

    if ( phBson ) {
        return phBson->p;
    }
    return NULL;
}

bson_value_t * bson_value_hbparam( int iParam )
{
    PHB_BSON phBson = hbbson_param( iParam, _hbbson_value_t_ );

    if ( phBson ) {
        return phBson->p;
    }
    return NULL;
}

bson_reader_t * bson_reader_hbparam( int iParam )
{
    PHB_BSON phBson = hbbson_param( iParam, _hbbson_reader_t_ );

    if ( phBson ) {
        return phBson->p;
    }
    return NULL;
}

bson_json_opts_t * bson_json_opts_hbparam( int iParam )
{
    PHB_BSON phBson = hbbson_param( iParam, _hbbson_json_opts_t_ );

    if ( phBson ) {
        return phBson->p;
    }
    return NULL;
}

bson_json_reader_t * bson_json_reader_hbparam( int iParam )
{
    PHB_BSON phBson = hbbson_param( iParam, _hbbson_json_reader_t_ );

    if ( phBson ) {
        return phBson->p;
    }
    return NULL;
}

bson_writer_t * bson_writer_hbparam( int iParam )
{
    PHB_BSON phBson = hbbson_param( iParam, _hbbson_writer_t_ );

    if ( phBson ) {
        return phBson->p;
    }
    return NULL;
}

bson_t * get_bson_item(PHB_ITEM pItem)
{
    if ( pItem ) {
        if ( hb_itemType( pItem ) & HB_IT_POINTER ) {
            PHB_BSON phBson = hbbson_hbparam( pItem, _hbbson_t_ );
            if ( phBson ) {
                return phBson->p;
            }
        } else if ( hb_itemType( pItem ) & HB_IT_STRING ) {
            const char * szJSON = NULL;
            szJSON = hb_itemGetC( pItem );
            bson_t * bson = bson_new_from_json( ( const uint8_t * ) szJSON, -1, NULL );
            if ( bson ) {
                return bson;
            }
        } else if ( hb_itemType( pItem ) & ( HB_IT_HASH | HB_IT_ARRAY ) ) {
            char * szJSON = hb_jsonEncode( pItem, NULL, false );
            if ( szJSON ) {
                bson_t * bson = bson_new_from_json( ( const uint8_t * ) szJSON, -1, NULL );
                hb_xfree( szJSON );
                if ( bson ) {
                    return bson;
                }
            }
        }
    }
    return NULL;
}

bson_t * bson_hbparam( int iParam, long lMask )
{
    PHB_ITEM pItem = hb_param( iParam, lMask );

    return get_bson_item(pItem);
}

void bson_hbstor_byref_error( int iParam, bson_error_t * error, HB_BOOL valid )
{
    if ( HB_ISBYREF( iParam ) ) {
        if ( ! valid && error && strlen( error->message ) > 0 ) {
            PHB_ITEM pItemHash = hb_itemNew( NULL );
            hb_hashNew( pItemHash );

            PHB_ITEM pItemKey;
            PHB_ITEM pItemValue;

            pItemKey = hb_itemNew( NULL );
            pItemValue = hb_itemNew( NULL );
            hb_itemPutC( pItemKey, "domain" );
            hb_itemPutNI( pItemValue, error->domain );
            hb_hashAdd( pItemHash, pItemKey, pItemValue );
            hb_itemRelease( pItemKey );
            hb_itemRelease( pItemValue );

            pItemKey = hb_itemNew( NULL );
            pItemValue = hb_itemNew( NULL );
            hb_itemPutC( pItemKey, "code" );
            hb_itemPutNI( pItemValue, error->code );
            hb_hashAdd( pItemHash, pItemKey, pItemValue );
            hb_itemRelease( pItemKey );
            hb_itemRelease( pItemValue );

            pItemKey = hb_itemNew( NULL );
            pItemValue = hb_itemNew( NULL );
            hb_itemPutC( pItemKey, "message" );
            hb_itemPutC( pItemValue, error->message );
            hb_hashAdd( pItemHash, pItemKey, pItemValue );
            hb_itemRelease( pItemKey );
            hb_itemRelease( pItemValue );

            hb_itemParamStoreRelease( iParam, pItemHash );
        } else {
            hb_stor( iParam );
        }
    }
}

char * hbbson_as_json( const bson_t * bson )
{
    char * szJSON = NULL;
    switch ( s_hbmongoc_return_json_type ) {
        case _HBRETJSON_SIMPLE_:
            szJSON = bson_as_legacy_extended_json( bson, NULL );
            break;
        case _HBRETJSON_CANONICAL_:
            szJSON = bson_as_canonical_extended_json( bson, NULL );
            break;
        case _HBRETJSON_RELAXED_:
            szJSON = bson_as_relaxed_extended_json( bson, NULL );
            break;
    }
    return szJSON;
}

HB_LONGLONG hb_dtToUnix(double dTimeStamp)
{
    int iYear, iMonth, iDay, iHour, iMinute, iSecond, iMSec;

    /* returns unpacked local time */
    hb_timeStampUnpack(dTimeStamp, &iYear, &iMonth, &iDay, &iHour, &iMinute, &iSecond, &iMSec);

    struct tm t;
    time_t timet;

    t.tm_year = iYear - 1900;
    t.tm_mon = iMonth - 1;
    t.tm_mday = iDay;
    t.tm_hour = iHour;
    t.tm_min = iMinute;
    t.tm_sec = iSecond;
    t.tm_isdst = -1;
    timet = mktime(&t);

    return timet * 1000 + iMSec;
}

HB_FUNC( HB_DTTOUNIX )
{
    if (hb_pcount() == 0 || HB_ISDATETIME(1)) {
        double dTimeStamp;

        if(HB_ISDATETIME(1)) {
            dTimeStamp = hb_partd(1);
        } else {
            long lDate, lTime;
            hb_timeStampGet(&lDate, &lTime);
            dTimeStamp = (double) lDate + (double) lTime / HB_MILLISECS_PER_DAY;
        }

        hb_retnll(hb_dtToUnix(dTimeStamp));

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( HB_UNIXTOT )
{
    PHB_ITEM pItem = hb_param( 1, HB_IT_NUMERIC );

    if ( pItem ) {
        /* unusable on current hb
        double julian = ((hb_itemGetND( pItem ) + hb_timeUTCOffset() * 1000) / 86400000.0 ) + 2440587.5;
        needs to be 2440587.5
        item->asDateTime.julian is LONG, and needs to be double to correct calculation:
        t := hb_sToT("19700101000000")
        hb_tToN(t) = 2440588.00 // which is false, needs to be 2440587.50
        hb_dtToUnix(t) = 21600000 // which is false, needs to be 0
        */
        double julian = 0.0;
        HB_LONGLONG secs = hb_parnll(1) / 1000;
        time_t timet = secs;
        struct tm *pt = localtime(&timet);

        if (pt != NULL) {
            julian = hb_timeStampPack(pt->tm_year + 1900, pt->tm_mon + 1, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec, (int)(hb_parnl(1) - secs * 1000));
        }

        hb_rettd( julian );

    } else {
        HBBSON_ERR_ARGS();
    }
}

PHB_BSON hbbson_hbparam( PHB_ITEM pItem, hbbson_t_ hbbson_type )
{
    PHB_BSON phBson = hb_itemGetPtrGC( pItem, &s_gc_bson_funcs );

    return phBson && phBson->hbbson_type == hbbson_type ? phBson : NULL;
}

PHB_BSON hbbson_new_dataContainer( hbbson_t_ hbbson_type, void * p )
{
    if ( p ) {
        PHB_BSON phBson = hb_gcAllocate( sizeof( HB_BSON ), &s_gc_bson_funcs );

        phBson->hbbson_type = hbbson_type;

        switch ( hbbson_type ) {
            case _hbbson_t_:
            case _hbbson_oid_t_:
            case _hbbson_iter_t_:
            case _hbbson_context_t_:
            case _hbbson_decimal128_t_:
            case _hbbson_value_t_:
            case _hbbson_reader_t_:
            case _hbbson_json_opts_t_:
            case _hbbson_json_reader_t_:
            case _hbbson_writer_t_:
                phBson->p = p;
                break;
        }

        return phBson;

    } else {
        HBBSON_ERR_ARGS();
    }

    return NULL;
}

PHB_BSON hbbson_param( int iParam, hbbson_t_ hbbson_type )
{
    PHB_ITEM pItem = hb_param( iParam, HB_IT_POINTER );

    if ( pItem ) {
        PHB_BSON phBson = hbbson_hbparam( pItem, hbbson_type );
        if ( phBson && phBson->hbbson_type == hbbson_type ) {
            switch ( hbbson_type ) {
                case _hbbson_t_:
                case _hbbson_oid_t_:
                case _hbbson_iter_t_:
                case _hbbson_context_t_:
                case _hbbson_decimal128_t_:
                case _hbbson_value_t_:
                case _hbbson_reader_t_:
                case _hbbson_json_opts_t_:
                case _hbbson_json_reader_t_:
                case _hbbson_writer_t_:
                    if ( phBson->p ) {
                        return phBson;
                    }
                    break;
            }
        }
    }
    return NULL;
}


/* Harbour api */

HB_FUNC( BSON_ARRAY_AS_JSON )
{
    const bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        size_t length;
        const char * result = bson_array_as_legacy_extended_json( bson, &length );
        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( (HB_LONG) length, 2 );
        }
        hb_retc( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_AS_CANONICAL_EXTENDED_JSON )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON = bson_as_canonical_extended_json( bson, NULL );
        if( szJSON ) {
            hb_retc( szJSON );
            bson_free( szJSON );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_AS_JSON )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON = bson_as_legacy_extended_json( bson, NULL );
        if( szJSON ) {
            hb_retc( szJSON );
            bson_free( szJSON );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_AS_RELAXED_EXTENDED_JSON )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON = bson_as_relaxed_extended_json( bson, NULL );
        if ( szJSON ) {
            hb_retc( szJSON );
            bson_free( szJSON );
        }
    }
    else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_CHECK_VERSION )
{
    if ( HB_ISNUM( 1 ) && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) ) {
        hb_retl( bson_check_version( hb_parni( 1 ), hb_parni( 2 ), hb_parni( 3 ) ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_CONCAT )
{
    bson_t * dst = bson_hbparam( 1, HB_IT_POINTER );
    const bson_t * src = bson_hbparam( 2, HB_IT_POINTER );

    if ( dst && src ) {
        bool result = bson_concat( dst, src );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_COPY )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        bson_t * copy = bson_copy( bson );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, copy );
        hb_retptrGC( phBson );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_COUNT_KEYS )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if (bson) {
        uint32_t result = bson_count_keys(bson);
        hb_retni(result);
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_DECIMAL128_FROM_STRING )
{
    const char * string = hb_parc( 1 );

    if ( string && HB_ISBYREF( 2 ) ) {
        bson_decimal128_t * dec = hb_xgrab( sizeof( bson_decimal128_t ) );
        bool result = bson_decimal128_from_string( string, dec );
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

HB_FUNC( BSON_DECIMAL128_TO_STRING )
{
    const bson_decimal128_t * dec = bson_decimal128_hbparam( 1 );

    if ( dec ) {
        char string[ BSON_DECIMAL128_STRING ];
        bson_decimal128_to_string( dec, string );
        if ( HB_ISBYREF( 2 ) ) {
            hb_storc( string, 2 );
        }
        hb_retc( string );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_DESTROY )
{
    PHB_BSON phBson = hbbson_param( 1, _hbbson_t_ );

    if ( phBson ) {
        bson_destroy( phBson->p );
        phBson->p = NULL;
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_GET_MAJOR_VERSION )
{
    hb_retni( bson_get_major_version() );
}

HB_FUNC( BSON_GET_MICRO_VERSION )
{
    hb_retni( bson_get_micro_version() );
}

HB_FUNC( BSON_GET_MINOR_VERSION )
{
    hb_retni( bson_get_minor_version() );
}

HB_FUNC( BSON_GET_VERSION )
{
    hb_retc( bson_get_version() );
}

HB_FUNC( BSON_HAS_FIELD )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key ) {
        bool result = bson_has_field( bson, key );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_INIT )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        bson_init( bson );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_INIT_FROM_JSON )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * data = hb_parc( 2 );

    if ( bson && data ) {
        size_t len = hb_parnsdef( 3, hb_parclen( 2 ) );
        bson_error_t error;
        bool result = bson_init_from_json( bson, data, len, &error );
        bson_hbstor_byref_error( 4, &error, result );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

/*
HB_FUNC( BSON_INIT_STATIC )
 * bson_init_static() uses bson_t on stack and create internal references to itself
 * so it's not practical using it on a Harbour envå
 */

HB_FUNC( BSON_NEW )
{
    if ( hb_pcount() == 0 ) {
        bson_t * bson = bson_new();
        if ( bson ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson );
            hb_retptrGC( phBson );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_NEW_FROM_DATA )
{
    const uint8_t * data = ( const uint8_t * ) hb_parc( 1 );

    if ( data ) {
        size_t length = hb_parnsdef( 2, hb_parclen( 1 ) );
        bson_t * bson = bson_new_from_data( data, length );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson );
        hb_retptrGC( phBson );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_NEW_FROM_JSON )
{
    PHB_ITEM pItem = hb_param( 1, HB_IT_ANY );
    bson_t * bson = NULL;
    bson_error_t error;

    if ( hb_itemType( pItem ) & HB_IT_STRING ) {
        const char * data = hb_parc( 1 );
        int len = hb_parnidef( 2, ( int ) hb_parclen( 1 ) );
        bson = bson_new_from_json( ( const uint8_t * ) data, len, &error );
    } else if ( hb_itemType( pItem ) & ( HB_IT_HASH | HB_IT_ARRAY ) ) {
        char * szJSON = hb_jsonEncode( pItem, NULL, false );
        bson = bson_new_from_json( ( const uint8_t * ) szJSON, -1, &error );
        hb_xfree( szJSON );
    } else {
        HBBSON_ERR_ARGS();
        return;
    }

    bson_hbstor_byref_error( 3, &error, bson != NULL );

    if ( bson ) {
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson );
        hb_retptrGC( phBson );
    } else {
        hb_ret();
    }
}

HB_FUNC( BSON_REINIT )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        bson_reinit( bson );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_VALIDATE )
{
    const bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        bson_validate_flags_t flags = hb_parnidef( 2, BSON_VALIDATE_NONE );
        size_t offset;
        bool result = bson_validate( bson, flags, &offset );
        if ( HB_ISBYREF( 3 ) ) {
            hb_storns( offset, 3 );
        }
        hb_retl( result );
    }
}

HB_FUNC( HB_BSON_AS_HASH )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON;
        szJSON = hbbson_as_json( bson );
        if ( szJSON ) {
            PHB_ITEM pItem = hb_itemNew( NULL );
            hb_jsonDecode( szJSON, pItem );
            bson_free( szJSON );
            hb_itemReturnRelease( pItem );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( HB_BSON_AS_JSON )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON;
        szJSON = hbbson_as_json( bson );
        if ( szJSON ) {
            hb_retc( szJSON );
            bson_free( szJSON );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( HB_BSON_SET_RETURN_JSON_TYPE )
{
    if ( hb_pcount() > 0 ) {
        if ( hb_stricmp( hb_parc( 1 ), _STR_JSON_SIMPLE_ ) == 0 ) {
            s_hbmongoc_return_json_type = _HBRETJSON_SIMPLE_;
        } else if ( hb_stricmp( hb_parc( 1 ), _STR_JSON_CANONICAL_ ) == 0 ) {
            s_hbmongoc_return_json_type = _HBRETJSON_CANONICAL_;
        } else if ( hb_stricmp( hb_parc( 1 ), _STR_JSON_RELAXED_ ) == 0 ) {
            s_hbmongoc_return_json_type = _HBRETJSON_RELAXED_;
        } else {
            HBBSON_ERR_ARGS();
        }
    }
    switch ( s_hbmongoc_return_json_type ) {
        case _HBRETJSON_SIMPLE_:
            hb_retc( _STR_JSON_SIMPLE_ );
            break;
        case _HBRETJSON_CANONICAL_:
            hb_retc( _STR_JSON_CANONICAL_ );
            break;
        case _HBRETJSON_RELAXED_:
            hb_retc( _STR_JSON_RELAXED_ );
            break;
    }
}

HB_FUNC( HB_BSON_VERSION )
{
    hb_storni( BSON_MAJOR_VERSION, 1 );
    hb_storni( BSON_MINOR_VERSION, 2 );
    hb_storni( BSON_MICRO_VERSION, 3 );
}

HB_FUNC( BSON_VALIDATE_WITH_ERROR )
{
    const bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        bson_validate_flags_t flags = hb_parnidef( 2, BSON_VALIDATE_NONE );
        bson_error_t error;

        bool result = bson_validate_with_error( bson, flags, &error );

        bson_hbstor_byref_error( 3, &error, result );

        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_VALIDATE_WITH_ERROR_AND_OFFSET )
{
    const bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        bson_validate_flags_t flags = hb_parnidef( 2, BSON_VALIDATE_NONE );
        size_t offset = 0;
        bson_error_t error;

        bool result = bson_validate_with_error_and_offset( bson, flags, &offset, &error );

        if ( HB_ISBYREF( 3 ) ) {
            hb_storns( (HB_LONG) offset, 3 );
        }

        bson_hbstor_byref_error( 4, &error, result );

        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_AS_JSON_WITH_OPTS )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson && HB_ISBYREF( 3 ) ) {
        const bson_json_opts_t * opts = bson_json_opts_hbparam( 2 );
        size_t length = 0;

        char * szJSON = bson_as_json_with_opts( bson, &length, opts );

        if ( szJSON ) {
            hb_storclen( szJSON, length, 3 );
            hb_retc( szJSON );
            bson_free( szJSON );
        } else {
            hb_stor( 3 );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_AS_LEGACY_EXTENDED_JSON )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON = bson_as_legacy_extended_json( bson, NULL );
        if ( szJSON ) {
            hb_retc( szJSON );
            bson_free( szJSON );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ARRAY_AS_CANONICAL_EXTENDED_JSON )
{
    const bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        size_t length;
        const char * result = bson_array_as_canonical_extended_json( bson, &length );
        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( (HB_LONG) length, 2 );
        }
        hb_retc( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ARRAY_AS_RELAXED_EXTENDED_JSON )
{
    const bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        size_t length;
        const char * result = bson_array_as_relaxed_extended_json( bson, &length );
        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( (HB_LONG) length, 2 );
        }
        hb_retc( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ARRAY_AS_LEGACY_EXTENDED_JSON )
{
    const bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        size_t length;
        const char * result = bson_array_as_legacy_extended_json( bson, &length );
        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( (HB_LONG) length, 2 );
        }
        hb_retc( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_COMPARE )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const bson_t * other = bson_hbparam( 2, HB_IT_POINTER );

    if ( bson && other ) {
        hb_retni( bson_compare( bson, other ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_EQUAL )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const bson_t * other = bson_hbparam( 2, HB_IT_POINTER );

    if ( bson && other ) {
        hb_retl( bson_equal( bson, other ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_COPY_TO )
{
    bson_t * src = bson_hbparam( 1, HB_IT_POINTER );

    if ( src && HB_ISBYREF( 2 ) ) {
        PHB_BSON phBson = hbbson_param( 2, _hbbson_t_ );
        bson_t * dst = NULL;

        if ( phBson ) {
            dst = phBson->p;
        } else {
            dst = bson_new();
        }

        if ( dst ) {
            bson_copy_to( src, dst );

            if ( ! phBson ) {
                phBson = hbbson_new_dataContainer( _hbbson_t_, dst );
                hb_storptrGC( phBson, 2 );
            }
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_COPY_TO_EXCLUDING_NOINIT )
{
    bson_t * src = bson_hbparam( 1, HB_IT_POINTER );

    if ( src && HB_ISBYREF( 2 ) ) {
        PHB_BSON phBson = hbbson_param( 2, _hbbson_t_ );
        bson_t * dst = NULL;

        if ( phBson ) {
            dst = phBson->p;
        } else {
            dst = bson_new();
        }

        if ( dst ) {
            bson_copy_to_excluding_noinit( src, dst, NULL, NULL, NULL );

            if ( ! phBson ) {
                phBson = hbbson_new_dataContainer( _hbbson_t_, dst );
                hb_storptrGC( phBson, 2 );
            }
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_DESTROY_WITH_STEAL )
{
    PHB_BSON phBson = hbbson_param( 1, _hbbson_t_ );

    if ( phBson && phBson->p ) {
        bool steal = hb_parldef( 2, false );
        uint32_t length = 0;

        uint8_t * data = bson_destroy_with_steal( phBson->p, steal, &length );

        phBson->p = NULL;

        if ( data && steal ) {
            if ( HB_ISBYREF( 3 ) ) {
                hb_storclen( ( const char * ) data, length, 3 );
            }
            hb_xfree( data );
        }

        hb_ret();
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_GET_DATA )
{
    const bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        const uint8_t * data = bson_get_data( bson );
        uint32_t length = bson->len;

        if ( HB_ISBYREF( 2 ) ) {
            hb_storclen( ( const char * ) data, length, 2 );
        }

        hb_ret();
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_STEAL )
{
    PHB_BSON phDst = hbbson_param( 1, _hbbson_t_ );
    PHB_BSON phSrc = hbbson_param( 2, _hbbson_t_ );

    if ( phDst && phSrc && phDst->p && phSrc->p ) {
        bool result = bson_steal( phDst->p, phSrc->p );
        if ( result ) {
            phSrc->p = NULL;
        }
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_SIZED_NEW )
{
    if ( HB_ISNUM( 1 ) ) {
        bson_t * bson = bson_sized_new( ( size_t ) hb_parnll( 1 ) );
        if ( bson ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson );
            hb_retptrGC( phBson );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_RESERVE_BUFFER )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson && HB_ISNUM( 2 ) ) {
        uint8_t * data = bson_reserve_buffer( bson, ( uint32_t ) hb_parni( 2 ) );

        if ( HB_ISBYREF( 3 ) && data ) {
            hb_storclen( ( const char * ) data, (HB_SIZE) bson->len, 3 );
        }

        hb_ret();
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_NEW_FROM_BUFFER )
{
    if ( HB_ISBYREF( 1 ) && HB_ISNUM( 2 ) ) {
        char * szBuffer = hb_parc( 1 );
        HB_SIZE bufLen = hb_parclen( 1 );
        HB_SIZE capacity = (HB_SIZE) hb_parnll( 2 );

        if ( szBuffer && capacity >= bufLen ) {
            uint8_t * data = ( uint8_t * ) hb_xgrab( capacity );
            memcpy( data, szBuffer, bufLen );
            size_t data_len = bufLen;
            uint8_t * buf = data;

            bson_t * bson = bson_new_from_buffer( &buf, &data_len, NULL, NULL );

            if ( bson ) {
                PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson );
                hb_retptrGC( phBson );
            } else {
                hb_xfree( data );
                hb_ret();
            }
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_BSON_GET_MONOTONIC_TIME )
{
    hb_retnll( bson_get_monotonic_time() );
}

HB_FUNC( BSON_GET_MONOTONIC_TIME )
{
    hb_retnll( bson_get_monotonic_time() );
}

HB_FUNC( BSON_GETTIMEOFDAY )
{
    struct timeval tv;
    memset( &tv, 0, sizeof( tv ) );
    bson_gettimeofday( &tv );

    if ( HB_ISBYREF( 1 ) ) {
        hb_stornd( (double) tv.tv_sec, 1 );
    }

    hb_retnll( (HB_LONGLONG) tv.tv_sec );
}

HB_FUNC( BSON_UINT32_TO_STRING )
{
    if ( HB_ISNUM( 1 ) ) {
        char szKey[ 32 ];
        const char * strKey = NULL;
        bson_uint32_to_string( ( uint32_t ) hb_parni( 1 ), &strKey, szKey, sizeof( szKey ) );
        hb_retc( strKey ? strKey : NULL );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_SET_ERROR )
{
    if ( HB_ISBYREF( 1 ) && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) && HB_IS_STRING( 4 ) ) {
        bson_error_t error;
        bson_set_error( &error, hb_parni( 2 ), hb_parni( 3 ), hb_parc( 4 ) );
        bson_hbstor_byref_error( 1, &error, false );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ERROR_CLEAR )
{
    if ( HB_ISBYREF( 1 ) ) {
        bson_error_t error;
        memset( &error, 0, sizeof( error ) );
        bson_error_clear( &error );
        bson_hbstor_byref_error( 1, &error, true );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_STRERROR_R )
{
    if ( HB_ISNUM( 1 ) && HB_ISBYREF( 2 ) ) {
        char buffer[ 1024 ];
        char * szError = bson_strerror_r( hb_parni( 1 ), buffer, sizeof( buffer ) );
        hb_storc( szError, 2 );
        hb_retc( szError );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_IS_POWER_OF_TWO )
{
    if ( HB_ISNUM( 1 ) ) {
        hb_retl( bson_is_power_of_two( ( uint32_t ) hb_parni( 1 ) ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_NEXT_POWER_OF_TWO )
{
    if ( HB_ISNUM( 1 ) ) {
        hb_retni( ( int ) bson_next_power_of_two( ( uint32_t ) hb_parni( 1 ) ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_DECIMAL128_FROM_STRING_W_LEN )
{
    const char * string = hb_parc( 1 );

    if ( string && HB_ISNUM( 2 ) && HB_ISBYREF( 3 ) ) {
        bson_decimal128_t * dec = hb_xgrab( sizeof( bson_decimal128_t ) );
        bool result = bson_decimal128_from_string_w_len( string, hb_parni( 2 ), dec );

        if ( result ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_decimal128_t_, dec );
            hb_storptrGC( phBson, 3 );
        } else {
            hb_xfree( dec );
            hb_stor( 3 );
        }

        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_JSON_OPTS_NEW )
{
    if ( HB_ISNUM( 1 ) && HB_ISNUM( 2 ) ) {
        bson_json_mode_t mode = ( bson_json_mode_t ) hb_parni( 1 );
        bson_json_opts_t * opts = bson_json_opts_new( mode, ( int32_t ) hb_parni( 2 ) );
        if ( opts ) {
            PHB_BSON phOpts = hbbson_new_dataContainer( _hbbson_json_opts_t_, opts );
            hb_retptrGC( phOpts );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_JSON_OPTS_DESTROY )
{
    PHB_BSON opts = hbbson_param( 1, _hbbson_json_opts_t_ );

    if ( opts ) {
        bson_json_opts_destroy( opts->p );
        opts->p = NULL;
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_JSON_OPTS_SET_OUTERMOST_ARRAY )
{
    bson_json_opts_t * opts = bson_json_opts_hbparam( 1 );

    if ( opts && HB_ISLOG( 2 ) ) {
        bson_json_opts_set_outermost_array( opts, hb_parl( 2 ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_JSON_READER_NEW )
{
    bson_json_reader_t * reader = bson_json_reader_new( NULL, 0, 0, false, false );

    if ( reader ) {
        PHB_BSON phReader = hbbson_new_dataContainer( _hbbson_json_reader_t_, reader );
        hb_retptrGC( phReader );
    } else {
        hb_ret();
    }
}

HB_FUNC( BSON_JSON_READER_NEW_FROM_FILE )
{
    const char * filename = hb_parc( 1 );

    if ( filename ) {
        bson_error_t error;
        bson_json_reader_t * reader = bson_json_reader_new_from_file( filename, &error );

        bson_hbstor_byref_error( 2, &error, reader != NULL );

        if ( reader ) {
            PHB_BSON phReader = hbbson_new_dataContainer( _hbbson_json_reader_t_, reader );
            hb_retptrGC( phReader );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_JSON_READER_DESTROY )
{
    PHB_BSON reader = hbbson_param( 1, _hbbson_json_reader_t_ );

    if ( reader ) {
        bson_json_reader_destroy( reader->p );
        reader->p = NULL;
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_JSON_READER_READ )
{
    bson_json_reader_t * reader = bson_json_reader_hbparam( 1 );

    if ( reader && HB_ISBYREF( 2 ) ) {
        bson_t bson;
        bson_init( &bson );
        bson_error_t error;

        int result = bson_json_reader_read( reader, &bson, &error );

        if ( result > 0 ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( &bson ) );
            hb_storptrGC( phBson, 2 );
        } else {
            hb_stor( 2 );
        }

        bson_destroy( &bson );

        bson_hbstor_byref_error( 3, &error, result >= 0 );

        hb_retni( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_JSON_DATA_READER_NEW )
{
    if ( HB_ISLOG( 1 ) && HB_ISNUM( 2 ) ) {
        bool allow_multiple = hb_parl( 1 );
        bson_json_reader_t * reader = bson_json_data_reader_new( allow_multiple, ( size_t ) hb_parnll( 2 ) );

        if ( reader ) {
            PHB_BSON phReader = hbbson_new_dataContainer( _hbbson_json_reader_t_, reader );
            hb_retptrGC( phReader );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_JSON_DATA_READER_INGEST )
{
    bson_json_reader_t * reader = bson_json_reader_hbparam( 1 );
    const char * data = hb_parc( 2 );

    if ( reader && data ) {
        size_t len = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bson_json_data_reader_ingest( reader, ( const uint8_t * ) data, len );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_READER_NEW_FROM_FILE )
{
    const char * path = hb_parc( 1 );

    if ( path ) {
        bson_error_t error;
        bson_reader_t * reader = bson_reader_new_from_file( path, &error );

        bson_hbstor_byref_error( 2, &error, reader != NULL );

        if ( reader ) {
            PHB_BSON phReader = hbbson_new_dataContainer( _hbbson_reader_t_, reader );
            hb_retptrGC( phReader );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_READER_NEW_FROM_DATA )
{
    const char * data = hb_parc( 1 );

    if ( data ) {
        size_t length = hb_parnidef( 2, ( int ) hb_parclen( 1 ) );
        bson_reader_t * reader = bson_reader_new_from_data( ( const uint8_t * ) data, length );

        if ( reader ) {
            PHB_BSON phReader = hbbson_new_dataContainer( _hbbson_reader_t_, reader );
            hb_retptrGC( phReader );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_READER_DESTROY )
{
    PHB_BSON reader = hbbson_param( 1, _hbbson_reader_t_ );

    if ( reader ) {
        bson_reader_destroy( reader->p );
        reader->p = NULL;
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_READER_READ )
{
    bson_reader_t * reader = bson_reader_hbparam( 1 );

    if ( reader && HB_ISBYREF( 2 ) ) {
        bool reached_eof = false;
        const bson_t * bson = bson_reader_read( reader, &reached_eof );

        if ( bson ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( bson ) );
            hb_storptrGC( phBson, 2 );
        } else {
            hb_stor( 2 );
        }

        if ( HB_ISBYREF( 3 ) ) {
            hb_storl( reached_eof, 3 );
        }

        hb_retl( bson != NULL );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_READER_TELL )
{
    bson_reader_t * reader = bson_reader_hbparam( 1 );

    if ( reader ) {
        hb_retnll( (HB_LONGLONG) bson_reader_tell( reader ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_READER_RESET )
{
    bson_reader_t * reader = bson_reader_hbparam( 1 );

    if ( reader ) {
        bson_reader_reset( reader );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_WRITER_NEW )
{
    bson_writer_t * writer = bson_writer_new( NULL, NULL, 0, NULL, NULL );

    if ( writer ) {
        PHB_BSON phWriter = hbbson_new_dataContainer( _hbbson_writer_t_, writer );
        hb_retptrGC( phWriter );
    } else {
        hb_ret();
    }
}

HB_FUNC( BSON_WRITER_DESTROY )
{
    PHB_BSON writer = hbbson_param( 1, _hbbson_writer_t_ );

    if ( writer ) {
        bson_writer_destroy( writer->p );
        writer->p = NULL;
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_WRITER_GET_LENGTH )
{
    bson_writer_t * writer = bson_writer_hbparam( 1 );

    if ( writer ) {
        hb_retnll( (HB_LONGLONG) bson_writer_get_length( writer ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_WRITER_BEGIN )
{
    bson_writer_t * writer = bson_writer_hbparam( 1 );

    if ( writer && HB_ISBYREF( 2 ) ) {
        bson_t * bson = NULL;
        bool result = bson_writer_begin( writer, &bson );

        if ( result && bson ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson );
            hb_storptrGC( phBson, 2 );
        } else {
            hb_stor( 2 );
        }

        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_WRITER_END )
{
    bson_writer_t * writer = bson_writer_hbparam( 1 );

    if ( writer ) {
        bson_writer_end( writer );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_WRITER_ROLLBACK )
{
    bson_writer_t * writer = bson_writer_hbparam( 1 );

    if ( writer ) {
        bson_writer_rollback( writer );
    } else {
        HBBSON_ERR_ARGS();
    }
}

/* string helpers */

HB_FUNC( BSON_STRDUP )
{
    const char * str = hb_parc( 1 );

    if ( str ) {
        char * copy = bson_strdup( str );
        if ( copy ) {
            hb_retc( copy );
            bson_free( copy );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_STRNDUP )
{
    const char * str = hb_parc( 1 );

    if ( str && HB_ISNUM( 2 ) ) {
        char * copy = bson_strndup( str, ( size_t ) hb_parnll( 2 ) );
        if ( copy ) {
            hb_retc( copy );
            bson_free( copy );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_STRNCPY )
{
    const char * src = hb_parc( 2 );

    if ( HB_ISBYREF( 1 ) && src && HB_ISNUM( 3 ) ) {
        size_t size = ( size_t ) hb_parnll( 3 );
        char * dst = hb_xgrab( size + 1 );
        bson_strncpy( dst, src, size );
        hb_storc( dst, 1 );
        hb_xfree( dst );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_STRNLEN )
{
    const char * s = hb_parc( 1 );

    if ( s ) {
        size_t maxlen = HB_ISNUM( 2 ) ? ( size_t ) hb_parnll( 2 ) : ( size_t ) hb_parclen( 1 );
        hb_retnll( (HB_LONGLONG) bson_strnlen( s, maxlen ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_STRCASECMP )
{
    const char * s1 = hb_parc( 1 );
    const char * s2 = hb_parc( 2 );

    if ( s1 && s2 ) {
        hb_retni( bson_strcasecmp( s1, s2 ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ISSPACE )
{
    if ( HB_ISNUM( 1 ) ) {
        hb_retl( bson_isspace( hb_parni( 1 ) ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ASCII_STRTOLL )
{
    const char * str = hb_parc( 1 );

    if ( str && HB_ISNUM( 2 ) ) {
        char * endptr = NULL;
        int64_t value = bson_ascii_strtoll( str, &endptr, hb_parni( 2 ) );
        hb_retnll( (HB_LONGLONG) value );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_SNPRINTF )
{
    if ( HB_IS_STRING( 1 ) ) {
        char buffer[ 4096 ];
        const char * format = hb_parc( 1 );
        int n;

        switch ( hb_pcount() ) {
            case 1:
                n = bson_snprintf( buffer, sizeof( buffer ), format );
                break;
            case 2:
                n = bson_snprintf( buffer, sizeof( buffer ), format, hb_parni( 2 ) );
                break;
            case 3:
                n = bson_snprintf( buffer, sizeof( buffer ), format, hb_parni( 2 ), hb_parni( 3 ) );
                break;
            default:
                n = bson_snprintf( buffer, sizeof( buffer ), format, hb_parni( 2 ), hb_parni( 3 ), hb_parni( 4 ) );
                break;
        }

        if ( n >= 0 ) {
            hb_retc( buffer );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_VALUE_COPY )
{
    const bson_value_t * src = bson_value_hbparam( 1 );

    if ( src && HB_ISBYREF( 2 ) ) {
        bson_value_t * dst = hb_xgrab( sizeof( bson_value_t ) );
        bson_value_copy( src, dst );
        PHB_BSON phValue = hbbson_new_dataContainer( _hbbson_value_t_, dst );
        hb_storptrGC( phValue, 2 );
        hb_retptrGC( phValue );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_VALUE_DESTROY )
{
    PHB_BSON value = hbbson_param( 1, _hbbson_value_t_ );

    if ( value ) {
        bson_value_destroy( value->p );
        value->p = NULL;
    } else {
        HBBSON_ERR_ARGS();
    }
}
