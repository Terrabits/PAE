#!/usr/bin/env python

import numpy as np

Vdd    = 10.0
r_Ohms = 50.0

freq, pin_dBm = np.loadtxt('Pin dBm.csv', skiprows=3, delimiter=',', unpack=True)
_,    gain_dB = np.loadtxt('Gain dB.csv', skiprows=3, delimiter=',', unpack=True)
_,    ir_A    = np.loadtxt('Ir A.csv',    skiprows=3, delimiter=',', unpack=True)
_,    qtde    = np.loadtxt('Drain Efficiency.csv', skiprows=3, delimiter=',', unpack=True)

pout_dBm = pin_dBm + gain_dB
pout_W   = 0.001 * 10.0**(pout_dBm / 10.0)
pdc_W    = ir_A**2 * r_Ohms
de       = pout_W / pdc_W

delta = de - qtde
