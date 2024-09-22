#include <ESP8266WiFi.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

WiFiServer server(80);

// Fixed disposal location coordinates
float fixedLatitude = 23.7733; // Fixed disposal latitude
float fixedLongitude = 90.4242; // Fixed disposal longitude

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    server.begin();
    Serial.println("Server started");
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        String request = client.readStringUntil('\r');
        client.flush();

        if (request.indexOf("POST /move") != -1) {
            String command = parseCommand(request);
            handleCommand(command);
        }
    }
}

String parseCommand(String request) {
    int startIndex = request.indexOf("command=") + 8;
    int endIndex = request.indexOf("\"", startIndex);
    return request.substring(startIndex, endIndex);
}

void handleCommand(String command) {
    if (command.startsWith("move to")) {
        if (command == "move to fixed disposal location") {
            Serial.println("Moving to fixed disposal location...");
            if (moveToFixedLocation()) {
                sendResponse("Success");
            } else {
                sendResponse("Failed");
            }
        } else {
            Serial.println("Moving to specified location...");
            sendResponse("Success"); // Modify as needed
        }
    }
}

bool moveToFixedLocation() {
    float currentLatitude = getCurrentLatitude(); // Implement GPS reading
    float currentLongitude = getCurrentLongitude(); // Implement GPS reading

    // Simple navigation logic
    while (true) {
        if (isAtTarget(currentLatitude, currentLongitude, fixedLatitude, fixedLongitude)) {
            Serial.println("Reached fixed disposal location.");
            return true; // Successfully moved to the location
        }

        // Basic movement logic (implement your motor control)
        if (currentLatitude < fixedLatitude) {
            moveForward();
        } else {
            moveBackward();
        }

        if (currentLongitude < fixedLongitude) {
            turnRight();
        } else {
            turnLeft();
        }

        currentLatitude = getCurrentLatitude(); // Update position
        currentLongitude = getCurrentLongitude(); // Update position

        delay(1000); // Adjust speed as necessary
    }

    return false; // In case of failure to reach the destination
}

bool isAtTarget(float currentLat, float currentLon, float targetLat, float targetLon) {
    const float tolerance = 0.0001; // Adjust tolerance as needed
    return (abs(currentLat - targetLat) < tolerance && abs(currentLon - targetLon) < tolerance);
}

float getCurrentLatitude() {
    // Implement GPS reading logic
    return 23.7700; // Example: return dummy value for now
}

float getCurrentLongitude() {
    // Implement GPS reading logic
    return 90.4200; // Example: return dummy value for now
}

void moveForward() {
    // Implement motor control to move forward
    Serial.println("Moving forward...");
}

void moveBackward() {
    // Implement motor control to move backward
    Serial.println("Moving backward...");
}

void turnRight() {
    // Implement motor control to turn right
    Serial.println("Turning right...");
}

void turnLeft() {
    // Implement motor control to turn left
    Serial.println("Turning left...");
}

void sendResponse(String status) {
    WiFiClient client = server.available();
    if (client) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        client.print("{\"status\":\"");
        client.print(status);
        client.println("\"}");
        client.stop();
    }
}
