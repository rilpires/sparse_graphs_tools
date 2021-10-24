Useful programs for calculating the Wiener index of sparse graphs. Instead the O(n^3) complexity from the usual Floyd-Warshall algorithm, this one is O((n-n1-n2)^3), where n1 and n2 are the number of vertices of degree 1 and 2, so for graphs with lots of degree2- vertices, this program runs way faster.


More usability (like importing custom graph data) will soon be added.

The executable programs are:

- `sparse_wiener`     (Used to calculate the Wiener index of any graph, although it is optimized for sparse graphs)
- `partitions`        (Auxiliary tool to generate partitions of any given number)

These programs below are used to explore the best wiener index of such topologies, which are all biconnecteds with (n+2) edges.

- `tetrahedron`
- `thetagraph`
- `four33`
- `cup`

All programs are generated just by running `make`.
