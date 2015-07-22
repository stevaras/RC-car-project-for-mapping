import hypermedia.net.*;
boolean prosimo = true;
UDP udp;  // define the UDP object

String ip       = "192.168.1.101"; // the remote IP address
int port        = 8888;        // the destination port

int arithmos=0;

//------------------

String val="";
String val2="10000";
String val3="10000";
String data="";
float[] upsometro = new float[200];
float[] upsometro2 = new float[200];
float[] kentrox = new float[200];
float[] kentroy = new float[200];
float[] metakinhsh= new float[200];
float[] metakinhsh2= new float[200];
float[] moires=    new float[200];
float[] moires2=    new float[200];
int [] apostash1= new int[200];
int [] apostash2= new int[200];
float startingx[] = new float[200];
float startingy[] = new float[200];
float x1, x2, x3, x4, y1, y2, y3, y4, nx1, nx2, nx3, nx4, ny1, ny2, ny3, ny4, px, py;
int x = 0;
int i;
boolean flag=true;
int testtt=1;
String sa;
boolean udpflag=true;

//---------------- Apo to allo eggrafo
void setup() {
  udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
  //udp.log( true );         // <-- printout the connection activity
  udp.listen( true );           // and wait for incoming message

    //---------------------------------

  fill(0);
  rectMode(CENTER);
  size(1200, 1000);
  fill(200);

  i=1;
  startingx[0]=300;//42;
  startingy[0]=300;//98;
  for (int j=0; j<100; j++)
  {

    metakinhsh[j]=0;
    moires[j]=0;
    kentrox[j]=0;
    kentroy[j]=0;
  }

  //---------------------------------
}

void draw()
{


  stroke(0);


  background(200);
  //####################################################
  //SXEDIASTIKA TEST

  //line(1000, 0, 1000, 200);
  // line(1000, 200, 1200, 200);

  //************************************************
  for (int k=0; k<i; k++) {
    pushMatrix();

    strokeWeight(6);
    beginShape();
    nx1 = rotX(startingx[k], startingy[k], startingx[k]+50+metakinhsh[k], startingy[k]+50, moires[k]);
    ny1 = rotY(startingx[k], startingy[k], startingx[k]+50+metakinhsh[k], startingy[k]+50, moires[k]);
    //vertex(nx1,ny1);
    nx2 = rotX(startingx[k], startingy[k], startingx[k]-50+metakinhsh[k], startingy[k]+50, moires[k]);
    ny2 = rotY(startingx[k], startingy[k], startingx[k]-50+metakinhsh[k], startingy[k]+50, moires[k]);
    //vertex(nx2,ny2);
    nx3 = rotX(startingx[k], startingy[k], startingx[k]-50+metakinhsh[k], startingy[k]-50, moires[k]);
    ny3 = rotY(startingx[k], startingy[k], startingx[k]-50+metakinhsh[k], startingy[k]-50, moires[k]);
    //vertex(nx3,ny3);
    nx4 = rotX(startingx[k], startingy[k], startingx[k]+50+metakinhsh[k], startingy[k]-50, moires[k]);
    ny4 = rotY(startingx[k], startingy[k], startingx[k]+50+metakinhsh[k], startingy[k]-50, moires[k]);
    //vertex(nx4,ny4);

    //vertex(nx1,ny1);
    endShape();
    kentrox[k]=findCenterSxhmatosX(nx1, nx3);
    kentroy[k]=findCenterSxhmatosY(ny1, ny3);
    strokeWeight(5);
    point(kentrox[k], kentroy[k]); 
    if (k>0) {
      line(kentrox[k], kentroy[k], kentrox[k-1], kentroy[k-1]);
    }


    popMatrix();
    startingx[k+1]=kentrox[k];
    startingy[k+1]=kentroy[k];
  }


  pushMatrix();

  translate(startingx[i], startingy[i]);
  rotate(radians(moires[i]));
  stroke(255, 0, 0);
  line(0, 0, 10, 0);
  popMatrix();

  //----------------------------------------------
}

