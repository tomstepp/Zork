#include "Game.h"
Game::Game(char* input){
	setup(input);
}

Game::~Game(){
}

//functions
void Game::setup(char* input){
	file<> xmlFile(input);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	
	xml_node<> *root = doc.first_node();

	//my lists
	queue<xml_node<>*> roomsxml;
	queue<xml_node<>*> itemsxml;
	queue<xml_node<>*> contsxml;
	queue<xml_node<>*> creatxml;

	//separate xml nodes
	for (xml_node<>*node = root->first_node(); node; node=node->next_sibling()){
		string s = string(node->name());
		if(s == string("room")){
			//cout << "add xml room node" << endl;
			roomsxml.push(node);
		}
		else if (s == string("item")){
			itemsxml.push(node);
		}
		else if (s == string("container")){
			contsxml.push(node);
		}
		else if (s == string("creature")){
			creatxml.push(node);
		}
	}

	while(roomsxml.size() != 0){
		//cout << "add room" << endl;
		Room *room = new Room(roomsxml.front());
		rooms[room->name] = room;
		roomsxml.pop();
	}
	while(itemsxml.size() != 0){
		Item *item = new Item(itemsxml.front());
		items[item->name] = item;
		itemsxml.pop();
	}
	while(contsxml.size() != 0){
		Container *c = new Container(contsxml.front());
		containers[c->name] = c;
		contsxml.pop();
	}
	while(creatxml.size() != 0){
		Creature *c = new Creature(creatxml.front());
		creatures[c->name] = c;
		creatxml.pop();
	}
	return;
}

void Game::play(){
	cout << endl;
	cout << "Welcome to ZORK." << endl;
	string command;
	curr = rooms.find("Entrance");

	if (curr != rooms.end()){
		cout << curr->second->description << endl;
	}
	
	while(true){
		cout << ">>  ";
		getline(cin, command);
		string command2 = command;

		if (command == ""){
			cout << "Type something!" << endl;
		}
		if (command == "quit"){
			cout << "quitting..." << endl;
			exit(EXIT_SUCCESS);
		}
		if (!triggerCheck(command)){
			continue;
		}
		if (command == "n" || command == "s" || command == "e" || command == "w"){
			string dir;
			if (command == "n"){
				dir = "north";
			}
			else if (command == "s"){
				dir = "south";
			}
			else if (command == "e"){
				dir = "east";
			}
			else if (command == "w"){
				dir = "west";
			}
			changeRoom(dir);
		}
		else if (command == "i"){
			typedef map<string, Item*>::iterator I;
			I i=inv.begin();
			if (i != inv.end()){
				cout << "Inventory: " << i->first;
				i++;
			}
			else{
				cout << "Inventory: empty" << endl;
				continue;
			}
			while (i != inv.end()){
				cout << ", " << i->first;
				i++;
			}
			cout << endl;
		}

		else if (command == "open exit"){
			if (curr->second->type == "exit"){
				cout << "Victory!" << endl;
				return;
			}
			else{
				cout << "you are not at the exit" << endl;
			}
		}
		else if (command.find("take") == 0){
			take(command.erase(0, 5));
		}
		else if (command.find("open") == 0){
			open(command.erase(0, 5));
		}
		else if (command.find("read") == 0){
			read(command.erase(0, 5));
		}
		else if (command.find("drop") == 0){
			drop(command.erase(0, 5));
		}
		else if (command.find("put") == 0){
			put(command.erase(0, 4));
		}
		else if (command.find("turn on") == 0){
			turnon(command.erase(0, 8));
		}
		else if (command.find("attack") == 0){
			attack(command.erase(0, 7));
		}
		else{
			cout << "I do not recognize that." << endl;
		}
		creatureTrigger(command2);
	}
	return;
}

