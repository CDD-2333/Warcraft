#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;

#define epsilon 1e-9
const string warrior_names[5]={"dragon","ninja","iceman","lion","wolf"},weapon_names[3]={"sword","bomb","arrow"};
const int order_of_red[5]={2,3,4,1,0},order_of_blue[5]={3,0,1,2,4};
int battleVictory[25][2];//dimension1-citynumber,dimension2-0red,1blue
int cityFlag[25];//0-none,1-red,2-blue
int cityAttribute[25];//0/1/2 stands for types of weapons in city battle,to tackle the settlement in 40
bool war_over=false;

class Weapon{//To store weapons of warriors
	public:
		int type;
		int attribute;
		Weapon(int type,int dur):type(type),attribute(dur){}
};

class Warrior{//To store warriors
	public:
		string color;//headquarter of warrior:red/blue
		int id;//number in the headquarter it belongs to
		int type;//01234-dragon/ninja/iceman/lion/wolf
		int pastLifeElement;
		int idOfClass;//id in the class it belongs to
		int remainLifeElement;//hp remaining 
		int attack;//warrior's attack
		int position;//number of present city warrior in
		int headquarterLifeElement;//used when lions are born
		int totalCity;//to judge whether arrived in headquarter of opponent
		bool alive;//to judge the result of battle
		bool counterAttack;
		bool reportMove;
		bool isShotDead;
		bool isBombDead;
		vector<Weapon> weaponList;//to store the weapons
		
		Warrior(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):color(color),id(id),type(type),pastLifeElement(strength),idOfClass(idOfClass),remainLifeElement(remainLifeElement),attack(attack),position(position),alive(true),headquarterLifeElement(headquarterLifeElement),totalCity(totalCity),reportMove(true),counterAttack(true),isBombDead(false),isShotDead(false){}
	
		virtual void born(int time){
			printf("%03d:%02d %s %s %d born\n",time/60,time%60,color.c_str(),warrior_names[type].c_str(),id);
		}
		
		virtual void yell(int time){}//characteristic of dragon

		virtual void sortWeaponsForReport(){
			sort(weaponList.begin(),weaponList.end(),[](Weapon& a,Weapon& b){
				if(a.type==b.type){
					return a.attribute>b.attribute;
				}else return a.type>b.type;
			});
		}
		
		virtual void move(int step){//char of iceman
			if(color=="red") position+=step;
			else position-=step;
		} 
		
		virtual void decreaseLoyalty(int k){}//char of lion
		
		virtual bool runAway(){return 0;}//char of lion
		
		virtual void reportWeapon(int time){//to report weapons and lifeelements
			if(weaponList.empty()){
				printf("%03d:%02d %s %s %d has no weapon\n",time/60,time%60,color.c_str(),warrior_names[type].c_str(),id);	
				return;
			}
			printf("%03d:%02d %s %s %d has ",time/60,time%60,color.c_str(),warrior_names[type].c_str(),id);
			sortWeaponsForReport();
			auto it=weaponList.begin();
			while(it!=weaponList.end()-1){
				printf("%s",weapon_names[it->type].c_str());
				switch(it->type){
					case 2:printf("(%d),",it->attribute);break;
					case 0:printf("(%d),",it->attribute);break;
					case 1:printf(",");
				}
				++it;
			}
			printf("%s",weapon_names[it->type].c_str());
			switch(it->type){
				case 2:printf("(%d)\n",it->attribute);break;
				case 0:printf("(%d)\n",it->attribute);break;
				case 1:printf("\n");
			}
		}

		virtual void changeMorale(double delta){}

		virtual double getMorale(){return 0;}
		
		virtual void claimWeapon(Warrior* opponent){}//used when lion wins
};

