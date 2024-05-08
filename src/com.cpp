#include <esp32notifications.h>

#define HARDWARE_STANDARD

#ifdef HARDWARE_STANDARD
    #define BUTTON_A    15 // left button - use this GPIO pin
    #define BUTTON_B    26 // center button - use this GPIO pin
    #define BUTTON_C    27 // right button - use this GPIO pin
#else
    #error Hardware buttons not supported!
#endif

//////////
// Example code begins

// Create an interface to the BLE notification library
BLENotifications notifications;

// Holds the incoming call's ID number, or zero if no notification
uint32_t incomingCallNotificationUUID;

// This callback will be called when a Bluetooth LE connection is made or broken.
// You can update the ESP 32's UI or take other action here.
void onBLEStateChanged(BLENotifications::State state) {
  switch(state) {
      case BLENotifications::StateConnected:
          Serial.println("StateConnected - connected to a phone or tablet"); 
          break;

      case BLENotifications::StateDisconnected:
          Serial.println("StateDisconnected - disconnected from a phone or tablet"); 
          /* We need to startAdvertising on disconnection, otherwise the ESP 32 will now be invisible.
          IMO it would make sense to put this in the library to happen automatically, but some people in the Espressif forums
          were requesting that they would like manual control over re-advertising.*/
          notifications.startAdvertising(); 
          break; 
  }
}

void onNotificationArrived(const ArduinoNotification * notification, const Notification * rawNotificationData) {
    Serial.print("Got notification: ");   
    Serial.println(notification->title); // The title, ie name of who sent the message
    Serial.println(notification->message); // The detail, ie "be home for dinner at 7".
    Serial.println(notification->type);  // Which app sent it
    Serial.println(notifications.getNotificationCategoryDescription(notification->category));  // ie "social media"
    Serial.println(notification->categoryCount); // How may other notifications are there from this app (ie badge number)
    if (notification->category == CategoryIDIncomingCall) {
		// If this is an incoming call, store it so that we can later send a user action.
        incomingCallNotificationUUID = notification->uuid;
        Serial.println("--- INCOMING CALL: PRESS A TO ACCEPT, C TO REJECT ---"); 
    }
    else {
        incomingCallNotificationUUID = 0; // Make invalid - no incoming call
    }
}


// A notification was cleared
void onNotificationRemoved(const ArduinoNotification * notification, const Notification * rawNotificationData) {
     Serial.print("Removed notification: ");   
     Serial.println(notification->title);
     Serial.println(notification->message);
     Serial.println(notification->type);  
}


void com_setup_BLE(){
// Button configuration. It is usual to have buttons configured as INPUT_PULLUP in the hardware design,
  // but check the details for your specific device 
  pinMode(BUTTON_A, INPUT_PULLUP);

    Serial.println("ESP32-ANCS-Notifications Example");
    Serial.println("------------------------------------------");    

    // Set up the BLENotification library
    notifications.begin("BLEConnection device name");
    notifications.setConnectionStateChangedCallback(onBLEStateChanged);
    notifications.setNotificationCallback(onNotificationArrived);
    notifications.setRemovedCallback(onNotificationRemoved);
}

void com_loop(){
      if (incomingCallNotificationUUID > 0) {
		// Check to see if the user has pressed an action button
	    if (digitalRead(BUTTON_A) == HIGH) {
	      Serial.println("Positive action."); 
	        notifications.actionPositive(incomingCallNotificationUUID);
	    }
    }
}
