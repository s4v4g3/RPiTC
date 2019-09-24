
var chart;
var duration = 3600; // default = 1 hr of data
var skipSetPointUpdate = false;

function changeSetPoint(value) {
    post_data = {
        pid_config: {
            set_point: value
        }
    }
    $.ajax({
        type: "POST",
        url: "/settings",
        data: JSON.stringify(post_data),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        success: function(data){
            skipSetPointUpdate = true
            console.log(JSON.stringify(data))
        },
        failure: function(errMsg) {
            alert(errMsg);
        }
    });
}

function updateSetPoint(value){
    if (!skipSetPointUpdate) {
        $('#setPointSlider').val(value).change();
    }
    skipSetPointUpdate = false
}


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
    updatePidTable(data[data.length-1])
    updateSetPoint(data[data.length-1].set_point)
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

function updatePidTable(data) {
    function appendData(row, value) {
        row += `<td>${value}</td>`
        return row
    }
    let row = "<tr>"
    row = appendData(row, data.output)
    row = appendData(row, data.kp)
    row = appendData(row, data.kd)
    row = appendData(row, data.ki)
    row = appendData(row, data.avg_oven_temp)
    row = appendData(row, data.oven_temp)
    row = appendData(row, data.p_term)
    row = appendData(row, data.d_term)
    row = appendData(row, data.error_sum)
    row += "</tr>"
    $('#pid_data').replaceWith(row);
}

function createPlot(data) {
    updatePidTable(data[data.length-1])
    updateSetPoint(data[data.length-1].set_point)
    
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

    // initSettingsHandlers();
    requestData(createPlot);
    setInterval(updateData, 15000);

    // For ie8 support
    var $document = $(document);
    var textContent = ('textContent' in document) ? 'textContent' : 'innerText';

    function valueOutput(element) {
        var value = element.value;
        var output = element.parentNode.getElementsByTagName('output')[0] || element.parentNode.parentNode.getElementsByTagName('output')[0];
        output[textContent] = value;
    }

    var selector = '#setPointSlider'
    $document.on('input', selector, function(e) {
        valueOutput(e.target);
    });      

    // rangeslider initialization
    $(selector).rangeslider({

        // Deactivate the feature detection
        polyfill: false,

        // Callback function
        onInit: function() {
            valueOutput(this.$element[0]);
        },

        // Callback function
        onSlide: function(position, value) {
            console.log('onSlide');
            console.log('position: ' + position, 'value: ' + value);
        },

        // Callback function
        onSlideEnd: function(position, value) {
            console.log('onSlideEnd');
            console.log('position: ' + position, 'value: ' + value);
            changeSetPoint(value);
        }
    });

    

});
