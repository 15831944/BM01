<template>
  <div id="main-sidebar">
    <el-menu
        :default-active="activeIndex"
        class="sidebar"
        :router="true"
        :collapse="true">
      <el-menu-item index="/position">
        <i class="el-icon-location" style="color: #049fdb;"></i>
        <span slot="title">车辆定位</span>
      </el-menu-item>
      <!--  <el-submenu index="/operations">
          <template slot="title">
            <i class="el-icon-setting"></i>
            <span>烧写操作管理</span>
          </template>
          <el-menu-item index="/operations/default">操作管理表</el-menu-item>
          <el-menu-item index="/operations/bind">版本操作绑定表</el-menu-item>
        </el-submenu>
        <el-menu-item index="/users" v-if="this.$store.state.userType > 2">
          <i class="el-icon-t-users"></i>
          <span slot="title">用户管理</span>
        </el-menu-item>-->
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
      $route: function (val) {
        this.activeIndex = val.path
      }
    },
    mounted() {
      this.activeIndex = this.$route.path
    },
    methods: {
     /* logout: function () {
        let options = {
          url: logoutUrl,
          data: {}
        };
        axiosPost(options).then(res => {
          if (res.data.result === 200 || res.data.result === 400) {
            store.commit('setLoginToken', '');
            store.commit('setUserType', '');
            localStorage.removeItem('token');
            localStorage.removeItem('userType');
            window.location.href = window.g.SYSTEM_PATH + '/#/login'
          } else {
            this.$alertDanger(res.data.result)
          }
        }).catch(err => {
          console.log(JSON.stringify(err));
        })

      }*/
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
</style>