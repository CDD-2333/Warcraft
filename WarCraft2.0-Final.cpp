#include <iostream>
#include <iomanip> 
#include <string>
using namespace std;

const string warrior_names[5]={"dragon","ninja","iceman","lion","wolf"},weapon_names[3]={"sword","bomb","arrow"};
const int order_of_red[5]={2,3,4,1,0},order_of_blue[5]={3,0,1,2,4};

class Warrior{
	public:
		string color;
		int id;
		int type;
		int strength;
		int idOfClass;
		int remainLifeElement;
		
		Warrior(string s,int a,int b,int c,int d,int e):color(s),id(a),type(b),strength(c),idOfClass(d),remainLifeElement(e){}
		
		virtual void born(int time){
			printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",time,color.c_str(),warrior_names[type].c_str(),id,strength,idOfClass,warrior_names[type].c_str(),color.c_str());
		}
};

class Dragon:public Warrior{
	public:
		Dragon(string s,int a,int c,int d,int e):Warrior(s,a,0,c,d,e){}
		
		void born(int time){
			Warrior::born(time);
			printf("It has a %s,and it's morale is %.2lf\n",weapon_names[id%3].c_str(),double(remainLifeElement)/double(strength));
		}
};

class Ninja:public Warrior{
	public:
		Ninja(string s,int a,int c,int d,int e):Warrior(s,a,1,c,d,e){}
		
		void born(int time){
			Warrior::born(time);
			printf("It has a %s and a %s\n",weapon_names[id%3].c_str(),weapon_names[(id+1)%3].c_str());
		}
};

class Iceman:public Warrior{
	public:
		Iceman(string s,int a,int c,int d,int e):Warrior(s,a,2,c,d,e){}
		
		void born(int time){
			Warrior::born(time);
			printf("It has a %s\n",weapon_names[id%3].c_str());
		}
};

class Lion:public Warrior{
	public:
		Lion(string s,int a,int c,int d,int e):Warrior(s,a,3,c,d,e){}
		
		void born(int time){
			Warrior::born(time);
			printf("It's loyalty is %d\n",remainLifeElement);
		}
};

class Wolf:public Warrior{
	public:
		Wolf(string s,int a,int c,int d,int e):Warrior(s,a,4,c,d,e){}
};

class Headquarter{
	private:
		string color;
		int lifeElements;
		int orderIndex;
		const int *warrior_order;
		bool stop_produce;
		int warriorCount[5];
		int totalWarriors;
		int bornStrength[5]; 
	public:
		Headquarter(const string& color,int lifeElements,const int * warrior_order,int strength[]):color(color),lifeElements(lifeElements),orderIndex(0),warrior_order(warrior_order),totalWarriors(0),stop_produce(0){
			for(int i=0;i<5;i++){
				warriorCount[i]=0;
				bornStrength[i]=strength[i];
			}
		}
		
		Warrior* makeWarrior(bool& stop){
			stop=false;
			if(stop_produce) return 0;
			
			for(int tried=0;tried<5;tried++){
				int type=warrior_order[orderIndex];
				if(lifeElements>=bornStrength[type]){
					lifeElements-=bornStrength[type];
					int id=++totalWarriors;
					int idOfClass=++warriorCount[type];
					int strength=bornStrength[type];
					int remainLifeElements=lifeElements;
					orderIndex=(orderIndex+1)%5;
					
					switch(type){
						case 0:return new Dragon(color,id,strength,idOfClass,remainLifeElements);
						case 1:return new Ninja(color,id,strength,idOfClass,remainLifeElements);
						case 2:return new Iceman(color,id,strength,idOfClass,remainLifeElements);
						case 3:return new Lion(color,id,strength,idOfClass,remainLifeElements);
						case 4:return new Wolf(color,id,strength,idOfClass,remainLifeElements);
					}
				}
				orderIndex=(orderIndex+1)%5;
			}
			stop_produce=true;
			stop=true;
			return 0;
		}
		
		bool getStop(){return stop_produce;}
};

int main(){
	int T;
	cin>>T;
	for(int Case=1;Case<=T;Case++){
		int m;
		cin>>m;
		int strength[5];
		for(int i=0;i<5;i++) cin>>strength[i];
		
		Headquarter Red("red",m,order_of_red,strength);
		Headquarter Blue("blue",m,order_of_blue,strength);
		printf("Case:%d\n",Case);
		
		for(int time=0;!Red.getStop()||!Blue.getStop();time++){
			if(!Red.getStop()){
				bool stop;
				Warrior *p=Red.makeWarrior(stop);
				if(stop){
					printf("%03d red headquarter stops making warriors\n",time);
				}else if(p){
					p->born(time);
					delete p;
				}
			}
			if(!Blue.getStop()){
				bool stop;
				Warrior *p=Blue.makeWarrior(stop);
				if(stop){
					printf("%03d blue headquarter stops making warriors\n",time);
				}else if(p){
					p->born(time);
					delete p;
				}
			}
		}
	}
	return 0;
}
