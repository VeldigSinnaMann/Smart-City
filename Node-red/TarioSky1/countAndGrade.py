#denne koden leser av all data logg.csv og gir en rating på hver bil fra 10-0 basert på antall 
# prosent av tiden de har kjørt over 1.8m/S^2.

import pandas as pd
from datetime import datetime, timedelta

timeNow = datetime.now()

#last ned csv filer
df_logg = pd.read_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/logg.csv')
df_database = pd.read_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/IDnumber.csv')

#lag liste med alle bilnummere
plateNumberList = df_database['car-ID'].tolist()


one_day_ago = timeNow - timedelta(days=1)

def getBreachProsentage(plateNumber, limit):
    #filtrer ut etter skiltnummer og fjerner logg som er eldre enn 1 dag
    df_car = df_logg[(df_logg.carId == int(plateNumber)) & (pd.to_datetime(df_logg.timestamp) >= one_day_ago)]
    numberOfReadings = df_car.shape[0]
    #fjern de lesingene under grensen
    df_car = df_car[df_car.score >= limit] 
    #les av hvor mange brytninger vi har
    breaches = df_car.shape[0]
    prosentage = breaches/numberOfReadings
    #returner prosentandel av siste dags lesninger bilen bryter grensen
    return prosentage
    
#oppdater kjørescoren til alle bilene.
for car in plateNumberList:
    carProsentage = getBreachProsentage(car, 1.8)
    carRow = df_database[df_database['car-ID'] == car].index[0]
    print(carRow)
    #her formaterer vi fra prosnet til en skala fra en til 10
    df_database.iloc[carRow, 2] = round(10-carProsentage*10, 2)

#skriv til IDnumber.csv
df_database.to_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/IDnumber.csv', index=False)
