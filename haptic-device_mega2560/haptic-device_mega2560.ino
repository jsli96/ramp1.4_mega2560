# define y_Enable A8
# define y_Step A6
# define y_Dir A7
# define x_Enable A2
# define x_Step A0
# define x_Dir A1


void setup(){
    Serial.begin(9600);
    pinMode(y_Dir, OUTPUT);
    pinMode(y_Enable, OUTPUT);
    pinMode(y_Step, OUTPUT);
}

void loop(){


}