class Dragon:public Warrior{//Derived from Warrior,derived classes below are similiar
	double morale;
	public:
		Dragon(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity),morale(double(headquarterLifeElement)/double(remainLifeElement)){}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:if(attack*2/10>0) weaponList.push_back(Weapon(0,attack*2/10));break;
				case 1:weaponList.push_back(Weapon(1,1));break;
				case 2:weaponList.push_back(Weapon(2,3));
			}
			cout<<fixed<<setprecision(2)<<"Its morale is "<<morale<<endl;
			cout.unsetf(ios::fixed);
			//printf("Its morale is %.2f\n",morale);
		}
		
		void yell(int time){//used when dragon gain a victory
			if(morale>0.8-epsilon) printf("%03d:%02d %s dragon %d yelled in city %d\n",time/60,time%60,color.c_str(),id,position);
		}

		void changeMorale(double delta){morale+=delta;}

		double getMorale(){return morale;}
};

class Ninja:public Warrior{
	public:
		Ninja(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity){counterAttack=false;}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:if(attack*2/10>0) weaponList.push_back(Weapon(0,attack*2/10));break;
				case 1:weaponList.push_back(Weapon(1,1));break;
				case 2:weaponList.push_back(Weapon(2,3));
			}
			switch((id+1)%3){
				case 0:if(attack*2/10>0) weaponList.push_back(Weapon(0,attack*2/10));break;
				case 1:weaponList.push_back(Weapon(1,1));break;
				case 2:weaponList.push_back(Weapon(2,3));
			}
		}
};

class Iceman:public Warrior{
	bool decreaseHP;
	public:
		Iceman(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity),decreaseHP(false){}
		
		void born(int time){
			Warrior::born(time);
			switch(id%3){
				case 0:if(attack*2/10>0) weaponList.push_back(Weapon(0,attack*2/10));break;
				case 1:weaponList.push_back(Weapon(1,1));break;
				case 2:weaponList.push_back(Weapon(2,3));
			}
		}
		
		void move(int step){//every two moves of iceman will deduct its lifeelement
			if(color=="red") position+=step;
			else position-=step;
			if(decreaseHP){
				remainLifeElement-=9;
				attack+=20;
				if(remainLifeElement<=0) remainLifeElement=1;
			}
			decreaseHP=!decreaseHP;
		}
};

class Lion:public Warrior{
	public:
		int loyalty;
		Lion(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity),loyalty(headquarterLifeElement){}
		
		void born(int time){
			Warrior::born(time);
			printf("Its loyalty is %d\n",loyalty);
		}
		
		bool runAway(){return loyalty<=0;}//when loyalty is <=0 lion will escape
		
		void move(int step){//every move of lion will deduct its loyalty
			if(color=="red") position+=step;
			else position-=step;
		}

		void decreaseLoyalty(int k){loyalty-=k;}
};

class Wolf:public Warrior{
	public:
		Wolf(string color,int id,int type,int strength,int idOfClass,int remainLifeElement,int attack,int position,int headquarterLifeElement,int totalCity):Warrior(color,id,type,strength,idOfClass,remainLifeElement,attack,position,headquarterLifeElement,totalCity){}
		
		virtual void claimWeapon(Warrior* opponent){//used when battle ends up in victory
			if(opponent->weaponList.size()==0) return;
			bool weaponType[3]={0};
			for(auto w:weaponList) weaponType[w.type]=true;
			
			vector<Weapon> temp;
			opponent->sortWeaponsForReport();//sort rule:sword-bomb-arrow(unused-used)
			
			auto p=opponent->weaponList.begin();
			while(p!=opponent->weaponList.end()){
				if(!weaponType[p->type]){
					temp.push_back(*p);
					weaponType[p->type]=true;
				}
				p=opponent->weaponList.erase(p);
			}//iterate opponent's weaponlist to claim weapons
			
			for(auto p:temp){weaponList.push_back(p);}
		}
};

class Headquarter{
	private:
		string color;//side of HQ
		int lifeElements;//remaining hp in HQ
		int orderIndex;//the index of warriors need to be produced right now
		const int *warrior_order;//warrior's produce order
		int warriorCount[5];//count of each kind of warrior
		int totalWarriors;//sum of warriors
		int bornStrength[5]; //born hp of warriors
		int bornAttack[5];//born attack of warriors
		int nextId;
		int totalCity;
		int enemyAmount;
	public:
		Headquarter(const string& color,int lifeElements,const int * warrior_order,int strength[],int attack[],int totalCity):color(color),lifeElements(lifeElements),orderIndex(0),warrior_order(warrior_order),totalWarriors(0),nextId(1),totalCity(totalCity),enemyAmount(0){
			for(int i=0;i<5;i++){
				warriorCount[i]=0;
				bornStrength[i]=strength[i];
				bornAttack[i]=attack[i];
			}
		}
		
