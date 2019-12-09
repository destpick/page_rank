#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <iostream>
#include <vector>
///////////////////ADD GETTER AND SETTER FOR PORTS
class Page
{
private:
	std::vector<int> *outgoing_links;
	int page_id;
	int times_visited;
	long double page_rank;

public:
	////////////////////////////////////////// CONSTRUCTORS //////////////////////
	//Empty
	Page()
	{
		this->outgoing_links = new std::vector<int>();
		this->page_id = -1;
		this->times_visited = 0;
		this->page_rank = 0;
	}

	// Adding a page that has an id already
	Page(int id)
	{
		this->outgoing_links = new std::vector<int>();
		this->page_id = id;
		this->times_visited = 0;
		this->page_rank = 0;
	}

	////////////////////////// APPROPRIATE GETTERS AND SETTERS ///////////////////////

	int get_outlink_size()
	{
		return this->outgoing_links->size();
	}

	int get_page_id()
	{
		return this->page_id;
	}

	void set_page_id(int id)
	{
		this->page_id = id;
	}

	void set_page_rank(int total_visits)
	{
		this->page_rank = (long double)this->times_visited / total_visits;
	}

	long double get_page_rank()
	{
		return this->page_rank;
	}
	int get_time_visited()
	{
		return this->times_visited;
	}

	void increase_time_visited()
	{
		this->times_visited++;
	}

	void set_time_visited(int num)
	{
		this->times_visited = num;
	}

	int get_outgoing_link_id(int index)
	{
		return this->outgoing_links->at(index);
	}

	void set_outgoing_link(int id)
	{
		
		if (std::find(this->outgoing_links->begin(), this->outgoing_links->end(), id) == this->outgoing_links->end())
		{
			this->outgoing_links->push_back(id);
		}
	}
};

void create_empty_size_five_array(Page** temp)
{
	Page* new_refrance = new Page(-1);

	for (int i = 0; i < 5; i++)
	{
		temp[i] = new_refrance;
	}
}

#endif