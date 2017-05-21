

#define BT_COUNT          3
#define BT_DELAY          50
#define BT_LONGTIMING     250

struct {
  int Pin;
  int NoteDown;
  int NotePressed;
  int Repeat;
  int State;
} CMD[] = {
  { 2, 0x1E, 0x3E, 2, 0 },
  { 4, 0x1F, 0x3F, 1, 0 },
  { 3, 0x20, 0x40, 1, 0 },
};

int CurrentMode = 0;

void setup() {
  Serial.begin(38400);
  while(!Serial);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}



void SendCMD(int i) {
  int Note = 0;
  int n, r = 1;
  
  if(CMD[i].State > BT_LONGTIMING)
    Note = CMD[i].NotePressed;
  else {
    Note = CMD[i].NoteDown;
    r = CMD[i].Repeat;
  }
  
  CMD[i].State = 0;

  for(n = 0; n < r; n++) {
    Serial.write(0x90);
    Serial.write(Note);
    Serial.write(0x45);
    Serial.write(0x80);
    Serial.write(Note);
    Serial.write(0x00);
    delay(BT_DELAY);
  }
}

void loop() {
  for(int i = 0; i < BT_COUNT; i++) {
    if(digitalRead(CMD[i].Pin) == 0)
      CMD[i].State++;
    else {
      if(CMD[i].State == 0)
        continue;

      SendCMD(i);
    }
  }

  delay(1);
}

