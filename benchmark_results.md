# VectorPro Benchmark Results

## Observer

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Push Back, 0 listeners | 10K | 32.77 ms |
| Push Back, 0 listeners | 100K | 322.83 ms |
| Push Back, 0 listeners | 1M | 3.34 s |
| Push Back, 1 listener | 10K | 95.05 ms |
| Push Back, 1 listener | 100K | 1.01 s |
| Push Back, 1 listener | 1M | 25.24 s |
| Push Back, 8 listeners | 10K | 1.02 s |
| Push Back, 8 listeners | 100K | 10.46 s |
| Push Back, 8 listeners | 1M | 103.95 s |
| Push Back, capturing listener | 10K | 253.06 ms |
| Push Back, capturing listener | 100K | 2.58 s |
| Push Back, capturing listener | 1M | 25.82 s |
| Subscribe x100 | 10K | 98.03 ms |
| Subscribe x100 | 100K | 977.37 ms |
| Subscribe x100 | 1M | 9.69 s |
| Unsubscribe x100 | 10K | 1.07 s |
| Unsubscribe x100 | 100K | 10.44 s |
| Unsubscribe x100 | 1M | 101.65 s |
