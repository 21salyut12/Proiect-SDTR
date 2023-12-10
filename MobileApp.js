import BleManager from 'react-native-ble-manager';

const initializeBluetooth = async () => {
    try { 
        // Turn on bluetooth
        await BleManager.enableBluetooth();
        console.log('Bluetooth enabled');
        
        // Scan for available devices
        const scanResults = await BleManager.scan([], 5, true);
        console.log(`Scanned devices: ${scanResults}`);

        const deviceID = 'deviceId' // Device id from the scanned results
        
        // Connect to a specific device based on its' id
        await BleManager.connect(deviceID);
        console.log(`Connected to device: ${deviceID}`);

        const data = await BleManager.read(deviceID, serviceUUID, characteristicUUID);
        console.log(`Received data: ${data}`);
        // Update app state with received data
    } catch (error) {
        switch (error.errorCode) {
            case 'BLE_TURNED_OFF':
                console.log('Bluetooth is turned off. Please turn it on.');
                // Prompt the user to enable Bluetooth
                break;
            case 'BLE_SCAN_FAILED':
                console.log('Scanning for devices failed. Please try again.');
                // Attempt scanning again
                BleManager.scan([], 5, true)
                    .then((scanResults) => {
                        console.log(`Scanned devices: ${scanResults}`);
                        // Handle successful scan results
                    })
                    .catch((scanError) => {
                        console.log('Scanning failed again:', scanError);
                        // Handle re-scanning failure
                    });
                break;
            case 'BLE_CONNECT_FAILED':
                console.log('Connection to the device failed.');
                // Retry connection
                BleManager.connect(deviceID)
                    .then(() => {
                        console.log(`Connected to device: ${deviceID}`);
                        // Handle successful re-connection
                    })
                    .catch((connectError) => {
                        console.log('Connection failed again:', connectError);
                        // Handle re-connection failure
                    });
                break;
            case 'BLE_READ_FAILED':
                console.log('Failed to read data from the device.');
                // Retry reading data
                BleManager.read(deviceID, serviceUUID, characteristicUUID)
                    .then((data) => {
                        console.log(`Received data: ${data}`);
                        // Handle received data
                    })
                    .catch((readError) => {
                        console.log('Data reading failed again:', readError);
                        // Handle re-reading failure
                    });
                break;
            default:
                console.log('An unexpected error occurred:', error);
                // Handle other errors or notify the user
                break;
        }
    }
};

// Start Bluetooth operations by calling the function above
initializeBluetooth();