		Warrior* makeWarrior(){//construct new pointers of warriors and store it in vector
			int type=warrior_order[orderIndex];
			if(lifeElements>=bornStrength[type]){
				lifeElements-=bornStrength[type];
				int id=++totalWarriors;
				int idOfClass=++warriorCount[type];
				int strength=bornStrength[type];
				int remainLifeElements=lifeElements;
				int attack=bornAttack[type];
				orderIndex=(orderIndex+1)%5;//intitalize characteristics of newly born warrior
					
				switch(type){
					case 0:return new Dragon(color,id,type,strength,idOfClass,bornStrength[type],attack,color=="red"?0:totalCity+1,remainLifeElements,totalCity);
					case 1:return new Ninja(color,id,type,strength,idOfClass,bornStrength[type],attack,color=="red"?0:totalCity+1,remainLifeElements,totalCity);
					case 2:return new Iceman(color,id,type,strength,idOfClass,bornStrength[type],attack,color=="red"?0:totalCity+1,remainLifeElements,totalCity);
					case 3:return new Lion(color,id,type,strength,idOfClass,bornStrength[type],attack,color=="red"?0:totalCity+1,remainLifeElements,totalCity);
					case 4:return new Wolf(color,id,type,strength,idOfClass,bornStrength[type],attack,color=="red"?0:totalCity+1,remainLifeElements,totalCity);
				}//construct object of derive class 
			}
			return nullptr;
		}
		
		int getLifeElements(){return lifeElements;}//lifeElements is private
		void changeEnemyAmount(){enemyAmount++;}
		void increaseLifeElement(int k){lifeElements+=k;}
		void checkTaken(int time){
			if(enemyAmount==2){
				printf("%03d:%02d %s headquarter was taken\n",time/60,time%60,color.c_str());
				war_over=true;
			}
		}
};

vector<Warrior*> redWarrior;
vector<Warrior*> blueWarrior;
int m,n,r,k,t,strength[5],attack[5],cityLifeElement[25];
Headquarter Red("red",0,order_of_red,strength,attack,0);
Headquarter Blue("blue",0,order_of_blue,strength,attack,0);//global variables,initialized at the beginning of cases

void sortWarriorPtrvec(vector<Warrior*>& temp){
	sort(temp.begin(),temp.end(),[](Warrior* a,Warrior* b){
		if(a->position==b->position){
			return a->color>b->color;
		}else return a->position<b->position;
	});
}

void CaseInitialize(){//initialize the status at the beginning of every case
	for(auto& p:redWarrior) if(p) delete p;
	for(auto& p:blueWarrior) if(p) delete p;
	redWarrior.clear();
	blueWarrior.clear();//clear all the vectors to isolate cases
	memset(cityLifeElement,0,sizeof(cityLifeElement));
	memset(cityFlag,0,sizeof(cityFlag));
	memset(battleVictory,0,sizeof(battleVictory));
		
	cin>>m>>n>>r>>k>>t;//n-cities/k-loyalty decrease of every move/t-final timestamp
	for(int i=0;i<5;i++) cin>>strength[i];
	for(int i=0;i<5;i++) cin>>attack[i];
	
	Headquarter Newred("red",m,order_of_red,strength,attack,n);
	Headquarter Newblue("blue",m,order_of_blue,strength,attack,n);
	Red=Newred,Blue=Newblue;//update the HQ
	war_over=false;
} 

void ProduceWarriors(Headquarter& a,Headquarter& b,int time){//HQs produce warriors
	Warrior* rtemp=Red.makeWarrior();
	if(rtemp){
		redWarrior.push_back(rtemp);
		rtemp->born(time);//born warriors and store the ptr into vec
	}

	Warrior* btemp=Blue.makeWarrior();
	if(btemp){
		blueWarrior.push_back(btemp);
		btemp->born(time);
	}
}

