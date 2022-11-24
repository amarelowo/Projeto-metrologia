#include <Arduino.h>

// numero de medições a serem feitas para calculo da média
#define N 500

// declarando as variaveis que receberam os pinos a serem utilizados
const int trigPin = 26;
const int echoPin = 25;


// declarando o coeficiente de student e a variavel goblal "tempo"
double t = 2.005;
double tempo;


//criando um novo tipo de varivavel para armazenar os dados coletados
typedef struct {
  double media = 0.0;
  double desvP = 0.0 ;
  double precisao = 0.0;
  double referencia = 0.0;
  double espessura = 0.0;
} dados;


// iniciando a variavel "dados"
dados Dados;



double distancia(){
  //Função responsavel por acionar o sensor ultrassonico e calcular a distancia


  // envia um pulso por 10 milisegundos
  digitalWrite(trigPin, HIGH); 

  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // recebe o retorno do pulso
  tempo = pulseIn(echoPin, HIGH);

  // calcula a distancia pelo tempo percorrido
  double mmMsec = (tempo/29.4/2)*10;

  return mmMsec;
}
//oi
double calibracao(double media){
  //aplica as correções de acordo com as medições feitas em sala
  double result;

  if(media < 100){
    result = media;
  }
  else if((media >= 100) || (media < 200)){
    result = media - 0.91; 
  }
  else if ((media >= 200)||(media < 300)){
    result = media - 0.99;
  }
  else if ((media >= 300)||(media < 400)){
    result = media - 1.16;
  }
  else if ((media >=400)||(media < 500)){
    result = media + 0.90;
  }
  else if ((media >=500)||(media < 600)){
    result = media + 19.7;
  }
  else if(media < 0){
    result = 0;
  }
  else{
    result = media + 16.1;
  }
  return result;
}

double distanciaCalibrada(){
  // função responsavel por capturar 50 valores, calcular a média e aplicar as correções com a função calibracao();
  
  double mediaInst=0, result; //media instantanea para uma medição, resultado a ser retornado pela função
  int i=0, n=50;

  for(i=0;i<n;i++){
    mediaInst = mediaInst + distancia(); 
  }
  mediaInst = mediaInst/n;

  result = calibracao(mediaInst);  

  return result;
}

void definirDados(){
  //função responsavel por calcular a media, desvio padrão, precisão do sensor, referencia do sistema e a espessura da chapa e armazenar esses dados

  //declaração das variaveis uteis à função
  double *vet;
  int i;

  //iniciando a alocação dinamica de memoria
  vet = (double *)malloc(N * sizeof(double));


  // inicia o ciclo de 500 medições para calcular os dados
  Serial.println("Medicoes: ");
  for(i=0;i<N;i++){
    vet[i] = distancia();
    Dados.media = Dados.media + vet[i];
    Serial.println(vet[i]);
  }

  //define a media
  Dados.media = Dados.media/N;
  Serial.print("Media: ");
  Serial.println(Dados.media);
  Serial.print("Coeficiente de Student para N medicoes: ");
  Serial.println(t);

  //define o desvio padrão
  Serial.print("Desvio Padrao: ");
  for(i=0;i<N;i++){
    Dados.desvP = Dados.desvP + sq(vet[i]-Dados.media); 
  }
  Dados.desvP = sqrt(Dados.desvP/(N-1));
  Serial.println(Dados.desvP);

  //define a precisao
  Serial.print("Precisao: ");
  Dados.precisao = Dados.desvP*t;
  Serial.println(Dados.precisao);

  //captura a altura do sensor, para colocar como referencia
  Serial.print("Referencia para as medições: ");
  Dados.referencia = calibracao(Dados.media);
  Serial.println(Dados.referencia);


  // inicia o ciclo de capturar a espessuras das chapas, vale resaltar que essa definição serve apenas para chapas de mesma espessura
  Serial.println("Coloque a chapa em frente ao sensor, em seguida precione qualquer tecla para proseguir: ");

  while(Serial.available() == 0){
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" ");

  while(true){
    Serial.println("Definindo espessura chapa");
    Dados.espessura = Dados.referencia - distanciaCalibrada();
    
    if (Dados.espessura > 5){
      Serial.print("Espessura da chapa definida: ");
      Serial.println(Dados.espessura);
      break;
    }
    else{
      Serial.println("Espessura invalida, reposicione a chapa e precione qualquer tecla para tentar novamente: ");
      if(Serial.available() > 0){
        Serial.read();
      }
      while(Serial.available() == 0){
        Serial.print(".");
        delay(1000);
      }
    Serial.println(" ");  
    } 

  }
  
  //limpa a variavel para que não ocupe memoria sem necessidade
  free(vet);
}


void setup() {
  Serial.begin(9600); // Inicia a comunicação serial
  
   // Define os pinos que vão ser utilizados
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  pinMode(echoPin, INPUT); 

  Serial.println("Coletando Dados");
  delay(500);
  
  
  definirDados();
}

void loop() {

  //declara as variaveis que serão apresentadas no terminal
  double medicaoLinear, numChapas;

  //medição
  Serial.print("Medição Linear: ");
  medicaoLinear = Dados.referencia - distanciaCalibrada(); 
  if(medicaoLinear < 0){
    medicaoLinear = 0;
  }
  Serial.print(medicaoLinear);
  Serial.print(" | N. de chapas: ");
  numChapas = medicaoLinear / Dados.espessura;
  Serial.println(numChapas);
  delay(1000);
  
}