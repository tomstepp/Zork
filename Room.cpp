#include "Room.h"

Room::Room(xml_node<>* root){
	for (xml_node<>* node = root->first_node(); node; node=node->next_sibling()){
		string s = string(node->name());
		if (s == "name"){
			name = node->value();
		}
		else if (s == "description"){
			description = node->value();
		}
		else if (s == "status"){
			status = node->value();
		}
		else if (s == "type"){
			type = node->value();
		}
		else if (s == "border"){
			string dir;
			string room;
//			string dir = node->first_node()->value();
//			string room = node->first_node()->next_sibling()->value();
//			if (node->first_node()->name() != "direction"){
//				string tmp = dir;
//				string dir = room;
//				string room = tmp;
//			}
			if (string(node->first_node()->name()) == string("direction")){
				dir = node->first_node()->value();
				room = node->first_node()->next_sibling()->value();
			}
			else{
				dir = node->first_node()->next_sibling()->value();
				room = node->first_node()->value();
			}
			//cout << dir << endl;
			//cout << room << endl;
			borders[dir] = room;
		}
		else if (s =="container"){
			containers.insert(node->value());
		}
		else if (s == "item"){
			items.insert(node->value());
		}
		else if (s == "creature"){
			creatures.insert(node->value());
		}
		else if (s == "trigger"){
			Trigger *t = new Trigger(node);
			triggers.push_back(t);
		}
	}
}

Room::~Room(){
}

