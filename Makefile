
default: partitions sparse_wiener tetrahedron thetagraph four33 cup

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

# Exploratory programs
thetagraph tetrahedron four33 cup : .FORCE
	$(CC) $(CFLAGS) $(@).c graph.c vector.c list.c wiener.c partitions.c -o $(@)

