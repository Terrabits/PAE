from rohdeschwarz.bus.visa import VisaBus
from rohdeschwarz.bus.tcp import TcpBus

class QuickVisa():
	def __init__(self):
		self._bus = VisaBus()
		self._buffer = ''

	def __getattr__(self, name):
		if name == "write":
			def write(value):
				term_char = self._bus._instr.write_termination
				if self._buffer:
					self._buffer += term_char + value
				else:
					self._buffer = value
			return write
		elif name == "read":
			self.flush()
			return self._bus.read
		if name == "flush":
			def flush():
				if self._buffer:
					self._bus.write(self._buffer)
				self._buffer = ''
			return flush
		else:
			return getattr(self._bus, name)

class QuickTcp():
	def __init__(self):
		self._bus = TcpBus()
		self._buffer = ''

	def __getattr__(self, name):
		if name == "write":
			def write(value):
				term_char = '\n'
				if self._buffer:
					self._buffer += term_char + value
				else:
					self._buffer = value
			return write
		elif name == "read":
			self.flush()
			return self._bus.read
		if name == "flush":
			def flush():
				if self._buffer:
					self._bus.write(self._buffer)
				self._buffer = ''
			return flush
		else:
			return getattr(self._bus, name)