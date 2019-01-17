import Vue from 'vue'
import Router from 'vue-router'
import Main from './views/Main'
import PositionMain from './views/position/PositionMain'
import PositionDetails from './views/position/details/PositionDetails'
import MonitorMain from './views/monitor/MonitorMain'
import MonitorDetails from './views/monitor/details/MonitorDetails'
import StatsMain from './views/stats/StatsMain'
import StatsDetails from './views/stats/details/StatsDetails'
import ScheduleMain from './views/schedule/ScheduleMain'
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
        },
        {
          path: 'monitor',
          name: 'Monitor',
          component: MonitorMain,
          children: [
            {
              path: 'details',
              component: MonitorDetails
            }
          ]
        },
        {
          path: 'stats',
          name: 'Stats',
          component: StatsMain,
          children: [
            {
              path: 'details',
              component: StatsDetails
            }
          ]
        },
        {
          path: 'schedule',
          name: 'Schedule',
          component: ScheduleMain
        }
      ]
    }
  ]
})
