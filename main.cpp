#include <iostream>
#include <cstdlib>
#include <allegro.h>
#include <fstream>


using namespace std;

BITMAP *buffer;
BITMAP *muertebmp;
BITMAP *tanquebmp;
BITMAP *tanque;
BITMAP *roca;
BITMAP *trofeo;
BITMAP *bala;

//pos inicial tanque
int direccion = 0;

//int pos_x = 30*14;
//int pos_y = 30*15;

int anterior_pos_x;
int anterior_pos_y;

//puntaje
int score = 0;
int score_anterior = score;

//estado del jugador
bool vida = true;

//mapa
int const ancho = 21;
int const alto = 31;

//bala

int pos_b_x;
int pos_b_y;
int origen_b_x;
int origen_b_y;

class Juego{

public:
    void crearMapa();
    bool finJuego();
    void imprimirSprite();
    void Inicializar();

protected:

    char mapa[ancho][alto]={
       "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
       "X|           |||           |X",
       "X                           X",
       "X       X            X      X",
       "X       X            X      X",
       "X       X            X      X",
       "X       X            X      X",
       "X                           X",
       "X|                         |X",
       "X                           X",
       "X|                         |X",
       "X                           X",
       "X|                         |X",
       "X                           X",
       "X       X            X      X",
       "X       X            X      X",
       "X       X            X      X",
       "X       X            X      X",
       "X            XXX            X",
       "X|          |XoX|          |X",
       "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    };
};


void Juego::crearMapa()
{
   for(int fila = 0; fila < ancho; fila++)
    {
        for(int columna = 0; columna < alto; columna++)
        {
            if(mapa[fila][columna] == 'X')
            {
                draw_sprite(buffer, roca, columna*30, fila*30);
            }
            else if(mapa[fila][columna] == 'o')
            {
                draw_sprite(buffer, trofeo, columna*30, fila*30);
            }
        }
   }
}


bool Juego::finJuego()
{
   for(int fila = 0; fila < ancho; fila++)
    {
        for(int columna = 0; columna < alto; columna++)
        {
            if(mapa[fila][columna] == 'o')
            {
                return true;
            }
        }
   }
   return false;
}


void Juego::imprimirSprite()
{
   blit(buffer, screen, 0,0,0,0,880,630);
}

void Juego::Inicializar()
{
    buffer = create_bitmap(880, 630);
    roca = load_bitmap("bloque.bmp",NULL);
    tanquebmp = load_bitmap("tanque.bmp",NULL);
    tanque = create_bitmap(33, 33);
    trofeo = load_bitmap("trofeo.bmp",NULL);
    bala = load_bitmap("bala.bmp",NULL);
    muertebmp = load_bitmap("muerte.bmp", NULL);

}

class Sprite
{
    public:
        virtual void CrearSprite() = 0;
};

class Tanque: public Sprite, public Juego
{
    public:
        void CrearSprite();
        void direccionamiento(int &, int &, int &);
    protected:
        int pos_x = 30*14;
    int pos_y = 30*15;

};

void Tanque::direccionamiento(int &direccion, int &pos_x, int &pos_y)
{

    if(key[KEY_LEFT])
        direccion = 0;
       else if(key[KEY_RIGHT])
           direccion = 1;
       else if(key[KEY_UP])
           direccion = 2;
       else if(key[KEY_DOWN])
           direccion = 3;

           if(direccion == 0){
                if(mapa[pos_y/30][(pos_x-30)/30] != 'X')
                   pos_x -= 30;
                else direccion = 0;
           }

           if(direccion == 1){
                if(mapa[pos_y/30][(pos_x+30)/30] != 'X')
                   pos_x += 30;
                else direccion = 1;
           }

           if(direccion == 2){
                if(mapa[(pos_y-30)/30][pos_x/30] != 'X')
                  pos_y -= 30;
                else direccion = 2;
           }

           if(direccion == 3){
                if(mapa[(pos_y+30)/30][pos_x/30] != 'X')
                   pos_y += 30;
                else direccion = 3;
           }
}

void Tanque::CrearSprite()
{
    blit(tanquebmp, tanque, direccion*33,0,0,0,33,33);
    draw_sprite(buffer, tanque, pos_x, pos_y);
}


class Enemigo: public Sprite, public Juego, public Tanque
{
    public:
        Enemigo();
        Enemigo(int, int);
        void CrearSprite();
        void Colision();
        void movimientoEnemigo();



