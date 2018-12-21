import Vue from 'vue'
import App from './App.vue'
import axios from 'axios'
import router from './router'
import store from './store'
import './plugins/element.js'

//自定义注册图标
import './assets/icons/iconfont.css'

//全局注册toast提示框
import {alertDanger, alertInfo, alertSuccess,alertWarning} from "./utils/modal";

//加载动画
import {openFSLoading, closeFSLoading} from "./utils/loading";

Vue.prototype.$openFSLoading = openFSLoading;
Vue.prototype.$closeFSLoading = closeFSLoading;


Vue.prototype.$alertDanger = alertDanger;
Vue.prototype.$alertInfo = alertInfo;
Vue.prototype.$alertSuccess = alertSuccess;
Vue.prototype.$alertWarning = alertWarning;

Vue.prototype.$axios = axios

Vue.config.productionTip = false

new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
