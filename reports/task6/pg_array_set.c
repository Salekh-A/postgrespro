#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "utils/builtins.h"
#include "utils/typcache.h"
#include "utils/lsyscache.h"

PG_MODULE_MAGIC;

static void
check_array(ArrayType *arr, Oid elem_type, int16 elmlen, bool elmbyval,
            char elmalign, Datum **elems, int *nelems)
{
    bool *nulls;
    int   i;

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
                (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                 errmsg("only 1-D arrays are supported")));

    deconstruct_array(arr, elem_type, elmlen, elmbyval, elmalign,
                      elems, &nulls, nelems);

    for (i = 0; i < *nelems; i++)
        if (nulls[i])
            ereport(ERROR,
                    (errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED),
                     errmsg("NULL elements are not allowed")));
}

static bool
contains(Datum *arr, int n, Datum val, TypeCacheEntry *tc, Oid collation)
{
    int i;

    for (i = 0; i < n; i++)
    {
        Datum eq = FunctionCall2Coll(&tc->eq_opr_finfo, collation,
                                     arr[i], val);
        if (DatumGetBool(eq))
            return true;
    }
    return false;
}

PG_FUNCTION_INFO_V1(array_unique);

Datum
array_unique(PG_FUNCTION_ARGS)
{
    ArrayType      *input = PG_GETARG_ARRAYTYPE_P(0);
    Oid             elem_type;
    int16           elmlen;
    bool            elmbyval;
    char            elmalign;
    Datum          *elems;
    int             nelems;
    Datum          *result;
    int             result_count = 0;
    int             i;
    TypeCacheEntry *tc;
    Oid             collation = PG_GET_COLLATION();

    if (ARR_NDIM(input) == 0)
        PG_RETURN_ARRAYTYPE_P(input);

    elem_type = ARR_ELEMTYPE(input);
    get_typlenbyvalalign(elem_type, &elmlen, &elmbyval, &elmalign);

    check_array(input, elem_type, elmlen, elmbyval, elmalign,
                &elems, &nelems);

    tc = lookup_type_cache(elem_type,
                           TYPECACHE_HASH_PROC_FINFO |
                           TYPECACHE_EQ_OPR_FINFO);

    result = (Datum *) palloc(sizeof(Datum) * nelems);

    for (i = 0; i < nelems; i++)
    {
        if (!contains(result, result_count, elems[i], tc, collation))
            result[result_count++] = elems[i];
    }

    PG_RETURN_ARRAYTYPE_P(construct_array(result, result_count,
                                          elem_type, elmlen, elmbyval, elmalign));
}

PG_FUNCTION_INFO_V1(array_intersection);

Datum
array_intersection(PG_FUNCTION_ARGS)
{
    ArrayType      *a = PG_GETARG_ARRAYTYPE_P(0);
    ArrayType      *b = PG_GETARG_ARRAYTYPE_P(1);
    Oid             elem_type;
    int16           elmlen;
    bool            elmbyval;
    char            elmalign;
    Datum          *ea, *eb;
    int             na = 0, nb = 0;
    Datum          *result;
    int             result_count = 0;
    int             i;
    TypeCacheEntry *tc;
    Oid             collation = PG_GET_COLLATION();

    if (ARR_NDIM(a) == 0 || ARR_NDIM(b) == 0)
        PG_RETURN_ARRAYTYPE_P(construct_empty_array(ARR_ELEMTYPE(a)));

    elem_type = ARR_ELEMTYPE(a);
    get_typlenbyvalalign(elem_type, &elmlen, &elmbyval, &elmalign);

    check_array(a, elem_type, elmlen, elmbyval, elmalign, &ea, &na);
    check_array(b, elem_type, elmlen, elmbyval, elmalign, &eb, &nb);

    tc = lookup_type_cache(elem_type,
                           TYPECACHE_HASH_PROC_FINFO |
                           TYPECACHE_EQ_OPR_FINFO);

    result = (Datum *) palloc(sizeof(Datum) * na);

    for (i = 0; i < na; i++)
        if (contains(eb, nb, ea[i], tc, collation) &&
            !contains(result, result_count, ea[i], tc, collation))
            result[result_count++] = ea[i];

    PG_RETURN_ARRAYTYPE_P(construct_array(result, result_count,
                                          elem_type, elmlen, elmbyval, elmalign));
}

PG_FUNCTION_INFO_V1(array_union);

