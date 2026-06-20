import re

meters_in_astronomical_unit = 149597870700.0

with open('ELEMENTS.NUMBR', 'r') as infile:
	with open('../asteroids.tsv', 'w') as outfile:
		for i in range(2): # header
			next(infile) 
		for line in infile:
			number = line[0:7].strip()
			label = line[7:24].strip().lower()
			# jd = re.search('JD ?[0-9]+', line[24:32].strip())
			mjd = line[24:31].strip()
			j2000 = str(float(mjd) - 51506.5) # convert modified julian date to offset from j2000 epoch
			a = str(float(line[32:42].strip()) * meters_in_astronomical_unit)
			e = line[42:54].strip()
			i = line[54:64].strip()
			w = line[64:74].strip()
			n = line[74:84].strip()
			m = line[84:96].strip()
			outfile.write('\t'.join([label,'sol', '', j2000,a,e,i,n,w,m])+'\n')
