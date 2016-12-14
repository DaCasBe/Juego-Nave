#include <cstdio>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <list>

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

using namespace std;

void situar_cursor(int x,int y){
	HANDLE consola;
	COORD posicion_cursor;
	
	consola=GetStdHandle(STD_OUTPUT_HANDLE);
	
	posicion_cursor.X=x;
	posicion_cursor.Y=y;
	
	SetConsoleCursorPosition(consola,posicion_cursor);
}

void ocultar_cursor(){
	HANDLE consola;
	CONSOLE_CURSOR_INFO cursor;
	
	consola=GetStdHandle(STD_OUTPUT_HANDLE);
	
	cursor.bVisible=false;
	
	SetConsoleCursorInfo(consola,&cursor);
}

void pintar_limites(){
	int i;
	
	for(i=2;i<78;i++){
		situar_cursor(i,3);
		printf("%c",205);
		
		situar_cursor(i,33);
		printf("%c",205);
	}
	
	for(i=4;i<33;i++){
		situar_cursor(2,i);
		printf("%c",186);
		
		situar_cursor(77,i);
		printf("%c",186);
	}
	
	situar_cursor(2,3);
	printf("%c",201);
	
	situar_cursor(2,33);
	printf("%c",200);
	
	situar_cursor(77,3);
	printf("%c",187);
	
	situar_cursor(77,33);
	printf("%c",188);
}

class Nave{
	private:
		int x_;
		int y_;
		int corazones_;
		int vidas_;
		
	public:
		Nave(int x,int y,int corazones,int vidas);
		void setX(int x);
		int getX();
		void setY(int y);
		int getY();
		void setCorazones(int corazones);
		int getCorazones();
		void setVidas(int vidas);
		int getVidas();
		void pintar();
		void borrar();
		void mover();
		void pintar_corazones();
		void explosion();
};

Nave::Nave(int x,int y,int corazones,int vidas){
	x_=x;
	y_=y;
	corazones_=corazones;
	vidas_=vidas;
}

void Nave::setX(int x){
	x_=x;
}

int Nave::getX(){
	return x_;
}

void Nave::setY(int y){
	y_=y;
}

int Nave::getY(){
	return y_;
}

void Nave::setCorazones(int corazones){
	corazones_=corazones;
}

int Nave::getCorazones(){
	return corazones_;
}

void Nave::setVidas(int vidas){
	vidas_=vidas;
}

int Nave::getVidas(){
	return vidas_;
}

void Nave::pintar(){
	situar_cursor(getX(),getY());
	printf("  %c",30);
	
	situar_cursor(getX(),getY()+1);
	printf(" %c%c%c",40,207,41);
	
	situar_cursor(getX(),getY()+2);
	printf("%c%c %c%c",30,190,190,30);
}

void Nave::borrar(){
	situar_cursor(getX(),getY());
	printf("      ");
	
	situar_cursor(getX(),getY()+1);
	printf("      ");
	
	situar_cursor(getX(),getY()+2);
	printf("      ");
}

void Nave::mover(){
	char tecla;
	
	if(kbhit()){
			tecla=getch();
			borrar();
			
			if(tecla==IZQUIERDA && getX()>3){
				setX(getX()-1);
			}
			
			if(tecla==DERECHA && getX()+5<77){
				setX(getX()+1);
			}
			
			if(tecla==ARRIBA && getY()>4){
				setY(getY()-1);
			}
			
			if(tecla==ABAJO && getY()+3<33){
				setY(getY()+1);
			}
			
			if(tecla=='e'){
				setCorazones(getCorazones()-1);
			}
						
			pintar();
			pintar_corazones();
		}
}

void Nave::pintar_corazones(){
	int i;
	
	situar_cursor(50,2);
	printf("Vidas %d",getVidas());
	
	situar_cursor(64,2);
	printf("Escudo");
	
	situar_cursor(71,2);
	printf("      ");
	
	for(i=0;i<getCorazones();i++){
		situar_cursor(71+i,2);
		printf("%c",3);
	}
}

void Nave::explosion(){
	if(getCorazones()==0){
		borrar();
		
		situar_cursor(getX()-1,getY());
		printf("   **   ");
		
		situar_cursor(getX()-1,getY()+1);
		printf("  ****  ");
		
		situar_cursor(getX()-1,getY()+2);
		printf("   **   ");
		
		Sleep(200);
		
		borrar();
		
		situar_cursor(getX()-1,getY());
		printf(" * ** * ");
		
		situar_cursor(getX()-1,getY()+1);
		printf("  ****  ");
		
		situar_cursor(getX()-1,getY()+2);
		printf(" * ** * ");
		
		Sleep(200);
		
		borrar();
		setVidas(getVidas()-1);
		setCorazones(3);
		pintar_corazones();
		pintar();
	}
}

