<template>
  <div id="sm-details">
    <el-row class="details-controller" type="flex" align="bottom" style="flex-wrap: wrap; width: 100%;">
      <el-col style="width: unset; margin: 5px 5px 0;">
        <span class="details-title">日期范围:</span>
        <el-date-picker
            size="mini"
            v-model="timeRange"
            type="datetimerange"
            :picker-options="pickerOptions"
            range-separator="-"
            prefix-icon="el-icon-date"
            start-placeholder="开始日期"
            end-placeholder="结束日期"
            value-format="yyyy-MM-dd HH:mm:ss">
        </el-date-picker>
      </el-col>
      <el-col style="width: unset; margin: 0 5px;">
        <el-button size="mini" type="primary" style="padding: 6px 10px;" @click="queryData">
          查询
        </el-button>
      </el-col>
    </el-row>
    <div class="divider"></div>
    <div class="details-content">
      <el-alert
          show-icon
          style="margin-bottom: 10px"
          :closable="false"
          :type="messageOptions.type"
          :title="messageOptions.title">
      </el-alert>
      <el-tabs type="border-card">
        <!--<el-tab-pane label="图表">图表</el-tab-pane>-->
        <el-tab-pane label="详细">
          <el-table
              :data="shownTableData"
              max-height="560"
              stripe
              style="width: 100%">
            <el-table-column v-for="(item, index) in getTableColumns"
                             :key="index"
                             :prop=item.prop
                             :label=item.label
                             :width=item.width
                             :sortable=item.sortable>
            </el-table-column>
          </el-table>
          <!--分页控制-->
          <el-pagination
              background
              :current-page.sync="paginationOptions.currentPage"
              :page-sizes="[20, 40]"
              :page-size.sync="paginationOptions.pageSize"
              layout="total, sizes, prev, pager, next, jumper"
              :total="paginationOptions.total"
              @current-change="pagination"
              @size-change="pagination"
              class="page-pagination">
          </el-pagination>
        </el-tab-pane>
      </el-tabs>
    </div>
  </div>
</template>

<script>
  import {getIOStatsUrl} from "../../../config/globalUrl";
  import {axiosPost} from "../../../utils/fetchData";
  import {tableColumns} from "../../../config/statsConfig";

  export default {
    name: "StatsDetails",
    data() {
      return {
        timeRange: '',
        pickerOptions: {
          shortcuts: [{
            text: '最近一天',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近三天',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 3);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近一周',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 7);
              picker.$emit('pick', [start, end]);
            }
          }]
        },
        isPending: false,
        messageOptions: {
          type: 'info',
          title: '暂无数据'
        },
        tableData: [],
        shownTableData: [],
        //分页选项
        paginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },
        queryStartTime: '',
        queryEndTime: ''
      }
    },
    watch: {
      $route: function (val) {
        this.fetchData();
        this.$closeFSLoading()
      }
    },
    mounted() {
      this.fetchData();
      this.$closeFSLoading();

    },
    computed: {
      getTableColumns: function () {
        return tableColumns()
      }
    },
    methods: {
      queryData: function () {
        if (this.timeRange === "" || !this.timeRange) {
          this.$alertWarning("日期未选择");
          return;
        }
        this.fetchData({
          imei: this.$route.query.imei,
          beginTime: this.timeRange[0],
          endTime: this.timeRange[1]
        })
      },
      pagination: function () {
        // this.fetchData({
        //   imei: this.$route.query.imei,
        //   beginTime: this.queryStartTime,
        //   endTime: this.queryEndTime,
        //   currentPage: this.paginationOptions.currentPage,
        //   pageSize: this.paginationOptions.pageSize
        // })
        let start = (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize;
        let end = start + this.paginationOptions.pageSize;
        this.shownTableData = this.tableData.slice(start, end);
      },
      fetchData: function (data) {
        if (!data) {
          data = {
            imei: this.$route.query.imei,
            beginTime: this.getCurrentFormattedDate(24),
            endTime: this.getCurrentFormattedDate()
          };
        }
        this.queryStartTime = data.beginTime;
        this.queryEndTime = data.endTime;
        if (!this.isPending) {
          axiosPost({
            url: getIOStatsUrl,
            data: data
          }).then(res => {
            if (res.data.result === 200) {
              if (res.data.data.list.length === 0) {
                this.messageOptions.title = "该车在 " + this.queryStartTime + " 至 " + this.queryEndTime + " 时间段内无上下站信息";
              } else {
                this.messageOptions.title = "正显示该车在 " + this.queryStartTime + " 至 " + this.queryEndTime + " 时间段内数据报表";
              }
              this.tableData = res.data.data.list;
              //this.shownTableData = res.data.data.list;
              this.paginationOptions.currentPage = 1;
              this.paginationOptions.total = res.data.data.totalRow;
              this.pagination();

            }
          })
        }
      },

      //init chart


      //offset: previous offset hours
      getCurrentFormattedDate: function (offset) {
        let date = new Date();
        if (offset) {
          date.setTime(date.getTime() - 3600 * 1000 * offset)
        }
        let separatorDate = "-";
        let separatorTime = ":";
        let year = date.getFullYear();
        let month = date.getMonth() + 1;
        let strDate = date.getDate();
        let hours = date.getHours();
        let minutes = date.getMinutes();
        let seconds = date.getSeconds();
        if (month >= 1 && month <= 9) {
          month = "0" + month;
        }
        if (strDate >= 0 && strDate <= 9) {
          strDate = "0" + strDate;
        }
        if (hours >= 0 && hours <= 9) {
          hours = "0" + hours;
        }
        if (minutes >= 0 && minutes <= 9) {
          minutes = "0" + minutes;
        }
        if (seconds >= 0 && seconds <= 9) {
          seconds = "0" + seconds;
        }
        return year + separatorDate + month + separatorDate + strDate + ' ' + hours + separatorTime + minutes + separatorTime + seconds;
      },

    }
  }
</script>

<style scoped>
  #sm-details {
    position: relative;
    background: rgba(255, 255, 255, 0.9);
    width: 100%;
    height: 100%;
    border-radius: 4px;
  }

  .details-controller {
    position: relative;
    padding: 20px 30px;
  }

  .details-title {
    display: block;
    line-height: 10px;
    font-size: 10px;
    color: #888;
    margin-bottom: 12px;
  }

  .divider {
    width: calc(100% - 40px);
    height: 2px;
    border-top: 1px solid #dcdfe6;
    margin: 0 auto;
  }

  .details-content {
    width: calc(100% - 40px);
    /*border: 1px solid #ebebeb;*/
    /*border-radius: 3px;*/
    padding: 20px 20px;
  }

  .details-content .el-tabs {
    box-shadow: none;
    border-radius: 3px;
  }

  .page-pagination {
    display: flex;
    flex-wrap: wrap;
    margin: 20px 0 0;
    padding: 0 20px;
  }
</style>