#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

const string warrior_names[5]={"dragon","ninja","iceman","lion","wolf"},weapon_names[3]={"sword","bomb","arrow"};
const int order_of_red[5]={2,3,4,1,0},order_of_blue[5]={3,0,1,2,4};
bool war_over=false;

class Move{
	public:
		string color;
		int type;
		int id;
		int position;
		int remainLifeElement;
		int attack;
		
		Move(string color,int type,int id,int position,int remainLifeElement,int attack):color(color),type(type),id(id),position(position),remainLifeElement(remainLifeElement),attack(attack){}
}; 

class Weapon{
	public:
		int type;
		int durability;
		Weapon(int type,int dur):type(type),durability(dur){}
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
	
		virtual void born(int time){
			printf("%03d:%02d %s %s %d born\n",time/60,time%60,color.c_str(),warrior_names[type].c_str(),id);
		}
		
		virtual void yell(int time){}
		
		virtual bool bombSelfHarm(){return true;}
		
		virtual void move(int step,int k=0){
			if(color=="red") position+=step;
			else position-=step;
		} 
		
		virtual void decreaseLoyalty(int k){}
		
		virtual bool runAway(){return 0;}
		
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
		
		virtual void stealWeapon(Warrior* opponent,int time){}
		
		virtual void claimWeapon(Warrior* opponent){
			if(opponent->weaponList.size()==0) return;
			
			vector<Weapon> temp;
			sort(opponent->weaponList.begin(),opponent->weaponList.end(),[](Weapon a,Weapon b){
				if(a.type==b.type){
					return a.durability>b.durability;
				}else return a.type<b.type;
			});
			
			auto p=opponent->weaponList.begin();
			while(weaponList.size()+temp.size()<10&&p!=opponent->weaponList.end()){
				temp.push_back(*p);
				p=opponent->weaponList.erase(p);
			}
			
			for(auto p:temp){weaponList.push_back(p);}
		}
};

class Dragon:public Warrior{
	public:
		Dragon(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity){}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:weaponList.push_back(Weapon(0,10000));break;
				case 1:weaponList.push_back(Weapon(1,1));break;
				case 2:weaponList.push_back(Weapon(2,2));
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
				case 0:weaponList.push_back(Weapon(0,10000));break;
				case 1:weaponList.push_back(Weapon(1,1));break;
				case 2:weaponList.push_back(Weapon(2,2));
			}
			switch((id+1)%3){
				case 0:weaponList.push_back(Weapon(0,10000));break;
				case 1:weaponList.push_back(Weapon(1,1));break;
				case 2:weaponList.push_back(Weapon(2,2));
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
				case 0:weaponList.push_back(Weapon(0,10000));break;
				case 1:weaponList.push_back(Weapon(1,1));break;
				case 2:weaponList.push_back(Weapon(2,2));
			}
		}
		
		void move(int step,int k=0){
			if(color=="red") position+=step;
			else position-=step;
			remainLifeElement-=remainLifeElement*step/10;
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
				case 0:weaponList.push_back(Weapon(0,10000));break;
				case 1:weaponList.push_back(Weapon(1,1));break;
				case 2:weaponList.push_back(Weapon(2,2));
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
		
		virtual void stealWeapon(Warrior* opponent,int time){
			if(opponent->type==4) return;
			if(opponent->weaponList.empty()) return;
			if(opponent->position!=position) return; 
			
			vector<Weapon> temp;
			sort(opponent->weaponList.begin(),opponent->weaponList.end(),[](Weapon a,Weapon b){
				if(a.type==b.type) return a.durability>b.durability;
				else return a.type<b.type;
			});
			int minType=opponent->weaponList[0].type;
			
			auto it=opponent->weaponList.begin();
			while(it!=opponent->weaponList.end()){
				if(it->type==minType&&weaponList.size()+temp.size()<10){
					temp.push_back(*it);
					it=opponent->weaponList.erase(it);
				}else if(it->type==minType) ++it;
				else break;
			}
			
			if(temp.empty()) return;
			
			for(auto p:temp) weaponList.push_back(p);
			
			int sum=int(temp.size());
			printf("%03d:%02d %s wolf %d took %d %s from %s %s %d in city %d\n",time/60,time%60,color.c_str(),id,sum,weapon_names[minType].c_str(),opponent->color.c_str(),warrior_names[opponent->type].c_str(),opponent->id,position);
		}
};

class BattleMatch{
	public:
		Warrior* w1;
		Warrior* w2;//w1-red warrior/w2-blue warrior
		BattleMatch(Warrior* a,Warrior* b):w1(a),w2(b){}
		
