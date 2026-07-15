# VectorPro Regression Report

## Element Access

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| at() | 10K | 263.16 ns | 264.84 ns | +0.6% |
| at() | 100K | 255.87 ns | 263.55 ns | +3.0% |
| at() | 1M | 255.19 ns | 262.68 ns | +2.9% |
| front/back | 10K | 1.23 ns | 1.35 ns | +9.8% |
| front/back | 100K | 1.79 ns | 1.62 ns | -9.5% |
| front/back | 1M | 1.23 ns | 1.26 ns | +2.4% |
| data_ptr() | 10K | 255.94 ns | 260.99 ns | +2.0% |
| data_ptr() | 100K | 255.62 ns | 262.69 ns | +2.8% |
| data_ptr() | 1M | 255.05 ns | 262.02 ns | +2.7% |

## Iteration

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| range-for | 10K | 257.98 ns | 264.33 ns | +2.5% |
| range-for | 100K | 255.95 ns | 262.9 ns | +2.7% |
| range-for | 1M | 255.46 ns | 262.42 ns | +2.7% |
| iterator loop | 10K | 254.78 ns | 261.68 ns | +2.7% |
| iterator loop | 100K | 255.8 ns | 263.27 ns | +2.9% |
| iterator loop | 1M | 256.46 ns | 263.53 ns | +2.8% |
| index loop | 10K | 253.39 ns | 260.04 ns | +2.6% |
| index loop | 100K | 254.46 ns | 262.03 ns | +3.0% |
| index loop | 1M | 255.51 ns | 262.48 ns | +2.7% |
| reverse iteration | 10K | 442.62 ns | 455.35 ns | +2.9% |
| reverse iteration | 100K | 444.86 ns | 456.74 ns | +2.7% |
| reverse iteration | 1M | 443.12 ns | 455.5 ns | +2.8% |
| accumulate | 10K | 256.86 ns | 723.45 ns | +181.7% |
| accumulate | 100K | 255.72 ns | 655.12 ns | +156.2% |
| accumulate | 1M | 261.59 ns | 387.34 ns | +48.1% |

## Search

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| contains hit (1k) | 10K | 545.08 ns | 560.16 ns | +2.8% |
| contains hit (1k) | 100K | 543.67 ns | 578.82 ns | +6.5% |
| contains hit (1k) | 1M | 543.19 ns | 558.52 ns | +2.8% |
| contains miss (1k) | 10K | 1070.65 ns | 1105.11 ns | +3.2% |
| contains miss (1k) | 100K | 1070.49 ns | 1103.87 ns | +3.1% |
| contains miss (1k) | 1M | 1068.98 ns | 1101.95 ns | +3.1% |
| contains hit (10k) | 10K | 10577.7 ns | 12040.3 ns | +13.8% |
| contains hit (10k) | 100K | 10566.5 ns | 11816.5 ns | +11.8% |
| contains hit (10k) | 1M | 10563.6 ns | 24835.2 ns | +135.1% |
| find hit (1k) | 10K | 544.68 ns | 1265.94 ns | +132.4% |
| find hit (1k) | 100K | 548.45 ns | 1257.96 ns | +129.4% |
| find hit (1k) | 1M | 685.26 ns | 1256.4 ns | +83.3% |
| find miss (1k) | 10K | 1088.28 ns | 2507.02 ns | +130.4% |
| find miss (1k) | 100K | 3624.55 ns | 2500.76 ns | -31.0% |
| find miss (1k) | 1M | 1756.04 ns | 2501.5 ns | +42.5% |

## Erase

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| erase front | 10K | 243.02 ns | 596.42 ns | +145.4% |
| erase front | 100K | 261.19 ns | 581.67 ns | +122.7% |
| erase front | 1M | 533.32 ns | 586.36 ns | +9.9% |
| erase middle | 10K | 319.91 ns | 826.06 ns | +158.2% |
| erase middle | 100K | 319.19 ns | 821.53 ns | +157.4% |
| erase middle | 1M | 773.39 ns | 821.14 ns | +6.2% |
| erase end | 10K | 593.16 ns | 584.56 ns | -1.4% |
| erase end | 100K | 585.06 ns | 584.13 ns | -0.2% |
| erase end | 1M | 579.78 ns | 583.72 ns | +0.7% |

