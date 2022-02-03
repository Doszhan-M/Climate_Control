let menuToggle = document.querySelector('.manual_control');

menuToggle.onclick = function () {
  menuToggle.classList.toggle('manual_control_on')
}

setInterval(function () {
  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 20000);