		void Battle(int time){
			auto weaponSort=[](Weapon a,Weapon b){
				if(a.type==b.type) return a.durability<b.durability;
				return a.type<b.type;
			};
			auto getDamage=[](int weaponType,int warriorAttack){
				if(weaponType==0) return warriorAttack*2/10;
				if(weaponType==1) return warriorAttack*4/10;
				if(weaponType==2) return warriorAttack*3/10;
				return 0;
			};
			
			sort(w1->weaponList.begin(),w1->weaponList.end(),weaponSort);
			sort(w2->weaponList.begin(),w2->weaponList.end(),weaponSort);
			bool battle_order=(w1->position%2==0);
			int index1=0,index2=0;
			bool battle_over=false;
			
			while(!battle_over){
				if(!w1->alive&&!w2->alive){
					printf("%03d:%02d both red %s %d and blue %s %d died in city %d\n",time/60,time%60,warrior_names[w1->type].c_str(),w1->id,warrior_names[w2->type].c_str(),w2->id,w1->position);
					break;
				}
				if(!w2->alive){
					printf("%03d:%02d red %s %d killed blue %s %d in city %d remaining %d elements\n",time/60,time%60,warrior_names[w1->type].c_str(),w1->id,warrior_names[w2->type].c_str(),w2->id,w1->position,w1->remainLifeElement);
					w1->claimWeapon(w2);
					break;
				}
				if(!w1->alive){
					printf("%03d:%02d blue %s %d killed red %s %d in city %d remaining %d elements\n",time/60,time%60,warrior_names[w2->type].c_str(),w2->id,warrior_names[w1->type].c_str(),w1->id,w1->position,w2->remainLifeElement);
					w2->claimWeapon(w1);
					break;
				}
				
				bool noChange=true;
				for(auto& p:w1->weaponList){
					if(getDamage(p.type,w1->attack)>0||p.type!=0){
						noChange=false;
						break;
					}
				}
				if(noChange){
					for(auto& p:w2->weaponList){
						if(getDamage(p.type,w2->attack)>0||p.type!=0){
							noChange=false;
							break;
						} 
					}
				}
				if(noChange){
					printf("%03d:%02d both red %s %d and blue %s %d were alive in city %d\n",time/60,time%60,warrior_names[w1->type].c_str(),w1->id,warrior_names[w2->type].c_str(),w2->id,w1->position);
					break;
				}
				
				Warrior* attacker=battle_order?w2:w1;
				Warrior* defender=battle_order?w1:w2;
				int& index=battle_order?index2:index1;
				
				if(!attacker->weaponList.empty()){
					if(index>=attacker->weaponList.size()) index=0;
					Weapon& presentWeapon=attacker->weaponList[index];
					
					int damage=getDamage(presentWeapon.type,attacker->attack);
					defender->remainLifeElement-=damage;
					
					if(defender->remainLifeElement<=0){
						defender->remainLifeElement=0;
						defender->alive=false;
					}
					
					if(presentWeapon.type==1&&attacker->bombSelfHarm()){
						attacker->remainLifeElement-=damage/2;
						if(attacker->remainLifeElement<=0){
							attacker->remainLifeElement=0;
							attacker->alive=false;
						}
					}
					
					if(presentWeapon.type==1){
						attacker->weaponList.erase(attacker->weaponList.begin()+index);
						if(index>=attacker->weaponList.size()) index=0;
					}else if(presentWeapon.type==2){
						presentWeapon.durability--;
						if(presentWeapon.durability==0){
							attacker->weaponList.erase(attacker->weaponList.begin()+index);
							if(index>=attacker->weaponList.size()) index=0;
						}else index=(index+1)%(attacker->weaponList.size());
					}else{
						index=(index+1)%(attacker->weaponList.size());
					}
				}
				
				battle_order=!battle_order;
			}
			
			if(w1->alive&&w1->type==0) w1->yell(time);
			if(w2->alive&&w2->type==0) w2->yell(time);
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
vector<BattleMatch> battleList;
vector<Move> moveList;

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
		for(int i=0;i<5;i++) cin>>strength[i];
		for(int i=0;i<5;i++) cin>>attack[i];
		
		Headquarter Red("red",m,order_of_red,strength,attack,n);
		Headquarter Blue("blue",m,order_of_blue,strength,attack,n);
		printf("Case %d:\n",Case);
		
		for(int time=0;!war_over&&time<=t;time++){
			moveList.clear();
			battleList.clear();
			
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
					if(p&&p->type==3&&p->alive&&p->runAway()&&p->position!=n+1){
						printf("%03d:%02d red lion %d ran away\n",time/60,time%60,p->id);
						delete p;
						it=redWarrior.erase(it);
					}else ++it;
				}
				it=blueWarrior.begin();
				while(it!=blueWarrior.end()){
					Warrior* p=*it;
					if(p&&p->type==3&&p->alive&&p->runAway()&&p->position!=0){
						printf("%03d:%02d blue lion %d ran away\n",time/60,time%60,p->id);
						delete p;
						it=blueWarrior.erase(it);
					}else ++it;
				}
			}else if(time%60==10){
				for(auto& p:redWarrior){
					if(p){
						p->move(1,k);
						if(p->remainLifeElement<=0) p->alive=false;
						moveList.push_back(Move(p->color,p->type,p->id,p->position,p->remainLifeElement,p->attack));
					}
				}
				for(auto& p:blueWarrior){
					if(p){
						p->move(1,k);
						if(p->remainLifeElement<=0) p->alive=false;
						moveList.push_back(Move(p->color,p->type,p->id,p->position,p->remainLifeElement,p->attack));
					}
				}
				
				sort(moveList.begin(),moveList.end(),[](Move a,Move b){
					if(a.position==b.position){
						return a.color>b.color;
					}else return a.position<b.position;
				});
				
				for(auto& p:moveList){
					if(p.position>=1&&p.position<=n){
						printf("%03d:%02d %s %s %d marched to city %d with %d elements and force %d\n",time/60,time%60,p.color.c_str(),warrior_names[p.type].c_str(),p.id,p.position,p.remainLifeElement,p.attack);
					}else{
						const char* head=(p.position==0)?"red":"blue";
						printf("%03d:%02d %s %s %d reached %s headquarter with %d elements and force %d\n",time/60,time%60,p.color.c_str(),warrior_names[p.type].c_str(),p.id,head,p.remainLifeElement,p.attack);
						Warrior* w=NULL;
						if(p.color=="red"){
							for(auto* ww:redWarrior)
								if(ww->id==p.id&&ww->type==p.type){w=ww;break;}
						}else{
							for(auto* ww:blueWarrior)
								if(ww->id==p.id&&ww->type==p.type){w=ww;break;}
						}
						if(w&&w->alive){
							printf("%03d:%02d %s headquarter was taken\n",time/60,time%60,head);
							war_over=true;
						}
					}
				}
				
				auto remove_dead=[](vector<Warrior*>& warriors){
					auto it=warriors.begin();
					while(it!=warriors.end()){
						Warrior* w=*it;
						if(!w->alive){
							delete w;
							it=warriors.erase(it);
						}else ++it;
					}
				};
				remove_dead(redWarrior);
				remove_dead(blueWarrior);
			}else if(time%60==35){
				vector<Warrior*> temp;
				for(auto p:redWarrior) if(p->type==4) temp.push_back(p);
				for(auto p:blueWarrior) if(p->type==4) temp.push_back(p);
				sort(temp.begin(),temp.end(),[](Warrior* a,Warrior* b){
					if(a->position==b->position){
						return a->color>b->color;
					}else return a->position<b->position;
				});
				
				for(auto p:temp){
					if(p->color=="red"){
						for(auto pp:blueWarrior)
							p->stealWeapon(pp,time);
					}else{
						for(auto pp:redWarrior)
							p->stealWeapon(pp,time);
					}
				}
			}else if(time%60==40){
				sort(redWarrior.begin(),redWarrior.end(),[](Warrior* a,Warrior* b){return a->position<b->position;});
				for(auto p:redWarrior){
					int city=p->position;
					for(auto pp:blueWarrior){
						if(pp->position==city&&city>=1&&city<=n){
							battleList.push_back(BattleMatch(p,pp));
							break;
						}
					}
				}
				
				for(auto p:battleList) p.Battle(time);
				
				auto p1=redWarrior.begin();
				while(p1!=redWarrior.end()){
					Warrior* temp=*p1;
					if(temp->alive==false){
						delete temp; 
						p1=redWarrior.erase(p1);
					}
					else ++p1;
				}
				auto p2=blueWarrior.begin();
				while(p2!=blueWarrior.end()){
					Warrior* temp=*p2;
					if(temp->alive==false){
						delete temp;
						p2=blueWarrior.erase(p2);
					}
					else ++p2;
				}
			}else if(time%60==50){
				printf("%03d:%02d %d elements in red headquarter\n",time/60,time%60,Red.getLifeElements());
				printf("%03d:%02d %d elements in blue headquarter\n",time/60,time%60,Blue.getLifeElements());
			}else if(time%60==55){
				vector<Warrior*> temp;
				for(auto p:redWarrior) temp.push_back(p);
				for(auto p:blueWarrior) temp.push_back(p);
				sort(temp.begin(),temp.end(),[](Warrior* a,Warrior* b){
					if(a->position==b->position){
						return a->color>b->color;
					}else return a->position<b->position;
				});
				
				for(auto p:temp) p->reportCondition(time);
			}
		}
	}
	return 0;
}
