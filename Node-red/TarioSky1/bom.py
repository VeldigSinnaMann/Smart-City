import sys 
import pandas as pd

#det er det som blir printet som blir sendt til neste ledd i node-red

#sys.argv[1] er stringen etter python3 filnamn.py og i dette tilfelle bilnummer
inputCarId = int(sys.argv[1])

#les av csv filen 
df = pd.read_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/IDnumber.csv')


def bomImport():
    #hent hvilken rad bilen er i (gjøres for å holde koden enklere)
    carRow = df[df['car-ID'] == inputCarId].index[0]

    #sjekker riktig rad og kolonne for den spesifikke bil
    iselectric = df.iloc[carRow, 1]
    
    #sikkerhet for å passe på at vi sender ut riktig data
    if (iselectric == 1 or iselectric == 0):
        print(iselectric, end="")

bomImport()