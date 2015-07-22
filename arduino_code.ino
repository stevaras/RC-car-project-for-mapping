// Libraries
#include <Wire.h>   // Gia thn epikoinwnia me ton aisthitira LSM303
#include <LSM303.h> // omoiws
#include <Adafruit_Sensor.h> // genikotera gia diaforous aisthirites... (sunthrhtika)
//
//

int mprostapisw=0;
boolean plhsiazwflag=false;
// Initalization
boolean startflag = false; //gia arxikopoihsh tou programmatos
// Return To Starting Point
boolean returnflag = false; //gia na energopoihthei h diadikasia epistrofhs sthn arxikh thesh
boolean right = true; //gia na stripsei deksiostrofa (h' false=aristerostrofa) sthn epistrofh
float retArrayComp[100]; //Pinakas me tis times tou Compass gia kathe kinhsh pou exei kanei
int retArrayDist[100]; // Pinakas me tis times twn bhmatwn pou ekane to moter gia kathe kinhsh
int kinhseis = 0; // To sunolo twn kinhsewn poy exei kanei kai ara exei na kanei gia na epistrepsei
//
boolean flag = false;
// new flags gia debugging tou mprosta kai stribw... diladi mono me X tha stamataei
boolean flagperistrofhs = true;
boolean flageutheia = true;
// Ultrasonic Sensor
#define trigPin1 3 // Orismwn twn pins
#define echoPin1 12 // gia echo kai trig

float duration1 , distance1; // metablhtes gia thn diarkeia tou shmatos tou sensor kai gia thn apostash apo ayton
// Klish
int htimhtouy = 0;  // h times tou aksona y tou accelerometer gia thn klish
int meshtimh = 0;  // afou paroume sthn arxh 5 times bgazoume ton meso oro gia na kseroume ti na thewroume fusiologiko!!!
LSM303 compass;   // Arxikopoieitai h pyksida me onoma compass :p
// Inputs?
char val = 'q'; // To val einai h timh pou diabazei seiriaka...!
//Kathorizei thn arxikopoihsh kai to stamathma, thn kateythinsh, to stop kai alla tetoia
// Moter kai kinhsh
// moter 1 ta Pins
byte A = 4;
byte B = 5;
byte C = 6;
byte D = 7;
// moter 2 ta Pins
byte E = 8;
byte F = 9;
byte G = 10;
byte H = 11;

byte _step = 4; // to step asxoleitai me tis faseis twn magnhtwn mesa sta moter

byte turncheckcomp = 0; // einai flag gia na mhn diabazei times o magnhths otan kanei peristrofh
byte count = 0; // exei na kanei me ta bhmata tou moter
boolean dir = false;// false = mprosta true = pisw kathorizei thn kateythinsh pou paei to robot
boolean cmpsflag = true; // einai to flag pou kathorizei an tha diabazei h puksida h' oxi
boolean turnflagcomp = false; // einai gia an ginetai peristrofh... wste na mpei se ena allo mode sundiasmou kinhshs-puksidas


void setup() {
  // Arxikopoihseis Serial kai Compass
  Serial.begin(9600);
  Serial.flush();
  Wire.begin();
  compass.init();
  compass.enableDefault();

  // Orismos twn Pin tou Ultrasonic Sensor
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  // Orismos twn Pin twn Moter
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);

  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(H, OUTPUT);

  //Perasma twn calibrated timws ths pyksidas gia pio aksiopistes metrhseis
  compass.m_min = (LSM303::vector<int16_t>) {
    -525,   -423,   -449
  };
  compass.m_max = (LSM303::vector<int16_t>) {
    +411,   +522,   +400
  };
  // diadikasia na paroume enan meso oro ths klishs
  for (int m = 0; m < 5; m++)
  {
    compass.read();
    meshtimh += compass.a.y;
    //Serial.println(meshtimh);
    delay(100);
  }
  meshtimh = meshtimh / 5; 
  //Serial.print("h");
  //Serial.println(meshtimh);
  // h mesh timh ths klishs, pleon metro sugkrishs
  //Serial.println(meshtimh);
  // arxikopoihsh-mhdenismos twn pinakwn gia to return
  for (int p = 0; p < 100; p++)
  {
    retArrayComp[p] = 0;
    retArrayDist[p] = 0;
  }

}



