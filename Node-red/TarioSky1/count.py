import pandas as pd
from datetime import datetime
import sys

licensePlate = sys.argv[1]
timeNow = datetime.now()

df_logg = pd.read_csv('/home/jjhrasberry1/Desktop/Zumo-Smart-City/Node-red/TarioSky1/logg.csv')

# Convert timestamp to datetime
df_logg['timestamp'] = pd.to_datetime(df_logg['timestamp'])

# Filter records for the given license plate number
df_license_plate = df_logg[df_logg['car-id'] == int(licensePlate)]

# Filter records for the last day
last_day_records = df_license_plate[df_license_plate['timestamp'] >= (timeNow - pd.Timedelta(days=1))]

# Count the number of times the score is over 2 m/s^2
count_high_scores = (last_day_records['score'] > 2).sum()

print(f"The car with license plate number {licensePlate} had a score over 2 m/s^2 {count_high_scores} times in the last day.")
