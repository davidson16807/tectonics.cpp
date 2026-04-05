
class ElementsVectorCodec:
	def __init__(self):
		pass
	def decode(self, vector:List[str]):
		return Elements(*[float(element) for element in vector])
	def encode(self, elements:Elements, vector:List[str]):
		vector.append(str(elements.semi_major_axis))
		vector.append(str(elements.eccentricity))
		vector.append(str(elements.inclination))
		vector.append(str(elements.longitude_of_ascending_node))
		vector.append(str(elements.argument_of_periapsis))
		vector.append(str(elements.mean_anomaly))

class DelimitedTableLineCodec:
	def __init__(self, row_delimiter='\n', column_delimiter='\t'):
		self.row_delimiter = row_delimiter
		self.column_delimiter = column_delimiter
	def decode(self, line: str, vector: List[str]):
		vector.clear()
		vector += line.split(self.column_delimiter)
	def encode(self, cells: List[str]) -> str:
		return self.column_delimiter.join(cells)+self.row_delimiter

class ElementsFiles:
	def __init__(self, delimited_table_line_codec, elements_vector_codec):
		self.delimited_table_line_codec = delimited_table_line_codec
		self.elements_vector_codec = elements_vector_codec
	def read(self, filename, elements, parent_ids, label_for_id):
		scratch = []
		with open(filename, 'r') as file:
			for line in file:
				cells = []
				self.delimited_table_line_codec.decode(line, cells)
				scratch.append(cells)
		id_for_label = {}
		label_for_id.clear()
		for i, row in enumerate(scratch):
			label_for_id.append(row[0])
			id_for_label[row[0]] = i
		parent_ids.clear()
		for row in scratch:
			parent_ids.append(id_for_label[row[1]])
		elements.clear()
		for row in scratch:
			elements.append(self.elements_vector_codec.decode(scratch[2:]))
	def write(self, filename, elements, parent_ids, label_for_id):
		vector = []
		with open(filename, 'w') as file:
			for i, (element, parent_id) in enumerate(zip(elements, parent_ids)):
				vector.clear();
				vector.append(label_for_id[i])
				vector.append(label_for_id[parent_id])
				subvector = []
				self.elements_vector_codec.encode(element, subvector)
				vector += subvector
				file.write(self.delimited_table_line_codec.encode(vector))
