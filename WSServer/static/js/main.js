"use strict"

const STORED_DATA_JSON_URL = "http://0.0.0.0:50000/static/data/stored_data.json";

function testFetch() {
    fetch('static/stored_data.json').then((response) => console.log(response));
}

function generateFakeWindData() {
  let rawData = [];

    for (let i = 0; i < 15; i++) {
        let data = {x: moment().add(360*i,'s').toDate(), 
                    y: Math.round(Math.random()*50),
                    dir: Math.round(Math.random()*359)}
    

        rawData.push(data);
    }

    return rawData;

}


let c = Date.now();
function dynamicDataGen() {
    let date
}


let data = generateFakeWindData();

// Arrow image for points on windchart graph (wind direction)



function generateWindChart () {
    let windchart_ctx = document.getElementById('windchart').getContext('2d');

    let newArrow = new Image();
    newArrow.src = '/static/img/arrow.svg';
    
    
    let windchart = new Chart(windchart_ctx, {
        type: 'line',
        data: {
            datasets: [{
                label: "Windspeed",
                pointStyle: newArrow,
                data: data,
                fill: false,
                borderColor: "rgb(75,192,192)",
                lineTension: 0.2,
                pointBackgroundColor: 'red',
                
                radius: 20,
                rotation: function (context) {
                    let index = context.dataIndex;
                    let dir = context.dataset.data[index].dir;
                    return dir
                }
            },]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            title: "Windspeed",
            scales: {
                xAxes: [{
                    type: 'time',
                    time: {
                        unit: "minute",
                        unitStepSize: 5
                    },
                    display: true,
                    scaleLabel: {
                        display: true,
                    }
                }],
                yAxes: [{
                    gridLines: {
                      //  drawBorder: false
                    }
                }]
            },
            
            
        }
    
    })
}

window.onload = generateWindChart();