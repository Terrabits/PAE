from rohdeschwarz.instruments.genericinstrument import GenericInstrument


points = 21
filename = 'LOG'


dmm = GenericInstrument()
dmm.open_tcp('192.168.1.118')

print(dmm.id_string())

dmm.timeout_ms = 10000
dmm.clear_status()
dmm.write('*ESE 1')
dmm.write('*SRE 32')
dmm.preset()
dmm.pause(10000)

print("PRESET finished")


## Trigger
print("Setting trigger...")
dmm.write('TRIG:MODE SING')
dmm.write('TRIG:INT  1')
dmm.write('TRIG:COUN 1')

## LOG
print("Setting log...")
dmm.write("DATA:LOG:FNAM '{0}.CSV', INT".format(filename))
dmm.write('DATA:LOG:MODE COUN')
dmm.write('DATA:LOG:TIME 1')
dmm.write('DATA:LOG:COUN {0}'.format(points))
dmm.clear_status()
dmm.write('DATA:LOG ON')

## Send triggers
for i in range(0, points+1):
	dmm.write("*TRG")
	print("{0}: {1}".format(i+1, dmm.query('READ?')))
	dmm.pause()


# dmm.write('DATA:LOG OFF')

dmm.pause(10000)
with open('{0}.CSV'.format(filename), 'wb') as f:
	data = dmm.query("DATA:DATA? '{0}.CSV', INT".format(filename))
	f.write(data)

# errors?

dmm.close()