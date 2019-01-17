<template>
  <div id="main-sidebar">
    <el-menu
        class="sidebar"
        :router="true"
        :collapse="false">
      <el-menu-item index="/position">
        <i class="el-icon-location" :class="{isChosed: isChosed('position')}"></i>
        <span slot="title">车辆定位</span>
      </el-menu-item>
      <el-menu-item index="/monitor">
        <i class="el-icon-t-monitor" :class="{isChosed: isChosed('monitor')}"></i>
        <span slot="title">视频监控</span>
      </el-menu-item>
      <el-menu-item index="/stats">
        <i class="el-icon-t-table" :class="{isChosed: isChosed('stats')}"></i>
        <span slot="title">数据统计</span>
      </el-menu-item>
      <el-menu-item index="/schedule">
        <i class="el-icon-t-date" :class="{isChosed: isChosed('schedule')}"></i>
        <span slot="title">职员排班</span>
      </el-menu-item>
    </el-menu>

    <div class="setting">
      <el-tooltip class="item" effect="dark" content="配置" placement="top">
        <div class="power-icon">
          <i class="el-icon-setting"></i>
        </div>
      </el-tooltip>

    </div>
  </div>
</template>

<script>
  import store from '../store'
  // import {logoutUrl} from "../config/globalUrl";
  import {axiosPost} from "../utils/fetchData";

  export default {
    name: "SideBar",
    data() {
      return {
        activeIndex: ''
      }
    },
    watch: {
    },
    mounted() {
    },
    computed: {

    },
    methods: {
      isChosed: function (val) {
        return this.$route.path.split('/')[1] === val

      }
    }
  }
</script>

<style scoped>
  #main-sidebar {
    position: relative;
    display: flex;
    flex-direction: column;
    /*width: 220px;*/
    height: 100%;
    background-color: #ffffff;
  }

  .sidebar {
    border: none;
  }

  .setting {
    position: relative;
    display: flex;
    /*width: 220px;*/
    min-height: 40px;
    border-top: 1px solid #ddd;
    margin-top: auto;
    justify-content: center;
    align-items: center;
    background-color: #ffffff;
  }

  .power-icon {
    display: block;
    width: 16px;
    height: 16px;
    border-radius: 16px;
    padding: 8px;
    cursor: pointer;
  }

  .power-icon i {
    font-size: 16px;
    display: block;
    color: #aaa;
  }
  .isChosed {
    color: #409EFF;
  }
</style>