//----------------------------

float findCenterSxhmatosX(float x1, float x2)
{
  float x;
  return x = x1/2 + x2/2;
}

float findCenterSxhmatosY(float y1, float y2)
{
  float y;
  return y = y1/2 + y2/2;
}

float rotX(float cx, float cy, float x, float y, float angle) {
  //float radians = (Math.PI / 180) * angle;
  //float cos = Math.cos(radians);
  //float sin = Math.sin(radians);
  float nx = (cos(radians(angle)) * (x - cx)) - (sin(radians(angle)) * (y - cy)) + cx;
  float ny = (sin(radians(angle)) * (x - cx)) + (cos(radians(angle)) * (y - cy)) + cy;
  return nx;
}
float rotY(float cx, float cy, float x, float y, float angle) {
  //float radians = (Math.PI / 180) * angle;
  //float cos = Math.cos(radians);
  //float sin = Math.sin(radians);
  float nx = (cos(radians(angle)) * (x - cx)) - (sin(radians(angle)) * (y - cy)) + cx;
  float ny = (sin(radians(angle)) * (x - cx)) + (cos(radians(angle)) * (y - cy)) + cy;

  return  ny;
}


//---------------------



void keyPressed() {

  if ( key == 'i') //i gia start
  {
    udp.send("1", ip, port);
  } 
  if ( key == 'r') //r gia return
  {
    udp.send("r", ip, port);
  } 
  if ( key == 'o') //o gia stop
  {
    udp.send("0", ip, port);
  } 
  /*if ( key == 'e') //E gia exit last year
   {
   udp.send("", ip, port);
   } */
  if ( key == 's') //back
  {
    udp.send("s", ip, port);
    //metakinhsh[i]=-20;
    //i++;
    //prosimo = false;
    udpflag=false;
  }

  if ( key == 'w') //forward
  {
    udp.send("w", ip, port);
    // metakinhsh[i]=+20;
    //i++;
    //prosimo = true;
    udpflag=false;
  }
  if ( key == 'a') //forward
  {
    udp.send("a", ip, port);
  }

  if ( key == 'd') //forward
  {
    udp.send("d", ip, port);
  }

  if ( key == 'x') //brake
  {
    udp.send("x", ip, port);
  }

  /*if ( key == 'c') //forward
   {
   udp.send("c", ip, port);
   }*/

  if (x <100)
  {

    x=x+1;
    sa = nf(x, 3);
  }
  saveFrame("screen-"+sa+".png");
}

void receive( byte[] data ) {          // <-- default handler
  //void receive( byte[] data, String ip, int port ) {   // <-- extended handler
  String myString = new String(data);

  if (data!= null) {
    String[] valarray = splitTokens(myString, "*");

    if (valarray.length>1)
    {
      println(valarray[0]);
      println(valarray[1]);
      if (valarray[0].equals("d"))
      {
        if (valarray[1].equals("0")==false) {

          metakinhsh[i]=float(valarray[1])/16 + float(valarray[1])%16;//
          metakinhsh2=subset(metakinhsh, 1, i);
          saveStrings("metakinhsh.txt", str(metakinhsh2));
        }
        i++;
      }
      if (valarray[0].equals("h"))
      {

        println("To ypsometro einai " + valarray[1]);
        upsometro[i]=float(valarray[1]);
        upsometro2=subset(upsometro, 1, i);
        saveStrings("upsometro.txt", str(upsometro2));
      }
      if (valarray[0].equals("u"))
      {
        println("H apostash apo toixo einai " + valarray[1] +" cm!");
      }
    } else {
      moires[i]=float(valarray[0]);
      moires2=subset(moires, 1, i);
      saveStrings("moires.txt", str(moires2));
    }


    if (i==100)
    {
      udp.send("r", ip, port);
    }
  }
}
