from rohdeschwarz.instruments.vna import Vna
from rohdeschwarz.instruments.genericinstrument import GenericInstrument
from functions import dbm_to_w as to_w
import csv

r_zero          = 0.015
r               = 0.715
frequencies_GHz = [0.5, 1, 2, 3, 4, 5, 6]
powers_dBm      = [-20, -10, 0]

# VNA setup
vna = Vna()
vna.open_tcp('192.168.1.119')

vna.preset()
vna.pause()

ch1 = vna.channel(1)
ch1.cal_group    = 'pae_cal'
ch1.sweep_type   = 'CW'
ch1.points       = 1
ch1.manual_sweep = True

vna.traces = ['Pin', 'Pout']

pin_trc    = vna.trace('Pin')
pin_trc.diagram    = 1
pin_trc.parameter  = 'A1D1(P1)'

pout_trc   = vna.trace('Pout')
pout_trc.diagram   = 1
pout_trc.parameter = 'B2D1(P1)'

# DMM setup
dmm = GenericInstrument()
dmm.open_tcp('192.168.1.120')


f    = open('results.md', 'wb')
f.write("| Pin (W) | Pout (W) | Vr (V) | PAE (%) |\n")
f.write("|---------|----------|--------|---------|")
csvw = csv.writer(f, delimiter="|", lineterminator=" |\n")
for freq in frequencies_GHz:
	ch1.frequency_Hz = freq, 'GHz'
	for power in powers_dBm:
		ch1.power_dBm    = power
		ch1.start_sweep()
		vna.pause()
		pin  = to_w(pin_trc.y_formatted[0])
		pout = to_w(pout_trc.y_formatted[0])
		vr  = float(dmm.query("READ?"))
		pdc  = (5 - vr) * (vr / r)
		pae  = ((pout - pin) / pdc * 100.0)
		f.write("| ")
		csvw.writerow([pin, pout, vr, pae])

f.close()
dmm.close()
vna.close()



