/* This is free and unencumbered software released into the public domain. */

#include "bitcache_arch.h"
#include "bitcache_map.h"
#include "config.h"
#include <assert.h>
#include <errno.h>
#include <strings.h>

//////////////////////////////////////////////////////////////////////////////
// Map API

int
bitcache_map_init(bitcache_map_t* map, const GDestroyNotify key_destroy_func, const GDestroyNotify value_destroy_func) {
  if (unlikely(map == NULL))
    return -(errno = EINVAL); // invalid argument

  bzero(map, sizeof(bitcache_map_t));

  bitcache_map_crlock(map);
  map->hash_table = g_hash_table_new_full(
    (GHashFunc)bitcache_id_hash,
    (GEqualFunc)bitcache_id_equal,
    key_destroy_func, value_destroy_func);

  return 0;
}

int
bitcache_map_reset(bitcache_map_t* map) {
  if (unlikely(map == NULL))
    return -(errno = EINVAL); // invalid argument

  bitcache_map_rmlock(map);
  if (likely(map->hash_table != NULL)) {
    g_hash_table_destroy(map->hash_table);
    map->hash_table = NULL;
  }

  return 0;
}

int
bitcache_map_clear(bitcache_map_t* map) {
  if (unlikely(map == NULL))
    return -(errno = EINVAL); // invalid argument

  bitcache_map_wrlock(map);
  if (likely(map->hash_table != NULL)) {
    g_hash_table_remove_all(map->hash_table);
  }
  bitcache_map_unlock(map);

  return 0;
}

ssize_t
bitcache_map_count(bitcache_map_t* map) {
  if (unlikely(map == NULL))
    return -(errno = EINVAL); // invalid argument

  ssize_t count = 0;

  bitcache_map_rdlock(map);
  if (likely(map->hash_table != NULL)) {
    count += g_hash_table_size(map->hash_table);
  }
  bitcache_map_unlock(map);

  return count;
}

bool
bitcache_map_lookup(bitcache_map_t* map, const bitcache_id_t* key, void** value) {
  if (unlikely(map == NULL || key == NULL))
    return errno = EINVAL, FALSE; // invalid argument

  bool found = FALSE;

  bitcache_map_rdlock(map);
  if (likely(map->hash_table != NULL)) {
    found = g_hash_table_lookup_extended(map->hash_table, key, NULL, value);
  }
  bitcache_map_unlock(map);

  return found;
}

int
bitcache_map_insert(bitcache_map_t* map, const bitcache_id_t* key, const void* value) {
  if (unlikely(map == NULL || key == NULL))
    return -(errno = EINVAL); // invalid argument

  bitcache_map_wrlock(map);
  if (likely(map->hash_table != NULL)) {
    g_hash_table_insert(map->hash_table, (void*)key, (void*)value);
  }
  else {
    assert(map->hash_table != NULL);
  }
  bitcache_map_unlock(map);

  return 0;
}

int
bitcache_map_remove(bitcache_map_t* map, const bitcache_id_t* key) {
  if (unlikely(map == NULL || key == NULL))
    return -(errno = EINVAL); // invalid argument

  bitcache_map_wrlock(map);
  if (likely(map->hash_table != NULL)) {
    g_hash_table_remove(map->hash_table, (void*)key);
  }
  bitcache_map_unlock(map);

  return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Map Iterator API

int
bitcache_map_iter_init(bitcache_map_iter_t* iter, bitcache_map_t* map) {
  if (unlikely(iter == NULL || map == NULL))
    return -(errno = EINVAL); // invalid argument

  bzero(iter, sizeof(bitcache_map_iter_t));
  iter->map = map;
  g_hash_table_iter_init(&iter->hash_table_iter, map->hash_table);

  return 0;
}

bool
bitcache_map_iter_next(bitcache_map_iter_t* iter, bitcache_id_t** key, void** value) {
  if (unlikely(iter == NULL || iter->map == NULL))
    return errno = EINVAL, FALSE; // invalid argument

  int more = FALSE;

  if (likely(g_hash_table_iter_next(&iter->hash_table_iter, (void**)key, value) != FALSE)) {
    iter->position++;
    more = TRUE;
  }

  return more;
}

int
bitcache_map_iter_remove(bitcache_map_iter_t* iter) {
  if (unlikely(iter == NULL || iter->map == NULL))
    return -(errno = EINVAL); // invalid argument

  g_hash_table_iter_remove(&iter->hash_table_iter);

  return 0;
}

int
bitcache_map_iter_done(bitcache_map_iter_t* iter) {
  if (unlikely(iter == NULL || iter->map == NULL))
    return -(errno = EINVAL); // invalid argument

  bzero(iter, sizeof(bitcache_map_iter_t));

  return 0;
}
