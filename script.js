function getLocation() {
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(showPosition);
    } else {
        document.getElementById("location").innerHTML = "Geolocation is not supported by this browser.";
    }
}

function showPosition(position) {
    const lat = position.coords.latitude;
    const lon = position.coords.longitude;
    document.getElementById("location").innerHTML = `Latitude: ${lat}, Longitude: ${lon}`;
    document.getElementById("moveButton").style.display = "block"; // Show the move button
}

function sendCommand() {
    const locationText = document.getElementById("location").innerText;
    fetch('http://your-robot-ip/move', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ command: `move to ${locationText}` }),
    })
    .then(response => response.json())
    .then(data => {
        if (data.status === "Success") {
            alert("Successfully moved to your location!");
        } else {
            alert("Failed to move.");
        }
    })
    .catch((error) => console.error('Error:', error));
}
function moveToFixedLocation() {
    fetch('http://your-robot-ip/move', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ command: "move to fixed disposal location" }),
    })
    .then(response => response.json())
    .then(data => {
        console.log(data); // Log the response for debugging
        if (data.status === "Success") {
            swal("Success!", "Successfully moved garbage to the fixed disposal location!", "success");
        } else {
            swal("Error!", "Failed to move garbage.", "error");
        }
    })
    .catch((error) => {
        console.error('Error:', error);
        swal("Error!", "An error occurred while communicating with the robot.", "error");
    });
}
