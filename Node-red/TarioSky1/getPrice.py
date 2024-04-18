#denne funksjonen tar inn skiltnummer og sender og justert pris
import sys 
import pandas as pd
import requests
from datetime import datetime
from datetime import date


#sys.argv[1] er stringen etter python3 filnamn.py og i dette tilfelle bilnummer
inputCarId = int(sys.argv[1])

#les av csv filen 
df_grade = pd.read_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/IDnumber.csv')


def getPrice():
    #finn dato. f-string for å faa to siffer ved tall < 10
    today = date.today()
    timeToday = datetime.now()
    year = today.year
    month = f"{today.month:02d}"
    day = f"{today.day:02d}"
    hour = timeToday.hour
    
    #produser riktig url for tiden naa og hent json
    urlString ="https://www.hvakosterstrommen.no/api/v1/prices/" + str(year) + "/" + str(month) + "-" + str(day) + "_NO5.json"
    response = requests.get(urlString) 
    response_dict = response.json()
    priceNow = response_dict[hour]['NOK_per_kWh'] #hent ut pris fra json fil

    #hent hvilken rad bilen er i (gjøres for å holde koden enklere)
    carRow = df_grade[df_grade['car-ID'] == inputCarId].index[0]

    #sjekker riktig rad og kolonne for den spesifikke bil.
    carGrade = df_grade.iloc[carRow, 2]
    
    #pris til kunde 
    chargingPrice = priceNow*carGrade
    
    #print er output av function
    print(chargingPrice)
    
    
getPrice()