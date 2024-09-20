window.onload = () => {
  getParam(PIDParams.Kp);
  getParam(PIDParams.Kd);
  getParam(PIDParams.Ki);
}

const baseUrl = 'http://192.168.0.143'

function getParamUrl(param) {
  return baseUrl + '/get?param=' + param;
}

function setParamUrl(param, value) {
  return baseUrl + '/set?param=' + param + "&value=" + value;
}

const PIDParams = {
  Kp: 'kp',
  Ki: 'ki',
  Kd: 'kd'
}

const classIsLoading = 'is-loading';

function getParam(param) {
  fetch(getParamUrl(param))
    .then((async (resp) => {
      const value = await resp.text();
      document.getElementById(param).innerHTML = value;
    }))
    .catch(console.error);
}

function setParam(param, value) {
  const buttonName = "btnSubmit-" + param;
  document.getElementById(buttonName).classList.add(classIsLoading);
  
  fetch(setParamUrl(param, value), {method: "POST"})
  .then(async (resp) => {
    const value = await resp.text();
    document.getElementById(param).innerHTML = value;
    console.log(value);        
  })  
  .catch(console.error)
  .finally(() => {
    document.getElementById(buttonName).classList.remove(classIsLoading);
  });
}

function validateValue(value) {
  if (!value || isNaN(value)) {
    alert("un numerito pls");
    return false;
  } else if (value.length > 4) {
    alert("casi grande");
    return false;
  }

  return true;
}

const kpForm = document.getElementById("formKp");
kpForm.addEventListener("submit", (evt) => {
  evt.preventDefault();

  const formData = new FormData(kpForm);

  const value = formData.get("value");
  if (!validateValue(value)) 
    return;
  
  setParam(PIDParams.Kp, value);
})

const kiForm = document.getElementById("formKi");
kiForm.addEventListener("submit", (evt) => {
  evt.preventDefault();

  const formData = new FormData(kiForm);

  const value = formData.get("value");
  if (!validateValue(value)) 
    return;
    
  setParam(PIDParams.Ki, value);
})

const kdForm = document.getElementById("formKd");
kdForm.addEventListener("submit", (evt) => {
  evt.preventDefault();

  const formData = new FormData(kdForm);

  const value = formData.get("value");
  if (!validateValue(value)) 
    return;
  
  setParam(PIDParams.Kd, value);
})

function getKp(evt) {
    // var xhttp = new XMLHttpRequest();
    // xhttp.onreadystatechange = function() {
    //   if (this.readyState == 4 && this.status == 200) {
    //     document.getElementById("Kp").innerHTML =
    //     this.responseText;
    //   }
    // };
    // xhttp.open("GET", "/getKp", true);
    // xhttp.send();
    
  }
  
  function getKi() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("Ki").innerHTML =
        this.responseText;
      }
    };
    xhttp.open("GET", "/getKi", true);
    xhttp.send();
  }
  
  function getKd() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("Kd").innerHTML =
        this.responseText;
      }
    };
    xhttp.open("GET", "/getKd", true);
    xhttp.send();
  }
  
  // var chartT = new Highcharts.Chart({
  //   chart:{ renderTo : 'chart-temperature' },
  //   title: { text: 'PID Error' },
  //   series: [{
  //     showInLegend: false,
  //     data: []
  //   }],
  //   plotOptions: {
  //     line: { animation: false,
  //       dataLabels: { enabled: true }
  //     },
  //     series: { color: '#059e8a' }
  //   },
  //   xAxis: { type: 'datetime',
  //     dateTimeLabelFormats: { second: '%H:%M:%S' }
  //   },
  //   yAxis: {
  //     title: { text: 'Error' }
  //     //title: { text: 'Temperature (Fahrenheit)' }
  //   },
  //   credits: { enabled: false }
  // });
  
  // setInterval(function( ) {
  //   var xhttp = new XMLHttpRequest();
  //   xhttp.onreadystatechange = function() {
  //     if (this.readyState == 4 && this.status == 200) {
  //       var x = (new Date()).getTime(),
  //           y = parseFloat(this.responseText);
  //       //console.log(this.responseText);
  //       if(chartT.series[0].data.length > 40) {
  //         chartT.series[0].addPoint([x, y], true, true, true);
  //       } else {
  //         chartT.series[0].addPoint([x, y], true, false, true);
  //       }
  //     }
  //   };
  //   xhttp.open("GET", "/error", true);
  //   xhttp.send();
  // }, 1000 ) ;