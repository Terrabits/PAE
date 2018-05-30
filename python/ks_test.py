#!/usr/bin/env python
from   rohdeschwarz.instruments.vna               import Vna
from   rohdeschwarz.instruments.genericinstrument import GenericInstrument
from   keysight    import   Dmm
from   bus         import   QuickVisa, QuickTcp
from   functions   import   measure
from   collections import   OrderedDict
import numpy             as np
import matplotlib.pyplot as plt
from   time        import   perf_counter

# DMM settings (~1 mV)
v_dc    = 50
r_ohms  = 5e-3
v_range = "100 mV" # 1 mV too low for dmm?
nplc    = 0.001    # 20 us capture time
delay_s = 20e-6
#

vna = Vna()
# vna.open_tcp('192.168.35.6')
vna.open('gpib', 20)
vna.timeout_ms = 60*1e3 # 1 min
ch1            = vna.channel(1)
trc            = vna.trace('Trc2')
points         = ch1.points

dmm1 = Dmm()
# dmm1.open_tcp('192.168.35.7')
# dmm1.open('gpib', 11)
dmm1.bus = QuickVisa()
dmm1.bus.open('gpib', 11)
# dmm1.bus.open('192.168.35.7')
dmm1.clear_status()

dmm2 = Dmm()
# dmm2.open_tcp('192.168.35.8')
# dmm2.open('gpib', 12)
dmm2.bus = QuickVisa()
dmm2.bus.open('gpib', 12)
# dmm2.bus.open('192.168.35.8')
dmm2.clear_status()

dmm1.range          = v_range
dmm1.nplc           = nplc
dmm1.trigger_source = 'EXT'
dmm1.trigger_slope  = 'POS'
dmm1.trigger_delay  = delay_s
dmm1.trigger_count  = points
dmm1.sample_source  = 'IMM'
dmm1.sample_count   = 1
dmm1.data_format    = "ASC"

dmm2.range          = v_range
dmm2.nplc           = nplc
dmm2.trigger_source = 'EXT'
dmm2.trigger_slope  = 'POS'
dmm2.trigger_delay  = delay_s
dmm2.trigger_count  = points
dmm2.sample_source  = 'IMM'
dmm2.sample_count   = 1
dmm2.data_format    = "ASC"

# delays  = list(range(0, 17, 2))
delays  = [10]
results = OrderedDict()
begin   = perf_counter()
for delay in delays:
	begin_in = perf_counter()
	dmm1.write("TRIG:DEL {0}".format(1.0e-6*delay))
	dmm2.write("TRIG:DEL {0}".format(1.0e-6*delay))
	result          = measure(vna, ch1, trc, dmm1, dmm2, v_dc, r_ohms)
	results[delay]  = result
	print("  {0}".format(perf_counter() - begin_in))
print(perf_counter() - begin)

freq_GHz = 1.0e-9 * ch1.frequencies_Hz

# plt.suptitle('DMM DE Test')
plt.suptitle('Drain Efficiency')

# plt.subplot(2,1,1)
plt.title("Drain Efficiency")
plt.xlabel('Freq (GHz)')
plt.ylabel('DE (%)')
for key in results:
	plt.plot(freq_GHz, results[key][0], label="{0}us".format(key))

# plt.subplot(2,1,2)
# plt.title('Voltage')
# plt.xlabel('Freq (GHz)')
# plt.ylabel('Volts')
# for key in results:
# 	plt.plot(freq_GHz, results[key][1], label="{0}us".format(key))
# plt.legend(title="Delay")

plt.show()
