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

def navigate(c, start, av):
    if start in av:
        return 0
    else:
        av.add(start)

    count = 1

    next = c[start]
    for n in next:
        count += navigate(c, n, av)

    return count

def detect_recurse(c, start, av):
    if start in av:
        return start
    else:
        av.add(start)

    next = c[start]
    for n in next:
        recurse = detect_recurse(c, n, av)
        if recurse:
            return (start, recurse)

    return None

def value_pairs(c_orig):
    c = copy.deepcopy(c_orig)
    vp = set()
    for key in c_orig:
        for val in c_orig[key]:
            if key > val:
                vp.add((key, val))
            else:
                vp.add((val, key))

    return vp

def cut_wires(c_orig):
    # av = set()
    for c in c_orig:
        print(c, len(c_orig[c]))
    # print(detect_recurse(c_orig, c_orig.keys()[0], av))
    # vp = value_pairs(c_orig)
    # l = 0
    # for a, b, c in combinations(vp, 3):
        # # print(a, b, c)
        # l += 1

    # print(l, len(vp))


# sys.setrecursionlimit(1000000)

# nodes, edges = parse_components("input_test.txt")
nodes, edges = parse_components("input.txt")

G = nx.Graph()
G.add_nodes_from(nodes)
G.add_edges_from(edges)

print(len(list(k_edge_components(G, 4))))

# for node in G:
    # print(node, G.degree(node))


# av = set()
# size = navigate(c, 'fss', av)
