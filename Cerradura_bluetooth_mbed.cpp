#include "mbed.h"

//Buses Comunicación                            
UnbufferedSerial PUERTO_SERIAL_COM(PA_2,PA_3); // USBTX - USBRX
UnbufferedSerial PUERTO_SERIAL_RES(PA_9,PA_10); // PA_2 - PA_3 // puerto de respaldo bluetooth
DigitalOut TESTIGO_LED(PA_0); // LED testigo
//Buses Display
BusOut DATOS_LCD(PA_13, PA_14, PA_15, PC_13, PB_15, PB_14, PB_13, PC_4); //D7...D0
DigitalOut ENABLES_LCD(PC_6); // pin de enable LCD
DigitalOut RS_LCD(PC_8); // pin de RS LCD
//Buses Entrada
DigitalIn INTERRUPTOR_PUSH(PA_1); //Pulsador
BusIn INTERRUPTORES_DIP(PC_0,PC_1,PC_3,PC_2); //Dipswitch
//Resultados
DigitalOut CORRECTO_LED(PB_9); // led testigo de conexion correcta
DigitalOut INCORRECTO_LED(PB_8); // led testigo de conexion fallida

//Bluetooth
char mensaje_bluetooth[500];
uint8_t indice_bluetooth = 0;
char dato_rx;
char BUFFER_RX[10];

//Variables
char clave_acceso[4]{' ',' ',' ',' '};
char mensaje_saludo[50];
char mensaje_saludo2[50];
char mensaje_ingreso[50];
char mensaje_ingreso2[50];
char mensaje_ingreso3[50];
char mensaje_ingreso4[50];
char mensaje_dispositivo[50];
char mensaje_dispositivo2[50];
char mensaje_ok[50];
char mensaje_ok2[50];
char mensaje_nook[50];
char mensaje_nook2[50];
char mensaje_bienvenido[50];
char mensaje_bienvenido2[50];
char mensaje_digitado[50];
char mensaje_digitado2[50];
int contador = 0;

//Rutinas
void mostrar_saludo(void);
void inicio_sistema(void);
void ingreso_clave(void);
void captura_clave(void);
void mostrar_clave(void);
void mensaje_dispositivo_1(void);
void comunicacion_bluetooth(void);
void mostrar_digitado(void);
void comparar_claves(void);
//Rutinas internas
void flash_led(void);
void presionar_enter(void);
char obtener_caracter(void);
void mostrar_ok(void);
void mostrar_nook(void);
void mostrar_bienvenida(void);
//Display
void enviar_comando_lcd(uint8_t COMANDO);
void enviar_dato_lcd(uint8_t CARACTER);
void inicializar_lcd(void);

//-----------------------------------------INICIO----------------------------------------------

int main(void)
{       
        PUERTO_SERIAL_RES.baud(9600); // configuramos los baudios a 9600 para la comunicacion
        //printf("Bienvenido al control del sistema\n");
        inicializar_lcd();
        mostrar_saludo();
        inicio_sistema();
        ingreso_clave();
        captura_clave();
        mostrar_clave();
        mensaje_dispositivo_1();
        comunicacion_bluetooth();
        mostrar_digitado();
        comparar_claves();
        
}   

void mostrar_saludo(void)
{
    uint8_t indice_saludo;
    uint8_t longitud_saludo = 0;

	//enviar_comando_lcd(128);
    enviar_comando_lcd(131);// Centramos

    longitud_saludo = sprintf(mensaje_saludo,"Seguidores ");
 
    for(indice_saludo = 0;indice_saludo<longitud_saludo;indice_saludo++)
	{
        enviar_dato_lcd(mensaje_saludo[indice_saludo]);
		ThisThread::sleep_for(50ms);
	}

	//enviar_comando_lcd(192);
    enviar_comando_lcd(194); // Centramos

    longitud_saludo = sprintf(mensaje_saludo2,"De la Grasa :V");
 
    for(indice_saludo = 0;indice_saludo<longitud_saludo;indice_saludo++)
	{
        enviar_dato_lcd(mensaje_saludo2[indice_saludo]);
		ThisThread::sleep_for(50ms);
	}
}

void inicio_sistema(void)
{
    TESTIGO_LED = 0;
    ThisThread::sleep_for(500ms);      
    TESTIGO_LED = 1;
    ThisThread::sleep_for(500ms); 
    TESTIGO_LED = 0; 
    ThisThread::sleep_for(500ms);      
    TESTIGO_LED = 1; 
    ThisThread::sleep_for(500ms);
}

