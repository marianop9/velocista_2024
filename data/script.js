window.onload = function () {
  getKp();
  getKd();
  getKi();
}

function getKp() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("Kp").innerHTML =
        this.responseText;
      }
    };
    xhttp.open("GET", "/getKp", true);
    xhttp.send();
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
  
  var chartT = new Highcharts.Chart({
    chart:{ renderTo : 'chart-temperature' },
    title: { text: 'PID Error' },
    series: [{
      showInLegend: false,
      data: []
    }],
    plotOptions: {
      line: { animation: false,
        dataLabels: { enabled: true }
      },
      series: { color: '#059e8a' }
    },
    xAxis: { type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { text: 'Error' }
      //title: { text: 'Temperature (Fahrenheit)' }
    },
    credits: { enabled: false }
  });
  
  setInterval(function( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var x = (new Date()).getTime(),
            y = parseFloat(this.responseText);
        //console.log(this.responseText);
        if(chartT.series[0].data.length > 40) {
          chartT.series[0].addPoint([x, y], true, true, true);
        } else {
          chartT.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/error", true);
    xhttp.send();
  }, 1000 ) ;