    private:
        BITMAP *enemigobmp;
        BITMAP *enemigo;
        //pos enemigo
        int direccion_enemigo = 0;
        int pos_x_e = 30*14;
        int pos_y_e = 30*15;
};


Enemigo::Enemigo()
{
    pos_x_e = 30*14;
    pos_y_e = 30*8;
    direccion_enemigo = rand()%4;
    enemigo = create_bitmap(30, 30);
    enemigobmp = load_bitmap("enemigo1.bmp",NULL);
}


Enemigo::Enemigo(int x, int y)
{
    pos_x_e = x;
    pos_y_e = y;
    direccion_enemigo = rand()%4;
    enemigo = create_bitmap(30, 30);
    enemigobmp = load_bitmap("enemigo1.bmp",NULL);
}


void Enemigo::CrearSprite()
{
    blit(enemigobmp, enemigo, 0,0,0,0,30,30);
    draw_sprite(buffer, enemigo, pos_x_e, pos_y_e);
}


void Enemigo::Colision()
{
    if((pos_y == pos_y_e && pos_x == pos_x_e) || (pos_y_e == anterior_pos_y && pos_x_e == anterior_pos_x)){
        for(int i = 0; i <= 5; i++){
            clear(tanque);
            clear(buffer);
            crearMapa();

            blit(muertebmp, tanque, i*33,0,0,0,33,33);
            draw_sprite(buffer, tanque,pos_x, pos_y);

            imprimirSprite();
            rest(80);
            vida = false;
        }
    }

    if((pos_b_y == pos_y_e && pos_b_x == pos_x_e)){
        pos_x_e = 30*50;
        pos_y_e = 30*50;
        score += 10;
        }
    }


void Enemigo::movimientoEnemigo()
{
   CrearSprite();
   Colision();

   if(mapa[pos_y_e/30][pos_x_e/30] == '|')
    {
           direccion_enemigo = rand()%4;
    }
   if(direccion_enemigo == 0)
    {
       if(mapa[pos_y_e/30][(pos_x_e-30)/30] != 'X')
       {
           pos_x_e -= 30;
       }
       else direccion_enemigo = rand()%4;
   }
   if(direccion_enemigo == 1)
   {
       if(mapa[pos_y_e/30][(pos_x_e+30)/30] != 'X')
       {
           pos_x_e += 30;
       }
       else direccion_enemigo = rand()%4;
   }
   if(direccion_enemigo == 2)
   {
       if(mapa[(pos_y_e-30)/30][pos_x_e/30] != 'X')
       {
           pos_y_e -= 30;
       }
       else direccion_enemigo = rand()%4;
   }
   if(direccion_enemigo == 3)
   {
       if(mapa[(pos_y_e+30)/30][pos_x_e/30] != 'X')
       {
           pos_y_e += 30;
       }
       else direccion_enemigo = rand()%4;
   }
}



class Bala: public Juego, public Sprite{
    public:
        void disparo(int &, int&);
        void CrearSprite();
    private:

};


void Bala::disparo(int &pos_b_x, int &pos_b_y){
    origen_b_x = pos_x;
    origen_b_y = pos_y;
    if(key[KEY_SPACE] && direccion == 0){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
    }
    pos_b_x -= 30;//esto es para que la bala tenga movimiento libre


    if(key[KEY_SPACE] && direccion == 1){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
            pos_b_x += 30;
    }



    if(key[KEY_SPACE] && direccion == 2){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
            pos_b_y -= 30;
    }


    if(key[KEY_SPACE] && direccion == 3){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
            pos_b_y += 30;
    }

}

void Bala::CrearSprite()
{
    draw_sprite(buffer, bala, pos_b_x, pos_b_y);
}



int main()
{
    allegro_init();
	install_keyboard();

	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 880, 630, 0, 0);

    Juego main;

    main.Inicializar();

    Enemigo *A = new Enemigo;
    Enemigo *B = new Enemigo;
    Enemigo *C = new Enemigo;
    Enemigo *D = new Enemigo;

    Tanque *tan = new Tanque;

    Bala *balin = new Bala;

    while(!key[KEY_ESC] && main.finJuego() && vida){
       anterior_pos_x = pos_x;
       anterior_pos_y = pos_y;

       tan->direccionamiento(direccion, pos_x, pos_y);
       balin->disparo(pos_b_x, pos_b_y);


       clear(buffer);
	   main.crearMapa();
	   tan->CrearSprite();
	   balin->CrearSprite();


	   A->movimientoEnemigo();
	   B->movimientoEnemigo();
	   C->movimientoEnemigo();
	   D->movimientoEnemigo();

	   main.imprimirSprite();
	   rest(70);

	   clear(tanque);
       main.imprimirSprite();
       rest(90);

       ofstream guardar_puntaje;
       guardar_puntaje.open("Puntuacion_obtenida.txt");

       guardar_puntaje << "Tu puntuacion fue:" << score;
       guardar_puntaje.close();
    }

	extern char allegro_id[];
	cout << allegro_id << endl;

	return 0;
}
END_OF_MAIN();


