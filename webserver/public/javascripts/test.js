
var chart;

function reorgData(data) {
    return {
        pit_temp: data.map(a => [a.local_time*1000, a.oven_temp]),
        set_point: data.map(a => [a.local_time*1000, a.set_point]),
        output: data.map(a => [a.local_time*1000, a.output])
    }
}

function createPlot(data) {
    // reorg data
    let reorg_data = reorgData(data)
    // Create the chart
    chart = Highcharts.stockChart('container', {


        rangeSelector: {
            allButtonsEnabled: true,
            selected: 1,
            buttons: [{
                type: 'minute',
                count: 10,
                text: '10m'
            },
            {
                type: 'hour',
                count: 1,
                text: '1h'
            },
            {
                type: 'hour',
                count: 4,
                text: '4h'
            },
            {
                type: 'hour',
                count: 8,
                text: '1h'
            },
            {
                type: 'hour',
                count: 12,
                text: '12h'
            },
            {
                type: 'day',
                count: 1,
                text: '1d'
            }, {
                type: 'day',
                count: 3,
                text: '3d'
            }, {
                type: 'all',
                text: 'All'
            }]
        },

        title: {
            text: 'Big Green Egg Temperature'
        },
        xAxis: {
            type: 'datetime',
            labels: {
                format: '{value:%b%e %l:%M %p}'
            }
        },

        series: [{
            name: 'Pit Temperature',
            data: reorg_data.pit_temp,
            tooltip: {
                valueDecimals: 2
            }
        },
        {
            name: 'Set Point',
            data: reorg_data.set_point,
            tooltip: {
                valueDecimals: 0
            }
        },
        {
            name: 'Output',
            data: reorg_data.output,
            tooltip: {
                valueDecimals: 0
            }
        }]
    });
}

// update plot with new data
function updatePlot(data) {
    // reorg data
    let reorg_data = reorgData(data)

    // update the chart
    chart.series[0].setData(reorg_data.pit_temp, redraw=true)
    chart.series[1].setData(reorg_data.set_point, redraw=true)
    chart.series[2].setData(reorg_data.output, redraw=true)
}

function requestData(callback) {
    $.ajax({
        url: '/data',
        success: function(data) {
            callback(data);
        },
        complete: function(jq, status) {
        },
        cache: false
    });
}


$(() => {
    function doPlot() {
        requestData(updatePlot);
    }

    requestData(createPlot);
    setInterval(doPlot, 15000);

});
