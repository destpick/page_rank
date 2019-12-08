#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <cstdlib>
#include <omp.h>
#include "Page.h"

class Graph
{

private:
	Page** complete_list;
	int num_of_pages;

public:
	Graph()
	{
		this->complete_list = nullptr;
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

	Page* get_complete_list(int index)
	{
		return this->complete_list[index];
	}
	Page** return_list()
	{
		return this->complete_list;
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
		int placement = 0;
		// If network is empty want to signal that we need to add this new id
		if (this->complete_list == nullptr)
		{
			std::cout << "Empty network." << std::endl;
		}

		else
		{
			for (int index = 0; index < this->num_of_pages; index++)
			{
				if (id == this->complete_list[index]->get_page_id())
				{
					placement = 1;
				}
			}

		}
		return placement;
	}
	void remove_duplicate()
	{
		for (int j = 0; j < num_of_pages; j++)
		{
			for (int i = 1; i < num_of_pages-1; i++)
			{
				int temp_i = this->complete_list[i]->get_page_id();
				int temp_j = this->complete_list[j]->get_page_id();
				if (temp_i == temp_j)
				{
					Page* temp = this->complete_list[i];
					for (int k = i; k < num_of_pages; k++)
					{
						this->complete_list[k] = this->complete_list[k + 1];
					}
				}
			}
		}
	}
		// Returns the placement of node
		int position_of_node(int id)
		{

			for (int index = 0; index < this->num_of_pages; index++)
			{
				if (id == this->complete_list[index]->get_page_id())
				{
					return index;
				}
			}
		}

		/*
				Adding a page to the existing network
				Return:
						true if successfully added
						false if some error occurred
		*/
		void insert_page(int coming_from, int going_to)
		{

			if (this->complete_list == nullptr)
			{
				this->complete_list = new Page * [1];
			}

			if ((this->find_existing_page(coming_from)) == 1)
			{
				this->complete_list[this->position_of_node(coming_from)]->set_outgoing_link(going_to);
			}

			if ((this->find_existing_page(coming_from)) == 0)
			{
				Page* new_page = new Page(coming_from);
				this->complete_list = (Page**)realloc(complete_list, (this->num_of_pages + 1) * sizeof(Page*));
				this->complete_list[this->num_of_pages] = new_page;
				this->complete_list[this->num_of_pages]->set_outgoing_link(going_to);
				this->num_of_pages++;
			}

			// Adds child page to the network if it doesnt already exist
			if ((this->find_existing_page(going_to)) == 0)
			{
				Page* new_page = new Page(going_to);
				this->complete_list = (Page**)realloc(complete_list, (this->num_of_pages + 1) * sizeof(Page*));
				this->complete_list[this->num_of_pages] = new_page;
				this->num_of_pages++;
			}

		}
	};
	// Parses file and creates the appropriate represented network
	void create_network(Graph*& network, std::ifstream& input_file)
	{
		int parent_id = 0;
		int child_id = 0;

		std::string current_line;

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

	void print_id(Graph*& network)
	{
		for (int j = 0; j < network->get_num_of_pages(); j++)
		{
			for (int i = j+1; i < network->get_num_of_pages(); i++)
			{
				if (network->get_complete_list(j)->get_page_id() == network->get_complete_list(i)->get_page_id())
				{
					std::cout << network->get_complete_list(j)->get_page_id() << " " << network->get_complete_list(i)->get_page_id() << std::endl;
					std::cout << "duplicates" << std::endl;
				}
			}
		}
		std::cout << "no duplicates" << std::endl;
	}

	bool coin_flip(double dampening_ratio)
	{
		double random_chance = 0.0;
		srand((int)time(NULL) ^ omp_get_thread_num());

		random_chance = ((double)rand()) / RAND_MAX;

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
		#pragma omp parallel for schedule(static)shared(network, walk_count, dampening_ratio, start_node)
		for (start_node = 0; start_node < network->get_num_of_pages(); start_node++)
		{
			int current_node = start_node;
			for (current_step = 0; current_step < walk_count; current_step++)
			{
				
				// coin flip == false then must jump to a random neighbors page
				if (!(coin_flip(dampening_ratio)))
				{
					// If no outgoing ports randomly select the next page
					if (network->get_complete_list(current_node)->get_ports() == 0)
					{

						int index = random_number(network->get_num_of_pages());
						current_node = index;
					}
					else
					{
						int index = random_number(network->get_complete_list(current_node)->get_ports());
						Page* temp = network->get_complete_list(current_node);
						int temp_id = temp->get_outgoing_link_id(index);

						current_node = network->position_of_node(temp_id);
					}
				}

				// coin flip == true then must jump to a random page
				else
				{
					int index = random_number(network->get_num_of_pages());
					current_node = index;
				}
				network->get_complete_list(current_node)->increase_time_visited();
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
			network->get_complete_list(index)->set_page_rank(total_visits);
		}
	}
	void find_first_five_max(Graph*& network, Page** first_five, int total_visits)
	{
		calculate_page_rank(network, total_visits);
		// Sorts the network so the greatest are stored in the top_five

		for (int index = 0; index < network->get_num_of_pages(); index++)
		{
			auto current_page = network->get_complete_list(index);
			if (current_page->get_page_rank() > (*first_five[0]).get_page_rank())
			{
				for (int guide = 0; guide < 5; guide++)
				{
					first_five[guide + 1] = first_five[guide];
				}

				first_five[0] = current_page;
			}

			else if (current_page->get_page_rank() > (*first_five[1]).get_page_rank())
			{
				for (int guide = 1; guide < 5; guide++)
				{
					first_five[guide + 1] = first_five[guide];
				}

				first_five[1] = current_page;
			}

			else if (current_page->get_page_rank() > (*first_five[2]).get_page_rank())
			{
				for (int guide = 2; guide < 5; guide++)
				{
					first_five[guide + 1] = first_five[guide];
				}

				first_five[2] = current_page;
			}

			else if (current_page->get_page_rank() > (*first_five[3]).get_page_rank())
			{
				for (int guide = 3; guide < 5; guide++)
				{
					first_five[guide + 1] = first_five[guide];
				}

				first_five[3] = current_page;
			}

			else if (current_page->get_page_rank() > (*first_five[4]).get_page_rank())
			{
				first_five[4] = current_page;
			}


		}
	}


#endif