void LionEscape(Headquarter& a,Headquarter& b,int time){//judge all the lions needed to escape 
	sortWarriorPtrvec(redWarrior);
	sortWarriorPtrvec(blueWarrior);

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
	}//lions arrived in opponent's HQ doesn't need to escape
}

void WarriorsMove(Headquarter& a,Headquarter& b,int time){//deal with all the moves of warriors
	vector<Warrior*> movedWarriors;
	for(auto& p:redWarrior){
		if(p&&p->reportMove&&p->alive){
			p->move(1);
			movedWarriors.push_back(p);
		}
	}
	for(auto& p:blueWarrior){
		if(p&&p->reportMove&&p->alive){
			p->move(1);
			movedWarriors.push_back(p);
		}
	}//while moving,caution that iceman may die-update its status
	
	sortWarriorPtrvec(movedWarriors);//sort rule:position-color(red-blue)
				
	for(auto& p:movedWarriors){
		const char* head=(p->color=="red")?"red":"blue";
		const char* enemyhead=(p->color=="red")?"blue":"red";
		if(p->position>=1&&p->position<=n){
			printf("%03d:%02d %s %s %d marched to city %d with %d elements and force %d\n",time/60,time%60,p->color.c_str(),warrior_names[p->type].c_str(),p->id,p->position,p->remainLifeElement,p->attack);
		}else{
			printf("%03d:%02d %s %s %d reached %s headquarter with %d elements and force %d\n",time/60,time%60,p->color.c_str(),warrior_names[p->type].c_str(),p->id,enemyhead,p->remainLifeElement,p->attack);
			Warrior* w=NULL;//specially judge the arrival in opponent's HQ
			if(p->color=="red"){
				for(auto* ww:redWarrior)
					if(ww->id==p->id&&ww->type==p->type){w=ww;break;}
			}else{
				for(auto* ww:blueWarrior)
					if(ww->id==p->id&&ww->type==p->type){w=ww;break;}
			}
			if(w&&w->alive){
				if(w->color=="red"){
					Blue.changeEnemyAmount();
					Blue.checkTaken(time);
				}
				if(w->color=="blue"){
					Red.changeEnemyAmount();
					Red.checkTaken(time);
				}
			}
		}
	}
				
	auto change_arrival=[](vector<Warrior*>& warriors){
		auto it=warriors.begin();
		while(it!=warriors.end()){
			Warrior* w=*it;
			if((w->color=="red"&&w->position==n+1)||(w->color=="blue"&&w->position==0)){
				w->reportMove=false;
			}++it;
		}
	};//check if any warrior arrived in opponent's HQ in the move this round and delete their ptrs
	change_arrival(redWarrior);
	change_arrival(blueWarrior);
}

void GenerateLifeElement(int time){
	for(auto& p:cityLifeElement) p+=10;
	cityLifeElement[0]=cityLifeElement[n+1]=0;
}

void WarriorsGetLifeElement(Headquarter& a,Headquarter& b,int time){
	vector<Warrior*> temp;

	sort(redWarrior.begin(),redWarrior.end(),[](Warrior* a,Warrior* b){return a->position<b->position;});
	for(auto p:redWarrior){
		int pos=p->position;
		if(pos==0||pos==n+1) continue;
		bool flag=true;
		for(auto pp:blueWarrior){
			if(pp->position==p->position){
				flag=false;
				break;
			}
		}
		if(flag){
			a.increaseLifeElement(cityLifeElement[p->position]);
			temp.push_back(p);
		}
	}

	sort(blueWarrior.begin(),blueWarrior.end(),[](Warrior* a,Warrior* b){return a->position<b->position;});
	for(auto p:blueWarrior){
		int pos=p->position;
		if(pos==0||pos==n+1) continue;
		bool flag=true;
		for(auto pp:redWarrior){
			if(pp->position==p->position){
				flag=false;
				break;
			}
		}
		if(flag){
			b.increaseLifeElement(cityLifeElement[p->position]);
			temp.push_back(p);
		}
	}

	sortWarriorPtrvec(temp);
	for(auto p:temp){
		printf("%03d:%02d %s %s %d earned %d elements for his headquarter\n",time/60,time%60,p->color.c_str(),warrior_names[p->type].c_str(),p->id,cityLifeElement[p->position]);
		cityLifeElement[p->position]=0;
	}
}

