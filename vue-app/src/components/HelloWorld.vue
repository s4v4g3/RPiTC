<template>
    <v-container>
        <v-row class="text-center">
            <v-col cols="12">
                <highcharts :constructor-type="'stockChart'" :options="chartOptions" ref="chart" />
            </v-col>
        </v-row>
    </v-container>
</template>

<script>
import axios from "axios";
export default {
    name: "Chart",

    data: () => ({
        // stuff here
        chartOptions: {
            rangeSelector: {
                allButtonsEnabled: true,
                selected: 3, // 8 hr
                buttons: [
                    {
                        type: "minute",
                        count: 10,
                        text: "10m",
                        events: {
                            click: function () {},
                        },
                    },
                    {
                        type: "hour",
                        count: 1,
                        text: "1h",
                        events: {
                            click: () => {},
                        },
                    },
                    {
                        type: "hour",
                        count: 4,
                        text: "4h",
                        events: {
                            click: function () {},
                        },
                    },
                    {
                        type: "hour",
                        count: 8,
                        text: "8h",
                        events: {
                            click: function () {},
                        },
                    },
                    {
                        type: "hour",
                        count: 12,
                        text: "12h",
                        events: {
                            click: function () {},
                        },
                    },
                    {
                        type: "day",
                        count: 1,
                        text: "1d",
                        events: {
                            click: function () {},
                        },
                    },
                    {
                        type: "day",
                        count: 3,
                        text: "3d",
                        events: {
                            click: function () {},
                        },
                    },
                    {
                        type: "all",
                        text: "All",
                        events: {
                            click: function () {
                                /*
                        chart.rangeSelector.selected = 7
                        console.log(this)
                        duration = 450000000 // 10 yr
                        updateData();
                        */
                            },
                        },
                    },
                ],
            },

            title: {
                text: "Big Green Egg Temperature",
            },
            xAxis: {
                type: "datetime",
                labels: {
                    format: "{value:%b%e %l:%M %p}",
                },
                events: {
                    setExtremes: function (e) {
                        console.log(this);
                        if (typeof e.rangeSelectorButton !== "undefined") {
                            console.log("setExtremes:");
                            console.log(
                                `count: ${e.rangeSelectorButton.count}  text: ${e.rangeSelectorButton.text} type: ${e.rangeSelectorButton.type}`
                            );
                            let btnRange = e.rangeSelectorButton._range / 1000;
                            btnRange += 3600;
                            if (btnRange > this.currDuration) {
                                // button range exceeds our current duration
                                // so update it and request the data
                                this.currDuration = btnRange;
                                this.requestData();
                            } else {
                                // button range is smaller than the current
                                // duration.  Just decrease duration for the next
                                // data update but don't force another update
                                this.currDuration = btnRange;
                            }
                        }
                    },
                },
            },
            yAxis: [
                {
                    opposite: true,
                    title: {
                        text: "temperature",
                    },
                },
                {
                    opposite: false,
                    title: {
                        text: "output",
                    },
                    labels: {
                        align: "left",
                    },
                    min: 0,
                    max: 100,
                },
            ],

            series: [
                {
                    name: "Pit Temperature",
                    data: [],
                    tooltip: {
                        valueDecimals: 2,
                    },
                },
                {
                    name: "Set Point",
                    data: [],
                    tooltip: {
                        valueDecimals: 0,
                    },
                },
                {
                    name: "Output",
                    data: [],
                    tooltip: {
                        valueDecimals: 0,
                    },
                    yAxis: 1,
                },
            ],
        },
        pitTempData: [],
        setPointData: [],
        outputData: [],
        currDuration: 3600,
        interval: null,
    }),
    created() {
        /*
        this.chartOptions.rangeSelector.buttons.forEach((button, i) => {
            button.events.click = (event) => {
                this.buttonCallback(event, i);
            };
        });
        */
        this.chartOptions.xAxis.events.setExtremes = (e) => {
            this.buttonCallback(e);
        };
        this.interval = setInterval(this.requestData, 15000);
        this.requestData();
    },
    destroyed() {
        console.log("Destroying!")
        clearInterval(this.interval)
    },
    methods: {
        requestData() {
            let url = `http://pibbq.savage.zone/data?sec=${this.currDuration}`;
            console.log(url)
            axios.get(url).then((response) => {
                console.log(response)
                this.pitTempData = response.data.map((a) => [
                    a.local_time * 1000,
                    a.oven_temp,
                ]);
                this.setPointData = response.data.map((a) => [
                    a.local_time * 1000,
                    a.set_point,
                ]);
                this.outputData = response.data.map((a) => [
                    a.local_time * 1000,
                    a.output,
                ]);
                this.chartOptions.series[0].data = this.pitTempData
                this.chartOptions.series[1].data = this.setPointData
                this.chartOptions.series[2].data = this.outputData
                
                //this.$refs.chart.chart.series[0].setData(this.pitTempData);
                //this.$refs.chart.chart.series[1].setData(this.setPointData);
                //this.$refs.chart.chart.series[2].setData(this.outputData);
            });
        },
        buttonCallback(e) {
            if (typeof e.rangeSelectorButton !== "undefined") {
                console.log("setExtremes:");
                console.log(e)
                console.log(
                    `count: ${e.rangeSelectorButton.count}  text: ${e.rangeSelectorButton.text} type: ${e.rangeSelectorButton.type}  range: ${e.rangeSelectorButton._range }`
                );
                let btnRange = e.rangeSelectorButton._range / 1000;
                if (isNaN(btnRange)) btnRange = 450000000
                console.log(btnRange)
                btnRange += 3600;
                if (btnRange > this.currDuration) {
                    // button range exceeds our current duration
                    // so update it and request the data
                    this.currDuration = btnRange;
                    this.requestData();
                } else {
                    // button range is smaller than the current
                    // duration.  Just decrease duration for the next
                    // data update but don't force another update
                    this.currDuration = btnRange;
                }
            }
        },
    },
};
</script>
