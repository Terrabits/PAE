from rohdeschwarz.instruments.genericinstrument import GenericInstrument


class Dmm(GenericInstrument):
	def __init__(self):
		GenericInstrument.__init__(self)

	def _set_range(self, value):
		self.write("SENS:VOLT:DC:RANG {0}".format(value))
	range = property(None, _set_range)

	def _set_nplc(self, value):
		self.write("SENS:VOLT:DC:NPLC {0}".format(value))
	nplc = property(None, _set_nplc)

	def _set_trigger_source(self, value):
		self.write("TRIG:SOUR {0}".format(value))
	trigger_source = property(None, _set_trigger_source)

	def _set_trigger_delay(self, value):
		self.write("TRIG:DEL {0}".format(value))
	trigger_delay = property(None, _set_trigger_delay)

	def _set_trigger_count(self, value):
		self.write("TRIG:COUN {0}".format(value))
	trigger_count = property(None, _set_trigger_count)

	def _set_trigger_slope(self, value):
		self.write("TRIG:SLOP {0}".format(value))
	trigger_slope = property(None, _set_trigger_slope)

	def _set_sample_source(self, value):
		self.write("SAMP:SOUR {0}".format(value))
	sample_source = property(None, _set_sample_source)

	def _set_sample_count(self, value):
		self.write("SAMP:COUN {0}".format(value))
	sample_count = property(None, _set_sample_count)

	def next_error(self):
		code = 0;
		message = '';
		result = self.query(':SYST:ERR?').strip()
		comma_index = result.find(',')
		code = int(result[:comma_index])
		message = result[comma_index+2:-1]
		if (code != 0):
			return(code, message)
		else:
			return None
	def _errors(self):
		errors = []
		error = self.next_error()
		while error:
			errors.append(error)
			error = self.next_error()
		return errors
	errors = property(_errors)

	def _set_data_format(self, value):
		self.write("FORM:DATA {0}".format(value))
	data_format = property(None, _set_data_format)

	def _set_data_bit_order(self, value):
		self.write("FORM:BORD {0}".format(value))
	data_bit_order = property(None, _set_data_bit_order)
