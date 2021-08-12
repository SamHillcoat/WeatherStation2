"""
weatherServer.py

The main flask server file for the weather station server
Serves the web page and handles incoing data from the station

Written by: Sam Hillcoat
Last Modified: 31/01/21 9:15 PM

"""

from flask import Flask, render_template, request, make_response, redirect, flash, url_for, session
import datetime, json, os
from functools import wraps

# Admin Login
ADMIN_USERNAME = 'admin'
ADMIN_PASSWORD = 'pass123'

LOGGED_IN = False


app = Flask(__name__)
app.secret_key = b'_5#y2L"F4Q8z\n\xec]/'

# Main server-side code for weather station
@app.route('/')
def index():
    return render_template('index.html')


# Function for handling incoming data and saving it to the server
cwdir = os.path.dirname(__file__) # Current working directory
json_path = os.path.join(cwdir, 'static/stored_data.json')

def requires_login(f):
    @wraps(f)
    def wrap(*args, **kwargs):
        if not "logged_in" in session or session['logged_in'] == False:
            flash("Please Login First")
            return redirect(url_for('login'))
        return f(*args, **kwargs)
    return wrap

@app.route('/senddata', methods=['POST'])
def send_data():
    json_data = request.json
    wind_speed = json_data['windspeed']
    wind_dir = json_data['winddir']
    now = datetime.datetime.now()
    timestamp = now.strftime("%a %b %d %Y %X")
    print(request)

    new_data = {'x':timestamp, 'y':wind_speed, 'dir':wind_dir}
    output_to_json(new_data)
    print(new_data)
    return make_response(
        'Data Recieved',
        200
    )

@app.route('/clearalldata', methods=['GET'])
@requires_login
def clearAllData():
    # Clears all data in stored_data.json for reseting while testing
    with open(json_path, 'w') as json_file:
        json_file.write('[]')

    flash("Data Cleared")
    return redirect(url_for('admin'))

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'GET':
        return render_template('login.html')
    elif request.method == 'POST':
        username = request.form['username']
        password = request.form['password']

        if check_auth(username, password):
            session['logged_in'] = True
            return redirect(url_for('admin'))
        else:
            flash("Incorrect login details")
            return redirect(url_for('login'))
        

@app.route('/admin')
@requires_login
def admin():
    return render_template('admin.html')





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

    
def check_auth(username, password):
    return username == ADMIN_USERNAME and password == ADMIN_PASSWORD




if (__name__ == '__main__'):
    app.run(debug = True, host='0.0.0.0')


