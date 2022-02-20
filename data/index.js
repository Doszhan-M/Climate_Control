let menuToggle = document.querySelector('.manual_control');
menuToggle.onclick = function () {
  menuToggle.classList.toggle('manual_control_on')
  alert("Control type changed!");
}


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
      document.getElementById("state").innerHTML = this.responseText;
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
      document.getElementById("humidity").innerHTML = this.responseText;
      console.log('get_valve_state')
    }
  };
  xhttp.open("GET", "/get_valve_state", true);
  xhttp.send();
}, 20000);


function submitMinimum() {
  alert("Minimum value saved");
  setTimeout(function(){ document.location.reload(false); }, 500);
};
function submitMaximum() {
  alert("Maximum value saved");
  setTimeout(function(){ document.location.reload(false); }, 500);
};