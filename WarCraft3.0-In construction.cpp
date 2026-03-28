#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

const string warrior_names[5]={"dragon","ninja","iceman","lion","wolf"},weapon_names[3]={"sword","bomb","arrow"};
const int order_of_red[5]={2,3,4,1,0},order_of_blue[5]={3,0,1,2,4};
bool war_over=false,occupied=false;

class Weapon{
	public:
		int type;
		int durability;
		double attack;
		Weapon(int type,int dur,int attack):type(type),durability(dur),attack(attack){}
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
		int position;//number of present city
		int headquarterLifeElement;
		int totalCity;
		bool alive;
		vector<Weapon> weaponList;
		
		Warrior(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):color(color),id(id),type(type),strength(strength),idOfClass(idOfClass),remainLifeElement(remainLifeElement),attack(attack),position(position),alive(true),headquarterLifeElement(headquarterLifeElement),totalCity(totalCity){}
		
		void detectOccupation(int time){
			if(color=="red"&&position==totalCity+1){
				printf("%03d:%02d red %s %d reached blue headquarter with %d elements and force %d\n",time/60,time%60,warrior_names[type],id,remainLifeElement,attack);
				printf("%03d:%02d blue headquarter was taken\n",time/60,time%60);
				occupied=true;
			}
			if(color=="blue"&&position==0){
				printf("%03d:%02d blue %s %d reached red headquarter with %d elements and force %d\n",time/60,time%60,warrior_names[type],id,remainLifeElement,attack);
				printf("%03d:%02d red headquarter was taken\n",time/60,time%60);
				occupied=true;
			}
		}
		
		virtual void born(int time){
			printf("%03d:%02d %s %s %d born\n",time/60,time%60,color.c_str(),warrior_names[type].c_str(),id);
		}
		
		virtual void yell(){}
		
		virtual bool bombSelfHarm(){return true;}
		
		virtual void move(int step,int k=0){
			if(color=="red") position+=step;
			else position-=step;
		} 
		
		virtual void decreaseLoyalty(int k){}
		
		virtual bool runAway(){return 0;}
		
		virtual void stealWeapon(Warrior* opponent,int time){}
		
		virtual void reportCondition(int time){
			int swordSum=0,bombSum=0,arrowSum=0;
			for(auto& p:weaponList){
				switch(p.type){
					case 0:swordSum++;break;
					case 1:bombSum++;break;
					case 2:arrowSum++;
				}
			}
			printf("%03d:%02d %s %s %d has %d sword %d bomb %d arrow and %d elements\n",time/60,time%60,color.c_str(),warrior_names[type].c_str(),id,swordSum,bombSum,arrowSum,remainLifeElement);
		}
};

class Dragon:public Warrior{
	public:
		Dragon(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity){}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:weaponList.push_back(Weapon(0,10000,attack*2/10));break;
				case 1:weaponList.push_back(Weapon(1,1,attack*4/10));break;
				case 2:weaponList.push_back(Weapon(2,2,attack*3/10));
			}
		}
		
		void yell(int time){
			printf("%03d:%02d %s dragon %d yelled in city %d\n",time/60,time%60,color.c_str(),id,position);
		}
};

class Ninja:public Warrior{
	public:
		Ninja(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity){}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:weaponList.push_back(Weapon(0,10000,attack*2/10));break;
				case 1:weaponList.push_back(Weapon(1,1,attack*4/10));break;
				case 2:weaponList.push_back(Weapon(2,2,attack*3/10));
			}
			switch((id+1)%3){
				case 0:weaponList.push_back(Weapon(0,10000,attack*2/10));break;
				case 1:weaponList.push_back(Weapon(1,1,attack*4/10));break;
				case 2:weaponList.push_back(Weapon(2,2,attack*3/10));
			}
		}
		
		bool bombSelfHarm(){return false;}
};

class Iceman:public Warrior{
	public:
		Iceman(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity){}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:weaponList.push_back(Weapon(0,10000,attack*2/10));break;
				case 1:weaponList.push_back(Weapon(1,1,attack*4/10));break;
				case 2:weaponList.push_back(Weapon(2,2,attack*3/10));
			}
		}
		
		void move(int step,int k=0){
			if(color=="red") position+=step;
			else position-=step;
			remainLifeElement-=strength*step/10;
			remainLifeElement=(remainLifeElement<0)?0:remainLifeElement; 
		}
};

class Lion:public Warrior{
	public:
		int loyalty;
		Lion(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity),loyalty(headquarterLifeElement){}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:weaponList.push_back(Weapon(0,10000,attack*2/10));break;
				case 1:weaponList.push_back(Weapon(1,1,attack*4/10));break;
				case 2:weaponList.push_back(Weapon(2,2,attack*3/10));
			}
			printf("Its loyalty is %d\n",loyalty);
		}
		
		bool runAway(){return loyalty<=0;}
		
		void move(int step,int k=0){
			if(color=="red") position+=step;
			else position-=step;
			loyalty-=k;
		}
};

class Wolf:public Warrior{
	public:
		Wolf(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity){}
		
