# VectorPro Regression Report

## Element Access

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| at() | 10K | 142.54 ns | 280.58 ns | +96.8% |
| at() | 100K | 142.23 ns | 270.23 ns | +90.0% |
| at() | 1M | 141.87 ns | 270.63 ns | +90.8% |
| front/back | 10K | 0.31 ns | 1.35 ns | +335.5% |
| front/back | 100K | 0.3 ns | 1.67 ns | +456.7% |
| front/back | 1M | 0.3 ns | 1.32 ns | +340.0% |
| data_ptr() | 10K | 141.76 ns | 268.92 ns | +89.7% |
| data_ptr() | 100K | 141.72 ns | 269.63 ns | +90.3% |
| data_ptr() | 1M | 149.58 ns | 269.18 ns | +80.0% |

## Iteration

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| range-for | 10K | 142.2 ns | 269.21 ns | +89.3% |
| range-for | 100K | 142.22 ns | 270.22 ns | +90.0% |
| range-for | 1M | 153.19 ns | 269.57 ns | +76.0% |
| iterator loop | 10K | 144.29 ns | 271.64 ns | +88.3% |
| iterator loop | 100K | 151.54 ns | 270.29 ns | +78.4% |
| iterator loop | 1M | 141.76 ns | 270.6 ns | +90.9% |
| index loop | 10K | 142.41 ns | 270.24 ns | +89.8% |
| index loop | 100K | 141.79 ns | 287.34 ns | +102.7% |
| index loop | 1M | 141.93 ns | 270.01 ns | +90.2% |
| reverse iteration | 10K | 142.53 ns | 1623.94 ns | +1039.4% |
| reverse iteration | 100K | 141.99 ns | 990.3 ns | +597.4% |
| reverse iteration | 1M | 142.42 ns | 1167.13 ns | +719.5% |
| accumulate | 10K | 141.72 ns | 972.28 ns | +586.1% |
| accumulate | 100K | 141.4 ns | 1520.17 ns | +975.1% |
| accumulate | 1M | 142.7 ns | 1071.45 ns | +650.8% |

## Search

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| contains hit (1k) | 10K | 85.57 ns | 1369.62 ns | +1500.6% |
| contains hit (1k) | 100K | 86.29 ns | 1415.64 ns | +1540.6% |
| contains hit (1k) | 1M | 86.28 ns | 1386.97 ns | +1507.5% |
| contains miss (1k) | 10K | 179.93 ns | 2727.51 ns | +1415.9% |
| contains miss (1k) | 100K | 179.86 ns | 2732.23 ns | +1419.1% |
| contains miss (1k) | 1M | 180.4 ns | 2777.86 ns | +1439.8% |
| contains hit (10k) | 10K | 1719.07 ns | 27837.3 ns | +1519.3% |
| contains hit (10k) | 100K | 1722.16 ns | 27867.2 ns | +1518.2% |
| contains hit (10k) | 1M | 1742.61 ns | 27741.1 ns | +1491.9% |
| find hit (1k) | 10K | 93.23 ns | 1380.38 ns | +1380.6% |
| find hit (1k) | 100K | 90.82 ns | 1368.41 ns | +1406.7% |
| find hit (1k) | 1M | 90.61 ns | 1377.83 ns | +1420.6% |
| find miss (1k) | 10K | 304.06 ns | 2789.23 ns | +817.3% |
| find miss (1k) | 100K | 181.9 ns | 2757.31 ns | +1415.8% |
| find miss (1k) | 1M | 180.75 ns | 2781.01 ns | +1438.6% |

## Erase

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| erase front | 10K | 73.71 ns | 793.51 ns | +976.5% |
| erase front | 100K | 76.34 ns | 724.38 ns | +848.9% |
| erase front | 1M | 76.85 ns | 732.24 ns | +852.8% |
| erase middle | 10K | 68.88 ns | 979.48 ns | +1322.0% |
| erase middle | 100K | 67.87 ns | 1051.55 ns | +1449.4% |
| erase middle | 1M | 68.23 ns | 1022.91 ns | +1399.2% |
| erase end | 10K | 51.26 ns | 736.83 ns | +1337.4% |
| erase end | 100K | 51.36 ns | 718.97 ns | +1299.9% |
| erase end | 1M | 51.33 ns | 730.87 ns | +1323.9% |

