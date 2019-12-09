#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <cstdlib>
#include <omp.h>
#include <unordered_map>
#include "Page.h"

class Graph
{

private:
	std::unordered_map<int, Page*> *complete_list;
	int num_of_pages;

public:
	Graph()
	{
		this->complete_list = new std::unordered_map<int, Page*>();
		this->num_of_pages = 0;
	}

	~Graph()
	{
		// Messing with pointers will probably have to free some of them eventually
	}

	int get_num_of_pages()
	{
		return this->num_of_pages;
	}

	Page * get_page_at_position(int index)
	{
		return this->complete_list->at(index);
	}

	Page *return_complete_list(int temp)
	{
		return this->complete_list->at(temp);
	}
	std::unordered_map<int, Page *> get_list()
	{
		return *this->complete_list;
	}

	void increment_page()
	{
		this->num_of_pages++;
	}

	/*
			Sees if node already exist
			Return:
					true if node is new
					false if node already exist
	*/
	int find_existing_page(int id)
	{
		if (id >= this->num_of_pages)
		{
			return 0;
		}

		else
		{
			return 1;
		}
	}


	/*
	// Returns the placement of node
	int position_of_node(int id)
	{
		for (int index = 0; index < this->num_of_pages; index++)
		{
			if (id == this->complete_list[index])
			{
				return index;
			}
		}
	}
	*/
	/*
			Adding a page to the existing network
			Return:
					true if successfully added
					false if some error occurred
	*/
	void insert_page(int coming_from, int going_to)
	{
		 if (this->complete_list->count(coming_from) == 0)
		{
			Page* new_node = new Page(coming_from);
			new_node->set_outgoing_link(going_to);
			this->complete_list->insert({ coming_from,new_node });
			this->increment_page();

		}
		 if (this->complete_list->count(going_to) == 0)
		 {
			 Page* new_node = new Page(going_to);
			 this->complete_list->insert({ going_to, new_node });
			 this->increment_page();
		 }
		else
		{
			this->complete_list->at(coming_from)->set_outgoing_link(going_to);
		}
	}
};

// Parses file and creates the appropriate represented network
void create_network(Graph*& network, std::ifstream& input_file)
{
	int parent_id = 0;
	int child_id = 0;

	std::string current_line;
	int counter = 0;
	while (std::getline(input_file, current_line))
	{
		// Ignore all lines that are comments
		if (current_line[0] == '#')
		{
			continue;
		}

		// creates nodes
		else
		{
			std::stringstream pair_nodes(current_line);
			pair_nodes >> parent_id >> child_id;
			network->insert_page(parent_id, child_id);
			
		}
	}
}

bool coin_flip(double dampening_ratio)
{
	double random_chance = 0.0;
	srand((int)time(NULL)); //^ omp_get_thread_num());

	random_chance = ((double)rand()) / RAND_MAX;

	std::cout << random_chance << std::endl;
	if (dampening_ratio > random_chance)
	{
		return false;
	}
	return true;
}

int random_number(int max_length)
{
	srand((int)time(NULL) ^ omp_get_thread_num());

	return (rand() % max_length);

}
void page_rank(Graph*& network, int walk_count, double dampening_ratio)
{
	int start_node = 0;
	int current_step = 0;
	#pragma omp parallel for schedule(static)shared(network, walk_count, dampening_ratio) private(start_node, current_step)
	for (start_node = 0; start_node < network->get_num_of_pages(); start_node++)
	{
		int current_node = start_node;
		for (current_step = 0; current_step < walk_count; current_step++)
		{

			// coin flip == false then must jump to a random neighbors page
			if (!(coin_flip(dampening_ratio)))
			{
				auto size_of_current_outgoing_link = network->return_complete_list(current_node)->get_outlink_size();
				// If no outgoing ports randomly select the next page
				if (size_of_current_outgoing_link == 0)
				{
					int index = random_number(network->get_num_of_pages());
					current_node = index;
				}
				else
				{
					int index = random_number(size_of_current_outgoing_link);
					Page* temp = network->return_complete_list(current_node);
					int temp_id = temp->get_outgoing_link_id(index);
					current_node = temp_id;
				}
			}

			// coin flip == true then must jump to a random page
			else
			{
				int index = random_number(network->get_num_of_pages());
				current_node = index;
			}
			network->return_complete_list(current_node)->increase_time_visited();
		}
	}
}

void display_network(Page** cur_list, int itt)
{
	for (int i = 0; i < itt; i++)
	{
		std::cout << "--------------" << std::endl;
		std::cout << "page id" << cur_list[i]->get_page_id() << std::endl;
		std::cout << "page_rank" << cur_list[i]->get_page_rank() << std::endl;
		std::cout << "--------------" << std::endl;
	}
}

void calculate_page_rank(Graph*& network, int total_visits)
{
	for (int index = 0; index < network->get_num_of_pages(); index++)
	{
		network->return_complete_list(index)->set_page_rank(total_visits);
	}
}
void find_first_five_max(Graph*& network, Page** first_five, int total_visits)
{
	calculate_page_rank(network, total_visits);
	// Sorts the network so the greatest are stored in the top_five

	for (int index = 0; index < network->get_num_of_pages(); index++)
	{
		auto current_page = network->return_complete_list(index);
		if (current_page->get_page_rank() >= (*first_five[0]).get_page_rank())
		{
			for (int guide = 0; guide < 5; guide++)
			{
				first_five[guide + 1] = first_five[guide];
			}

			first_five[0] = current_page;
		}

		else if (current_page->get_page_rank() >= (*first_five[1]).get_page_rank())
		{
			for (int guide = 1; guide < 5; guide++)
			{
				first_five[guide + 1] = first_five[guide];
			}

			first_five[1] = current_page;
		}

		else if (current_page->get_page_rank() >= (*first_five[2]).get_page_rank())
		{
			for (int guide = 2; guide < 5; guide++)
			{
				first_five[guide + 1] = first_five[guide];
			}

			first_five[2] = current_page;
		}

		else if (current_page->get_page_rank() >= (*first_five[3]).get_page_rank())
		{
			for (int guide = 3; guide < 5; guide++)
			{
				first_five[guide + 1] = first_five[guide];
			}

			first_five[3] = current_page;
		}

		else if (current_page->get_page_rank() >= (*first_five[4]).get_page_rank())
		{
			first_five[4] = current_page;
		}


	}
}


#endif


