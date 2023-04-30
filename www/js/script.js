//Display the date and time in the div with id "time"
function displayTime() {
    var time = document.getElementById("time");
    var date = new Date();
    time.innerHTML = date.toLocaleString();
}

// Call the function every second
setInterval(displayTime, 1000);