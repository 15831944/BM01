<template>
  <el-container>
    <el-aside width="186px">
      <div id="group-container">
        <el-menu
            :default-openeds="['menu']"
          :router="true">
          <el-submenu
              index="menu"
              class="menu-title"
          >
            <template slot="title">
              <i class="el-icon-t-sign"></i>
              <span>测试线路(1)</span>
            </template>
            <el-menu-item
              index="/monitor/details">
              <span class="status-circle" :class="getStatusColor(0)"></span>
              <span class="num-plate">粤L12345</span>
            </el-menu-item>
          </el-submenu>
        </el-menu>
        <div class="status-remarks">
          <div>
            <span class="status-circle green"></span>
            <p>正常</p>
          </div>
          <div>
            <span class="status-circle grey"></span>
            <p>静止</p>
          </div>
          <div>
            <span class="status-circle red"></span>
            <p>离线</p>
          </div>
        </div>
      </div>
    </el-aside>
    <el-main>
      <router-view/>
      <!--<el-footer class="page-footer" height="40px">-->
      <!--<copyright/>-->
      <!--</el-footer>-->
    </el-main>
  </el-container>
</template>

<script>
  import Copyright from "../../components/Copyright";
  import {getGroupsUrl, getGroupDetailsUrl, getRealtimeDataUrl} from "../../config/globalUrl";
  import {axiosPost} from "../../utils/fetchData";
  export default {
    name: "MonitorMain",
    components: {
      Copyright
    },
    data() {
      return {
        /*data format:
        * [{
        *   groupId: '1',
        *   groupName: '7路'
        * }]
        * */
        //组信息
        groupListData: [],
        //组内项目
        groupItemObject: {},
        isPending: false
      }
    },
    methods: {

      getStatusColor: function (status) {
        switch (status) {
          case 0:
            return 'green';
          case 1:
            return 'grey';
          case 2:
            return 'red';
        }
      },

    }
  }
</script>

<style scoped>
  .el-container {
    height: 100%;
  }

  .el-aside{
    box-shadow: 0 0 5px #ddd;
  }

  #group-container {
    position: relative;
    height: 100%;
    background-color: rgba(255, 255, 255, 0.8);
    border-right: 1px solid #ddd;
    display: flex;
    flex-direction: column;
  }

  .el-icon-t-sign {
    margin-bottom: 6px;
    font-size: 24px !important;
    color: #303133;
  }

  .el-menu {
    border: none;
    opacity: 1;
  }

  .status-remarks {
    position: relative;
    display:  flex;
    min-height: 40px;
    border-top: 1px solid #ddd;
    margin-top: auto;
    font-size: 8px;
    justify-content: space-around;
    align-items: center;
  }
  .status-remarks div {
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    width: 71px;
  }
  .status-circle  {
    height: 12px;
    width: 12px;
    border-radius: 6px;
  }
  .status-remarks div p {
    margin: 0;
  }
  .green {
    background-color: greenyellow;
    color: greenyellow;
  }
  .grey {
    background-color: grey;
    color: grey;
  }
  .red {
    background-color: red;
    color: red;
  }

  .menu-title >>> .el-submenu__title {
    background-color: #389aff;
    z-index: 100;
    margin: 4px 4px 4px 0;
    line-height: 36px;
    height: 36px;
    box-shadow: 0 1px 5px #ccc;
    border-radius: 0 18px 18px 0;
    padding-left: 12px !important;
    color: #ffffff;
  }

  .menu-title >>> .el-submenu__title i {
    color: #ffffff;
  }

  .menu-title >>> .el-menu-item {
    display: flex;
    align-items: center;
    line-height: 40px;
    height: 40px;
    padding: 2px 12px !important;
    border-radius: 20px 0 0 20px;
    box-shadow: 0 1px 5px #ddd;
    margin-bottom: 2px;
    margin-left: 20px;
    min-width: unset;

  }
  .menu-title >>> .el-menu-item .status-circle{
    display: inline-block;
    margin: 0 10px;
  }
  .menu-title >>> .el-menu-item.is-active{
    background-color: #f5f5f5;
  }
  .num-plate {
    display: inline-block;
    line-height: initial;
  }

  .bus-status {
    display: inline-block;
    line-height: initial;
  }
</style>