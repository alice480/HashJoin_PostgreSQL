# HashJoin_PostgreSQL

#### План запроса
---
```SQL
postgres=# create table t1 as select gen a, gen % 100 b from generate_series(1, 10000) gen;
SELECT 10000
postgres=# create table t2 as select gen a, gen % 100 b from generate_series(1, 10000) gen;
SELECT 10000
postgres=# analyze t1;
ANALYZE
postgres=# analyze t2;
ANALYZE
postgres=# explain (analyze, costs off) select t1.*, t2.* from t1 join t2 using(a);
QUERY PLAN
--------------------------------------------------------------------------
Hash Join (actual time=2.217..5.807 rows=10000 loops=1)
Hash Cond: (t1.a = t2.a)
-> Seq Scan on t1 (actual time=0.062..0.798 rows=10000 loops=1)
-> Hash (actual time=2.102..2.103 rows=10000 loops=1)
Buckets: 16384 Batches: 1 Memory Usage: 519kB
-> Seq Scan on t2 (actual time=0.020..0.818 rows=10000 loops=1)
Planning Time: 1.056 ms
Execution Time: 6.245 ms
(8 rows)
```
---

1. Описать алгоритм соединения таблиц, соответствующий приведенному выше
плану запроса;
2. Описать существующую реализацию hash-таблицы, используемой при
соединении;
3. Описать, как обрабатываются значения Null;
4. Описать логику обработки ситуации, когда hash-таблица не помещается в доступную процессу память;

Описание необходимо сопроводить ссылками на код на зеркале репозитория на GitHub
(https://github.com/postgres/postgres). За основу взять тег, соответствующий версии 15.2.
