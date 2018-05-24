import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';

/*
  Generated class for the RemoteProvider provider.

  See https://angular.io/guide/dependency-injection for more info on providers
  and Angular DI.
*/
@Injectable()
export class RemoteProvider {

  apiUrl = 'http://192.168.4.1/';

  constructor(public http: HttpClient) {
    console.log('Hello RemoteProvider Provider');
  }

  requisicao(msg) {
    return new Promise(resolve => {
      this.http.get(this.apiUrl+ "" + msg).subscribe(data => {
        resolve(data);
      }, err => {
        console.log(err);
      });
    });
  }

}
