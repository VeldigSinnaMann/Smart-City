#ikke i bruk


import sys
import pandas as pd
import requests
from datetime import datetime
from datetime import date

def getPriceToday():
    # finn dato. f-string for å faa to siffer ved tall < 10
    today = date.today()
    timeToday = datetime.now()
    year = today.year
    month = f"{today.month:02d}"
    day = f"{today.day:02d}"
    hour = timeToday.hour

    # produser riktig url for tiden naa og hent json
    urlString = "https://www.hvakosterstrommen.no/api/v1/prices/" + \
        str(year) + "/" + str(month) + "-" + str(day) + "_NO5.json"
    
    response = requests.get(urlString)
    response_dict = response.json()
    print(response_dict)
    priceNow = response_dict[hour]['NOK_per_kWh']  # hent ut pris fra json fil

    return priceNow

getPriceToday()