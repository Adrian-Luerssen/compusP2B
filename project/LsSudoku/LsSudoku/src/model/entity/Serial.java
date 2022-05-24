package model.entity;

import com.fazecast.jSerialComm.SerialPort;

public class Serial
{
    /* Error codes */
    public static final int ERR_NO_PORT = 1;
    public static final int ERR_PORT_OPEN = 2;
    public static final int ERR_OPEN_FAIL = 3;

    private volatile static Serial serialSingleton;
    private static final int BAUDRATE = 9600;

    private SerialPort[] availablePorts;
    private SerialPort selectedPort;
    private boolean isConnected;

    private Serial() {
        isConnected = false;
    }

    public static Serial getInstance() {
        if (serialSingleton == null) {
            synchronized (Serial.class) {
                if (serialSingleton == null) {
                    serialSingleton = new Serial();
                }
            }
        }

        return serialSingleton;
    }

    public void setPort(String portName) {
        for (SerialPort port : availablePorts) {
            if (port.getSystemPortName().equals(portName)) {
                selectedPort = port;
            }
        }
    }

    public String[] getAvailablePortNames() {
        /* Update ports */
        availablePorts = SerialPort.getCommPorts();

        /* Get port names */
        String[] availablePortNames = new String[availablePorts.length + 1];
        availablePortNames[0] = "AVAILABLE PORTS";

        for (int i = 1; i < availablePortNames.length; i++) {
            availablePortNames[i] = availablePorts[i - 1].getSystemPortName();
        }

        return availablePortNames;
    }

    public void setConnected(boolean isConnected)
    {
        this.isConnected = isConnected;
    }

    public boolean isConnected()
    {
        return isConnected;
    }

    public int connect()
    {
        if (selectedPort == null)
        {
            return ERR_NO_PORT;
        }

        if (selectedPort.isOpen())
        {
            return ERR_PORT_OPEN;
        }

        /* Non-blocking port */
        selectedPort.setComPortTimeouts(SerialPort.TIMEOUT_NONBLOCKING, 0, 0);
        selectedPort.setBaudRate(BAUDRATE);

        return (selectedPort.openPort()) ? 0 : ERR_OPEN_FAIL;
    }

    /* -------------------------------------- SERIAL TRANSMISSION -------------------------------------- */

    public boolean isBufferEmpty() {
        return selectedPort.bytesAvailable() == 0;
    }

    public void sendCharacter(final char character) {
        byte[] sendData = new byte[1];
        sendData[0] = (byte) character;
        selectedPort.writeBytes(sendData, 1);
    }

    public char readCharacter() {
        byte[] data = new byte[1];
        selectedPort.readBytes(data, 1);
        return (char) data[0];
    }
}
