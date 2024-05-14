import sys 
import pandas as pd
from datetime import datetime

# Get inputs from command line arguments
score = sys.argv[1]
licensePlate = sys.argv[2]
timeStamp = datetime.now()

# Read the existing CSV file into a DataFrame
df_logg = pd.read_csv("/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/logg.csv")

df_logg.loc[len(df_logg.index)] = [str(licensePlate), str(score), str(timeStamp)]

#print(df_logg)
# Save the updated DataFrame back to the CSV file
df_logg.to_csv("/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/logg.csv", index=False)
