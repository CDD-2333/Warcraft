#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <cstring>
using namespace std;

const string warrior_names[5]={"dragon","ninja","iceman","lion","wolf"},weapon_names[3]={"sword","bomb","arrow"};
const int order_of_red[5]={2,3,4,1,0},order_of_blue[5]={3,0,1,2,4};
const int MAX_CITY_NUMBER=20;

class Weapon{
	protected:
		int type;
		int durability;
		double attack;
	public:
		Weapon(int type,int dur,int attack):type(type),durability(dur),attack(attack){}
};

class Sword:public Weapon{
	public:
		Sword(int type,int dur,int attack):Weapon(type,dur,attack){}
}; 

class Bomb:public Weapon{
	public:
		Bomb(int type,int dur,int attack):Weapon(type,dur,attack){}
};

class Arrow:public Weapon{
	int usage;
	public:
		Arrow(int type,int dur,int attack):Weapon(type,dur,attack),usage(0){}
		
};

class Warrior{
	public:
		string color;
		int id;
		int type;
		int strength;//hp when born
		int idOfClass;//id in the class it belongs to
		int remainLifeElement;//hp remaining 
		int attack;
		int position;//number of city
		bool alive;
		vector<Sword> sword_list;//weapon_type weapon_usage
		vector<Bomb> bomb_list;
		vector<Arrow> arrow_list;
		
		Warrior(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position):color(color),id(id),type(type),strength(strength),idOfClass(idOfClass),remainLifeElement(remainLifeElement),attack(attack),position(position),alive(true){}
		
		virtual void born(int time){
			printf("%03d:%02d %s %s %d born\n",time/60,time%60,color.c_str(),warrior_names[type].c_str(),id);
		}
		
		virtual void yell(){}
		
		virtual bool bombSelfHarm(){return true;}
		
		virtual void move(int step){
			if(color=="red") position+=step;
			else position-=step;
		} 
		
		virtual void decreaseLoyalty(int k){}
		
		virtual bool runAway(){return 0;}
};

class Dragon:public Warrior{
	public:
		Dragon(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position){}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:sword_list.push_back(Sword(0,10000,attack*2.0/10.0));break;
				case 1:bomb_list.push_back(Bomb(1,1,attack*4.0/10.0));break;
				case 2:arrow_list.push_back(Arrow(2,2,attack*3.0/10.0));
			}
			printf("It has a %s,and it's morale is %.2lf\n",weapon_names[id%3].c_str(),double(remainLifeElement)/double(strength));
		}
		
		void yell(){
			printf("%s dragon %d yelled in city %d\n",color.c_str(),idOfClass,position);
		}
};

class Ninja:public Warrior{
	public:
		Ninja(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position){}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:sword_list.push_back(Sword(0,10000,attack*2.0/10.0));break;
				case 1:bomb_list.push_back(Bomb(1,1,attack*4.0/10.0));break;
				case 2:arrow_list.push_back(Arrow(2,2,attack*3.0/10.0));
			}
			switch((id+1)%3){
				case 0:sword_list.push_back(Sword(0,10000,attack*2.0/10.0));break;
				case 1:bomb_list.push_back(Bomb(1,1,attack*4.0/10.0));break;
				case 2:arrow_list.push_back(Arrow(2,2,attack*3.0/10.0));
			}
			printf("It has a %s and a %s\n",weapon_names[id%3].c_str(),weapon_names[(id+1)%3].c_str());
		}
		
		bool bombSelfHarm(){return false;}
};

class Iceman:public Warrior{
	public:
		Iceman(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position){}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:sword_list.push_back(Sword(0,10000,attack*2.0/10.0));break;
				case 1:bomb_list.push_back(Bomb(1,1,attack*4.0/10.0));break;
				case 2:arrow_list.push_back(Arrow(2,2,attack*3.0/10.0));
			}
			printf("It has a %s\n",weapon_names[id%3].c_str());
		}
		
		void move(int step){
			if(color=="red") position+=step;
			else position-=step;
			remainLifeElement-=(strength/10.0)*step;
		}
};

class Lion:public Warrior{
	public:
		int loyalty;
		Lion(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position),loyalty(double(remainLifeElement)){}
		
		void born(int time){
			Warrior::born(time);
			printf("It's loyalty is %d\n",loyalty);
		}
		
		bool runAway(){return loyalty<=0;}
		
		void decreaseLoyalty(int k){loyalty-=k;} 
};

class Wolf:public Warrior{
	public:
		Wolf(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position){}
		
		void stealWeapon(Warrior* opponent);
};

