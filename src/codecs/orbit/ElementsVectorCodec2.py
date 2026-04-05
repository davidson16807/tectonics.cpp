
# useful ideas, but not on the hot path:

class CodecComposition:
	def __init__(self, encode1, encode2):
		self.encode1 = encode1
		self.encode2 = encode2
	def decode(self, code):
		return self.encode1.decode(self.encode2.decode(code))
	def encode(self, content):
		return self.encode2.encode(self.encode1.encode(content))

class LineCodecFiles:
	def __init__(self, codec):
		self.codec = codec
	def read(self, filename, result):
		with open(filename, 'r') as file:
			for line in line:
				result.append(self.codec.decode(line))
	def write(self, filename, content):
		with open(filename, 'w') as file:
			for element in content:
				file.write(self.codec.encode(element))
