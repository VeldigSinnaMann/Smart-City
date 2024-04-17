#denne funksjonen tar inn skiltnummer og sender og justert pris
import sys 
import pandas as pd



#sys.argv[1] er stringen etter python3 filnamn.py og i dette tilfelle bilnummer
inputCarId = int(sys.argv[1])

#les av csv filen 
df_price = pd.read_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/prisIndex.csv')
df_grade = pd.read_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/IDnumber.csv')


def getPrice():
    #hent hvilken rad bilen er i (gjøres for å holde koden enklere)
    carRow = df_grade[df_grade['car-ID'] == inputCarId].index[0]

    #sjekker riktig rad og kolonne for den spesifikke bil.
    carGrade = df_grade.iloc[carRow, 2]
    
    #nåværende pris
    priceNow = df_price.iloc[0,0]
    
    #pris til kunde 
    chargingPrice = priceNow*carGrade
    
    #print er output av function
    print(chargingPrice)
    
    
getPrice()