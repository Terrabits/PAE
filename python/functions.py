import numpy as     np
import math
from   time  import sleep


def dbm_to_mw(power_dbm):
	return 10**(power_dbm/10)

def dbm_to_w(power_dbm):
	return dbm_to_mw(power_dbm) / 1000.0

def mw_to_dbm(power_mw):
	return 10*math.log10(power_mw)

def measure(vna, ch, pout_trc, dmm1, dmm2, v_dc, r_ohms):
	dmm1.write("INIT")
	dmm1.bus.flush()
	dmm2.write("INIT")
	dmm2.bus.flush()
	sleep(0.5)

	vna.start_sweeps()
	vna.pause()

	volts1 = []
	try:
		dmm1.pause()
		volts1 = dmm1.query("FETC?")
		volts1 = [float(i) for i in volts1.split(",")]
	except:
		print("Could not read points from dmm1")
		exit()
	# volts1 = list(range(1,ch.points+1))

	volts2 = []
	try:
		dmm2.pause()
		volts2 = dmm2.query("FETC?")
		volts2 = [float(i) for i in volts2.split(",")]
	except:
		print("Could not read points from dmm2")
		exit()
	# volts2 = list(range(0,len(volts1)))

	volts1  = np.array(volts1)
	pout_w  = dbm_to_w(pout_trc.y_formatted)
	# pout_w = list(range(1,len(volts1)+1))
	if len(volts1) != len(pout_w):
		print("Did not capture enough points on dmm1")
		exit()
	drain_w = (v_dc / r_ohms) * volts1
	de_pct  = 100* (pout_w / drain_w)
	return (de_pct, volts2)