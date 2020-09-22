
var chart;
const minDuration = 60 * 60 * 24 * 1; // default = 1 day of data
var currDuration = minDuration; 
var skipSetPointUpdate = false;

function doSettingsPost(post_data, success_cb, failure_cb) {
    $.ajax({
        type: "POST",
        url: "/settings",
        data: JSON.stringify(post_data),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        success: function(data){
            success_cb(data)
        },
        failure: function(errMsg) {
            failure_cb(errMsg)
        }
    });
}

function changeSetPoint(value) {
    post_data = {
        pid_config: {
            set_point: value
        }
    }
    console.log(`changing setpoint to ${value}`)
    doSettingsPost(post_data, 
        function(data) {
            skipSetPointUpdate = true
            console.log(JSON.stringify(data))
        },
        function(errMsg) {
            alert(errMsg);
        })
}

function clearWindup() {
    post_data = {
        pid_state: {
            error_sum: 0.0001
        }
    }
    doSettingsPost(post_data, 
        function(data) {
            console.log(JSON.stringify(data))
        },
        function(errMsg) {
            alert(errMsg);
        })
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
        url: `/data?sec=${currDuration}`,
        success: function(data) {
            callback(data);
        },
        complete: function(jq, status) {
        },
        cache: false
    });
}

function buttonCallback(btnData) {
    
    // button range in seconds
    let btnRange = btnData._range / 1000
    // add 1hr buffer to avoid button being de-selected
    btnRange += 3600
    if (btnRange > currDuration) {
        // button range exceeds our current duration
        // so update it and request the data
        currDuration = btnRange;
        updateData();
    }
    else {
        // button range is smaller than the current
        // duration.  Just decrease duration for the next
        // data update but don't force another update
        currDuration = btnRange
    }
}

function updatePidTable(data) {
    tableData = [
        data.output,
        data.kp,
        data.kd,
        data.ki,
        data.avg_oven_temp,
        data.oven_temp,
        data.p_term,
        data.d_term,
        data.error_sum,
    ]
    $('#pid_data td').each(function(i) {
        $(this).text(`${tableData[i]}`)
    });
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
            selected: 3, // 8 hr
            buttons: [{
                type: 'minute',
                count: 10,
                text: '10m',
                events: {
                    click: function() {
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
                        buttonCallback(this);
                    }
                }
            },
            {
                type: 'hour',
                count: 8,
                text: '8h',
                events: {
                    click: function() {
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
                        buttonCallback(this);
                    }
                }
            }, {
                type: 'day',
                count: 3,
                text: '3d',
                events: {
                    click: function() {
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


function initSetPointSlider() {
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
}

$(() => {

    // initSettingsHandlers();
    requestData(createPlot);
    setInterval(updateData, 15000);
    initSetPointSlider();

    $('#clearWindup').on("click", function(e){
        // clear windup
        clearWindup();
    });
    

    

});