## Insert

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| insert front | 10K | 1995.69 ns | 2037.81 ns | +2.1% |
| insert front | 100K | 1992.83 ns | 2019.92 ns | +1.4% |
| insert front | 1M | 6735.59 ns | 2058.23 ns | -69.4% |
| insert middle | 10K | 1976.12 ns | 1749.27 ns | -11.5% |
| insert middle | 100K | 965.82 ns | 1765.73 ns | +82.8% |
| insert middle | 1M | 733.82 ns | 1810.91 ns | +146.8% |
| insert end | 10K | 566.45 ns | 1486.12 ns | +162.4% |
| insert end | 100K | 845.04 ns | 1484.07 ns | +75.6% |
| insert end | 1M | 854.76 ns | 1491.53 ns | +74.5% |

## Pop Clear

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| pop_back | 10K | 468.68 ns | 858.58 ns | +83.2% |
| pop_back | 100K | 349.48 ns | 856.05 ns | +144.9% |
| pop_back | 1M | 459.81 ns | 862.46 ns | +87.6% |
| clear | 10K | 362.41 ns | 858.27 ns | +136.8% |
| clear | 100K | 508.78 ns | 852.24 ns | +67.5% |
| clear | 1M | 376.01 ns | 853.99 ns | +127.1% |

## Push Back

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| push_back | 10K | 4331.84 ns | 8103.51 ns | +87.1% |
| push_back | 100K | 4906.33 ns | 8087.66 ns | +64.8% |
| push_back | 1M | 7534.77 ns | 8164 ns | +8.4% |
| push_back reserved | 10K | 5009.12 ns | 4923.93 ns | -1.7% |
| push_back reserved | 100K | 4927.82 ns | 4928.52 ns | +0.0% |
| push_back reserved | 1M | 4984.92 ns | 4988.46 ns | +0.1% |
| emplace_back | 10K | 3711.51 ns | 3694.37 ns | -0.5% |
| emplace_back | 100K | 3702.1 ns | 3696.5 ns | -0.2% |
| emplace_back | 1M | 3705.48 ns | 3707.2 ns | +0.0% |

## Remove If

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| erase-remove_if | 10K | 10210.4 ns | 10245.8 ns | +0.3% |
| erase-remove_if | 100K | 10372 ns | 10251.8 ns | -1.2% |
| erase-remove_if | 1M | 10260.2 ns | 10392.8 ns | +1.3% |

## Construction

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default construct | 10K | 5.29 ns | 3.11 ns | -41.2% |
| default construct | 100K | 5.27 ns | 3.71 ns | -29.6% |
| default construct | 1M | 5.29 ns | 3.08 ns | -41.8% |
| fill construct | 10K | 154.74 ns | 332.84 ns | +115.1% |
| fill construct | 100K | 145.23 ns | 317.54 ns | +118.6% |
| fill construct | 1M | 224.57 ns | 317.24 ns | +41.3% |
| init list construct | 10K | 125.96 ns | 267.38 ns | +112.3% |
| init list construct | 100K | 211.67 ns | 270.55 ns | +27.8% |
| init list construct | 1M | 126.17 ns | 272.44 ns | +115.9% |

## Copy

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| copy construct | 10K | 701.6 ns | 1132.46 ns | +61.4% |
| copy construct | 100K | 704.03 ns | 1130.38 ns | +60.6% |
| copy construct | 1M | 810.53 ns | 1132.95 ns | +39.8% |
| copy assignment | 10K | 0.55 ns | 0.65 ns | +18.2% |
| copy assignment | 100K | 0.53 ns | 0.62 ns | +17.0% |
| copy assignment | 1M | 0.53 ns | 0.62 ns | +17.0% |

## Move

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| move construct | 10K | 345.02 ns | 320.58 ns | -7.1% |
| move construct | 100K | 482.74 ns | 316.86 ns | -34.4% |
| move construct | 1M | 290.44 ns | 316.47 ns | +9.0% |
| move assignment | 10K | 352.35 ns | 317.65 ns | -9.8% |
| move assignment | 100K | 146.9 ns | 318.58 ns | +116.9% |
| move assignment | 1M | 203.18 ns | 316.26 ns | +55.7% |

## Reallocation

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| growth reallocation | 10K | 905.08 ns | 2131.02 ns | +135.5% |
| growth reallocation | 100K | 1047.96 ns | 2120.44 ns | +102.3% |
| growth reallocation | 1M | 1090.72 ns | 2118.87 ns | +94.3% |

## Reserve

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| reserve | 10K | 253.1 ns | 562.39 ns | +122.2% |
| reserve | 100K | 506.29 ns | 556.58 ns | +9.9% |
| reserve | 1M | 391.3 ns | 558.08 ns | +42.6% |
| reserve growth | 10K | 3313.5 ns | 5010.76 ns | +51.2% |
| reserve growth | 100K | 3011.9 ns | 5009.34 ns | +66.3% |
| reserve growth | 1M | 2267.07 ns | 5265.89 ns | +132.3% |
| reserve no-op | 10K | 1.05 ns | 1.1 ns | +4.8% |
| reserve no-op | 100K | 1.05 ns | 1.08 ns | +2.9% |
| reserve no-op | 1M | 1.05 ns | 1.83 ns | +74.3% |