void WarriorsShootArrow(Headquarter& a,Headquarter& b,int time){
	vector<pair<Warrior*,Warrior*> > shotPairs;

	for(auto p:redWarrior){
		if(p->position==n) continue;
		bool hasArrow=false;
		for(auto w:p->weaponList){
			if(w.type==2){
				hasArrow=true;
				break;
			}
		}
		if(!hasArrow) continue;
		for(auto pp:blueWarrior){
			if(pp->position==p->position+1){
				shotPairs.emplace_back(p,pp);
				break;
			}
		}
	}

	for(auto p:blueWarrior){
		if(p->position==1) continue;
		bool hasArrow=false;
		for(auto w:p->weaponList){
			if(w.type==2){
				hasArrow=true;
				break;
			}
		}
		if(!hasArrow) continue;
		for(auto pp:redWarrior){
			if(pp->position==p->position-1){
				shotPairs.emplace_back(p,pp);
				break;
			}
		}
	}

	sort(shotPairs.begin(),shotPairs.end(),[](pair<Warrior*,Warrior*>& a,pair<Warrior*,Warrior*>& b){
		if(a.first->position==b.first->position){
			return a.first->color>b.first->color;
		}else return a.first->position<b.first->position;
	});

	for(auto& p:shotPairs){
		Warrior* attacker=p.first;
		Warrior* victim=p.second;
		attacker->sortWeaponsForReport();
		auto it=attacker->weaponList.begin();
		while(it!=attacker->weaponList.end()&&it->type!=2) ++it;
		if(it!=attacker->weaponList.end()){
			victim->remainLifeElement-=r;
			it->attribute--;
		}
	}

	for(auto& p:shotPairs){
		Warrior* attacker=p.first;
		Warrior* victim=p.second;

		attacker->sortWeaponsForReport();
		auto it=attacker->weaponList.begin();
		while(it!=attacker->weaponList.end()&&it->type!=2) ++it;
		if(it!=attacker->weaponList.end()&&it->attribute==0){
    		attacker->weaponList.erase(it);
		}

		if(attacker->color=="red"){
			printf("%03d:%02d red %s %d shot",time/60,time%60,warrior_names[attacker->type].c_str(),attacker->id);
		}else{
			printf("%03d:%02d blue %s %d shot",time/60,time%60,warrior_names[attacker->type].c_str(),attacker->id);
		}
		if(victim->remainLifeElement<=0){
			victim->alive=false;
			victim->isShotDead=true;
			printf(" and killed %s %s %d\n",victim->color.c_str(),warrior_names[victim->type].c_str(),victim->id);
		}else printf("\n");
	}
}

