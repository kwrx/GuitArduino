//
// MIT License

// Copyright (c) 2020 Antonino Natale

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.



#define BTNS_MAX            2
#define POTS_MAX            6
#define POTS_USR            4

#define LOG                 1
#define LIN                 0

#define POT(type, pin)      { pin, type, 0 }
#define BTN(pin)            { pin, 0 }


struct UserInput {
    int pin;
    int type;
    int state;
};

UserInput Pots[POTS_MAX] = {
    POT(LIN, A4), POT(LIN, A5), POT(LIN, A3),
    POT(LOG, A2), POT(LOG, A6), POT(LOG, A1),
};

UserInput Btns[BTNS_MAX] = {
    BTN(2), BTN(3)
};

int Mode = 0;




static void Wr(byte cmd, byte note, byte pressure) {

    Serial.write(cmd);
    Serial.write(note);
    Serial.write(pressure);
    
    delay(10);

}


void setup() {

    Serial.begin(38400);
        
    for(int i = 0; i < BTNS_MAX; i++)
        pinMode(Btns[i].pin, INPUT_PULLUP);
      
}



void loop() {

    
    for(int i = 0; i < BTNS_MAX; i++) {

        int s;
        if((s = digitalRead(Btns[i].pin)) == Btns[i].state)
            continue;
        
        Btns[i].state = s;
        

        switch(Mode) {

            case 0:

                Wr(0x90, 0x40 + (Mode * 5) + i, 0x40);
                Wr(0x80, 0x40 + (Mode * 5) + i, 0x00);
                break;


            case 2:

                if(s)
                    Wr(0x90, 0x40 + (Mode * 5) + i, 0x40);
                else
                    Wr(0x80, 0x40 + (Mode * 5) + i, 0x00);

                break;


            case 1:

                if(s)
                    Wr(0xB0, (Mode * 5) + i, 127);
                else
                    Wr(0xB0, (Mode * 5) + i, 0);

                break;


        default:

            Mode = 0;
            break;

        }
    }


  
    for(int i = 0; i < POTS_MAX; i++) {

        s = analogRead(Pots[i].pin);
        s = 1024 - s;
        

        if(Pots[i].type == LOG)
            s = (int) (sqrt((double) s) * 32 + 1);
        
        s = map(map(s, 0, 1024, 0, 32), 0, 64, 0, 127);
        

        if(s == Pots[i].state)
            continue;
        
        
        Pots[i].state = s;
        
        if(i != POTS_USR)
            Wr(0xB0, i, s);
        else
            Mode = map(s, 0, 127, 0, 3);


    }

}