		void stealWeapon(Warrior* opponent,int time){
			if(opponent->type==4) return;
			if(opponent->weaponList.size()==0) return;
			if(opponent->position!=position) return; 
			
			int minType=3;
			for(auto p:opponent->weaponList){
				if(p.type<minType) minType=p.type;
			}
			
			vector<Weapon> temp;
			auto it=opponent->weaponList.begin();
			while(it!=opponent->weaponList.end()){
				if(it->type==minType&&temp.size()+weaponList.size()<10){
					temp.push_back(*it);
					it=opponent->weaponList.erase(it);
				}else ++it;
			}
			
			sort(temp.begin(),temp.end(),[](Weapon a,Weapon b){
				if(a.type==2&&b.type==2){
					return a.durability>b.durability;
				}else return false;
			});
			
			for(auto p:temp){weaponList.push_back(p);}
			
			int sum=int(temp.size());
			printf("%03d:%02d %s wolf %d took %d %s from %s %s %d in city %d\n",time/60,time%60,color.c_str(),id,sum,weapon_names[minType].c_str(),opponent->color.c_str(),warrior_names[opponent->type].c_str(),opponent->id,position);
		}
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
				int attack=bornAttack[type];
				orderIndex=(orderIndex+1)%5;
					
				switch(type){
					case 0:return new Dragon(color,id,type,strength,idOfClass,bornStrength[type],attack,color=="red"?0:totalCity+1,remainLifeElements,totalCity);
					case 1:return new Ninja(color,id,type,strength,idOfClass,bornStrength[type],attack,color=="red"?0:totalCity+1,remainLifeElements,totalCity);
					case 2:return new Iceman(color,id,type,strength,idOfClass,bornStrength[type],attack,color=="red"?0:totalCity+1,remainLifeElements,totalCity);
					case 3:return new Lion(color,id,type,strength,idOfClass,bornStrength[type],attack,color=="red"?0:totalCity+1,remainLifeElements,totalCity);
					case 4:return new Wolf(color,id,type,strength,idOfClass,bornStrength[type],attack,color=="red"?0:totalCity+1,remainLifeElements,totalCity);
				}
			}
			stop_produce=true;
			stop=true;
			return 0;
		}
		
		bool getStop(){return stop_produce;}
		int getLifeElements(){return lifeElements;}
};

vector<Warrior*> redWarrior;
vector<Warrior*> blueWarrior;

int main(){
	int T;
	cin>>T;
	for(int Case=1;Case<=T;Case++){
		for(auto& p:redWarrior) if(p) delete p;
		for(auto& p:blueWarrior) if(p) delete p;
		redWarrior.clear();
		blueWarrior.clear(); 
		
		int m,n,k,t;
		cin>>m>>n>>k>>t;//n-cities;k-loyalty decrease;t-final timestamp
		int strength[5];
		int attack[5];
		war_over=false;
		occupied=false;
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
						redWarrior.push_back(temp);
						temp->born(time);
					}
				}
				if(!Blue.getStop()){
					bool stop;
					Warrior* temp=Blue.makeWarrior(stop);
					if(temp){
						blueWarrior.push_back(temp);
						temp->born(time);
					}
				}
			}else if(time%60==5){
				auto it=redWarrior.begin();
				while(it!=redWarrior.end()){
					Warrior* p=*it;
					if(p&&p->type==3&&p->alive&&p->runAway()){
						printf("%03d:%02d red lion %d ran away\n",time/60,time%60,p->id);
						delete p;
						it=redWarrior.erase(it);
					}else ++it;
				}
				it=blueWarrior.begin();
				while(it!=blueWarrior.end()){
					Warrior* p=*it;
					if(p&&p->type==3&&p->alive&&p->runAway()){
						printf("%03d:%02d blue lion %d ran away\n",time/60,time%60,p->id);
						delete p;
						it=blueWarrior.erase(it);
					}else ++it;
				}
			}else if(time%60==10){
				for(auto& p:redWarrior){
					if(p){
						p->move(1,k);
						printf("%03d:%02d red %s marched to city %d with %d elements and force %d\n",time/60,time%60,warrior_names[p->type].c_str(),p->position,p->remainLifeElement,p->attack);
						p->detectOccupation(time);
					}
				}
				for(auto& p:blueWarrior){
					if(p){
						p->move(1,k);
						printf("%03d:%02d blue %s marched to city %d with %d elements and force %d\n",time/60,time%60,warrior_names[p->type].c_str(),p->position,p->remainLifeElement,p->attack);
						p->detectOccupation(time);
					}
				}
				if(occupied) war_over=true;
			}else if(time%60==35){
				for(auto p:redWarrior)
					if(p&&p->type==4)
						for(auto pp:blueWarrior)
							p->stealWeapon(pp,time);
				for(auto p:blueWarrior)
					if(p&&p->type==4)
						for(auto pp:redWarrior)
							p->stealWeapon(pp,time);
			}else if(time%60==40){
				
			}else if(time%60==50){
				printf("%03d:%02d %d elements in red headquarter\n",time/60,time%60,Red.getLifeElements());
				printf("%03d:%02d %d elements in blue headquarter\n",time/60,time%60,Blue.getLifeElements());
			}else if(time%60==55){
				for(auto p:redWarrior)
					if(p)
						p->reportCondition(time);
				for(auto p:blueWarrior)
					if(p)
						p->reportCondition(time);
			}
		}
	}
	return 0;
}
