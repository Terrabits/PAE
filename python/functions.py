import math


def dbm_to_mw(power_dbm):
	return 10**(power_dbm/10)

def dbm_to_w(power_dbm):
	return dbm_to_mw(power_dbm) / 1000.0

def mw_to_dbm(power_mw):
	return 10*math.log10(power_mw)

