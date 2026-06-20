from __future__ import annotations

import time
import requests

from bs4 import BeautifulSoup
from urllib.parse import urlparse

def infobox(url: str) -> dict[str, str]:
    time.sleep(3) # rate limit for kindness
    try:
        html = requests.get(url,headers={'User-Agent': 
            'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.0.0 Safari/537.36'
        }).text
        soup = BeautifulSoup(html)
        infobox = soup.find("table", class_="infobox")
        epoch = infobox.find('td', class_='infobox-full-data')
        return {
            'epoch': ('' if epoch is None else epoch.text),
            **{
                row.find('th').text.replace('\xa0',' ').strip().lower() : 
                row.find('td').text.replace('\xa0',' ').strip().lower()
                for row in infobox.find_all("tr")
                if row.find('th') and row.find('td')
            }
        }
    except Exception as e:
        print(url, e)
        return {}

def append(label, lookup, filename, keys):
    if any(key in lookup for key in keys):
        with open(filename,'a') as file:
            file.write('\t'.join([label, *[(lookup[key] if key in lookup else '') for key in keys]])+'\n')

with open('../jpl-horizons/ELEMENTS.NUMBR', 'r') as infile:
    for i in range(2): #header
        next(infile)
    for line in infile:
        number = line[0:7].strip()
        Label = line[7:24].strip()
        label = Label.lower()
        url = f'https://en.wikipedia.org/wiki/{number}_{Label}'
        lookup = infobox(url)
        append(label, lookup, 'mass.tsv',       'mass'.split(';'))
        append(label, lookup, 'dimensions.tsv', 'dimensions;mean diameter;flattening'.split(';'))
        append(label, lookup, 'rotation.tsv',   'sidereal rotation period;synodic rotation period;axial tilt;north pole right ascention;north pole declination;pole ecliptic longitude;pole ecliptic latitude'.split(';'))
        append(label, lookup, 'albedo.tsv',     'geometric albedo'.split(';'))
        append(label, lookup, 'type.tsv',       'spectral type'.split(';'))
        append(label, lookup, 'orbit.tsv',      'epoch;semi-major axis;eccentricity;inclination;longitude of ascending node;argument of perihelion;mean anomaly'.split(';'))
