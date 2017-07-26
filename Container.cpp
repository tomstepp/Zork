#include "Container.h"

Container::Container(xml_node<> * root){
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
		else if (s == "accept"){
			accept.insert(node->value());
		}
		else if (s == "item"){
			items.insert(node->value());
		}
		else if (s == "trigger"){
			Trigger *t = new Trigger(node);
			//triggers.insert(t);
			triggers.push_back(t);
		}
	}
	open = false;
}

Container::~Container(){
}