bool Game::triggerCheck(string in){
	//check room
	if (in=="n" || in=="s" || in=="e" || in=="w"){
		for (unsigned int i=0; i < curr->second->triggers.size(); i++){
			string obj = curr->second->triggers[i]->object;
			string has = curr->second->triggers[i]->has;
			string own = curr->second->triggers[i]->owner;
			string print = curr->second->triggers[i]->print;
			string com = curr->second->triggers[i]->command;
			string status = curr->second->triggers[i]->status;
			if (com==in){
				if (own=="inventory" && inv.find(obj)==inv.end() && has=="no"){
					cout << print << endl;
					return false;
				}
				if (status!= "" && containers[obj]->status == status){
					cout << print << endl;
					return false;
				}
			}
		}
	}

	//check container
	if (in.find("open") == 0){
		string container = in.erase(0, 5);
		Container * c = containers[container];
		if (c==NULL){
			return true;
		}
		for (unsigned int i=0; i < c->triggers.size(); i++){
			//change container triggers to vector!!!
			string obj = c->triggers[i]->object;
			string has = c->triggers[i]->has;
			string own = c->triggers[i]->owner;
			string print = c->triggers[i]->print;
			if (containers[container]->items.find(obj)!=containers[container]->items.end() && has=="yes"){
				cout << print << endl;
				//perform actions
				for (unsigned int j=0; j< c->triggers[i]->actions.size(); j++){
					performAction(c->triggers[i]->actions[j]);
				}
				return true;

			}
			else {
				cout << "Can't open that...yet." << endl;
				return false;
			}
		}
	}
	return true;
}
void Game::creatureTrigger(string in){
	//check triggers of all creatures in room
	//cout << "cT" << endl;
	//cout << in << endl;
	typedef set<string>::iterator I;
	for (I i = curr->second->creatures.begin(); i!=curr->second->creatures.end(); i++){
		Creature *c = creatures[*i];
		for (unsigned int j = 0; j != c->triggers.size(); j++){
			//check condition
			Trigger *t = c->triggers[j];
			//cout << t->actions.size() << endl;
			if (t->command != "" && t->command != in){
				return;
			}
			//cout << "oi" << endl;
			if (t->owner!= "" && t->owner=="inventory"){
				if (inv.find(t->object)!=inv.end() && t->has=="yes"){
					cout << c->triggers[j]->print << endl;
					for (unsigned int k=0; k< c->triggers[k]->actions.size(); k++){
						performAction(c->triggers[j]->actions[k]);
					}
					return;
				}
				else{return;}
			}
			//cout << c->triggers[j]->object << endl;
			//cout << inv[c->triggers[j]->object]->status << endl;
			//cout << c->triggers[j]->status << endl;
			if (inv.find(c->triggers[j]->object)!=inv.end() && inv[c->triggers[j]->object]->status == c->triggers[j]->status){
				cout << c->triggers[j]->print << endl;
				//cout << "hello" << endl;
				for (unsigned int k=0; k< c->triggers[j]->actions.size(); k++){
					//cout << c->triggers[j]->actions[k] << endl;
					performAction(c->triggers[j]->actions[k]);
				}
				if (c->triggers[j]->type == "single"){
					c->triggers.erase(c->triggers.begin()+j);
					return;
				}
				return;
			}

			if (items.find(c->triggers[j]->object)!=items.end() && items[c->triggers[j]->object]->status == c->triggers[j]->status){
				cout << c->triggers[j]->print << endl;
				for (unsigned int k=0; k< c->triggers[j]->actions.size(); k++){
					performAction(c->triggers[j]->actions[k]);
				}
				if (c->triggers[j]->type == "single"){
					c->triggers.erase(c->triggers.begin()+j);
					return;
				}
				return;
			}
		}
	}
}

