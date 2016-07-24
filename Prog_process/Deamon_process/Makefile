bin = test
src = Deamon_p.c
cc = gcc

$(bin):$(src)
	$(cc) -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm -f $(bin)
