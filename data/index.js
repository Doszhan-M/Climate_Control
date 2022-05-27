let manualControlFlag = true

const getControlState = () => {

    let menuToggle = document.querySelector('.manual_control');
    let xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        console.log('get_control_state');

        if (this.responseText == "ON") {
          menuToggle.classList.add('manual_control_on');
          fetch('/manual_control_on')
          if (manualControlFlag) {
            manualControlFlag = false
            alert('Manual control active!')
          }
        };

        if (this.responseText == "OFF") {
          menuToggle.classList.remove('manual_control_on');
          manualControlFlag = true
          fetch('/manual_control_off')
        };
      };
    };
    xhttp.open("GET", "/manual_control_state", true);
    xhttp.send();
  };


const getTemperature = () => {

  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
      console.log('get_temperature')
    }
  };
  xhttp.open("GET", "/get_temperature", true);
  xhttp.send();
};


const getHumidity = () => {

  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
      console.log('get_humidity')
    }
  };
  xhttp.open("GET", "/get_humidity", true);
  xhttp.send();
};



const getValve_state = () => {

  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("state").innerHTML = this.responseText;
      console.log('get_valve_state')
    }
  };
  xhttp.open("GET", "/get_valve_state", true);
  xhttp.send();
};


const getDatetime = () => {

  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.querySelector(".clock").innerHTML = this.responseText;
      console.log('get_datetime')
    }
  };
  xhttp.open("GET", "/get_datetime", true);
  xhttp.send();
};

getControlState()
getTemperature()
getHumidity()
getValve_state()
getDatetime()

setInterval(function () {
  getControlState()
  getTemperature()
  getHumidity()
  getValve_state()
}, 10000);

setInterval(function () {
  getDatetime()
}, 60000);


function submitMinimum() {
  alert("Minimum value saved");
  setTimeout(function () { document.location.reload(false); }, 500);
};
function submitMaximum() {
  alert("Maximum value saved");
  setTimeout(function () { document.location.reload(false); }, 500);
};



// deprecated:
// const openValve = () => {
//   let menuToggle = document.querySelector('.manual_control');
//   let openBtn = document.querySelector('.on_btn');
//   let valveState = document.getElementById('state');

//   openBtn.onclick = function () {
//     if (menuToggle.classList.contains('manual_control_on')) {
//       valveState.innerHTML = "IN PROGRESS";

//       let xhttp = new XMLHttpRequest();
//       xhttp.onreadystatechange = function () {
//         if (this.readyState == 4 && this.status == 200) {
//           console.log(this.responseText);
//         };
//       };
//       xhttp.open("GET", "/open_valve", true);
//       xhttp.send();
//     }

//   };
// };

// const closeValve = () => {
//   let menuToggle = document.querySelector('.manual_control');
//   let openBtn = document.querySelector('.off_btn');
//   let valveState = document.getElementById('state');

//   openBtn.onclick = function () {
//     if (menuToggle.classList.contains('manual_control_on')) {
//       valveState.innerHTML = "IN PROGRESS";

//       let xhttp = new XMLHttpRequest();
//       xhttp.onreadystatechange = function () {
//         if (this.readyState == 4 && this.status == 200) {
//           console.log(this.responseText);
//         };
//       };
//       xhttp.open("GET", "/close_valve", true);
//       xhttp.send();
//     }
//   };
// };

// openValve();
// closeValve();
