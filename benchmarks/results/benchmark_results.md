# VectorPro Benchmark Results

## Element Access

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| At() | 10K | 2.81 ms | 2.71 ms | -3.4% |
| At() | 100K | 27.02 ms | 27.04 ms | +0.0% |
| At() | 1M | 270.63 ms | 269.21 ms | -0.5% |
| Front/back | 10K | 13.54 us | 16.69 us | +23.3% |
| Front/back | 100K | 166.85 us | 129.85 us | -22.2% |
| Front/back | 1M | 1.32 ms | 1.67 ms | +26.2% |
| Data Ptr() | 10K | 2.69 ms | 2.68 ms | -0.4% |
| Data Ptr() | 100K | 26.96 ms | 26.95 ms | -0.0% |
| Data Ptr() | 1M | 269.18 ms | 272.74 ms | +1.3% |

## Iteration

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Range-for | 10K | 2.69 ms | 2.71 ms | +0.5% |
| Range-for | 100K | 27.02 ms | 27.02 ms | -0.0% |
| Range-for | 1M | 269.57 ms | 269.33 ms | -0.1% |
| Iterator loop | 10K | 2.72 ms | 2.69 ms | -0.8% |
| Iterator loop | 100K | 27.03 ms | 26.95 ms | -0.3% |
| Iterator loop | 1M | 270.60 ms | 269.38 ms | -0.5% |
| Index loop | 10K | 2.70 ms | 2.68 ms | -0.7% |
| Index loop | 100K | 28.73 ms | 26.95 ms | -6.2% |
| Index loop | 1M | 270.01 ms | 269.64 ms | -0.1% |
| Reverse iteration | 10K | 16.24 ms | 8.22 ms | -49.4% |
| Reverse iteration | 100K | 99.03 ms | 86.65 ms | -12.5% |
| Reverse iteration | 1M | 1.17 s | 1.53 s | +31.3% |
| Accumulate | 10K | 9.72 ms | 12.30 ms | +26.5% |
| Accumulate | 100K | 152.02 ms | 91.24 ms | -40.0% |
| Accumulate | 1M | 1.07 s | 9.36 s | +773.7% |

## Search

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Contains hit (1k) | 10K | 13.70 ms | 13.67 ms | -0.2% |
| Contains hit (1k) | 100K | 141.56 ms | 140.31 ms | -0.9% |
| Contains hit (1k) | 1M | 1.39 s | 1.40 s | +0.9% |
| Contains miss (1k) | 10K | 27.28 ms | 27.43 ms | +0.6% |
| Contains miss (1k) | 100K | 273.22 ms | 278.16 ms | +1.8% |
| Contains miss (1k) | 1M | 2.78 s | 2.75 s | -0.9% |
| Contains hit (10k) | 10K | 278.37 ms | 280.90 ms | +0.9% |
| Contains hit (10k) | 100K | 2.79 s | 2.77 s | -0.6% |
| Contains hit (10k) | 1M | 27.74 s | 28.09 s | +1.3% |
| Find hit (1k) | 10K | 13.80 ms | 13.62 ms | -1.4% |
| Find hit (1k) | 100K | 136.84 ms | 138.34 ms | +1.1% |
| Find hit (1k) | 1M | 1.38 s | 1.39 s | +0.8% |
| Find miss (1k) | 10K | 27.89 ms | 27.27 ms | -2.2% |
| Find miss (1k) | 100K | 275.73 ms | 277.83 ms | +0.8% |
| Find miss (1k) | 1M | 2.78 s | 2.78 s | -0.1% |

## Erase

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Erase front | 10K | 7.94 ms | 7.36 ms | -7.2% |
| Erase front | 100K | 72.44 ms | 72.33 ms | -0.2% |
| Erase front | 1M | 732.24 ms | 760.14 ms | +3.8% |
| Erase middle | 10K | 9.79 ms | 9.77 ms | -0.3% |
| Erase middle | 100K | 105.16 ms | 107.74 ms | +2.5% |
| Erase middle | 1M | 1.02 s | 990.20 ms | -3.2% |
| Erase end | 10K | 7.37 ms | 7.14 ms | -3.1% |
| Erase end | 100K | 71.90 ms | 71.16 ms | -1.0% |
| Erase end | 1M | 730.87 ms | 720.99 ms | -1.4% |

