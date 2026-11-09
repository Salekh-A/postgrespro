SELECT array_union(ARRAY[1,2]::int[], ARRAY['1','2']::int[]);
SELECT array_union(ARRAY[1,2], ARRAY['1','2']);
SELECT array_intersection(ARRAY['a','b']::varchar[], ARRAY['b','c']::varchar[]);
SELECT array_except(ARRAY[1.5, 2.5]::numeric[], ARRAY[2.5]::numeric[]);
SELECT array_unique(ARRAY[true, false, true]::boolean[]);