## Insert

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| insert front | 10K | 137.45 ns | 2322.49 ns | +1589.7% |
| insert front | 100K | 136.43 ns | 2417.1 ns | +1671.7% |
| insert front | 1M | 138.15 ns | 2366.76 ns | +1613.2% |
| insert middle | 10K | 124.3 ns | 2188.32 ns | +1660.5% |
| insert middle | 100K | 127.24 ns | 2054.58 ns | +1514.7% |
| insert middle | 1M | 125.83 ns | 2084.71 ns | +1556.8% |
| insert end | 10K | 117.71 ns | 1773.44 ns | +1406.6% |
| insert end | 100K | 118.66 ns | 1780.23 ns | +1400.3% |
| insert end | 1M | 118.56 ns | 1779.53 ns | +1401.0% |

## Pop Clear

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| pop_back | 10K | 64.27 ns | 1000.34 ns | +1456.5% |
| pop_back | 100K | 63.92 ns | 1026.68 ns | +1506.2% |
| pop_back | 1M | 63.81 ns | 1029.69 ns | +1513.7% |
| clear | 10K | 63.69 ns | 1002.96 ns | +1474.8% |
| clear | 100K | 64.01 ns | 1022.98 ns | +1498.2% |
| clear | 1M | 63.59 ns | 1051.75 ns | +1554.0% |

## Push Back

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| push_back | 10K | 712.51 ns | 9462.15 ns | +1228.0% |
| push_back | 100K | 723.02 ns | 9662.4 ns | +1236.4% |
| push_back | 1M | 727.42 ns | 10013 ns | +1276.5% |
| push_back reserved | 10K | 576.73 ns | 5520.46 ns | +857.2% |
| push_back reserved | 100K | 845.37 ns | 5556.59 ns | +557.3% |
| push_back reserved | 1M | 581.22 ns | 5569.88 ns | +858.3% |
| emplace_back | 10K | 362.34 ns | 4126.61 ns | +1038.9% |
| emplace_back | 100K | 576.61 ns | 4273.17 ns | +641.1% |
| emplace_back | 1M | 576.85 ns | 4166.3 ns | +622.3% |

## Remove If

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| erase-remove_if | 10K | 1050.66 ns | 11258.9 ns | +971.6% |
| erase-remove_if | 100K | 1126.33 ns | 11470.2 ns | +918.4% |
| erase-remove_if | 1M | 1309.68 ns | 12555.3 ns | +858.7% |

## Construction

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default construct | 10K | 0.28 ns | 3.35 ns | +1096.4% |
| default construct | 100K | 0.27 ns | 3.34 ns | +1137.0% |
| default construct | 1M | 0.55 ns | 3.39 ns | +516.4% |
| fill construct | 10K | 21.41 ns | 435.28 ns | +1933.1% |
| fill construct | 100K | 21.55 ns | 425.09 ns | +1872.6% |
| fill construct | 1M | 21.82 ns | 421.16 ns | +1830.2% |
| init list construct | 10K | 10.84 ns | 357.42 ns | +3197.2% |
| init list construct | 100K | 10.91 ns | 353.95 ns | +3144.3% |
| init list construct | 1M | 10.91 ns | 359.42 ns | +3194.4% |

## Copy

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| copy construct | 10K | 69 ns | 1397.7 ns | +1925.7% |
| copy construct | 100K | 69.05 ns | 1461.97 ns | +2017.3% |
| copy construct | 1M | 68.9 ns | 1403.03 ns | +1936.3% |
| copy assignment | 10K | 45.84 ns | 0.7 ns | -98.5% |
| copy assignment | 100K | 45.98 ns | 0.67 ns | -98.5% |
| copy assignment | 1M | 45.73 ns | 0.67 ns | -98.5% |

## Move

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| move construct | 10K | 13.84 ns | 441.54 ns | +3090.3% |
| move construct | 100K | 16.67 ns | 422.27 ns | +2433.1% |
| move construct | 1M | 13.63 ns | 419.21 ns | +2975.6% |
| move assignment | 10K | 13.82 ns | 413.9 ns | +2894.9% |
| move assignment | 100K | 16.43 ns | 411.82 ns | +2406.5% |
| move assignment | 1M | 16.63 ns | 422.55 ns | +2440.9% |

## Reallocation

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| growth reallocation | 10K | 228.21 ns | 2506.47 ns | +998.3% |
| growth reallocation | 100K | 148.91 ns | 2497.17 ns | +1577.0% |
| growth reallocation | 1M | 145.45 ns | 2515.16 ns | +1629.2% |

## Reserve

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| reserve | 10K | 28.83 ns | 664.18 ns | +2203.8% |
| reserve | 100K | 29.06 ns | 666.61 ns | +2193.9% |
| reserve | 1M | 29.3 ns | 671.06 ns | +2190.3% |
| reserve growth | 10K | 141.23 ns | 5405.24 ns | +3727.3% |
| reserve growth | 100K | 141.7 ns | 5539.93 ns | +3809.6% |
| reserve growth | 1M | 148.37 ns | 5495.29 ns | +3603.8% |
| reserve no-op | 10K | 0.55 ns | 2.71 ns | +392.7% |
| reserve no-op | 100K | 0.54 ns | 2.67 ns | +394.4% |
| reserve no-op | 1M | 0.27 ns | 2.73 ns | +911.1% |

