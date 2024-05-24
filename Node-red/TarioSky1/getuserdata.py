import sys 
import pandas as pd

inUser = sys.argv[1]

df_userID = pd.read_csv(
    '/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/users.csv')

def getUserID():
    userRow = df_userID[df_userID['username'] == inUser].index[0]
    getUser_ID = df_userID.iloc[userRow, 2]

    return getUser_ID


df_userData = pd.read_csv(
    '/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/IDnumber.csv'
)

def getUserData():
    getDataRow = df_userData[df_userData['car-ID'] == getUserID()].index[0]
    getUserGrade = df_userData.iloc[getDataRow, 2]
    getUserBank = df_userData.iloc[getDataRow, 3]
    return getUserGrade, getUserBank

# print(getUserID())
print(getUserData())