Datum
array_union(PG_FUNCTION_ARGS)
{
    ArrayType      *a = PG_GETARG_ARRAYTYPE_P(0);
    ArrayType      *b = PG_GETARG_ARRAYTYPE_P(1);
    Oid             elem_type;
    int16           elmlen;
    bool            elmbyval;
    char            elmalign;
    Datum          *ea, *eb;
    int             na = 0, nb = 0;
    Datum          *result;
    int             result_count = 0;
    int             i;
    TypeCacheEntry *tc;
    Oid             collation = PG_GET_COLLATION();

    if (ARR_NDIM(a) == 0 && ARR_NDIM(b) == 0)
        PG_RETURN_ARRAYTYPE_P(construct_empty_array(ARR_ELEMTYPE(a)));

    elem_type = (ARR_NDIM(a) > 0) ? ARR_ELEMTYPE(a) : ARR_ELEMTYPE(b);
    get_typlenbyvalalign(elem_type, &elmlen, &elmbyval, &elmalign);

    if (ARR_NDIM(a) > 0)
        check_array(a, elem_type, elmlen, elmbyval, elmalign, &ea, &na);
    if (ARR_NDIM(b) > 0)
        check_array(b, elem_type, elmlen, elmbyval, elmalign, &eb, &nb);

    tc = lookup_type_cache(elem_type,
                           TYPECACHE_HASH_PROC_FINFO |
                           TYPECACHE_EQ_OPR_FINFO);

    result = (Datum *) palloc(sizeof(Datum) * (na + nb + 1));

    for (i = 0; i < na; i++)
        if (!contains(result, result_count, ea[i], tc, collation))
            result[result_count++] = ea[i];

    for (i = 0; i < nb; i++)
        if (!contains(result, result_count, eb[i], tc, collation))
            result[result_count++] = eb[i];

    PG_RETURN_ARRAYTYPE_P(construct_array(result, result_count,
                                          elem_type, elmlen, elmbyval, elmalign));
}

PG_FUNCTION_INFO_V1(array_except);

Datum
array_except(PG_FUNCTION_ARGS)
{
    ArrayType      *a = PG_GETARG_ARRAYTYPE_P(0);
    ArrayType      *b = PG_GETARG_ARRAYTYPE_P(1);
    Oid             elem_type;
    int16           elmlen;
    bool            elmbyval;
    char            elmalign;
    Datum          *ea, *eb;
    int             na = 0, nb = 0;
    Datum          *result;
    int             result_count = 0;
    int             i;
    TypeCacheEntry *tc;
    Oid             collation = PG_GET_COLLATION();

    if (ARR_NDIM(a) == 0)
        PG_RETURN_ARRAYTYPE_P(construct_empty_array(ARR_ELEMTYPE(a)));

    elem_type = ARR_ELEMTYPE(a);
    get_typlenbyvalalign(elem_type, &elmlen, &elmbyval, &elmalign);

    check_array(a, elem_type, elmlen, elmbyval, elmalign, &ea, &na);
    if (ARR_NDIM(b) > 0)
        check_array(b, elem_type, elmlen, elmbyval, elmalign, &eb, &nb);

    tc = lookup_type_cache(elem_type,
                           TYPECACHE_HASH_PROC_FINFO |
                           TYPECACHE_EQ_OPR_FINFO);

    result = (Datum *) palloc(sizeof(Datum) * na);

    for (i = 0; i < na; i++)
        if (!contains(eb, nb, ea[i], tc, collation) &&
            !contains(result, result_count, ea[i], tc, collation))
            result[result_count++] = ea[i];

    PG_RETURN_ARRAYTYPE_P(construct_array(result, result_count,
                                          elem_type, elmlen, elmbyval, elmalign));
}

PG_FUNCTION_INFO_V1(array_symmetric_difference);

Datum
array_symmetric_difference(PG_FUNCTION_ARGS)
{
    ArrayType      *a = PG_GETARG_ARRAYTYPE_P(0);
    ArrayType      *b = PG_GETARG_ARRAYTYPE_P(1);
    Oid             elem_type;
    int16           elmlen;
    bool            elmbyval;
    char            elmalign;
    Datum          *ea, *eb;
    int             na = 0, nb = 0;
    Datum          *result;
    int             result_count = 0;
    int             i;
    TypeCacheEntry *tc;
    Oid             collation = PG_GET_COLLATION();

    if (ARR_NDIM(a) == 0 && ARR_NDIM(b) == 0)
        PG_RETURN_ARRAYTYPE_P(construct_empty_array(ARR_ELEMTYPE(a)));

    elem_type = (ARR_NDIM(a) > 0) ? ARR_ELEMTYPE(a) : ARR_ELEMTYPE(b);
    get_typlenbyvalalign(elem_type, &elmlen, &elmbyval, &elmalign);

    if (ARR_NDIM(a) > 0)
        check_array(a, elem_type, elmlen, elmbyval, elmalign, &ea, &na);
    if (ARR_NDIM(b) > 0)
        check_array(b, elem_type, elmlen, elmbyval, elmalign, &eb, &nb);

    tc = lookup_type_cache(elem_type,
                           TYPECACHE_HASH_PROC_FINFO |
                           TYPECACHE_EQ_OPR_FINFO);

    result = (Datum *) palloc(sizeof(Datum) * (na + nb + 1));

    for (i = 0; i < na; i++)
        if (!contains(eb, nb, ea[i], tc, collation) &&
            !contains(result, result_count, ea[i], tc, collation))
            result[result_count++] = ea[i];

    for (i = 0; i < nb; i++)
        if (!contains(ea, na, eb[i], tc, collation) &&
            !contains(result, result_count, eb[i], tc, collation))
            result[result_count++] = eb[i];

    PG_RETURN_ARRAYTYPE_P(construct_array(result, result_count,
                                          elem_type, elmlen, elmbyval, elmalign));
}