class Headquarter{
	private:
		string color;
		int lifeElements;
		int orderIndex;
		const int *warrior_order;
		int warriorCount[5];
		int totalWarriors;
		int bornStrength[5]; 
		int bornAttack[5];
		int nextId;
		int totalCity;
		bool stop_produce;
	public:
		Headquarter(const string& color,int lifeElements,const int * warrior_order,int strength[],int attack[],int totalCity):color(color),lifeElements(lifeElements),orderIndex(0),warrior_order(warrior_order),totalWarriors(0),stop_produce(0),nextId(1),totalCity(totalCity){
			for(int i=0;i<5;i++){
				warriorCount[i]=0;
				bornStrength[i]=strength[i];
				bornAttack[i]=attack[i];
			}
		}
		
		Warrior* makeWarrior(bool& stop){
			stop=false;
			if(stop_produce) return 0;
			
			int type=warrior_order[orderIndex];
			if(lifeElements>=bornStrength[type]){
				lifeElements-=bornStrength[type];
				int id=++totalWarriors;
				int idOfClass=++warriorCount[type];
				int strength=bornStrength[type];
				int remainLifeElements=lifeElements;
				double attack=bornAttack[type];
				orderIndex=(orderIndex+1)%5;
					
				switch(type){
					case 0:return new Dragon(color,id,type,strength,idOfClass,remainLifeElements,attack,color=="red"?0:totalCity+1);
					case 1:return new Ninja(color,id,type,strength,idOfClass,remainLifeElements,attack,color=="red"?0:totalCity+1);
					case 2:return new Iceman(color,id,type,strength,idOfClass,remainLifeElements,attack,color=="red"?0:totalCity+1);
					case 3:return new Lion(color,id,type,strength,idOfClass,remainLifeElements,attack,color=="red"?0:totalCity+1);
					case 4:return new Wolf(color,id,type,strength,idOfClass,remainLifeElements,attack,color=="red"?0:totalCity+1);
				}
			}
			stop_produce=true;
			stop=true;
			return 0;
		}
		
		bool getStop(){return stop_produce;}
		int getLifeElements(){return lifeElements;}
};

Warrior* redWarrior[201];
Warrior* blueWarrior[201];

int main(){
	int T;
	cin>>T;
	for(int Case=1;Case<=T;Case++){
		memset(redWarrior,0,sizeof(redWarrior));
		memset(blueWarrior,0,sizeof(blueWarrior));
		
		int m,n,k,t;
		cin>>m>>n>>k>>t;//n-cities;k-loyalty decrease;t-final timestamp
		int strength[5];
		int attack[5];
		bool war_over=false;
		for(int i=0;i<5;i++) cin>>strength[i];
		for(int i=0;i<5;i++) cin>>attack[i];
		
		Headquarter Red("red",m,order_of_red,strength,attack,n);
		Headquarter Blue("blue",m,order_of_blue,strength,attack,n);
		printf("Case:%d\n",Case);
		
		for(int time=0;!war_over&&time<=t;time++){
			if(time%60==0){
				if(!Red.getStop()){
					bool stop;
					Warrior* temp=Red.makeWarrior(stop);
					if(temp){
						redWarrior[temp->id]=temp;
						redWarrior[temp->id]->born(time);
					}
				}
				if(!Blue.getStop()){
					bool stop;
					Warrior* temp=Blue.makeWarrior(stop);
					if(temp){
						blueWarrior[temp->id]=temp;
						blueWarrior[temp->id]->born(time);
					}
				}
			}else if(time%60==5){
				for(auto& p:redWarrior){
					if(p->type==3&&p->alive==true&&p->runAway()){
						p->alive=false;
						printf("%03d:%02d red lion %d ran away\n",time/60,time%60,p->id);
					}
				}
				for(auto& p:blueWarrior){
					if(p->type==3&&p->alive==true&&p->runAway()){
						p->alive=false;
						printf("%03d:%02d blue lion %d ran away\n",time/60,time%60,p->id);
					}
				}
			}else if(time%60==10){
				for(auto& p:redWarrior){
					p->move(1);
					printf("%03d:%02d red %s marched to city %d with %d elements and force %d",time/60,time%60,warrior_names[p->type].c_str(),p->remainLifeElement,p->attack);
				}
				for(auto& p:blueWarrior){
					p->move(-1);
				}
			}else if(time%60==35){
				
			}else if(time%60==40){
				
			}else if(time%60==50){
				printf("%03d:%02d %d elements in red headquarter\n",time/60,time%60,Red.getLifeElements());
				printf("%03d:%02d %d elements in blue headquarter\n",time/60,time%60,Blue.getLifeElements());
			}else if(time%60==55){
				
			}
		}
	}
	return 0;
}
