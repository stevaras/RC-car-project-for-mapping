import hypermedia.net.*;

import processing.serial.*;

String val="";
String val2="";
Serial myPort;

UDP udp;

String ip = "192.168.1.65";
int port = 6000;

void setup() {

String portName = Serial.list()[0];
myPort = new Serial(this, portName, 9600);
udp = new UDP(this, 8888);
udp.listen(true);
}

void draw()
{}

void keyPressed()
{
 if (key=='i')
{ myPort.write('1');
}
if (key=='o')
{ myPort.write('0');
}
if (key=='w')
{ myPort.write('w');
}
if (key=='a')
{ myPort.write('a');
}
if (key=='x')
{ myPort.write('x');
}
if (key=='r')
{ myPort.write('r');
}
}

void receive (byte[] data)
{
	String myString = new String(data);
	if (data != null)
{val=myString;
myPort.write(val);
}

}

void serialEvent ( Serial myPort)
{
	val2=myPort.readStringUntil('\n');

	if (val2 != null)
	{
	val2 = trim(val2);
	udp.send(val2, ip, port);
}
}

