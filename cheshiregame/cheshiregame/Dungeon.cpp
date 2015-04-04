#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>



using namespace std;
/*----------Private Helper Functions----------*/
void Dungeon::fill_dungeon(const string &text_file)
{
	ifstream inFS;

	//Fill up the vector of dungoen loot first

	//open entity file
	inFS.open(text_file.c_str());

	if (!inFS.is_open())
	{
		cout << "Error: cannot open entity txt file" << endl;
		return;
	}

	string dun_name; //dungeon name
	string diff; //difficulty

	string entity_name;
	int hp, level, experience, defend, gold;
	int s, a, v, d, l; //str, agil, vit, dex, luck

	string input;
	string item_name;
	int item_val;
	//Stores dungeon name
	getline(inFS, dun_name);
	dungeon_name = dun_name;

	//Stores difficulty
	inFS >> diff;
	dungeon_difficulty = diff;

	//Push backs entity into vector until reaches end of file

	while (inFS.good())
	{
		inFS >> entity_name;
		if (entity_name == "end")
		{
			inFS.close();
		}
		else
		{
			inFS >> hp >> level >> experience >> defend >> gold;
			inFS >> s >> a >> v >> d >> l;
			Entity* temp_entity = new Entity(entity_name, hp, level, experience, defend, gold);
			temp_entity->setEntityStats(s, a, v, d, l);
			inFS >> input; //reads in if its :loot: or <end>
			if (input == ":loot:")
			{
				inFS.ignore();
				getline(inFS, item_name);
				while (item_name != "<end>")
				{
					inFS >> item_val;
					Item* temp_item = new Item(item_name, item_val);
					temp_entity->add_loot(temp_item);
					dungeon_loot.push_back(temp_item);
					inFS.ignore();
					getline(inFS, item_name);
				}
			}

			entity_group.push_back(temp_entity);

		}
	}
}

void Dungeon::clear_dun()
{
	for (int i = 0; i < dungeon_loot.size(); i++)
	{
		delete dungeon_loot.at(i);
		dungeon_loot.at(i) = 0;
	}
}
Dungeon::Dungeon(const string &text_file)
{
	fill_dungeon(text_file);
}

Dungeon::~Dungeon()
{
	for (int i = 0; i < dungeon_loot.size(); i++)
	{
		delete dungeon_loot.at(i);
		dungeon_loot.at(i) = 0;
	}
}
void Dungeon::change_dungeon(const string &text_file)
{
	entity_group.clear();
	clear_dun();
	fill_dungeon(text_file);
}

const void Dungeon::display_dungeon()
{
	cout << "Dungeon name: " << dungeon_name << endl;
	cout << "Dungeon difficulty: " << dungeon_difficulty << endl;
	cout << "Monsters: " << endl;
	for (int i = 0; i < entity_group.size(); i++)
	{
		entity_group.at(i)->Print();
		entity_group.at(i)->print_loot();
		cout << endl;
	}
}

int Dungeon::size()
{
	return entity_group.size();
}

const string Dungeon::get_dun_name()
{
	return dungeon_name;
}
Entity* Dungeon::rand_monster()
{
	int index = rand() % (entity_group.size() - 1); // -1 because boss is in last index
	return entity_group.at(index);
}

Entity* Dungeon::get_boss()
{
	return entity_group.at(entity_group.size() - 1);
}