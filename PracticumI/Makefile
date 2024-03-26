NAME = test_store test_hashtable test_lrucache test_randomcache test_message test_message_store evaluation

CC = gcc
CFLAGS = -g

all: $(NAME)

.o:
	$(CC) $(CFLAGS) -c $<

hashtable.o: hashtable.c hashtable.h
test_hashtable.o: test_hashtable.c test.h


store.o: store.c store.h 
test_store.o: test_store.c test.h

lrucache.o: lrucache.c lrucache.h
test_lrucache.o: test_lrucache.c test.h

randomcache.o: randomcache.c randomcache.h
test_randomcache.o: test_randomcache.c test.h

message.o: message.c message.h
test_message.o:test_message.c test.h

message_store.o: message_store.c message_store.h
test_message_store.o: test_message_store.c test.h

evaluation.o :evaluation.c

test_hashtable: test_hashtable.o hashtable.o
	$(CC) $(CFLAGS) $^ -o $@
test_store: test_store.o store.o hashtable.o
	$(CC) $(CFLAGS) $^ -o $@
test_lrucache: test_lrucache.o lrucache.o hashtable.o
	$(CC) $(CFLAGS) $^ -o $@
test_randomcache: test_randomcache.o randomcache.o hashtable.o
	$(CC) $(CFLAGS) $^ -o $@
test_message:test_message.o message.o 
	$(CC) $(CFLAGS) $^ -o $@
test_message_store:test_message_store.o message_store.o store.o message.o hashtable.o
	$(CC) $(CFLAGS) $^ -o $@
evaluation: evaluation.o message_store.o store.o message.o hashtable.o lrucache.o randomcache.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(NAME) *.o