// the loop routine runs over and over again forever:
void loop() {
//Serial.println(mprostapisw);
  if (Serial.available() > 0) // an yparxei plhroforia
  {

    val = Serial.read(); // thn diabazoume kai thn bazoume sto val


  }
  
  if ( val == '1') // an to val einai '1' tote
  {
    startflag = true; // to startflag ginetai true kai ara ksekina na leitourgei OLO to programma

  }



  if (startflag) { // efoson trexei to programma...

    if ( val == 'r') // an h timh einai 'r' tote energopoieite h epistrofh
    {
      returnflag = true;
      startflag = false;
    }

    if ( val == '0') // me to mhden einai san na kanoume stop sto programma
    {
      startflag = false;
    }

    if (count != 0 && count % 128 == 0) // elegxos gia an exoun ginei 128 bhmata
    { //kanoume stop sto oxhma
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(H, LOW);
      delay(200); // gia poly ligo. O logos pou to kanoume ayto einai gia na mporesei na parei metrhseis to robot xwris na epireastei epeidh einia se kinhsh
      compass.read(); // diabazoume thn timh ths puksidas
      float heading = compass.heading(); // thn bazoume sto heading
      htimhtouy = compass.a.y; // diabazoume thn klish
      Serial.print(String(heading)); // Kanoume print thn metrhsh ths puksidas ousiastika stelnontas thn sto Raspberry Pi
      //-----------------------------
      retArrayComp[kinhseis] = heading; // bazoume ston pinaka gia tis times tou Compass thn metrhsh aytou
      if (dir) {
        retArrayDist[kinhseis] = 128; // an h kateythinsh htan pros ta mprosta bazoume +128 ston pinaka gia to return pou exei ta bhmata
      }
      else {
        retArrayDist[kinhseis] = -128;// enw -128 gia opisthen
      }
      kinhseis++; // oloklhrwthike mia kinhsh, ayksanoume gia thn epomenh
      //-----------------------------
      Serial.print('\n');
      Serial.print("d*"); //me 'd*' leme sto Raspberry pws akolouthoun ta bhmata
      if (dir) {
                  Serial.println(count);
        }
        else {
                  Serial.println(count*-1);
        } // ta bhmata (128)
      Serial.print("h*"); //me '*h' leme sto Raspberry pws akolouthei h klish
      Serial.println(htimhtouy);
      count = 0;  //mhdenizoume ta bhmata

      digitalWrite(trigPin1, HIGH); //stelnoume shma me ton Ultrasonic sensor
      delayMicroseconds(1000); //perimenoume apeiroelaxista
      digitalWrite(trigPin1, LOW); // Kleinoume to shma
      duration1 = pulseIn(echoPin1, HIGH);// kai diabazoume thn epistrofh tou shmatos
      distance1 = (duration1) / 58.2; // pratoume ton mathimatiko typo gia na kseroume thn apostash
      //      Serial.println(distance1); //ektypwnoume thn apostash ???? GIATI TO KANOUME AYTO? THA EPREPE NA EXEI KAPIO
      if (distance1 <= 6.7 && distance1 >= 1) // an yparxei empodio mikrotero twn 6 ekatostwn epeidh to robot kanei kinhsh 5 ekatostwn ... tha tou apagoreytei h kinhsh
      {
        Serial.print("u*");
        Serial.println(distance1);
        val = 'x'; // freno dioti tha trakarei
      }
    }

    if (_step == 0) // edw einai ta bhmata twn moter !!!
    {
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      digitalWrite(E, HIGH);
      digitalWrite(F, LOW);
      digitalWrite(G, HIGH);
      digitalWrite(H, LOW);
      if (dir == false) {
        count++;
      }
    }
    if (_step == 1)
    {
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(H, HIGH);
    }
    if (_step ==  2)
    {
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(E, LOW);
      digitalWrite(F, HIGH);
      digitalWrite(G, LOW);
      digitalWrite(H, HIGH);
    }
    if (_step ==  3)
    {
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      digitalWrite(H, LOW);
      if (dir == true) {
        count++;
      }
    }
    if (_step ==  6)
    {
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      digitalWrite(H, LOW);
    }
    if (_step ==  7)
    {
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(E, LOW);
      digitalWrite(F, HIGH);
      digitalWrite(G, LOW);
      digitalWrite(H, HIGH);
    }
    if (_step ==  8)
    {
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(H, HIGH);
    }
    if (_step == 9)
    {
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      digitalWrite(E, HIGH);
      digitalWrite(F, LOW);
      digitalWrite(G, HIGH);
      digitalWrite(H, LOW);
    }
    if (_step == 5)
    {
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(H, LOW);
    }

    if (flag == true)
    {
      if (dir) {
        _step++;
      }
      else {
        _step--;
      }
    }

    if (flageutheia == true) {
      
      if (val == 'w') // kinhsh mprosta
      {  if(mprostapisw!=2){
        mprostapisw=1;
        _step = 0;
        flag = true;
        dir = true;
        cmpsflag = false;
        val = 'q';
        flagperistrofhs = false;
      }}
      if (val == 's') //kinhsh pisw
      {
        if(mprostapisw!=1){
        mprostapisw=2;        
        _step = 3;
        flag = true;
        dir = false;
        cmpsflag = false;
        val = 'q';
        flagperistrofhs = false;
      }}
    }

    if (flagperistrofhs == true)
    {
      if (val == 'a') //kinhsh aristera
      {
        _step = 6;
        flag = true;
        dir = true;
        cmpsflag = true;
        turnflagcomp = true;
        turncheckcomp = 0;
        val = 'q';
        flageutheia = false;
      }
      if (val == 'd') //kinhsh deksia
      {
        _step = 6;
        flag = true;
        dir = false;
        cmpsflag = true;
        turncheckcomp = 0;
        turnflagcomp = true;
        val = 'q';
        flageutheia = false;
      }
    }

    if (val == 'x') //freno
    {
      mprostapisw=0;
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(H, LOW);
      delay(200);
      
      
      

      if (count > 0) //an enw exoume energopoihsei to freno exoun ginei bhmata
      {
        compass.read();
        float heading = compass.heading();
        Serial.print(String(heading));
        Serial.print('\n');
        Serial.print("d*");
        if (dir) {
                  Serial.println(count);
        }
        else {
                  Serial.println(count*-1);
        }
        //Serial.println(count);
        htimhtouy=compass.a.y;
        Serial.print("h*"); //me '*h' leme sto Raspberry pws akolouthei h klish
        Serial.println(htimhtouy);
        Serial.print('\n');
        retArrayComp[kinhseis] = heading; //tote apothikeyontai kai ston pinaka epistrofhs
        if (dir) {
          retArrayDist[kinhseis] = count; // kai ston pinaka kateythinshs
        }
        else {
          retArrayDist[kinhseis] = count * (-1);
        }
        kinhseis++;
      }

      _step = 5; // bhma gia freno
      flag = false;
      cmpsflag = true;
      turnflagcomp = false;
      count = 0;
      val = 'q';
      flageutheia = true;
      flagperistrofhs = true;
    }

    if ((_step == 4) && (flag == true)) { // ta steps einai me tetoion tropo pou yparxoun kena steps anamesa tous
      _step = 0;
    }
    if ((_step == 255) && (flag == true)) { // me tous elegxous aytous frontizoume gia na mhn mplextei gia paradeigma
      _step = 3;
    }
    if ((_step > 9) && (flag == true)) { // h kinhsh tou mpostra me thn pisw... h' tou pisw me to aristera ...
      _step = 6;
      count = 0;
    }
    if ((_step == 5) && (flag == true)) { // na epanerxontai sta swsta steps
      _step = 9;
      count = 0;
    }

    if (cmpsflag == false) { // an den kanoume metrhseis me pinaka bazoume thn kathisterhsh metaksu kathe bhmatos ????????????????????????
      if (dir == false) {
        delayMicroseconds(2000); //ish me 2000 microseconds
      }
      else {
        delayMicroseconds(2000);
      }
    }
    else
    {
      delayMicroseconds(5000); //an kanoume metrhseis???????????????????????????
    } // htan 2000 kai to bazw 1000

    if (turnflagcomp == true) //an kanoume peristrofh
    { turncheckcomp++;
      if (turncheckcomp % 4 == 0) //gia kathe 4 bhmata peristrofhs tha diabazoume thn pyksida
      {
        compass.read(); // ayto ginetai giati alliws argoun poly ta bhmata
        float heading = compass.heading();
        Serial.print(String(heading));
        Serial.print('\n');
      }
    }
    if (cmpsflag == true && turnflagcomp == false) //metrhseis puksidas
    {
      compass.read();
      float heading = compass.heading();
      Serial.print(String(heading));
      Serial.print('\n');
    }
  }

  if (returnflag == true) // h methodos gia thn epistrofh tou robot
  {
    for (int m = kinhseis - 1; m >= 0; m--)
    {
        plhsiazwflag=false;
       CompCalc(retArrayComp[m]);
       if (retArrayDist[m] < 0)
      {
        forward(retArrayDist[m]);
        Serial.print("d*");
        Serial.println(retArrayDist[m]*-1);
      }
      if (retArrayDist[m] > 0)
      {
        backwards(retArrayDist[m]);
        Serial.print("d*");
        Serial.println(retArrayDist[m]*-1);
      }
    }
    returnflag = false;
    kinhseis = 0;
  }


}



