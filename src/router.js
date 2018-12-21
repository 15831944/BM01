import Vue from 'vue'
import Router from 'vue-router'
import Main from './views/Main'
import PositionMain from './views/position/PositionMain'
import PositionDetails from './views/position/details/PositionDetails'
Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '_blank'
    },
    {
      path: '/',
      name: 'Main',
      component: Main,
      children: [
        {
          path: 'position',
          name: 'Position',
          component: PositionMain,
          children: [
            {
              path: 'details',
              name: 'PositionDetails',
              component: PositionDetails
            }
          ]
        }
      ]
    }
  ]
})
