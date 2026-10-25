SELECT array_intersection(ARRAY[]::int[], ARRAY[]::int[]);
SELECT array_union(ARRAY[]::int[], ARRAY[1,2]);
SELECT array_except(ARRAY[1,2], ARRAY[]::int[]);
SELECT array_symmetric_difference(ARRAY[]::int[], ARRAY[]::int[]);
SELECT array_unique(ARRAY[]::text[]);
