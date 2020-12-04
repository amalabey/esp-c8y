## Create "Credentials.h" header file with secrets
```
#define WIFI_SSID "wifi-ssid"
#define WIFI_PWD "wifi-password"

#define C8Y_HOST "c8y.domain.name"
#define C8Y_USERNAME "device-user-name"// fixed credentials can be registered in the Administration section
#define C8Y_PASSWORD "device-password"// create a user in usermanagement with the "device"role and fill the credentials here
#define C8Y_TENANT "c8y tenant name"//tenant ID can be found by clicking on your name in the top right corner of Cumulocity
#define C8Y_CLIENTID "device unique id"//Should be a unique identifier for this device, e.g. IMEI, MAC address or SerialNumber

#define ROOT_CA \
"-----BEGIN CERTIFICATE-----\n" \
"SERVER ROOT CA CERT \n" \
"-----END CERTIFICATE-----\n"
```