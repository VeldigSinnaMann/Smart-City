#tar inn et skildnummer som sys.argv[1] og sjekker antall ganger over 2m/s^2
import pandas as pd
from datetime import datetime
import sys

licensePlate = sys.argv[1]
timeNow = datetime.now()


df_logg = pd.read_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/logg.csv')

print(df_logg)

df_logg.sort_values(by=['car-id'])

print(df_logg)

