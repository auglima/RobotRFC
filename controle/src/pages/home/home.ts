import  { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import nipplejs from 'nipplejs'
import {RemoteProvider} from "../../providers/remote/remote";

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {

  host:string;
  port:number;

  consolee:string;

  pwm:number;
  parado:boolean;
  direcao:boolean;
  esquerda:boolean;
  direita:boolean;
  arma:boolean;


  constructor(public navCtrl: NavController, public remote:RemoteProvider) {
    this.pwm = 0;
    this.parado = true;
    this.direcao = false;
    this.esquerda = false;
    this.direita = false;
    this.arma = false;
  }

  ionViewDidLoad() {
    console.log(document.getElementById('zone_joystick'));

    let options = {
      zone:document.getElementById('zone_joystick'),
      mode:'static',
      multitouch: true,
      position: {left:'50%',top:'50%'},
      color:'#cecece',
      size: 360
    };

    let manager = nipplejs.create(options);

    manager.get(0)
      .on('move', function (evt, data) {
        if(data.angle) {
          if(Math.abs(data.force) <= 1) {
            // px, py are between 0 and 1
            let px = -Math.cos(data.angle.radian) * data.force;
            let py = +Math.sin(data.angle.radian) * data.force;
            let offset = 0.2;

            console.log(px + " | " + py);


            // Para frente
            if (py > offset && px < offset && px > -offset) {
              this.consolee = "Para frente\n";
              this.parado = false;
              this.direcao = false; // para frente
            }
            // Para tras
            else if (py < offset && px < offset && px > -offset) {
              this.consolee = "Para tras\n";
              this.parado = false;
              this.direcao = true;  // para tras
            }
            // Para direita
            else if (px < offset && py < offset && py > -offset) {
              this.consolee = "Rotacionar direita\n";
              this.parado = true;
              this.esquerda = false;
              this.direita = true;
            }
            // Para esquerda
            else if (px > offset && py < offset && py > -offset) {
              this.consolee = "Rotacionar esquerda\n";
              this.parado = true;
              this.esquerda = true;
              this.direita = false;
            }
            else if (px > 0 && py > 0) {
              this.consolee = "Curva a esquerda - Frente\n";
              this.parado = false;
              this.direcao = false; // para frente
              this.esquerda = true;
              this.direita = false;
            }
            else if (px < 0 && py > 0) {
              this.consolee = "Curva a direita - Frente\n";
              this.parado = false;
              this.direcao = false; // para frente
              this.esquerda = false;
              this.direita = true;
            }
            else if (px > 0 && py < 0) {
              this.consolee = "Curva a esquerda - Tras\n";
              this.parado = false;
              this.direcao = true;  // para tras
              this.esquerda = true;
              this.direita = false;
            }
            else if (px < 0 && py < 0) {
              this.consolee = "Curva a direita - Tras\n";
              this.parado = false;
              this.direcao = true;  // para tras
              this.esquerda = false;
              this.direita = true;
            }



            if (px < 0) { px = px * (-1); }
            if (py < 0) { py = py * (-1); }
            px = px * 100;
            py = py * 100;
            let p = (px > py) ? px : py;
            this.pwm =  parseInt(this.map(p, 0, 100, 0, 255),10);


            this.enviar();
          }
        }
      }.bind(this))
      .on('end', function (evt, data) {
        this.consolee = "Parou\n";
        this.parado = true;
        this.pwm = 0;
        this.esquerda = false;
        this.direita = false;

        this.enviar();
      }.bind(this));
  }

  acionarArma() {
    this.arma = (this.arma) ? false : true;
    this.enviar();
  }

  enviar() {
    let msg = ""+this.FormatNumberLength(this.pwm, 3)+ "" + ((this.parado) ? 1 : 0) + "" + ((this.direcao) ? 1 : 0) + "" + ((this.esquerda) ? 1 : 0) + "" + ((this.direita) ? 1 : 0) + "" + ((this.arma) ? 1 : 0)+"";
    //console.log(this.pwm + "|" + this.parado + "|" + this.direcao + "|" + this.esquerda + "|" + this.direita + "|" + this.arma);
    this.consolee += ( msg + "\n");

    this.remote.requisicao(msg).then((result) => {
      console.log(result);
    }, (err) => {
      console.log(err);
    });

  }

  map(num, in_min, in_max, out_min, out_max) {
    return (num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  FormatNumberLength(num, length) {
    var r = "" + num;
    while (r.length < length) {
      r = "0" + r;
    }
    return r;
  }

}
