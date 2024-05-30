import sys 
import pandas as pd

bankID = sys.argv[1] # kontonummer til bruker
chargelevel = sys.argv[2] # hvor mye skal en lade, i prosent
userprice = sys.argv[3] # justert pris til bruker

df_bank = pd.read_csv(
    '/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/bankDetail.csv'
)


def whatToPay():
    baterypack = float(40) # batterikapasitet på bateri
    amount = -(baterypack/100 * float(chargelevel)) * float(userprice)
    return amount

def getBankRow():
    bankRow = df_bank[df_bank['accountNumber'] == int(bankID)].index[0]
    return bankRow


def changeMoneyOnAccount(accountNumber, amount):
    accountRow = df_bank[df_bank['accountNumber'] == int(accountNumber)].index[0]
    balance = df_bank.iloc[accountRow, 1] 
   
    df_bank.iloc[accountRow, 1] = balance + amount # finn mengde spenn i konto
    return ( df_bank.iloc[accountRow, 1])


def checkAccount():
    balance = df_bank.iloc[getBankRow(), 1] 
    return balance


def payment():
    if(checkAccount() > whatToPay()):
        try:
             changeMoneyOnAccount(bankID, whatToPay())
             print(0, end="")
            #  print(checkAccount())
        except:
            print(1, end="")
    else:
        print(2, end="")


payment()

df_bank.to_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/bankDetail.csv', index=False)