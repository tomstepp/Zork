#include "Item.h"
//#include <iostream>

Item::Item(xml_node<>* root){
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
		else if (s == "turnon"){
			//turnon = node->value();
			print = node->first_node()->value();
			//cout << node->first_node()->value();
			for (xml_node<>* n = node->first_node()->next_sibling(); n;n=n->next_sibling()){
				actions.push_back(n->value());
			}
		}
		else if (s == "writing"){
			writing = node->value();
		}
		else if (s == "trigger"){
			Trigger *t = new Trigger(node);
			triggers.insert(t);
		}
	}
}

Item::~Item(){
}
