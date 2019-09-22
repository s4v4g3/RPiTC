
var chart;
var duration = 3600; // default = 1 hr of data

function reorgData(data) {
    return {
        pit_temp: data.map(a => [a.local_time*1000, a.oven_temp]),
        set_point: data.map(a => [a.local_time*1000, a.set_point]),
        output: data.map(a => [a.local_time*1000, a.output])
    }
}

function updateData() {
    requestData(updatePlot);
}

// update plot with new data
function updatePlot(data) {
    // reorg data
    let reorg_data = reorgData(data)

    // update the chart
    chart.series[0].setData(reorg_data.pit_temp, redraw=false)
    chart.series[1].setData(reorg_data.set_point, redraw=false)
    chart.series[2].setData(reorg_data.output, redraw=false)
    chart.redraw()
}

function requestData(callback) {
    $.ajax({
        url: `/data?sec=${duration}`,
        success: function(data) {
            callback(data);
        },
        complete: function(jq, status) {
        },
        cache: false
    });
}

function buttonCallback(btnData) {
    console.log(btnData)
    let range = btnData._range
    duration = range / 1000
    updateData();
}

function createPlot(data) {
    // reorg data
    let reorg_data = reorgData(data)
    // Create the chart
    chart = Highcharts.stockChart('container', {


        rangeSelector: {
            allButtonsEnabled: true,
            buttons: [{
                type: 'minute',
                count: 10,
                text: '10m',
                events: {
                    click: function() {
                        chart.rangeSelector.selected = 0
                        buttonCallback(this);
                        
                    }
                }
            },
            {
                type: 'hour',
                count: 1,
                text: '1h',
                events: {
                    click: function() {
                        chart.rangeSelector.selected = 1
                        buttonCallback(this);
                    }
                }
            },
            {
                type: 'hour',
                count: 4,
                text: '4h',
                events: {
                    click: function() {
                        chart.rangeSelector.selected = 2
                        buttonCallback(this);
                    }
                }
            },
            {
                type: 'hour',
                count: 8,
                text: '1h',
                events: {
                    click: function() {
                        chart.rangeSelector.selected = 3
                        buttonCallback(this);
                    }
                }
            },
            {
                type: 'hour',
                count: 12,
                text: '12h',
                events: {
                    click: function() {
                        chart.rangeSelector.selected = 4
                        buttonCallback(this);
                    }
                }
            },
            {
                type: 'day',
                count: 1,
                text: '1d',
                events: {
                    click: function() {
                        chart.rangeSelector.selected = 5
                        buttonCallback(this);
                    }
                }
            }, {
                type: 'day',
                count: 3,
                text: '3d',
                events: {
                    click: function() {
                        chart.rangeSelector.selected = 6
                        buttonCallback(this);
                    }
                }
            }, {
                type: 'all',
                text: 'All',
                events: {
                    click: function() {
                        chart.rangeSelector.selected = 7
                        console.log(this)
                        duration = 450000000 // 10 yr
                        updateData();
                    }
                }
            }]
        },

        title: {
            text: 'Big Green Egg Temperature'
        },
        xAxis: {
            type: 'datetime',
            labels: {
                format: '{value:%b%e %l:%M %p}'
            },
            events: {
                setExtremes: function(e) {
                    console.log(this);
                    if(typeof(e.rangeSelectorButton)!== 'undefined')
                    {
                      console.log('count: '+e.rangeSelectorButton.count + 'text: ' +e.rangeSelectorButton.text + ' type:' + e.rangeSelectorButton.type);
                    }
                }
            }
        },
        yAxis: [{
            opposite: true,
            title: {
                text: "temperature"
            },
        }, {
            opposite: false,
            title: {
                text: "output"
            },
            labels: {
                align: 'left'
            },
            min: 0,
            max: 100,
        }],

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
            },
            yAxis: 1
        }]
    });
}




$(() => {


    requestData(createPlot);
    setInterval(updateData, 15000);

});