void Game::performAction(string action){
	if (action.find("Add") == 0){
		add(action.erase(0, 4));
	}
	else if (action.find("Delete") == 0){
		del(action.erase(0, 7));
	}
	else if (action.find("Update") == 0){
		//cout << "update" << endl;
		update(action.erase(0, 7));
	}
	else if (action == "n"){
		changeRoom("north");
	}
	else if (action == "s"){
		changeRoom("south");
	}
	else if (action == "w"){
		changeRoom("west");
	}
	else if (action == "e"){
		changeRoom("east");
	}
	else if (action.find("take") == 0){
		take(action.erase(0, 5));
	}
	else if (action.find("open") == 0){
		open(action.erase(0, 5));
	}
	else if (action.find("read") == 0){
		read(action.erase(0, 5));
	}
	else if (action.find("drop") == 0){
		drop(action.erase(0, 5));
	}
	else if (action.find("put") == 0){
		put(action.erase(0, 4));
	}
	else if (action.find("turn on") == 0){
		turnon(action.erase(0, 8));
	}
	else if (action.find("attack") == 0){
		attack(action.erase(0, 7));
	}
	else if (action.find("Game Over") == 0){
		cout << "Victory!" << endl;
		exit(EXIT_SUCCESS);
	}
	else{
		cout << "I do not recognize that." << endl;
	}
}
void Game::add(string action){
	int pos = action.find("to");
	string object = action.substr(0, pos-1);
	string place = action.substr(pos+3);
	if (items.find(object)!=items.end()){
		// add item
		rooms[place]->items.insert(object);
	}
	else if (creatures.find(object)!=creatures.end()){
		// add creature
		rooms[place]->creatures.insert(object);
	}
	//curr->second->items.insert(object);
}
void Game::del(string object){
	curr->second->creatures.erase(object);
	//creatures.erase(object);
}
void Game::update(string action){
	int pos = action.find("to ");
	string obj = action.substr(0, pos-1);
	string status = action.substr(pos+3);

	//update inventory item
	if (items.find(obj) != items.end()){
		items[obj]->status = status;
	}
	//update inventory item
	if (inv.find(obj) != inv.end()){
		inv[obj]->status = status;
	}
	//update room container
	if (curr->second->containers.find(obj) != curr->second->containers.end()){
		containers[obj]->status = status;
	}
}

void Game::changeRoom(string direction){
	map<string, string>::iterator b = curr->second->borders.find(direction);
	if (b != curr->second->borders.end()){
		curr = rooms.find(b->second);
		cout << curr->second->description << endl;
	}
	else{
		cout << "can't go that way." << endl;
	}
	return;
}

