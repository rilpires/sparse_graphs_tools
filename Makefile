

.FORCE:

sparse_wiener: .FORCE
	$(CC) $(@).c graph.c vector.c list.c wiener.c -o $(@)

partitions: .FORCE
	$(CC) $(@).c vector.c -o $(@)

