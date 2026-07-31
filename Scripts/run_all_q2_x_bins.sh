#!/bin/bash

# Stop if any command fails
set -e

SOURCE_DIR="X_and_Q2_binning"

Q2_BINS=(
  "1 1.78"
  "1.78 3.16"
  "3.16 5.62"
  "5.62 10"
)

X_BINS=(
  "1e-4 2.5e-4"
  "2.5e-4 6.3e-4"
  "6.3e-4 1e-3"
  "1e-3 2.5e-3"
  "2.5e-3 6.3e-3"
  "6.3e-3 1e-2"
  "1e-2 2.5e-2"
  "2.5e-2 6.3e-2"
  "6.3e-2 0.1"
  "0.1 0.25"
  "0.25 0.7"
)

run_macros()
{
    echo ""
    echo "====================================="
    echo " Running Analysis"
    echo "====================================="
    root -l -b -q dvmp_jpsi.c

    echo ""
    echo "====================================="
    echo " Running Plotting Macro"
    echo "====================================="
    root -l -b -q plot_dvmp_jpsi.c

    echo ""
    echo "====================================="
    echo " Cross Section"
    echo "====================================="
    root -l -b -q dvmp_cross_section.c

    echo ""
    echo "====================================="
    echo " Fitting the t-distributions"
    echo "====================================="
    root -l -b -q dvmp_t_fitting.c

    echo ""
    echo "====================================="
    echo " Taking the Fourier Transform"
    echo "====================================="
    root -l -b -q dvmp_fourier.c

    echo ""
    echo "====================================="
    echo " Fitting the RECO JPsi Invariant Mass"
    echo "====================================="
    root -l -b -q dvmp_invariant_mass.c

    echo ""
    echo "====================================="
    echo " Using Modified Jihee's Code for the Fourier and Normalizing"
    echo "====================================="
    root -l -b -q plot_FT_Jihee.C
}

for q2bin in "${Q2_BINS[@]}"; do

    q2_low=$(echo $q2bin | awk '{print $1}')
    q2_high=$(echo $q2bin | awk '{print $2}')

    q2_folder="q2_${q2_low}_to_${q2_high}"
    mkdir -p "$q2_folder"

    for xbin in "${X_BINS[@]}"; do

        x_low=$(echo $xbin | awk '{print $1}')
        x_high=$(echo $xbin | awk '{print $2}')

        x_folder="x_${x_low}_to_${x_high}"
        workdir="${q2_folder}/${x_folder}"

        echo ""
        echo "====================================="
        echo " Creating folder: $workdir"
        echo " Q2: $q2_low to $q2_high"
        echo " x : $x_low to $x_high"
        echo "====================================="

        mkdir -p "$workdir"

        # Copy scripts
        cp ${SOURCE_DIR}/*.h  "$workdir"/ 2>/dev/null || true
        cp ${SOURCE_DIR}/*.C  "$workdir"/ 2>/dev/null || true
        cp ${SOURCE_DIR}/*.c  "$workdir"/ 2>/dev/null || true
        cp ${SOURCE_DIR}/*.sh "$workdir"/ 2>/dev/null || true

        # Modify dvmp_jpsi.h
        sed -i.bak \
            -e "s/double q2_low[[:space:]]*=.*q2_high[[:space:]]*=.*;/double q2_low  = ${q2_low},  q2_high = ${q2_high};/" \
            -e "s/double x_low[[:space:]]*=.*x_high[[:space:]]*=.*;/double x_low   = ${x_low},  x_high  = ${x_high};/" \
            "$workdir/dvmp_jpsi.h"

        rm -f "$workdir/dvmp_jpsi.h.bak"

        # Run scripts inside this bin folder
        (
            cd "$workdir"
            run_macros
        )

    done
done

echo ""
echo "All Q2 and x bins finished."
