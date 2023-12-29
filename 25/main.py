from itertools import combinations
from networkx.algorithms.connectivity.edge_kcomponents import k_edge_components
import copy
import networkx as nx
import sys

def parse_components(filename):
    fh = open(filename)

    edges = set()
    nodes = set()

    for line in fh:
        line = line.strip()
        key, vals_str = line.split(':')
        vals = set([x.strip() for x in vals_str.split(' ') if x != ''])

        nodes.add(key)
        
        for val in vals:
            nodes.add(val)
            if key > val:
                edges.add((key, val))
            else:
                edges.add((val, key))

    return nodes, edges

# nodes, edges = parse_components("input_test.txt")
nodes, edges = parse_components("input.txt")

G = nx.Graph()
G.add_nodes_from(nodes)
G.add_edges_from(edges)

# I found this solution by playing around with networkx, but I didn't know why it worked so I asked on reddit.
# https://old.reddit.com/r/adventofcode/comments/18tj28f/2023_day_25_why_does_my_solution_work/?
groups = list(k_edge_components(G, 4))
a = len(groups[0])
b = len(groups[1])

print(a*b)
