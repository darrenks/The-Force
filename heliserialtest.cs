//based off of http://arduino.cc/playground/Csharp/SerialCommsCSharp

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO.Ports;
using System.IO;
public class ArduinoControllerMain {
    public static void runProgram(SerialPort port)
    {
        //System.IO.Ports.SerialPort serialPort1;
        //System.ComponentModel.IContainer components = new System.ComponentModel.Container();
        //serialPort1 = new System.IO.Ports.SerialPort(components);
        //serialPort1.PortName = "COM4";
        //serialPort1.BaudRate = 9600;
        port.Open();

        // this turns on !
       // serialPort1.DtrEnable = true;
        Console.Write("starting loop");
        while (true)
        {
            //port.Write(new byte[] { 1 }, 0, 1);
            //System.Threading.Thread.Sleep(1000);
            //port.Write(new byte[] { 0 }, 0, 1);
            //System.Threading.Thread.Sleep(1000);
            Transmit(port, 63, 63, 40, 63);

            //wait for received
            while (port.BytesToRead==0)
            {
                System.Threading.Thread.Sleep(10);
            }
            if (port.ReadByte() != 103)
            {
                Console.Write("Bad ack");
                break;
            }
        }
    }

    public static void Transmit(SerialPort port, int x1, int x2, int x3, int x4) {
        port.Write(new byte[] {(byte) x1,(byte) x2,(byte) x3,(byte) x4, 103}, 0, 5);
    }

    public static void Main()
    {
        ArduinoControllerMain a= new ArduinoControllerMain();
        a.SetComPort();
        if (a.portFound)
        {
            runProgram(a.arduinoPort);
        }
        else
        {
            Console.Write("failed to detect arduino");
        }
        System.Threading.Thread.Sleep(10000);
    }
	SerialPort currentPort;
	bool portFound;
    SerialPort arduinoPort;
	private void SetComPort()
	{
	    try
	    {
		string[] ports = SerialPort.GetPortNames();
		foreach (string port in ports)
		{
            Console.WriteLine(port);
		    currentPort = new SerialPort(port, 9600);
		    if (DetectArduino())
		    {
			portFound = true;
            arduinoPort = currentPort;
			break;
		    }
		    else
		    {
			portFound = false;
		    }
		}
	    }
	    catch (Exception e)
	    {
	    }
	}
	private bool DetectArduino()
	{
	    try
	    {
		//The below setting are for the Hello handshake
		byte[] buffer = new byte[2];
		buffer[0] = Convert.ToByte(16);
		buffer[1] = Convert.ToByte(128);
		int intReturnASCII = 0;
		char charReturnValue = (Char)intReturnASCII;
		currentPort.Open();
        //currentPort.DtrEnable = true;
		currentPort.Write(buffer, 0, 2);
		Thread.Sleep(1000);
		int count = currentPort.BytesToRead;
		string returnMessage = "";
		while (count > 0)
		{
		    intReturnASCII = currentPort.ReadByte();
		    returnMessage = returnMessage + Convert.ToChar(intReturnASCII);
		    count--;
		}
		//ComPort.name = returnMessage;
		currentPort.Close();
		if (returnMessage.Contains("HELLO FROM ARDUINO"))
		{
		    return true;
		}
		else
		{
		    return false;
		}
	    }
	    catch (Exception e)
	    {
		return false;
	    }
}
}