void Game::take(string input){
	typedef set<string>::iterator I;
	I i = curr->second->items.find(input);
	if (i != curr->second->items.end()){
		cout << "Item '" << input << "' was added to inventory." << endl;
		inv[input] = items[input];
		curr->second->items.erase(input);

		// find container with item and remove it there too
		typedef map<string, Container *>::iterator M;
		for (M m = containers.begin(); m!=containers.end(); m++){
			Container *c = containers[m->second->name];
			I j = c->items.find(input);
			if (containers[m->second->name]->open && j!=c->items.end()){
				inv[input] = items[input];
				containers[m->second->name]->items.erase(input);
			}
		}
	}
	else{
		cout << "Can't take that." << endl;
	}
	return;
}
void Game::drop(string input){
	typedef map<string, Item*>::iterator I;
	I i = inv.find(input);
	if (i != inv.end()){
		cout << "Item '" << input << "' was dropped." << endl;
		curr->second->items.insert(input);
		inv.erase(input);
	}
	else{
		cout << "Can't drop that." << endl;
	}
	return;
}
void Game::read(string input){
	typedef map<string, Item*>::iterator I;
	I i = inv.find(input);
	if (i != inv.end() && inv[input]->writing != ""){
		//if(inv[input]->writing != ""){cout << inv[input]->writing << endl;}
		cout << inv[input]->writing << endl;
	}
	else{
		cout << "Nothing written." << endl;
	}
	return;
}
void Game::turnon(string input){
	typedef map<string, Item*>::iterator I;
	I i = inv.find(input);
	if (i != inv.end()){
		cout << "You activate the '" << input << "'." << endl;
		cout << inv[input]->print << endl;
		for (unsigned int i=0; i < inv[input]->actions.size(); i++){
			//perform actions
			performAction(inv[input]->actions[i]);
			if(inv.find(input) == inv.end()){
				break;
			}
		}
	}
	else{
		cout << "You do not have this item." << endl;
	}
	return;
}
void Game::open(string input){
	typedef set<string>::iterator I;
	I i = curr->second->containers.find(input);
	if (i != curr->second->containers.end()){
		containers[input]->open = true;
		//cout << "You opened the '" << input << "' container." << endl;
		// add items to room, so you can pick them up
		/*I j = containers[input]->items.begin();
		while (j != containers[input]->items.end()){
			curr->second->items.insert(*j);
			cout << "Found " << *j << endl;
			j++;
		}*/
		I j=containers[input]->items.begin();
		if (j != containers[input]->items.end()){
			cout << containers[input]->name << " contains " << *j;
			curr->second->items.insert(*j);
			j++;
		}
		else{
			cout << containers[input]->name << " is empty" << endl;
			return;
		}
		while (j != containers[input]->items.end()){
			cout << ", " << *j;
			curr->second->items.insert(*j);
			j++;
		}
		cout << endl;
		// move to check if container has it when you remove from room !
		// containers[input]->items
	}
	else{
		cout << "Can't open that." << endl;
	}
	return;
}
void Game::put(string input){
	int pos = input.find("in");
	string item = input.substr(0,pos-1);
	string container = input.substr(pos+3);

	typedef map<string, Item*>::iterator I;
	I i = inv.find(item);

	typedef set<string>::iterator J;
	J j = curr->second->containers.find(container);

	if (i != inv.end() && j != curr->second->containers.end()){
		Container *c = containers[container];
		if (c->accept.empty() || c->accept.find(item) != c->accept.end()){
			c->items.insert(item);
			inv.erase(item);
			if (!c->accept.empty()){inv.erase(item);}
			cout << "Item '" << item << "' added to " << container << endl;
			triggerCheck("open " + container);
		}
		else{
			cout << container << " does not accept that item." << endl;
		}
	}
	else{
		cout << "Can't put item in that." << endl;
	}
	return;
}
void Game::attack(string input){
	//cout << "attack" << endl;
	int pos = input.find("with");
	string creature = input.substr(0,pos-1);
	string item = input.substr(pos+5);

	typedef map<string, Item*>::iterator I;
	I i = inv.find(item);

	typedef set<string>::iterator J;
	J j = curr->second->creatures.find(creature);

	if (i != inv.end() && j != curr->second->creatures.end()){
		// fix this - DONE
		Creature *c = creatures[creature];
		if (c->vulnerability.find(item) != c->vulnerability.end()){
			cout << "You assault the " << creature << " with " << item << endl;
			if (c->object_status == "" || (inv.find(c->object) != inv.end() && inv[c->object]->status == c->object_status)){
				if (c->print != ""){
					cout << c->print << endl;
				}
				// perform actions
				for (unsigned int i=0; i< c->actions.size(); i++){
					performAction(c->actions[i]);
				}
			}
			else if (c->object_status == "" || (items.find(c->object) != items.end() && items[c->object]->status == c->object_status)){
				cout << c->print << endl;
				// perform actions
				for (unsigned int i=0; i< c->actions.size(); i++){
					performAction(c->actions[i]);
				}
			}
			else{
				cout << "did not hurt " << creature << endl;
			}
		}
		else{
			cout << creature << " cannot be killed with " << item << endl;
		}
	}
	else if (i == inv.end()){
		cout << "you do not have that item" << endl;
	}
	else if (j == curr->second->creatures.end()){
		cout << "that creature is not present" << endl;
	}
	else{
		cout << "Can't attack that." << endl;
	}
	return;
}
