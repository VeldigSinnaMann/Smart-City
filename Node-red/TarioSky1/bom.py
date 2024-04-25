import sys 
import pandas as pd

#det er det som blir printet som blir sendt til neste ledd i node-red

#sys.argv[1] er stringen etter python3 filnamn.py og i dette tilfelle bilnummer
inputCarId = int(sys.argv[1])

#les av csv filen 
df_carId = pd.read_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/IDnumber.csv')

#dataframe bank
df_bank = pd.read_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/bankDetail.csv')

def changeMoneyOnAccount(accountNumber, amount):
    accountRow = df_bank[df_bank['accountNumber'] == accountNumber].index[0]
    balance = df_bank.iloc[accountRow, 1] 
   
    df_bank.iloc[accountRow, 1] = balance+amount # finn mengde spenn i konto
    print( df_bank.iloc[accountRow, 1])

def bomImport():
    #hent hvilken rad bilen er i (gjøres for å holde koden enklere)
    carRow = df_carId[df_carId['car-ID'] == inputCarId].index[0]

    #sjekker riktig rad og kolonne for den spesifikke bil, og kontonummer
    iselectric = df_carId.iloc[carRow, 1]
    bankNumber = df_carId.iloc[carRow, 3]
    
    #sikkerhet for å passe på at vi sender ut riktig data
    if (iselectric == 0):
        changeMoneyOnAccount(bankNumber, -100)
    print(iselectric, end = "")

bomImport()

#oppdater csv filene med ny riktig informasjon. dette må hende til slutt
df_bank.to_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/bankDetail.csv', index=False)