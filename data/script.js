window.onload = () => {
  // obtiene los valores iniciales de cada parametro
  getParam(PIDParams.Kp);
  getParam(PIDParams.Kd);
  getParam(PIDParams.Ki);

  // por defecto no grafica el error
  document.getElementById('pollCheckbox').checked = false;
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

// clase para los botones mientras se ejecuta el request
const classIsLoading = 'is-loading';

// request GET para recuperar un parametro
function getParam(param) {
  fetch(getParamUrl(param))
    .then((async (resp) => {
      const value = await resp.text();
      document.getElementById(param).innerHTML = value;
    }))
    .catch(console.error);
}

// request POST para asignar un parametro
function setParam(param, value) {
  const buttonName = "btnSubmit-" + param;
  document.getElementById(buttonName).classList.add(classIsLoading);

  fetch(setParamUrl(param, value), { method: "POST" })
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

// valida el valor de cada parametro
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

// formulario KP
const kpForm = document.getElementById("formKp");
kpForm.addEventListener("submit", (evt) => {
  evt.preventDefault();

  const formData = new FormData(kpForm);

  const value = formData.get("value");
  if (!validateValue(value))
    return;

  setParam(PIDParams.Kp, value);
})

// formulario KI
const kiForm = document.getElementById("formKi");
kiForm.addEventListener("submit", (evt) => {
  evt.preventDefault();

  const formData = new FormData(kiForm);

  const value = formData.get("value");
  if (!validateValue(value))
    return;

  setParam(PIDParams.Ki, value);
})

// formulario KD
const kdForm = document.getElementById("formKd");
kdForm.addEventListener("submit", (evt) => {
  evt.preventDefault();

  const formData = new FormData(kdForm);

  const value = formData.get("value");
  if (!validateValue(value))
    return;

  setParam(PIDParams.Kd, value);
})

// grafico del error en tiempo real
// se inicializa vacio, sin datos
const ctx = document.getElementById("errorChart");
const chart = new Chart(ctx, {
  type: 'line',
  options: {
    animation: false,
  },
  data: {
    labels: [],
    datasets: [{
      label: 'Error',
      data: [],
      fill: false,
      borderColor: 'rgb(75, 192, 192)',
      tension: 0.1
    }]
  },
});

// recupera el error y lo agrega al grafico
function fetchError() {
  if (!chart)
    return;

  fetch(baseUrl + '/getError')
    .then(async (resp) => {
      // recupera el valor
      const error = await resp.text();

      // siempre deberia ser un numero
      if (isNaN(error)) {
        console.error('getError response - got: ' + error);
        return;
      }

      // recupera la fecha actual (mm:ss) y lo agrega al eje x
      const date = new Date();
      const label = `${date.getMinutes()}:${date.getSeconds()}`;
      chart.data.labels.push(label);

      // agrega el valor del error al eje y
      chart.data.datasets[0].data.push(error);

      chart.update();
    })
    .catch(console.error);
}

// checkbox que controla la actualizacion del grafico del error
let intervalId;
document.getElementById('pollCheckbox').addEventListener('change', (evt) => {
  const shouldPoll = evt.target.checked;

  if (shouldPoll) {
    intervalId = setInterval(fetchError, 2000);
  } else {
    clearInterval(intervalId);
    intervalId = null;
  }
})
