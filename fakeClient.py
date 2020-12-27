"""
Testing file for weather server

sends random data to the server via POST request
acts as a fake weather station

Written by: Sam Hillcoat
Last Modified: 26/12/20 9:25 PM
"""

import requests, time
from random import randint

#URL = "https://deviotweather.com/senddata"
URL = "http://127.0.0.1:5000/senddata"
#URL = "https://deviotweather.free.beeceptor.com"

def post_data(data):
    response = requests.post(URL, data = data)
    print(response)
    

def generate_wind_data():
    windspeed = randint(0,30)
    winddir = randint(0,359)
    data = {"windspeed" : windspeed, "winddir": winddir}
    post_data(data)
    print(data)


for i in range(0,15):
    generate_wind_data()
    time.sleep(5)