//Serial.println(count);


void CompCalc(float thesh)
{
   digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(H, LOW);
  delay(100);
  float thesh_new;
  compass.read();
  float thesh_now = compass.heading();
  /*Serial.print("THESH pou eimai ");
  Serial.print(thesh_now);
  Serial.print("THESH pou thelw ");
  Serial.println(thesh);*/
  if (thesh != thesh_now)
  {
    if (thesh_now >= 180)
    {
      thesh_new = thesh_now - 180;
      if ((thesh < thesh_now) && (thesh > thesh_new))
      {
        right = false;
      }
      else
      {
        right = true;
      }

    }
    else
    {
      thesh_new = thesh_now + 180;


      if ((thesh > thesh_now) && (thesh < thesh_new)) // ISWS AYTO NA EINAI TO IDIO ME TO APO PANW KAI ARA NA MHN XREIAZETAI !!!
      {
        right = true;
      }
      else
      {
        right = false;
      }
    }
  }
  /*Serial.print("KATEYTHINSH right=");
  Serial.println(right);*/

  turntofind(right, thesh);

}


void turntofind(boolean turn, float thesh)
{
  int i = 0;
  float thesh_new2;
  float thesh_now2;

  do {
    if (turn == false)
    {
      aristera();
    }
    else {
      deksia();
    }
    if (plhsiazwflag==true)
  {  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(H, LOW);
  delay(100);
}
    compass.read();
    float thesh_now2 = compass.heading();
    htimhtouy=compass.a.y;
    //Serial.println(thesh_now);
    if (plhsiazwflag==false){
    if (thesh_now2 >= (thesh - 2.0) && thesh_now2 <= (thesh + 2.0))
    {
      plhsiazwflag=true;
    }}
    if (plhsiazwflag==true)
    {
    if (thesh_now2 >= (thesh - 0.05) && thesh_now2 <= (thesh + 0.05))
    {
      Serial.println(thesh_now2);
      Serial.print("h*"); //me '*h' leme sto Raspberry pws akolouthei h klish
      Serial.println(htimhtouy);

      //Serial.println(thesh);
      break;
    }}
    if (thesh_now2 >= 180)
    {
      thesh_new2 = thesh_now2 - 180;
      if ((thesh < thesh_now2) && (thesh > thesh_new2))
      {
        turn = false;
      }
      else      {
        turn = true;
      }
    }
    else {
      thesh_new2 = thesh_now2 + 180;
      if ((thesh > thesh_now2) && (thesh < thesh_new2))
      {
        turn = true;
      }
      else      {
        turn = false;
      }
    }
 } while ( true);

  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(H, LOW);
}

