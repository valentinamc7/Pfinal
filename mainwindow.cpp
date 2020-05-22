//Grupo 3 ALSE 3
//Diego Bolivar, Valentina Mahecha y Maria Paula Montoya
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <fstream>
#include <ctime>
#define MAX_F 100
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Enviar_clicked()
{
    string carn;
    pid_t pid;
    QString car;
    char *tod[MAX_F];
    int c, x=1000000, y=10, l=0, i=0;
    int dc[6];
    char ac[50];
    float valores_fijos[]={0,0,1800,60, 120, 40};
    float valores_ingresados[10], errores[10];
    string data[10], num[10];
    QString vis;
    time_t rawtime = (int)valores_ingresados[6];
    time_t tt;
    struct tm ts;
    struct tm * ti;
    char buf[80];
    string::size_type sz;
    car=ui->carnet->text();
    carn=car.toStdString(); 
    tod[0]="./cliente.bin";
    tod[1]="186.155.208.171";
    //strcpy(tod[2],carn);
    tod[2]="2147299";
    tod[3]="65000";
    tod[4]=NULL;
    //identificar el carnet
    int total= carn.length();
    c=atoi(carn.c_str());
    for (int var = 0; var < total; ++var) {
        dc[var]=c/x;
        dc[var]=dc[var]-l;
        x=x/y;
        l=dc[var]+l;
        l=l*y;
    }
    //Hallar lo errores de precisión
    errores[0]=0;
    errores[1]=0;
    errores[2]=(dc[3]*10)+dc[4];
    errores[3]=0.05*valores_fijos[3];
    errores[4]=dc[2];
    errores[5]=dc[6];
    //Ejecución del proceso de comunicación
    pid=fork();
    if(pid==0)
    {
        execv("/home/alse/Pfinal/cliente.bin",tod);
        exit(0);
    }
    else
    {
        pid=wait(NULL);
        ifstream archivo;
        archivo.open("/home/alse/Pfinal/datos_motor.txt", ifstream::in);
        //Obtención de datos
        if(archivo.is_open())
        {
            while(archivo.good())
            {
                archivo.getline(ac,256,',');
                data[i]=ac;
                vis.append(QString::fromStdString(data[i]));
                size_t found = data[i].find(':');
                num[i]=data[i].substr(found+1);
                num[6]=num[6].substr(0,num[6].find('}'));
                valores_ingresados[i]=atof(num[i].c_str());
                i++;
            }
            //Visualización de datos en la interfaz
            ui->resultado->setPlainText(vis);
            archivo.close();
        }
        //Extracción de tiempo
        ts = *localtime(&rawtime);
        ti = localtime(&tt);
        strftime(buf, sizeof (buf), "%a %Y-%m-%d %H:&M:%S %Z", &ts);
        cout << buf << endl;
        cout << asctime(ti) << endl;
        ui->Tiempo->setState(1);

        //Indicadores de rendimiento del motor
        if (valores_ingresados[2]<(valores_fijos[2]+errores[2]) && valores_ingresados[2]> (valores_fijos[2]-errores[2])){
            ui->Velocidad->switchLedIndicator();
        }
        if (valores_ingresados[3]<(valores_fijos[3]+errores[3]) && valores_ingresados[3]> (valores_fijos[3]-errores[3])){
            ui->Frecuencia->switchLedIndicator();
        }
        if (valores_ingresados[4]<(valores_fijos[4]+errores[4]) && valores_ingresados[4]> (valores_fijos[4]-errores[4])){
            ui->Voltaje->switchLedIndicator();
        }
        if (valores_ingresados[5]<(valores_fijos[5]+errores[5]) && valores_ingresados[5]> (valores_fijos[5]-errores[5])){
            ui->Temperatura->switchLedIndicator();
        }

}
}
