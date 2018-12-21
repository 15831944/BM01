export const debounce = function (func, wait, immediate) {
  let time
  let debounced = function() {
    let context = this
    if(time) clearTimeout(time)

    if(immediate) {
      let callNow = !time
      if(callNow) func.apply(context, arguments)
      time = setTimeout(
        ()=>{time = null} //见注解
        , wait)
    } else {
      time = setTimeout(
        ()=>{func.apply(context, arguments)}
        , wait)
    }
  }

  debounced.cancel = function() {
    clearTimeout(time)
    time = null
  }

  return debounced
}


export const throttle = function (method,delay,duration){
  let timer=null, begin=new Date();
  return function(){
    let context=this, args=arguments, current=new Date();
    clearTimeout(timer);
    if(current-begin>=duration){
      method.apply(context,args);
      begin=current;
    }else{
      timer=setTimeout(function(){
        method.apply(context,args);
      },delay);
    }
  }
}