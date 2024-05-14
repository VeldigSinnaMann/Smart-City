import sys 
import pandas as pd

inUser = sys.argv[1]
inPasword = sys.argv[2]


df_user = pd.read_csv(
    '/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/users.csv')

whatToDo = 2
# hva skal sendes til node red

def getUsers():
    global userRow
    global whatToDo

    try:
        userRow = df_user[df_user['username'] == inUser].index[0]
        whatToDo = 2

    except:
        whatToDo = 1
    
    return whatToDo

def checkPassword():
    global whatToDo
    global getPassword
    if whatToDo == 2:
        getPassword = df_user.iloc[userRow, 1]
        if inPasword == getPassword:
            whatToDo = 0
        

    return whatToDo

def runSys():
    whatToDo = getUsers()
    whatToDo = checkPassword()
    print(whatToDo, end="")

runSys()

