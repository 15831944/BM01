import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    busData: {},
    autoRefresh: true
  },
  mutations: {
    setBusData (state, data) {
      state.busData = {...data}
    },
    setAutoRefreshState (state, data) {
      state.autoRefresh = data
    }
  },
  actions: {

  }
})