## Insert

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Insert front | 10K | 23.22 ms | 20.62 ms | -11.2% |
| Insert front | 100K | 241.71 ms | 200.12 ms | -17.2% |
| Insert front | 1M | 2.37 s | 2.01 s | -15.2% |
| Insert middle | 10K | 21.88 ms | 19.31 ms | -11.8% |
| Insert middle | 100K | 205.46 ms | 189.43 ms | -7.8% |
| Insert middle | 1M | 2.08 s | 1.93 s | -7.2% |
| Insert end | 10K | 17.73 ms | 18.56 ms | +4.7% |
| Insert end | 100K | 178.02 ms | 182.79 ms | +2.7% |
| Insert end | 1M | 1.78 s | 1.84 s | +3.4% |

## Pop Clear

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Pop Back | 10K | 10.00 ms | 10.01 ms | +0.1% |
| Pop Back | 100K | 102.67 ms | 102.11 ms | -0.5% |
| Pop Back | 1M | 1.03 s | 1.04 s | +0.8% |
| Clear | 10K | 10.03 ms | 10.35 ms | +3.2% |
| Clear | 100K | 102.30 ms | 104.29 ms | +2.0% |
| Clear | 1M | 1.05 s | 1.02 s | -2.9% |

## Push Back

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Push Back | 10K | 94.62 ms | 96.47 ms | +2.0% |
| Push Back | 100K | 966.24 ms | 947.00 ms | -2.0% |
| Push Back | 1M | 10.01 s | 9.62 s | -3.9% |
| Push Back reserved | 10K | 55.20 ms | 36.15 ms | -34.5% |
| Push Back reserved | 100K | 555.66 ms | 351.13 ms | -36.8% |
| Push Back reserved | 1M | 5.57 s | 3.63 s | -34.8% |
| Emplace Back | 10K | 41.27 ms | 41.37 ms | +0.2% |
| Emplace Back | 100K | 427.32 ms | 415.43 ms | -2.8% |
| Emplace Back | 1M | 4.17 s | 4.26 s | +2.2% |

## Remove If

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Erase-remove If | 10K | 112.59 ms | 78.25 ms | -30.5% |
| Erase-remove If | 100K | 1.15 s | 970.08 ms | -15.4% |
| Erase-remove If | 1M | 12.56 s | 8.22 s | -34.5% |

## Construction

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Default construct | 10K | 33.54 us | 33.54 us | +0.0% |
| Default construct | 100K | 333.54 us | 333.31 us | -0.1% |
| Default construct | 1M | 3.39 ms | 3.38 ms | -0.5% |
| Fill construct | 10K | 4.35 ms | 4.11 ms | -5.5% |
| Fill construct | 100K | 42.51 ms | 42.65 ms | +0.3% |
| Fill construct | 1M | 421.16 ms | 421.96 ms | +0.2% |
| Init list construct | 10K | 3.57 ms | 3.56 ms | -0.4% |
| Init list construct | 100K | 35.40 ms | 40.43 ms | +14.2% |
| Init list construct | 1M | 359.42 ms | 362.67 ms | +0.9% |

## Copy

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Copy construct | 10K | 13.98 ms | 13.12 ms | -6.1% |
| Copy construct | 100K | 146.20 ms | 133.81 ms | -8.5% |
| Copy construct | 1M | 1.40 s | 1.34 s | -4.4% |
| Copy assignment | 10K | 7.00 us | 6.16 ms | +87878.0% |
| Copy assignment | 100K | 66.77 us | 61.89 ms | +92587.4% |
| Copy assignment | 1M | 667.00 us | 624.38 ms | +93509.9% |

## Move

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Move construct | 10K | 4.42 ms | 4.24 ms | -4.1% |
| Move construct | 100K | 42.23 ms | 41.91 ms | -0.7% |
| Move construct | 1M | 419.21 ms | 424.45 ms | +1.2% |
| Move assignment | 10K | 4.14 ms | 4.30 ms | +4.0% |
| Move assignment | 100K | 41.18 ms | 41.19 ms | +0.0% |
| Move assignment | 1M | 422.55 ms | 418.14 ms | -1.0% |

## Reallocation

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Growth reallocation | 10K | 25.06 ms | 13.30 ms | -47.0% |
| Growth reallocation | 100K | 249.72 ms | 132.61 ms | -46.9% |
| Growth reallocation | 1M | 2.52 s | 1.34 s | -46.9% |