void ingreso_clave(void)
{
    uint8_t indice_ingreso;
    uint8_t longitud_ingreso = 0;

    enviar_comando_lcd(1);//Limpia display y deja en HOME

	//enviar_comando_lcd(128); // primer renglon 1
    enviar_comando_lcd(131);// Centramos

    longitud_ingreso = sprintf(mensaje_ingreso,"Defina su");
 
    for(indice_ingreso = 0;indice_ingreso<longitud_ingreso;indice_ingreso++)
	{
        enviar_dato_lcd(mensaje_ingreso[indice_ingreso]);
		ThisThread::sleep_for(50ms);
	}

	//enviar_comando_lcd(192);// segundo renglon 1
    enviar_comando_lcd(194); // Centramos

    longitud_ingreso = sprintf(mensaje_ingreso2,"clave: ");
 
    for(indice_ingreso = 0;indice_ingreso<longitud_ingreso;indice_ingreso++)
	{
        enviar_dato_lcd(mensaje_ingreso2[indice_ingreso]);
		ThisThread::sleep_for(50ms);
	}
}

void captura_clave(void)
{   
    uint8_t puerto = 0;
    uint8_t contador_local = 0;

    for(contador_local = 0;contador_local<10;contador_local++){clave_acceso[contador_local]='\0';}

    for (contador_local=0;contador_local<4;contador_local++)
        {
            presionar_enter();
            clave_acceso[contador_local]=obtener_caracter();
            enviar_dato_lcd(clave_acceso[contador_local]); // visualizamos la contraseña mientras la vamos escribiendo
            flash_led();
        }
}

void mostrar_clave(void)
{   
    uint8_t indice_mostrar;
    uint8_t longitud_mostrar = 0;

    enviar_comando_lcd(1);//Limpia display y deja en HOME
    //enviar_comando_lcd(128);// Primer renglon primer caracter
    enviar_comando_lcd(129);// Centrar

    longitud_mostrar = sprintf(mensaje_ingreso3,"Clave definida:");
 
    for(indice_mostrar = 0;indice_mostrar<longitud_mostrar;indice_mostrar++)
	{
        enviar_dato_lcd(mensaje_ingreso3[indice_mostrar]);
		ThisThread::sleep_for(50ms);
	}

	//enviar_comando_lcd(192);// Segundo renglon primer caracter
    enviar_comando_lcd(197); // Centrar

    longitud_mostrar = sprintf(mensaje_ingreso4,"%s",clave_acceso);

    for(indice_mostrar = 0;indice_mostrar<longitud_mostrar;indice_mostrar++)
	{
        enviar_dato_lcd(mensaje_ingreso4[indice_mostrar]);
		ThisThread::sleep_for(50ms);
	}

    ThisThread::sleep_for(2s);
}

void mensaje_dispositivo_1(void)
{
    uint8_t indice_dispositivo;
    uint8_t longitud_dispositivo = 0;

    enviar_comando_lcd(1);//Limpia display y deja en HOME

	//enviar_comando_lcd(128);
    enviar_comando_lcd(128);// Centrar

    longitud_dispositivo = sprintf(mensaje_dispositivo,"Digite clave en");
 
    for(indice_dispositivo = 0;indice_dispositivo<longitud_dispositivo;indice_dispositivo++)
	{
        enviar_dato_lcd(mensaje_dispositivo[indice_dispositivo]);
		ThisThread::sleep_for(50ms);
	}

	//enviar_comando_lcd(192);
    enviar_comando_lcd(194); // Centrar

    longitud_dispositivo = sprintf(mensaje_dispositivo2,"dispositivo");
 
    for(indice_dispositivo = 0;indice_dispositivo<longitud_dispositivo;indice_dispositivo++)
	{
        enviar_dato_lcd(mensaje_dispositivo2[indice_dispositivo]);
		ThisThread::sleep_for(50ms);
	}
}

