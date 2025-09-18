# Integer Set

Compactly representing bitsets as graphs. This is often required when you are computing multiple bitsets, but they 
cannot all fit in primary memory. Given that this representation provides a compact representation where multiple elements
are sharing the same nodes, we are considering avoiding wasteful allocation. This would be increasingly important while 
building subsets of elements

## Dependencies 

```bash
apt-get install -y libgmp-dev
```