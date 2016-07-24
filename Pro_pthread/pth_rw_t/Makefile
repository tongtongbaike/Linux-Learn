bin = test
src = pth_rw_t.c
cc = gcc

$(bin):$(src)
	$(cc) -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm -f $(bin)