## Reserve

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Reserve | 10K | 6.64 ms | 6.69 ms | +0.7% |
| Reserve | 100K | 66.66 ms | 66.67 ms | +0.0% |
| Reserve | 1M | 671.06 ms | 670.82 ms | -0.0% |
| Reserve growth | 10K | 54.05 ms | 59.18 ms | +9.5% |
| Reserve growth | 100K | 553.99 ms | 603.01 ms | +8.8% |
| Reserve growth | 1M | 5.50 s | 6.03 s | +9.7% |
| Reserve no-op | 10K | 27.08 us | 40.23 us | +48.6% |
| Reserve no-op | 100K | 267.00 us | 400.31 us | +49.9% |
| Reserve no-op | 1M | 2.73 ms | 4.05 ms | +48.6% |

## Shrink

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Shrink To Fit | 10K | 19.21 ms | 15.12 ms | -21.3% |
| Shrink To Fit | 100K | 197.68 ms | 155.92 ms | -21.1% |
| Shrink To Fit | 1M | 1.93 s | 1.53 s | -20.7% |

## Comparison

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Operator== equal | 10K | 182.34 ms | 176.16 ms | -3.4% |
| Operator== equal | 100K | 1.84 s | 1.78 s | -3.1% |
| Operator== equal | 1M | 18.77 s | 18.22 s | -2.9% |
| Operator== differs early | 10K | 174.15 us | 147.00 us | -15.6% |
| Operator== differs early | 100K | 1.80 ms | 1.47 ms | -18.7% |
| Operator== differs early | 1M | 17.58 ms | 14.83 ms | -15.6% |
| Operator== differs late | 10K | 181.00 ms | 176.05 ms | -2.7% |
| Operator== differs late | 100K | 1.90 s | 1.82 s | -4.2% |
| Operator== differs late | 1M | 18.45 s | 18.13 s | -1.8% |
| Operator== diff sizes | 10K | 13.62 us | 6.77 us | -50.3% |
| Operator== diff sizes | 100K | 134.38 us | 66.77 us | -50.3% |
| Operator== diff sizes | 1M | 1.45 ms | 668.00 us | -54.1% |
| Operator<=> equal | 10K | 415.53 ms | 414.15 ms | -0.3% |
| Operator<=> equal | 100K | 4.04 s | 4.17 s | +3.0% |
| Operator<=> equal | 1M | 40.01 s | 42.53 s | +6.3% |
| Operator<=> less | 10K | 107.00 us | 67.08 us | -37.3% |
| Operator<=> less | 100K | 1.13 ms | 667.08 us | -41.0% |
| Operator<=> less | 1M | 10.86 ms | 7.11 ms | -34.5% |

## Observer

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Push Back, 0 listeners | 10K | 56.52 ms |
| Push Back, 0 listeners | 100K | 553.54 ms |
| Push Back, 0 listeners | 1M | 4.15 s |
| Push Back, 1 listener | 10K | 219.12 ms |
| Push Back, 1 listener | 100K | 2.20 s |
| Push Back, 1 listener | 1M | 22.00 s |
| Push Back, 8 listeners | 10K | 1.05 s |
| Push Back, 8 listeners | 100K | 10.45 s |
| Push Back, 8 listeners | 1M | 114.24 s |
| Push Back, capturing listener | 10K | 258.49 ms |
| Push Back, capturing listener | 100K | 2.57 s |
| Push Back, capturing listener | 1M | 25.98 s |
| Subscribe x100 | 10K | 105.10 ms |
| Subscribe x100 | 100K | 1.02 s |
| Subscribe x100 | 1M | 10.11 s |
| Unsubscribe x100 | 10K | 1.08 s |
| Unsubscribe x100 | 100K | 10.84 s |
| Unsubscribe x100 | 1M | 311.20 s |

## Swap

| Test | Iteration | VectorPro | std::vector | Δ |
|---|---|---|---|---|
| Member swap | 10K | 23.73 ms | 22.09 ms | -6.9% |
| Member swap | 100K | 228.26 ms | 222.32 ms | -2.6% |
| Member swap | 1M | 2.32 s | 2.19 s | -5.7% |
| Free swap | 10K | 21.88 ms | 21.74 ms | -0.6% |
| Free swap | 100K | 224.55 ms | 218.61 ms | -2.6% |
| Free swap | 1M | 2.25 s | 2.19 s | -2.7% |
