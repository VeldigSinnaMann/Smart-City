# henter brukerkonto til bruker fra database

import sys 
import pandas as pd

#hent ut innloggingsnavn
inUser = sys.argv[1]

#last ned csv fil
df_userID = pd.read_csv(
    '/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/users.csv')

#finn korresponderende bilNummer
def getUserID():
    userRow = df_userID[df_userID['username'] == inUser].index[0]
    getUser_ID = df_userID.iloc[userRow, 2]

    return getUser_ID

#last ned IDnumber
df_userData = pd.read_csv(
    '/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/IDnumber.csv'
)


def getUserData():
    global getUserGrade
    global getUserBank
    getDataRow = df_userData[df_userData['car-ID'] == getUserID()].index[0]
    getUserGrade = df_userData.iloc[getDataRow, 2]
    getUserBank = df_userData.iloc[getDataRow, 3]

    return getUserBank

print(getUserData(), end="")
