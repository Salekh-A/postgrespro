CREATE EXTENSION IF NOT EXISTS pg_array_set;
SELECT array_intersection(ARRAY[1,2,3,2], ARRAY[2,3,4]);
SELECT array_union(ARRAY[1,2], ARRAY[2,3]);
SELECT array_except(ARRAY[1,2,3,4], ARRAY[2,4]);
SELECT array_symmetric_difference(ARRAY[1,2,3], ARRAY[3,4,5]);
SELECT array_unique(ARRAY[1,2,1,3,2]);