## Shrink

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| shrink_to_fit | 10K | 72.22 ns | 1921.29 ns | +2560.3% |
| shrink_to_fit | 100K | 73.12 ns | 1976.83 ns | +2603.5% |
| shrink_to_fit | 1M | 72.81 ns | 1934.4 ns | +2556.8% |

## Comparison

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| operator== equal | 10K | 693.85 ns | 18233.7 ns | +2527.9% |
| operator== equal | 100K | 690.87 ns | 18362.4 ns | +2557.9% |
| operator== equal | 1M | 691.07 ns | 18766.5 ns | +2615.6% |
| operator== differs early | 10K | 2.71 ns | 17.42 ns | +542.8% |
| operator== differs early | 100K | 2.98 ns | 18.04 ns | +505.4% |
| operator== differs early | 1M | 2.77 ns | 17.58 ns | +534.7% |
| operator== differs late | 10K | 692.02 ns | 18100.2 ns | +2515.6% |
| operator== differs late | 100K | 689.97 ns | 19039.8 ns | +2659.5% |
| operator== differs late | 1M | 690.21 ns | 18451.2 ns | +2573.3% |
| operator== diff sizes | 10K | 0.28 ns | 1.36 ns | +385.7% |
| operator== diff sizes | 100K | 0.27 ns | 1.34 ns | +396.3% |
| operator== diff sizes | 1M | 0.27 ns | 1.45 ns | +437.0% |
| operator<=> equal | 10K | 5459.78 ns | 41552.7 ns | +661.1% |
| operator<=> equal | 100K | 4102.48 ns | 40434.2 ns | +885.6% |
| operator<=> equal | 1M | 5466 ns | 40008 ns | +631.9% |
| operator<=> less | 10K | 0.68 ns | 10.7 ns | +1473.5% |
| operator<=> less | 100K | 0.68 ns | 11.31 ns | +1563.2% |
| operator<=> less | 1M | 0.7 ns | 10.86 ns | +1451.4% |

## Observer

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| push_back, 0 listeners | 10K | 584.64 ns | 5651.91 ns | +866.7% |
| push_back, 0 listeners | 100K | 576.87 ns | 5535.44 ns | +859.6% |
| push_back, 0 listeners | 1M | 845.22 ns | 4146.67 ns | +390.6% |
| push_back, 1 listener | 10K | 2143.29 ns | 21911.8 ns | +922.3% |
| push_back, 1 listener | 100K | 2168.42 ns | 22016 ns | +915.3% |
| push_back, 1 listener | 1M | 2184.03 ns | 21999.5 ns | +907.3% |
| push_back, 8 listeners | 10K | 13437.3 ns | 104646 ns | +678.8% |
| push_back, 8 listeners | 100K | 13438.2 ns | 104508 ns | +677.7% |
| push_back, 8 listeners | 1M | 13465.8 ns | 114243 ns | +748.4% |
| push_back, capturing listener | 10K | 2262.66 ns | 25849.4 ns | +1042.4% |
| push_back, capturing listener | 100K | 2261.34 ns | 25668.9 ns | +1035.1% |
| push_back, capturing listener | 1M | 2265.78 ns | 25985 ns | +1046.8% |
| subscribe x100 | 10K | 1025.66 ns | 10509.8 ns | +924.7% |
| subscribe x100 | 100K | 998.92 ns | 10188.3 ns | +919.9% |
| subscribe x100 | 1M | 994.79 ns | 10113.8 ns | +916.7% |
| unsubscribe x100 | 10K | 17143.2 ns | 107602 ns | +527.7% |
| unsubscribe x100 | 100K | 17188.3 ns | 108447 ns | +530.9% |
| unsubscribe x100 | 1M | 17184.3 ns | 311204 ns | +1711.0% |

## Swap

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| member swap | 10K | 181.31 ns | 2373.05 ns | +1208.8% |
| member swap | 100K | 198.09 ns | 2282.57 ns | +1052.3% |
| member swap | 1M | 626.63 ns | 2318.38 ns | +270.0% |
| free swap | 10K | 199.75 ns | 2187.75 ns | +995.2% |
| free swap | 100K | 182.27 ns | 2245.46 ns | +1131.9% |
| free swap | 1M | 895.36 ns | 2247.5 ns | +151.0% |
