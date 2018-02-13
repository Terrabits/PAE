#!/usr/bin/env python

import glob
import os
from   pathlib import Path
from   rohdeschwarz.instruments.vna import Vna

def clean_file(filename):
    lines = []
    with open(filename, 'r') as f:
        lines = f.readlines()
    with open(filename, 'w') as f:
        for line in lines:
            line = line.strip()
            if line.endswith(','):
                line = line[:-1]
            if line:
                f.write(line + '\n')

vna = Vna()
vna.open('gpib', 20)
vna.trace('Trc1').save_data_locally('Pin dBm.csv')
vna.trace('Trc2').save_data_locally('Gain dB.csv')
vna.trace('Stage1').save_data_locally('Ir A.csv')
vna.trace('DE').save_data_locally('Drain Efficiency.csv')

os.chdir(str(Path(__file__).parent))
for file in glob.glob('*.csv'):
    clean_file(file)
