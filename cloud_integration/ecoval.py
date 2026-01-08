import boto3
import pandas as pd
import time
from decimal import Decimal

dynamodb = boto3.resource('dynamodb', region_name='us-east-1',aws_access_key_id="ACCESS KEY",aws_secret_access_key="SECRET ACCESS KEY")

table = dynamodb.Table('beta4')

csv_file=["DATA1.csv","DATA2.csv","DATA3.csv"]

primary_key = 1
for i in csv_file:
    data = pd.read_csv(i)
  
    for index, row in data.iterrows():
        item = row.to_dict()
        for key, value in item.items():
            if isinstance(value, float):
                item[key] = Decimal(str(value)) 
        item['SNO'] = primary_key
        table.put_item(Item=item)
        primary_key += 1
        print(f"Inserted row with PrimaryKey: {primary_key-1}")
    time.sleep(1)
