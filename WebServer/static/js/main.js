"use strict"



function generateFakeWindData() {
  let rawData = [];

    for (let i = 0; i < 15; i++) {
        let data = {x: moment().add(360*i,'s').toDate(), 
                    y: Math.round(Math.random()*50)};

        rawData.push(data);
    }

    return rawData;

}

let c = Date.now();
function dynamicDataGen() {
    let date
}


let data = generateFakeWindData();


let windchart_ctx = document.getElementById('windchart').getContext('2d');

let windchart = new Chart(windchart_ctx, {
    type: 'line',
    data: {
        datasets: [{
            label: "Windspeed",
            data: data,
            fill: false,
            borderColor: "rgb(75,192,192)",
            lineTension: 0.2
        }]
    },
    options: {
        responsive: true,
        title: "Windspeed",
        scales: {
            xAxes: [{
                type: 'time',
                time: {
                    //unit: "minute",
                   // parser: "DATETIME_LOCAL_SECONDS"
                },
                display: true,
                scaleLabel: {
                    display: true,
                }
            }]
        }
    }

})