void WarriorsUseBomb(Headquarter& a,Headquarter& b,int time){
	vector<pair<Warrior*,Warrior*> > bombPairs;

	for(auto p:redWarrior){
		if(p->position==n+1||p->position==0) continue;
		for(auto pp:blueWarrior){
			if(pp->position==p->position){
				bombPairs.emplace_back(p,pp);
				break;
			}
		}
	}

	sort(bombPairs.begin(),bombPairs.end(),[](pair<Warrior*,Warrior*>& a,pair<Warrior*,Warrior*>& b){return a.first->position<b.first->position;});

	for(auto p:bombPairs){
		if(!p.first->alive||!p.second->alive) continue;
		bool skip=true;
		for(auto w:p.first->weaponList){
			if(w.type==1) skip=false;
		}
		for(auto w:p.second->weaponList){
			if(w.type==1) skip=false;
		}
		if(skip) continue;

		bool attackOrder;//true-red,false-blue
		if(cityFlag[p.first->position]==1){
			attackOrder=true;
		}else if(cityFlag[p.first->position]==2){
			attackOrder=false;
		}else{
			if(p.first->position%2==0){
				attackOrder=false;
			}else attackOrder=true;
		}

		Warrior* attacker=attackOrder?p.first:p.second;
		Warrior* defender=attackOrder?p.second:p.first;
		int attackerHarm=attacker->attack,defenderHarm=defender->attack/2;
		attacker->sortWeaponsForReport();
		defender->sortWeaponsForReport();
		bool attackerHasBomb=false,defenderHasBomb=false;
		for(auto w:attacker->weaponList){
			if(w.type==0){
				attackerHarm+=w.attribute;
				break;
			}
			if(w.type==1) attackerHasBomb=true;
		}
		for(auto w:defender->weaponList){
			if(w.type==0){
				defenderHarm+=w.attribute;
				break;
			}
			if(w.type==1) defenderHasBomb=true;
		}
		if(defender->type==1) defenderHarm=0;

		bool attackerUseBomb=false,defenderUseBomb=false;
		if(attackerHarm>=defender->remainLifeElement&&defenderHasBomb) defenderUseBomb=true;
		if(attackerHarm<defender->remainLifeElement&&defenderHarm>=attacker->remainLifeElement&&attackerHasBomb) attackerUseBomb=true;

		if(defenderUseBomb){
			attacker->isBombDead=defender->isBombDead=true;
			attacker->alive=false;
			defender->alive=false;
			printf("%03d:%02d %s %s %d used a bomb and killed %s %s %d\n",time/60,time%60,defender->color.c_str(),warrior_names[defender->type].c_str(),defender->id,attacker->color.c_str(),warrior_names[attacker->type].c_str(),attacker->id);
		}else if(attackerUseBomb){
			attacker->isBombDead=defender->isBombDead=true;
			attacker->alive=false;
			defender->alive=false;
			printf("%03d:%02d %s %s %d used a bomb and killed %s %s %d\n",time/60,time%60,attacker->color.c_str(),warrior_names[attacker->type].c_str(),attacker->id,defender->color.c_str(),warrior_names[defender->type].c_str(),defender->id);
		}
	}
}

