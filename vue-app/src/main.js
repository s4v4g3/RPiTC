import Vue from 'vue'
import App from './App.vue'
import vuetify from './plugins/vuetify';
import router from './router'
import store from './store'
import HighchartsVue from 'highcharts-vue'
import Highcharts from "highcharts";
import exportingInit from "highcharts/modules/exporting";
import annotationsInit from "highcharts/modules/annotations";
import stockInit from "highcharts/modules/stock"
import highchartsMore from "highcharts/highcharts-more";
import dumbbellInit from "highcharts/modules/dumbbell"
annotationsInit(Highcharts);
exportingInit(Highcharts);
stockInit(Highcharts)
highchartsMore(Highcharts)
dumbbellInit(Highcharts)

Vue.config.productionTip = false

Vue.use(HighchartsVue)

new Vue({
  vuetify,
  router,
  store,
  render: h => h(App)
}).$mount('#app')
