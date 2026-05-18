#!/bin/bash

echo "====================================="
echo " Generator Level Kinematic Checks"
echo "====================================="

# Run generator level check
root -l -b -q dvmp_lAger_level_check.C

echo "====================================="
echo " Running DVMP J/psi Analysis"
echo "====================================="

# Run analysis
root -l -b -q dvmp_jpsi.c

echo ""
echo "====================================="
echo " Running Plotting Macro"
echo "====================================="

# Run plotting
root -l -b -q plot_dvmp_jpsi.c

echo ""
echo "====================================="
echo " Cross Section"
echo "====================================="

# Cross Section Script
root -l -b -q dvmp_cross_section.c

echo ""
echo "====================================="
echo " Fitting the t-distributions"
echo "====================================="

# Fitting t-distribution
root -l -b -q dvmp_t_fitting.c

echo ""
echo "====================================="
echo " Taking the Fourier Transform"
echo "====================================="

# Fourier transform of t-distribution
root -l -b -q dvmp_fourier.c

echo ""
echo "====================================="
echo " Fitting the RECO JPsi Invariant Mass"
echo "====================================="

# Fourier invariant mass
root -l -b -q dvmp_invariant_mass.c

echo ""
echo "====================================="
echo " Using Modified Jihee's Code for the Forurier and Normalizing"
echo "====================================="

# Fourier invariant mass
root -l -b -q plot_FT_Jihee.C

echo ""
echo "====================================="
echo " Finished Successfully"
echo "====================================="
