#
#   KOPI 
#
#
import sys 
import pandas as pd
import requests
from datetime import datetime
from datetime import date


userAccountNumber = sys.argv[1] # kontonummer til bruker
chargeLevel = sys.argv[2] # hvor mye skal en lade, i prosent
userPrice = sys.argv[3] # justert pris til bruker

depositAccount = 12345678 #kontonummer til depositumskonto


df_bank = pd.read_csv(
    '/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/bankDetail.csv'
)


def getPriceNow():
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
    priceNow = response_dict[hour]['NOK_per_kWh']  # hent ut pris fra json fil

    return priceNow*10


def whatToPay():
    baterypack = float(40) # batterikapasitet på bateri
    amount = -(baterypack/100 * float(chargeLevel)) * float(userPrice)
    return amount

def getBankRow():
    bankRow = df_bank[df_bank['accountNumber'] == int(userAccountNumber)].index[0]
    return bankRow


def changeBalanceOnAccount(accountNumber, amount):
    accountRow = df_bank[df_bank['accountNumber'] == int(accountNumber)].index[0]
    balance = df_bank.iloc[accountRow, 1] 
   
    df_bank.iloc[accountRow, 1] = round(balance + amount, 2) # finn mengde spenn i konto
    return ( df_bank.iloc[accountRow, 1])


def checkAccount():
    balance = df_bank.iloc[getBankRow(), 1] 
    return balance


def payment():
    standardPrice = -(40/100 * float(chargeLevel)) * float(getPriceNow()) #basispris
    if(checkAccount() > whatToPay()):
        try:
             changeBalanceOnAccount(userAccountNumber, whatToPay()) #ta penger fra brukerkonto
             changeBalanceOnAccount(depositAccount, round(standardPrice-whatToPay(), 2)) #ta/gi penger fra depositumskonto
             
             print(0, end="")
            #  print(checkAccount())
        except:
            print(1, end="")
    else:
        print(2, end="")


payment()

df_bank.to_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/bankDetail.csv', index=False)