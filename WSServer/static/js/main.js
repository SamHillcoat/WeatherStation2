"use strict"

const STORED_DATA_JSON_URL = "http://0.0.0.0:50000/static/data/stored_data.json";

function fetchJsonData() {
    fetch('static/stored_data.json')
        .then(response => response.json())
        .then(data => updateData(windchart, data));
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



function updateData(chart, newData) {
    // callback from fetchJsonData(), runs when the json data is loaded from file
    // updates the data on the chart
    let diffData = newData.filter(obj => !chart.data.datasets[0].data.includes(obj["x"]));
    if (chart.data.datasets[0].data.length >= 0) {
        chart.data.datasets[0].data = newData;
        chart.update();
        return
    }
    /*if (diffData.length > 0) {
        // Append the different data to the chart data array
        chart.data.datasets[0].data.push.apply(chart.data.datasets[0].data, diffData);
        chart.update();
        console.log("Pushed new data to chart");
    } */else {
        return
    }

}



let windchart;
function generateWindChart () {
    let windchart_ctx = document.getElementById('windchart').getContext('2d');

    // Arrow image for wind direction
    let newArrow = new Image();
    newArrow.src = '/static/img/arrow.svg';
    
    
    windchart = new Chart(windchart_ctx, {
        type: 'line',
        data: {
            datasets: [{
                label: "Windspeed",
                pointStyle: newArrow,
                data: [],
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
                        unit: "second",
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
setInterval(fetchJsonData, 5000);