void comunicacion_bluetooth(void)
{
    int bandera_bluetooth = 1;
    int longitud_mensaje,a;

    indice_bluetooth = 0;
    longitud_mensaje = 0;
    
    ThisThread::sleep_for(1s);

    longitud_mensaje = sprintf(mensaje_bluetooth,"\n\rDigite su clave");

    a=0;

    do
    {
        if(PUERTO_SERIAL_RES.writeable())
        {
            PUERTO_SERIAL_RES.write(&mensaje_bluetooth[a],1);
            a++;
        }

    } while(a<longitud_mensaje);
    
        a=0;
        longitud_mensaje = sprintf(mensaje_bluetooth,"\n\rDigite su clave");

    for (a=0; a<10; a++) {BUFFER_RX[a] ='\0';}
    
    a = 0;

    do
    {
        if(PUERTO_SERIAL_RES.readable()) // Revisamos si hay algun dato para leer en el RX
        {
            PUERTO_SERIAL_RES.read(&BUFFER_RX[a],1);
            a++;

            if(a>=4){bandera_bluetooth = 0;}
        }

    }while(bandera_bluetooth);
}    

void mostrar_digitado(void)
{   
    uint8_t indice_mostrar;
    uint8_t longitud_mostrar = 0;

    enviar_comando_lcd(1);//Limpia display y deja en HOME
	//enviar_comando_lcd(128);
    enviar_comando_lcd(129);// Centramos

    longitud_mostrar = sprintf(mensaje_digitado,"Clave digitada:");
 
    for(indice_mostrar = 0;indice_mostrar<longitud_mostrar;indice_mostrar++)
	{
        enviar_dato_lcd(mensaje_digitado[indice_mostrar]);
		ThisThread::sleep_for(50ms);
	}

	//enviar_comando_lcd(192);// Segundo renglon primer caracter
    enviar_comando_lcd(197); // Centrar

    longitud_mostrar =  sprintf(mensaje_digitado2,"%s",BUFFER_RX);
 
    for(indice_mostrar = 0;indice_mostrar<longitud_mostrar;indice_mostrar++)
	{
        enviar_dato_lcd(mensaje_digitado2[indice_mostrar]);
		ThisThread::sleep_for(50ms);
	}

    ThisThread::sleep_for(2s);
}

void comparar_claves(void)
{
    int detector = 0;

    for(contador=0;contador<4;contador++) 
    {
        if(clave_acceso[contador] == BUFFER_RX[contador]) {detector++;}
    }

    if (detector == 4)
    
        {mostrar_ok();}

    else
        {mostrar_nook();}
}

void mostrar_bienvenida(void)
{
    uint8_t indice;
    uint8_t longitud = 0;

    enviar_comando_lcd(1);//Limpia display y deja en HOME

	//enviar_comando_lcd(128);
    enviar_comando_lcd(131);// Centrar

    longitud = sprintf(mensaje_bienvenido,"Bienvenido");
 
    for(indice = 0;indice<longitud;indice++)
	{
        enviar_dato_lcd(mensaje_bienvenido[indice]);
		ThisThread::sleep_for(50ms);
	}

	//enviar_comando_lcd(192);
    enviar_comando_lcd(192); // Centrar

    longitud = sprintf(mensaje_bienvenido2,"a su habitacion");
 
    for(indice = 0;indice<longitud;indice++)
	{
        enviar_dato_lcd(mensaje_bienvenido2[indice]);
		ThisThread::sleep_for(50ms);
	}
}

//--------------------------------- INTERNAS --------------------------------------------------

void presionar_enter(void)
{
    while (INTERRUPTOR_PUSH==1)
    {}
    ThisThread::sleep_for(40ms);

    while (INTERRUPTOR_PUSH==0)
    {}
    ThisThread::sleep_for(40ms);
}

char obtener_caracter(void)
{
    char puerto;    

	switch (INTERRUPTORES_DIP)  // Lee puertos dip switch
	{
        case 0  : puerto='0' ;break;
        case 1  : puerto='1' ;break;
        case 2  : puerto='2' ;break;
        case 3  : puerto='3' ;break;
        case 4  : puerto='4' ;break;
        case 5  : puerto='5' ;break;
        case 6  : puerto='6' ;break;
        case 7  : puerto='7' ;break;
        case 8  : puerto='8' ;break;
        case 9  : puerto='9' ;break;
        case 10 : puerto='A' ;break;
        case 11 : puerto='B' ;break;
        case 12 : puerto='C' ;break;
        case 13 : puerto='D' ;break;
        case 14 : puerto='E' ;break;
        case 15 : puerto='F' ;break;

   	    default : puerto='X' ;break;
	}   
        return (puerto);  
}

void flash_led(void)
{
    TESTIGO_LED = 1;

    for (contador=0;contador<4;contador++)
    {
        TESTIGO_LED = 1;
        ThisThread::sleep_for(40ms);      
        TESTIGO_LED = 0;
        ThisThread::sleep_for(40ms); 
        TESTIGO_LED = 1;
    }

    TESTIGO_LED = 1;
}

