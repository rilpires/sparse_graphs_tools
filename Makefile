
default: partitions sparse_wiener tetrahedron thetagraph

ifeq ($(DEBUG),TRUE)
CFLAGS += -g
else
CFLAGS += -O3
endif

.FORCE:

sparse_wiener: .FORCE
	$(CC) $(CFLAGS) $(@).c graph.c vector.c list.c wiener.c -o $(@)

partitions: .FORCE
	$(CC) $(CFLAGS) partitions_main.c partitions.c vector.c -o $(@)

thetagraph tetrahedron: .FORCE
	$(CC) $(CFLAGS) $(@).c graph.c vector.c list.c wiener.c partitions.c -o $(@)