void forward(int moves) { // tha dexetai to retArrayDist
  // Serial.println(moves);
  for (int i = 0; i > moves; i--) {
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, HIGH);
    digitalWrite(F, LOW);
    digitalWrite(G, HIGH);
    digitalWrite(H, LOW);
    delay(3);
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
    digitalWrite(H, HIGH);
    delay(3);
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, LOW);
    digitalWrite(H, HIGH);
    delay(3);
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(H, LOW);
    delay(3);
  }
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(H, LOW);
}

void backwards(int moves)
{
  for (int i = 0; i < moves; i++) {
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(H, LOW);
    delay(3);
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, LOW);
    digitalWrite(H, HIGH);
    delay(3);
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, LOW);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
    digitalWrite(H, HIGH);
    delay(3);
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, LOW);
    digitalWrite(E, HIGH);
    digitalWrite(F, LOW);
    digitalWrite(G, HIGH);
    digitalWrite(H, LOW);
    delay(3);
  }
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(H, LOW);
}

void aristera()
{
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(H, LOW);
  delay(2);
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(H, HIGH);
  delay(2);
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(H, HIGH);
  delay(2);
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, HIGH);
  digitalWrite(F, LOW);
  digitalWrite(G, HIGH);
  digitalWrite(H, LOW);
  delay(2);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(H, LOW);
  delay(10);
}

void deksia()
{
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, HIGH);
  digitalWrite(F, LOW);
  digitalWrite(G, HIGH);
  digitalWrite(H, LOW);
  delay(2);
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(H, HIGH);
  delay(2);
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(H, HIGH);
  delay(2);
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(H, LOW);
  delay(2);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(H, LOW);
  delay(10);
}
