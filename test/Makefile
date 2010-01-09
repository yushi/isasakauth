CC=g++
CFLAGS=-Wall -Werror -I/Users/yushi/local/include -L/Users/yushi/local/lib -lgtest

all: exec_test_ttstorage exec_test_memcache_storage

exec_test_ttstorage: test_ttstorage
	./test_ttstorage

test_ttstorage: test_ttstorage.cpp ../src/ttstorage.cpp
	$(CC) $(CFLAGS) $(^) -ltokyotyrant -o $(@)

exec_test_memcache_storage: test_memcache_storage
	./test_memcache_storage

test_memcache_storage: test_memcache_storage.cpp ../src/memcache_storage.cpp
	$(CC) $(CFLAGS) $(^) -lmemcached -o $(@)

clean:
	rm -rf  test_ttstorage test_memcache_storage