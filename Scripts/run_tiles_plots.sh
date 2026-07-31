#!/bin/bash

echo "====================================="
echo " t for different methods"
echo "====================================="

# tile for t distribution
root -l -q -b plot_t_tiles_by_q2.C

echo "====================================="
echo " Normalized Fourier"
echo "====================================="

# Run analysis
root -l -b -q plot_FT_tiles_by_q2.C

echo ""
echo "====================================="
echo " Tile plots done"
echo "====================================="
