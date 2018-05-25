/**
 * -------------------------------
 * |  A  |  B    |   Estados    |
 * |-------------------------------
 * |  0  |  0    |   Desligar    |
 * |  0  |  1    |   Frente      |
 * |  1  |  0    |   Atras         |
 * |  1  |  1    |   Bug           |
 * -------------------------------
 */

class Wheel {
  private:
    const int _pinA;
    const int _pinB;
    const int _pwm;
    
  public:
    Wheel(int pinA, int pinB, int pwm) : _pinA(pinA), _pinB(pinB), _pwm(pwm) {
      // Configurar pinos de saída
      pinMode(_pinA,   OUTPUT); 
      pinMode(_pinB,   OUTPUT); 
      pinMode(_pwm,   OUTPUT); 

      // Definindo estado inicial
      stopp();
    }
       
    void stopp() {      
      analogWrite(_pwm, 0);
      digitalWrite(_pinA, LOW);
      digitalWrite(_pinB, LOW);
    }
    
    void forward(int pwm) {
      analogWrite(_pwm, pwm);
      digitalWrite(_pinA, LOW);
      digitalWrite(_pinB, HIGH);
    }
    
    void backward(int pwm) {
      analogWrite(_pwm, pwm);
      digitalWrite(_pinA, HIGH);
      digitalWrite(_pinB, LOW);
    }
    
}; //end class roda
