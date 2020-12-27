"""
weatherServer.py

The main flask server file for the weather station server
Serves the web page and handles incoing data from the station

Written by: Sam Hillcoat
Last Modified: 26/12/20 9:15 PM

"""

from flask import Flask, render_template, request, make_response
import datetime, json, os

app = Flask(__name__)

# Main server-side code for weather station
@app.route('/')
def index():
    return render_template('index.html')


# Function for handling incoming data and saving it to the server
cwdir = os.path.dirname(__file__) # Current working directory
json_path = os.path.join(cwdir, 'static/stored_data.json')

@app.route('/senddata', methods=['POST'])
def send_data():
    wind_speed = request.form.get('windspeed', default = None, type=float)
    wind_dir = request.form.get('winddir', default = None, type=int)
    now = datetime.datetime.now()
    timestamp = now.strftime("%a %b %d %Y %X")
    print(request)

    new_data = {'x':timestamp, 'y':wind_speed, 'dir':wind_dir}
    output_to_json(new_data)
    print(new_data)
    return make_response(
        'Data Received',
        200
    )




def output_to_json(new_data):
    # new_data : data from POST request from send_data()
    # Open the json file, read all json data into python object, add the new data,
    # turn back into json data, overwrite the data into the file.

    with open(json_path, "r") as json_file:
        old_data = json.load(json_file) #type : list of dicts

    old_data.append(new_data) # append the new data to the list
    
    if (len(old_data) > 40):
        old_data.pop(0)

    #output to file as json
    with open(json_path, "w") as json_file:
        json.dump(old_data, json_file) #type : list of dicts

    




if (__name__ == '__main__'):
    app.run(debug = True, host='0.0.0.0')