class Asteroide{
	private:
		int x_;
		int y_;
		
	public:
		Asteroide(int x,int y);
		void setX(int x);
		int getX();
		void setY(int y);
		int getY();
		void pintar();
		void mover();
		void colision(Nave &n);
};

Asteroide::Asteroide(int x,int y){
	x_=x;
	y_=y;
	
	srand(time(NULL));
}

void Asteroide::setX(int x){
	x_=x;
}

int Asteroide::getX(){
	return x_;
}

void Asteroide::setY(int y){
	y_=y;
}

int Asteroide::getY(){
	return y_;
}

void Asteroide::pintar(){
	situar_cursor(getX(),getY());
	printf("%c",184);
}

void Asteroide::mover(){
	situar_cursor(getX(),getY());
	printf(" ");
	
	setY(getY()+1);
	
	if(getY()>32){
		setX((rand()%71)+4);
		setY(4);
	}
	
	pintar();
}

void Asteroide::colision(Nave &n){
	if(getX()>=n.getX() && getX()<n.getX()+6 && getY()>=n.getY() && getY()<=n.getY()+2){
		n.setCorazones(n.getCorazones()-1);
		
		if(n.getCorazones()==0){
			n.explosion();
		}
		
		n.borrar();
		n.pintar();
		n.pintar_corazones();
		setX((rand()%71)+4);
		setY(4);
	}
}

class Bala{
	private:
		int x_;
		int y_;
		
	public:
		Bala(int x,int y);
		void setX(int x);
		int getX();
		void setY(int y);
		int getY();
		void mover();
		bool fuera();
};

Bala::Bala(int x,int y){
	x_=x;
	y_=y;
}

void Bala::setX(int x){
	x_=x;
}

int Bala::getX(){
	return x_;
}

void Bala::setY(int y){
	y_=y;
}

int Bala::getY(){
	return y_;
}

void Bala::mover(){
	situar_cursor(getX(),getY());
	printf(" ");
	
	if(getY()>4){
		setY(getY()-1);
	}
	
	situar_cursor(getX(),getY());
	printf("*");
}

bool Bala::fuera(){
	if(getY()==4){
		return true;
	}
	
	else{
		return false;
	}
}

int main(){
	bool game_over=false;
	Nave n(37,30,3,3);
	list<Asteroide*> asteroides;
	list<Bala*> balas;
	char tecla;
	list<Bala*>::iterator b;
	list<Asteroide*>::iterator a;
	int i;
	int puntos=0;
	
	ocultar_cursor();
	pintar_limites();
	
	n.pintar();
	n.pintar_corazones();
	
	for(i=0;i<5;i++){
		asteroides.push_back(new Asteroide(rand()%75+3,rand()%5+4));
	}
	
	do{
		situar_cursor(4,2);
		printf("Puntos %d",puntos);
		
		if(kbhit()){
			tecla=getch();
			
			if(tecla=='f'){
				balas.push_back(new Bala(n.getX()+2,n.getY()-1));
			}
		}
		
		for(b=balas.begin();b!=balas.end();b++){
			(*b)->mover();
			
			if((*b)->fuera()){
				situar_cursor((*b)->getX(),(*b)->getY());
				printf(" ");
				
				delete(*b);
				b=balas.erase(b);
			}
		}
		
		for(a=asteroides.begin();a!=asteroides.end();a++){
			(*a)->mover();
			(*a)->colision(n);
		}
		
		for(a=asteroides.begin();a!=asteroides.end();a++){
			for(b=balas.begin();b!=balas.end();b++){
				if((*a)->getX()==(*b)->getX() && ((*a)->getY()+1==(*b)->getY() || (*a)->getY()==(*b)->getY())){
					situar_cursor((*b)->getX(),(*b)->getY());
					printf(" ");
					
					delete(*b);
					b=balas.erase(b);
					
					asteroides.push_back(new Asteroide(rand()%74+3,4));
					
					situar_cursor((*a)->getX(),(*a)->getY());
					printf(" ");
					
					delete(*a);
					a=asteroides.erase(a);
					
					puntos=puntos+5;
				}
			}
		}
		
		if(n.getVidas()==0){
			game_over=true;
		}
		
		n.explosion();
		n.mover();
		Sleep(30);
	}while(!game_over);
	
	system("cls");
	
	situar_cursor(36,15);
	printf("GAME OVER");
	
	return 0;
}
