class Wheel {
  private:
    const int _pinA;
    const int _pinB;
    //int _pwm;
    
  public:
    Wheel(int pinA, int pinB) : _pinA(pinA), _pinB(pinB) {
      // Configurar pinos de saída
      pinMode(_pinA,   OUTPUT); 
      pinMode(_pinB,   OUTPUT); 

      // Definindo estado inicial
      //velocity(0);
      stopp();
    }
    
    /*void velocity(int pwm) {
      _pwm = pwm;
    }*/
    
    void stopp() {
      analogWrite(_pinA, LOW);
      //delay(50);
      analogWrite(_pinB, LOW);
    }
    
    void forward(int pwm) {
      analogWrite(_pinA, pwm);
      //delay(50);
      analogWrite(_pinB, LOW);
    }
    
    void backward(int pwm) {
      analogWrite(_pinA, LOW);
      //delay(50);
      analogWrite(_pinB, pwm);
    }
    
}; //end class roda
