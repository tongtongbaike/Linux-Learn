bin = test
src = pth_sem.c
cc = gcc

$(bin):$(src)
	$(cc) -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm -f $(bin)
