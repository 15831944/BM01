<template>
  <el-container>
    <el-aside width="186px">
      <div id="group-container">
        <el-menu
            :collapse-transition='false'
            @select="selectItem"
            @open="expandGroup">
          <el-submenu
              :index="item.groupId.toString()"
              :key="item.groupId"
              v-for="item in groupListData"
              v-if="groupListData.length > 0"
              class="menu-title"
          >
            <template slot="title">
              <i class="el-icon-t-sign"></i>
              <span>{{item.groupName}} ({{item.count}})</span>
            </template>
            <el-menu-item :key="item.groupId + menuItem.imei" :index="(item.groupId + ':' + menuItem.imei).toString()"
                          v-for="menuItem in groupItemObject['item' + item.groupId]">
              <span class="status-circle" :class="getStatusColor(menuItem.status)"></span>
              <span class="num-plate">{{menuItem.numPlate}}</span>
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
  import {axiosPost} from "../../utils/fetchData";
  import {getIOStatsUrl, getGroupDetailsUrl, getGroupsUrl} from "../../config/globalUrl";

  export default {
    name: "StatsMain",
    data() {
      return {

        groupListData: [],
        //组内项目
        groupItemObject: {},
        isPending: false
      }
    },
    created() {
      axiosPost({url: getGroupsUrl}).then(res => {
        this.groupListData = res.data.data;
        //初始化一遍分组的数据结构
        for (let item in this.groupListData) {
          this.$set(this.groupItemObject, 'item' + (eval(item) + 1).toString(), [])
        }
      }).catch(err => {
        this.$alertWarning('获取分组信息失败，请刷新重试')
      })
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
      expandGroup: function (index) {
        axiosPost({
          url: getGroupDetailsUrl,
          data: {
            groupId: index
          }
        }).then(res => {
          //将返回数组内的组id作为对象的属性名
          this.$set(this.groupItemObject, "item" + index, JSON.parse(JSON.stringify(res.data.data)));
        }).catch(err => {
          this.$alertWarning(err.response.data)
        })
      },
      selectItem: function (val) {
        //index format: groupid:imei
        this.$openFSLoading();
        let groupId = val.split(':')[0];
        let imei = val.split(':')[1];
        this.showDetails(imei, groupId)
      },
      showDetails: function (imei, groupId) {
        this.$router.push('_blank');
        this.$router.replace({
          path: '/stats/details',
          query: {
            'id': groupId,
            'imei': imei
          }
        })
      }
    }
  }
</script>

<style scoped>
  .el-container {
    height: 100%;
  }

  .el-aside {
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
    display: flex;
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

  .status-circle {
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

  .menu-title >>> .el-menu-item .status-circle {
    display: inline-block;
    margin: 0 10px;
  }

  .menu-title >>> .el-menu-item.is-active {
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