void WarriorsBattle(Headquarter& a,Headquarter& b,int time){
	vector<pair<Warrior*,Warrior*> > battlePairs;
	vector<pair<Warrior*,int> > victoryWarriors;

	for(auto p:redWarrior){
		if(p->position==n+1||p->position==0) continue;
		for(auto pp:blueWarrior){
			if(pp->position==p->position){
				battlePairs.emplace_back(p,pp);
				break;
			}
		}
	}

	sort(battlePairs.begin(),battlePairs.end(),[](pair<Warrior*,Warrior*>& a,pair<Warrior*,Warrior*>& b){return a.first->position<b.first->position;});

	for(auto p:battlePairs){
		bool Shotskip=false;
		string result="not";
		bool attackOrder;//true-red,false-blue

		if(cityFlag[p.first->position]==1){
			attackOrder=true;
		}else if(cityFlag[p.first->position]==2){
			attackOrder=false;
		}else{
			if(p.first->position%2==0){
				attackOrder=false;
			}else attackOrder=true;
		}

		Warrior* attacker=attackOrder?p.first:p.second;
		Warrior* defender=attackOrder?p.second:p.first;
		attacker->sortWeaponsForReport();
		defender->sortWeaponsForReport();

		if(defender->isShotDead&&attacker->isShotDead) continue;
		if(defender->isShotDead){
			Shotskip=true;
			result=attacker->color;
		}
		if(attacker->isShotDead){
			Shotskip=true;
			result=defender->color;
		}
		if(attacker->isBombDead||defender->isBombDead) continue;
		attacker->pastLifeElement=attacker->remainLifeElement>0?attacker->remainLifeElement:0;
		defender->pastLifeElement=defender->remainLifeElement>0?defender->remainLifeElement:0;

		if(!Shotskip){
			int attackerHarm=attacker->attack,defenderHarm=defender->attack/2; 
			for(auto& w:attacker->weaponList){
				if(w.type==0){
					attackerHarm+=w.attribute;
					w.attribute=w.attribute*8/10;
					break;
				}
			}
			for(auto& w:defender->weaponList){
				if(w.type==0){
					defenderHarm+=w.attribute;
					break;
				}
			}
			if(!defender->counterAttack) defenderHarm=0;
			auto it=attacker->weaponList.begin();
			while(it!=attacker->weaponList.end()){
				if(it->attribute==0) it=attacker->weaponList.erase(it);
				else ++it;
			}

			defender->remainLifeElement-=attackerHarm;
			printf("%03d:%02d %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n",time/60,time%60,attacker->color.c_str(),warrior_names[attacker->type].c_str(),attacker->id,defender->color.c_str(),warrior_names[defender->type].c_str(),defender->id,attacker->position,attacker->remainLifeElement,attacker->attack);
			if(defender->remainLifeElement<=0){
				defender->alive=false;
			}
			if(defender->alive&&defender->type!=1){
				attacker->remainLifeElement-=defenderHarm;
				for(auto& w:defender->weaponList){
					if(w.type==0){
						w.attribute=w.attribute*8/10;
						break;
					}
				}
				auto it=defender->weaponList.begin();
				while(it!=defender->weaponList.end()){
					if(it->attribute==0) it=defender->weaponList.erase(it);
					else ++it;
				}
				printf("%03d:%02d %s %s %d fought back against %s %s %d in city %d\n",time/60,time%60,defender->color.c_str(),warrior_names[defender->type].c_str(),defender->id,attacker->color.c_str(),warrior_names[attacker->type].c_str(),attacker->id,attacker->position);
			}
			if(attacker->remainLifeElement<=0){
				attacker->alive=false;
			}

			vector<Warrior*> temp;
			if(!attacker->alive&&!attacker->isShotDead) temp.push_back(attacker);
			if(!defender->alive&&!defender->isShotDead) temp.push_back(defender);
			sort(temp.begin(),temp.end(),[](Warrior* a,Warrior* b){return a->color>b->color;});
			for(auto p:temp) printf("%03d:%02d %s %s %d was killed in city %d\n",time/60,time%60,p->color.c_str(),warrior_names[p->type].c_str(),p->id,p->position);

			if(((!attacker->alive&&defender->alive)||(!defender->alive&&attacker->alive))&&(!attacker->isShotDead&&!defender->isShotDead)){result=attacker->alive?attacker->color:defender->color;}
			if(attacker->alive&&defender->alive&&!attacker->isShotDead&&!defender->isShotDead) result="tie";
		}

		int side=result=="red"?0:1;
		if(result==attacker->color){
			attacker->claimWeapon(defender);
			if(defender->type==3) attacker->remainLifeElement+=defender->pastLifeElement;
			attacker->changeMorale(0.2);
			victoryWarriors.emplace_back(attacker,cityLifeElement[attacker->position]);
			attacker->yell(time);
			printf("%03d:%02d %s %s %d earned %d elements for his headquarter\n",time/60,time%60,attacker->color.c_str(),warrior_names[attacker->type].c_str(),attacker->id,cityLifeElement[attacker->position]);
			cityLifeElement[attacker->position]=0;
			battleVictory[attacker->position][1-side]=0;
			if(battleVictory[attacker->position][side]<2) battleVictory[attacker->position][side]++;
			if(battleVictory[attacker->position][side]==2&&cityFlag[attacker->position]!=side+1){
				cityFlag[attacker->position]=side+1;
				printf("%03d:%02d %s flag raised in city %d\n",time/60,time%60,attacker->color.c_str(),attacker->position);
			} 
		}
		if(result==defender->color){
			defender->claimWeapon(attacker);
			if(attacker->type==3) defender->remainLifeElement+=attacker->pastLifeElement;
			defender->changeMorale(0.2);
			victoryWarriors.emplace_back(defender,cityLifeElement[defender->position]);
			printf("%03d:%02d %s %s %d earned %d elements for his headquarter\n",time/60,time%60,defender->color.c_str(),warrior_names[defender->type].c_str(),defender->id,cityLifeElement[defender->position]);
			cityLifeElement[defender->position]=0;
			battleVictory[defender->position][1-side]=0;
			if(battleVictory[defender->position][side]<2) battleVictory[defender->position][side]++;
			if(battleVictory[defender->position][side]==2&&cityFlag[defender->position]!=side+1){
				cityFlag[defender->position]=side+1;
				printf("%03d:%02d %s flag raised in city %d\n",time/60,time%60,defender->color.c_str(),attacker->position);
			} 
		}
		if(result=="tie"){
			attacker->changeMorale(-0.2);
			defender->changeMorale(-0.2);
			attacker->decreaseLoyalty(k);
			defender->decreaseLoyalty(k);
			attacker->yell(time);
		}
		if(result=="not"){
			attacker->decreaseLoyalty(k);
			defender->decreaseLoyalty(k);
		}
		if(result=="tie"||result=="not"){
			for(int i=0;i<2;i++) battleVictory[attacker->position][i]=0;
		}
	}

	sort(victoryWarriors.begin(),victoryWarriors.end(),[](pair<Warrior*,int> a,pair<Warrior*,int>b){
		if(a.first->color==b.first->color){
			if(a.first->color=="red") return a.first->position>b.first->position;
			else return a.first->position<b.first->position;
		}else return a.first->color>b.first->color;
	});
	for(auto& p:victoryWarriors){
		if(p.first->color=="red"){
			if(a.getLifeElements()>=8){
				a.increaseLifeElement(-8);
				p.first->remainLifeElement+=8;
			}
		}
		if(p.first->color=="blue"){
			if(b.getLifeElements()>=8){
				b.increaseLifeElement(-8);
				p.first->remainLifeElement+=8;
			}
		}
	}
	
	sort(victoryWarriors.begin(),victoryWarriors.end(),[](pair<Warrior*,int> a,pair<Warrior*,int>b){return a.first->position<b.first->position;});
	for(auto p:victoryWarriors){
		if(p.first->color=="red") a.increaseLifeElement(p.second);
		if(p.first->color=="blue") b.increaseLifeElement(p.second);
	}

	auto remove_dead=[](vector<Warrior*>& w){
		auto it=w.begin();
		while(it!=w.end()){
			Warrior* warrior=*it;
			if(warrior->alive==false) it=w.erase(it);
			else ++it;
		}
	};
	remove_dead(redWarrior);
	remove_dead(blueWarrior);
}