## Shrink

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| shrink_to_fit | 10K | 842.62 ns | 1756.44 ns | +108.4% |
| shrink_to_fit | 100K | 944.96 ns | 981.11 ns | +3.8% |
| shrink_to_fit | 1M | 890.48 ns | 908.71 ns | +2.0% |

## Comparison

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| operator== equal | 10K | 6188.22 ns | 6715.15 ns | +8.5% |
| operator== equal | 100K | 15727.7 ns | 8382.48 ns | -46.7% |
| operator== equal | 1M | 6358.49 ns | 7152.03 ns | +12.5% |
| operator== differs early | 10K | 6.37 ns | 6.52 ns | +2.4% |
| operator== differs early | 100K | 6.71 ns | 6.53 ns | -2.7% |
| operator== differs early | 1M | 6.33 ns | 6.55 ns | +3.5% |
| operator== differs late | 10K | 6183.39 ns | 7141.23 ns | +15.5% |
| operator== differs late | 100K | 6209.86 ns | 6409.93 ns | +3.2% |
| operator== differs late | 1M | 6673.55 ns | 6536.12 ns | -2.1% |
| operator== diff sizes | 10K | 1.07 ns | 1.1 ns | +2.8% |
| operator== diff sizes | 100K | 1.05 ns | 1.08 ns | +2.9% |
| operator== diff sizes | 1M | 0.53 ns | 0.54 ns | +1.9% |
| operator<=> equal | 10K | 11879.9 ns | 12195.8 ns | +2.7% |
| operator<=> equal | 100K | 11738.3 ns | 11943.2 ns | +1.7% |
| operator<=> equal | 1M | 20879.5 ns | 46497.2 ns | +122.7% |
| operator<=> less | 10K | 9.87 ns | 3.24 ns | -67.2% |
| operator<=> less | 100K | 9.85 ns | 3.25 ns | -67.0% |
| operator<=> less | 1M | 9.98 ns | 3.29 ns | -67.0% |

## Observer

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| push_back, 0 listeners | 10K | 4945.91 ns | 3081.82 ns | -37.7% |
| push_back, 0 listeners | 100K | 4925.7 ns | 3076.21 ns | -37.5% |
| push_back, 0 listeners | 1M | 3755.69 ns | 3268.54 ns | -13.0% |
| push_back, 1 listener | 10K | 19672.7 ns | 9596.84 ns | -51.2% |
| push_back, 1 listener | 100K | 19678.3 ns | 9447.09 ns | -52.0% |
| push_back, 1 listener | 1M | 19669.9 ns | 9553.03 ns | -51.4% |
| push_back, 8 listeners | 10K | 93919.6 ns | 41507.4 ns | -55.8% |
| push_back, 8 listeners | 100K | 94081.3 ns | 41454.9 ns | -55.9% |
| push_back, 8 listeners | 1M | 101176 ns | 61662 ns | -39.1% |
| push_back, capturing listener | 10K | 24309.6 ns | 24475.9 ns | +0.7% |
| push_back, capturing listener | 100K | 23601.8 ns | 23882.8 ns | +1.2% |
| push_back, capturing listener | 1M | 23538.8 ns | 23764.9 ns | +1.0% |
| subscribe x100 | 10K | 9014 ns | 8901.72 ns | -1.2% |
| subscribe x100 | 100K | 8983.87 ns | 8876.98 ns | -1.2% |
| subscribe x100 | 1M | 9020.79 ns | 6460.48 ns | -28.4% |
| unsubscribe x100 | 10K | 99244.5 ns | 41270.2 ns | -58.4% |
| unsubscribe x100 | 100K | 98410.3 ns | 42044.9 ns | -57.3% |
| unsubscribe x100 | 1M | 99043.8 ns | 66592.5 ns | -32.8% |

## Swap

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| member swap | 10K | 1870.77 ns | 1858.74 ns | -0.6% |
| member swap | 100K | 1857.97 ns | 1843.99 ns | -0.8% |
| member swap | 1M | 1859.47 ns | 1844.1 ns | -0.8% |
| free swap | 10K | 1866.71 ns | 1841.54 ns | -1.3% |
| free swap | 100K | 1859.03 ns | 1861.98 ns | +0.2% |
| free swap | 1M | 1622.83 ns | 1852.7 ns | +14.2% |
