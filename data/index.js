const manualToggle = () => {

  let menuToggle = document.querySelector('.manual_control');
  menuToggle.onclick = function () {

    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        console.log(this.responseText);

        if (this.responseText == "OFF") {
          menuToggle.classList.add('manual_control_on');
          fetch('/manual_control_on')
        };

        if (this.responseText == "ON") {
          menuToggle.classList.remove('manual_control_on');
          fetch('/manual_control_off')
        };
      };
    };
    xhttp.open("GET", "/manual_control_state", true);
    xhttp.send();
  }
}

const manualOn = () => {
  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      console.log(this.responseText);
      let menuToggle = document.querySelector('.manual_control');
      if (this.responseText == "ON") {
        menuToggle.classList.add('manual_control_on');
        alert("Manual control active!");
      }
    };
  };
  xhttp.open("GET", "/manual_control_state", true);
  xhttp.send();
};

const openValve = () => {
  let menuToggle = document.querySelector('.manual_control');
  let openBtn = document.querySelector('.on_btn');
  let valveState = document.getElementById('state');

  openBtn.onclick = function () {
    if (menuToggle.classList.contains('manual_control_on')) {
      valveState.innerHTML = "IN PROGRESS";

      let xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          console.log(this.responseText);
        };
      };
      xhttp.open("GET", "/open_valve", true);
      xhttp.send();
    }

  };
};

const closeValve = () => {
  let menuToggle = document.querySelector('.manual_control');
  let openBtn = document.querySelector('.off_btn');
  let valveState = document.getElementById('state');

  openBtn.onclick = function () {
    if (menuToggle.classList.contains('manual_control_on')) {
      valveState.innerHTML = "IN PROGRESS";

      let xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          console.log(this.responseText);
        };
      };
      xhttp.open("GET", "/close_valve", true);
      xhttp.send();
    }
  };
};

openValve();
closeValve();
manualOn();
manualToggle();


setInterval(function () {
  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
      console.log('get_temperature')
    }
  };
  xhttp.open("GET", "/get_temperature", true);
  xhttp.send();
}, 20000);

setInterval(function () {
  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
      console.log('get_humidity')
    }
  };
  xhttp.open("GET", "/get_humidity", true);
  xhttp.send();
}, 20000);

setInterval(function () {
  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("state").innerHTML = this.responseText;
      console.log('get_valve_state')
    }
  };
  xhttp.open("GET", "/get_valve_state", true);
  xhttp.send();
}, 20000);


function submitMinimum() {
  alert("Minimum value saved");
  setTimeout(function () { document.location.reload(false); }, 500);
};
function submitMaximum() {
  alert("Maximum value saved");
  setTimeout(function () { document.location.reload(false); }, 500);
};