void ReportLifeElements(Headquarter& a,Headquarter& b,int time){
	printf("%03d:%02d %d elements in red headquarter\n",time/60,time%60,a.getLifeElements());
	printf("%03d:%02d %d elements in blue headquarter\n",time/60,time%60,b.getLifeElements());
}

void WarriorsReportWeaponStatus(Headquarter& a,Headquarter& b,int time){//report hp and weapons of every warrior
	sortWarriorPtrvec(redWarrior);
	sortWarriorPtrvec(blueWarrior);
	for(auto p:redWarrior) p->reportWeapon(time);
	for(auto p:blueWarrior) p->reportWeapon(time);
}

int main(){//Just to mention,beauty of encapsulization.
	int T;
	cin>>T;
	for(int Case=1;Case<=T;Case++){
		CaseInitialize();
		printf("Case %d:\n",Case);
		
		for(int time=0;!war_over&&time<=t;time++){
			if(time%60==0){
				ProduceWarriors(Red,Blue,time);
			}else if(time%60==5){
				LionEscape(Red,Blue,time);
			}else if(time%60==10){
				WarriorsMove(Red,Blue,time);
			}else if(time%60==20){
				GenerateLifeElement(time);
			}else if(time%60==30){
				WarriorsGetLifeElement(Red,Blue,time);
			}else if(time%60==35){
				WarriorsShootArrow(Red,Blue,time);
			}else if(time%60==38){
				WarriorsUseBomb(Red,Blue,time);
			}else if(time%60==40){
				WarriorsBattle(Red,Blue,time);
			}else if(time%60==50){
				ReportLifeElements(Red,Blue,time);
			}else if(time%60==55){
				WarriorsReportWeaponStatus(Red,Blue,time);
			}
		}
	}
	return 0;
}