void mostrar_ok(void)
{  
    int indice;
    int longitud = 0;
    int longitud_mensaje = 0;
    int a = 0;

    longitud_mensaje = sprintf(mensaje_bluetooth,"\n\rClave correcta");

    enviar_comando_lcd(1);//Limpia display y deja en HOME
	//enviar_comando_lcd(128);
    enviar_comando_lcd(133);// Centramos

    longitud = sprintf(mensaje_ok,"Clave");
 
    for(indice = 0;indice<longitud;indice++)
	{
        enviar_dato_lcd(mensaje_ok[indice]);
		ThisThread::sleep_for(50ms);
	}

	//enviar_comando_lcd(192);
    enviar_comando_lcd(196); // Centrar

    longitud = sprintf(mensaje_ok2,"Correcta");
 
    for(indice = 0;indice<longitud;indice++)
	{
        enviar_dato_lcd(mensaje_ok2[indice]);
		ThisThread::sleep_for(50ms);
	}

    do
    {
        if(PUERTO_SERIAL_RES.writeable())
        {
            PUERTO_SERIAL_COM.write(&mensaje_bluetooth[a],1);
            PUERTO_SERIAL_RES.write(&mensaje_bluetooth[a],1);
            a++;
        }

    } while(a<longitud_mensaje);

    CORRECTO_LED = 1;
    INCORRECTO_LED = 0;
    ThisThread::sleep_for(2s);
    mostrar_bienvenida();
}

void mostrar_nook(void)
{  
    int indice;
    int longitud = 0;
    int longitud_mensaje = 0;
    int a=0;

    longitud_mensaje = sprintf(mensaje_bluetooth,"\n\rClave incorrecta");

    enviar_comando_lcd(1);//Limpia display y deja en HOME
    enviar_comando_lcd(133);// Centramos

    longitud = sprintf(mensaje_nook,"Clave");
 
    for(indice = 0;indice<longitud;indice++)
	{
        enviar_dato_lcd(mensaje_nook[indice]);
		ThisThread::sleep_for(50ms);
	}

	//enviar_comando_lcd(192);
    enviar_comando_lcd(195); // Centramos

    longitud = sprintf(mensaje_nook2,"Incorrecta");
 
    for(indice = 0;indice<longitud;indice++)
	{
        enviar_dato_lcd(mensaje_nook2[indice]);
		ThisThread::sleep_for(50ms);
	}

    do
    {
        if(PUERTO_SERIAL_RES.writeable())
        {
            PUERTO_SERIAL_RES.write(&mensaje_bluetooth[a],1);
            PUERTO_SERIAL_COM.write(&mensaje_bluetooth[a],1);
            a++;
        }

    } while(a<longitud_mensaje);

    CORRECTO_LED = 0;
    INCORRECTO_LED = 1;
    ThisThread::sleep_for(2s);
    mensaje_dispositivo_1();
    comunicacion_bluetooth();
    mostrar_digitado();
    comparar_claves();
}

// ----------------------------------- DISPLAY ------------------------------------

void enviar_comando_lcd(uint8_t COMANDO)
{
	RS_LCD  	= 0;
	//RW  	= 0;
	ENABLES_LCD  = 0;
	DATOS_LCD   = COMANDO;
    
    ENABLES_LCD  = 1;
	wait_us(1);
	ENABLES_LCD  = 0;
    
	ThisThread::sleep_for(2ms);
}

void enviar_dato_lcd(uint8_t CARACTER)
{
	RS_LCD  	= 1;
	//RW  	= 0;
	ENABLES_LCD  = 0;
	DATOS_LCD   = CARACTER;
    
	ENABLES_LCD  = 1;
	wait_us(1);
	ENABLES_LCD  = 0;
    
	ThisThread::sleep_for(2ms);
}

void inicializar_lcd(void)
{
	ThisThread::sleep_for(50ms);
	enviar_comando_lcd(0b00110000);
    
	ThisThread::sleep_for(25ms);
	enviar_comando_lcd(0b00110000);
    
	ThisThread::sleep_for(1ms);
	enviar_comando_lcd(0b00110000);
    
	enviar_comando_lcd(0b00111000);  // Activa la segunda linea y establece la interfaz en 8 bits
	enviar_comando_lcd(0b00001100);  // enviar_comando_lcd(0b00001111); → poner el  cursor y el parpadeo
	enviar_comando_lcd(0b00000001);    
}
