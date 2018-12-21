<template>
  <div id="page-header">
    <img id="page-brand" src="../assets/brand.png" draggable="false">
  </div>
</template>

<script>
  export default {
    name: "PageHeader"
  }
</script>

<style scoped>
  #page-header {
    background: #FFF;
    box-shadow: 1px 1px 1px #ccc;
    box-sizing: border-box;
    user-select: none;
    border-bottom: 4px solid #389aff;
    margin-bottom: 4px;
  }
</style>